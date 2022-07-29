// Include your C header files here

#include "pth_msort.h"

void mergeSortParallel (const int* values, unsigned int N, int* sorted) {
	

}

void merge(int* value, int start, int mid, int end, int* test_sorted)
{
    int left_pointer = start;
    int right_pointer = mid+1;

    int total_array_size = end - start + 1;
    
    for(int merge_index = 0; merge_index <= total_array_size; merge_index++){
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
        value[start+merge_index] = test_sorted[merge_index];
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