//ONLY MODIFY THIS FILE!
//YOU CAN MODIFY EVERYTHING IN THIS FILE!

#include "bmm.h"
#include "math.h"

#define tx threadIdx.x
#define ty threadIdx.y
#define tz threadIdx.z

#define bx blockIdx.x
#define by blockIdx.y
#define bz blockIdx.z

// TILEX and TILEY are used to set the number of threads in a CUDA block 
#define TILEX 32
#define TILEY 16

// you may define other parameters here!
#define array2d(array, i, j)	array[i*n + j]

#define factor 4

#if TILEX > TILEY
	#define common_dim_of_matrixes (factor * TILEX)
#else
	#define common_dim_of_matrixes (factor * TILEY)
#endif
// you may define other functions here!


// 
struct SubMatrix
{
	int first_matrix_loop;
	int second_matrix_loop;
};

__host__ __device__ SubMatrix calculate_common_dimension(){
	int first_matrix_loop = 0;
	int second_matrix_loop = 0;
	// define a factor to minimize time
	if (TILEX > TILEY){
		first_matrix_loop = factor;
		second_matrix_loop = factor * TILEX / TILEY;
	}
	else{
		first_matrix_loop = factor * TILEY / TILEX;
		second_matrix_loop = factor;
	}
	struct SubMatrix sub_matrix = {first_matrix_loop, second_matrix_loop};
	return sub_matrix;
}

dim3 getDimGrid(const int m, const int n) {
	dim3 dimGrid(n/TILEX,n/TILEY);
	return dimGrid;
}
dim3 getDimBlock(const int m, const int n) {
	dim3 dimBlock(TILEX,TILEY);
	return dimBlock;
}
__global__ void kernelFunc(float* ad, float* bd, float* cd, const int m, const int n) {

	struct SubMatrix sub_matrix = calculate_common_dimension();

	__shared__ float A[TILEY][common_dim_of_matrixes];
	__shared__ float B[common_dim_of_matrixes][TILEX];

	int i = TILEY * by + ty;
	int j = TILEX * bx + tx;
	
	float C = 0;

	int number_of_iterations = common_dim_of_matrixes;
	int max_loop = max (sub_matrix.first_matrix_loop, sub_matrix.second_matrix_loop);
	for(int e = 0 ; e < n/number_of_iterations ; e++)
	{	
		if ( max_loop > sub_matrix.first_matrix_loop)
		{
			for(int k = 0; k < sub_matrix.first_matrix_loop ; k++){
				A[ty][k * TILEX + tx] = ad[i*n + (e*number_of_iterations + k * TILEX + tx)];
				B[k * TILEY + ty][tx] = bd[(e*number_of_iterations + k * TILEY + ty)*n + j];
			}
			for (int k = sub_matrix.first_matrix_loop; k < sub_matrix.second_matrix_loop; k++){
				B[k * TILEY + ty][tx] = bd[(e*number_of_iterations + k * TILEY + ty)*n + j];
			}
		}
		else if (max_loop > sub_matrix.second_matrix_loop){
			for(int k = 0; k < sub_matrix.second_matrix_loop ; k++){
				A[ty][k * TILEX + tx] = ad[i*n + (e*number_of_iterations + k * TILEX + tx)];
				B[k * TILEY + ty][tx] = bd[(e*number_of_iterations + k * TILEY + ty)*n + j];
			}
			for (int k = sub_matrix.second_matrix_loop; k < sub_matrix.first_matrix_loop; k++){
				A[ty][k * TILEX + tx] = ad[i*n + (e*number_of_iterations + k * TILEX + tx)];
			}

		}
		else{
			for(int k = 0; k < sub_matrix.second_matrix_loop ; k++){
				A[ty][k * TILEX + tx] = ad[i*n + (e*number_of_iterations + k * TILEX + tx)];
				B[k * TILEY + ty][tx] = bd[(e*number_of_iterations + k * TILEY + ty)*n + j];
			}
		}
		__syncthreads();

		for(int k=0; k < number_of_iterations;k++)
		{
			C += A[ty][k] * B[k][tx];
		}
		__syncthreads();
	}

	cd[i*n + j] = C;
}