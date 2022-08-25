// ONLY MODIFY THIS FILE

#include "scan2.h"
#include "gpuerrors.h"
#include "stdio.h"

#define tx threadIdx.x
#define ty threadIdx.y
#define tz threadIdx.z

#define bx blockIdx.x
#define by blockIdx.y
#define bz blockIdx.z

// you may define other parameters here!
const int max_array_size = 1024;
const int max_scan_len_one_step = 1<<25;
// you may define other macros here!
#define max(X, Y) (((X) > (Y)) ? (X) : (Y))
// you may define other functions here!

__global__ void Blelloch(float* input, float* total_sum, int n)
{
    __shared__ float scan[max_array_size];

    scan[tx] = input[bx * n + tx];
    __syncthreads();

    int step;
    for(step = 2; step <= n; step = step* 2)
    {
        if(tx % step == step-1)
        {
            scan[tx] = scan[tx - (step/2)] + scan[tx];
        }
        __syncthreads();
    }

    if(tx == 0)
    {
        total_sum[bx] = scan[(n - 1)];
        scan[(n - 1)] = 0;
    }
    __syncthreads();


    //////////////////////////
    //  left    right 
    //    \      /
    //     \    /
    //      \  /
    //       \/
    //       /\    
    //      /  \   
    //     /    \  
    //    /      \  
    // right   left+right
    //////////////////////////
    
    float right;
    float left;
    for(int step = n; step >= 2; step /= 2)
    {
        if(tx % step == step-1)
        {
            left = scan[tx - (step/2)];
            right = scan[tx];
            scan[tx - (step/2)] = right;
            right += left; 
            scan[tx] = right;
        }
        __syncthreads();
    }

    input[bx * n + tx] = scan[tx];
    __syncthreads();
}

__global__ void add_sum_of_last_block(float* input, float* total_sum_prev_block, int n)
{
    input[bx * n + tx] += total_sum_prev_block[bx];
    __syncthreads();
}

void gpuKernel(float* a, float* c,int n) {
	
	int number_of_iteration = max(n/max_scan_len_one_step, 1);

	float* elemets_array;
	float* sum_of_blockes_array;
	float* scan_in_sum_of_blocks;
	float* total_sum;

	cudaMalloc((void**)&elemets_array, n * sizeof(float));
	cudaMalloc((void**)&sum_of_blockes_array, (n / max_array_size ) * sizeof(float));
	cudaMalloc((void**)&scan_in_sum_of_blocks, (n / (max_array_size*max_array_size)) * sizeof(float));
	cudaMalloc((void**)&total_sum, sizeof(float));

	int array_size = n;	
	
	for (int i = 0; i < number_of_iteration; i++){

		n = array_size / number_of_iteration;
		if (i == 0) {
			cudaMemcpy(elemets_array, a , n * sizeof(float), cudaMemcpyHostToDevice);
		}
		else {
			cudaMemcpy(elemets_array, a+i*n-1 , n * sizeof(float), cudaMemcpyHostToDevice);
		}
		Blelloch<<< n/max_array_size , max_array_size >>>(elemets_array,sum_of_blockes_array,max_array_size);
		Blelloch<<< n/(max_array_size*max_array_size) , max_array_size >>>(sum_of_blockes_array,scan_in_sum_of_blocks,max_array_size);
		Blelloch<<< 1 , n/(max_array_size*max_array_size) >>>(scan_in_sum_of_blocks,total_sum,n/(max_array_size*max_array_size));

		add_sum_of_last_block <<< n/(max_array_size*max_array_size) , max_array_size>>> (sum_of_blockes_array,scan_in_sum_of_blocks,max_array_size);
		add_sum_of_last_block <<< n/max_array_size , max_array_size>>> (elemets_array,sum_of_blockes_array,max_array_size);

		
		if (i == 0) {
			cudaMemcpy(c, elemets_array+1, (n-1) * sizeof(float), cudaMemcpyDeviceToHost);
		}
		else {
			cudaMemcpy(c + i*n, elemets_array+2, (n-2) * sizeof(float), cudaMemcpyDeviceToHost);
		}
		

		if (i == 0){
			c[(1+i)*n-1] = c[(1+i)*n-2] + a[(1+i)*n-1];
			a[(1+i)*n-1] = c[(1+i)*n-1];
		}
		else if (i == number_of_iteration -1){
			c[(1+i)*n-2] = c[(1+i)*n-3] + a[(1+i)*n-2];
			c[(1+i)*n-1] = c[(1+i)*n-2] + a[(1+i)*n-1];
		}
		else{
			c[(1+i)*n-2] = c[(1+i)*n-3] + a[(1+i)*n-2];
			c[(1+i)*n-1] = c[(1+i)*n-2] + a[(1+i)*n-1];
			a[(1+i)*n-1] = c[(1+i)*n-1];
		}

	}

	cudaFree(elemets_array);
	cudaFree(sum_of_blockes_array);
	cudaFree(scan_in_sum_of_blocks);
	cudaFree(total_sum);

}
	