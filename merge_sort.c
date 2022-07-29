//C header files
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "timer.h"
#include "pth_msort.h"


void merge(int* value, int start, int mid, int end, int* test_sorted)
{
    int left_pointer = start;
    int right_pointer = mid+1;

    int total_array_size = end - start + 1;
    
    for(int merge_index = start; merge_index <= total_array_size+start; merge_index++){
        if (left_pointer >= mid +1){
            test_sorted[merge_index] = value[right_pointer];
            right_pointer++;            
        }
        else if (right_pointer >= end + 1) {
            test_sorted[merge_index] = value[left_pointer];
            left_pointer++; 
        }
        else if (value[left_pointer] <= value[right_pointer]){
            test_sorted[merge_index] =  value[left_pointer];
            left_pointer ++;
        }
        else {
            test_sorted[merge_index] = value[right_pointer];
            right_pointer ++;
        }

    }

    for(int merge_index = 0; merge_index <= total_array_size; merge_index++){
        value[start+merge_index] = test_sorted[merge_index+start];
    }
}

void merge_sort(int* value, int start_index, int end_index, int* sorted)
{   
    if (end_index <= start_index){
        return;
    }


    int mid_index = (start_index+end_index)/2;

    merge_sort(value, mid_index+1, end_index, sorted);
    merge_sort(value, start_index, mid_index, sorted);


    merge(value, start_index, mid_index, end_index, sorted);
}


void fillArray ( int** values, unsigned int N )
{
    srand(time(0));
	unsigned int i;
    for (i=0; i<N; i++)
        (*values)[i] = N-i;
}

int main ( int argc, char* argv[] )
{
    int N = 1000000000;
    
    int* values = (int*) malloc ( sizeof(int) * N );
	fillArray ( &values, N );

    printf("\n start \n \n");
    for( int i = N - 50; i< N ; i++){
        printf("%d \n", values[i]);
    }
    printf("\n");

	int* sorted = (int*) malloc ( sizeof(int) * N );

    merge_sort(values, 0, N-1, sorted);
    printf("\n final \n \n");
        for( int i = N - 50; i< N ; i++){
        printf("%d \n", values[i]);
    }
    printf("\n");
	//Sorted array
}