# Parallel Programming

## [Matrix multiplication using CUDA](./cuda_mm/)
Compute each element of the matrix independently, in other word parallel matrix multiplication is to let each thread do a vector-vector multiplication 

## [Block matrix multiplication using CUDA](./cuda_bmm/)
block matrix multiplication with CUDA

A and B are “float” matrices of size N × N, where N = 2^M. We would like to calculate C = A × B. Parameter M should be a command line argument to the main() function. This code works for M = 10 to 13

Rectangular tiles have been employed instead of square ones to give us one more freedom degree in calculating output blocks.

## [Implement inclusive scan using CUDA](./cuda_scan/)
Scan an array both inc/exc with CUDA

This code is able to scan an array of size n = 2 ^ M where M can be from 2 to 29! both inclusive and exclusive scan have been implemented. the default mode is inclusive. Blelloch algorithm (work-efficient) scan has been used.

compile : nvcc scan2_to_29.cu scan2_main.cu -o scan2
run : ./scan2 M

## [Implement parallel sort with Pthreads library](./parallel%20merge%20sort/)

By deploying pthread library (libpthread.so) we are able to launch multiple threads
and do tasks in parallel. In this code DLP (data level parallelism) has been implemented
and the objective is to sort a giant array of size **N = 2 ^ M  where M can be up to 30!**.

So it may take forever if you try to sort it serially.
```
Now the idea is to break the procedure into 3 stages :

		1) divide the array into 4 segments and sort them in parallel by 4 threads
			(each thread sorts one segment serially with mergesort) but the threads work in parallel
		
            2) now we have 4 sorted segments -> s1,s2,s3,s4
			one thread merges (s1,s2) and another thread merges (s3,s4)
			again the merges are serial, but 2 threads work in parallel
            
            3) now that we have 2 sorted arrays, each of sise N/2, we should do parallel merge 
			in this stage because the size of the arrays is big. so we launch 4 
			threads to do parallel merge of these 2 arrays and form the final sorted array
```		

