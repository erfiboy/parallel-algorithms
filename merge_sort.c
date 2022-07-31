//C header files
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "timer.h"
#include "pth_msort.h"
#include <stdbool.h>

int* value;
unsigned int n;
int* sort;



void merge_sort(int* value, int start_index, int end_index, int* sorted);
void mergeSortParallel(const int* values, unsigned int N, int* sorted);
void merge(int* value, int start, int mid, int end, int* test_sorted);
void binary(const int* values, int value);
void* merge_thread_handler(void* thread_index);
void* sort_thread_handler(void* thread_index);
int compare_point(const int a , const int b);
void struct_merge_sort(int* value, int start_index, int end_index, int* sorted, int* point_index, int* sorted_int_index, bool* point_A,bool* sorted_A);
void struct_merge(int* value, int start, int mid, int end, int* test_sorted, int* point_index, int* sorted_int_index, bool* point_A,bool* sorted_A);

void mergeSortParallel(const int* values, unsigned int N, int* sorted)
{
    value = (int*) values;
    n = N;
    sort = sorted;
    int* value = (int*) values;
    pthread_t* handles = (pthread_t*) malloc (4*sizeof(pthread_t));
    for (int thread_index =0; thread_index<4; thread_index++){
        pthread_create(&handles[thread_index], NULL, sort_thread_handler, (void*)thread_index);  
    } 

    for (int thread_index =0; thread_index<4; thread_index++){
        pthread_join(handles[thread_index], NULL);  
    } 

    // merge two merge sorts 

    for (int thread_index =0; thread_index<2; thread_index++){
        pthread_create(&handles[thread_index], NULL, merge_thread_handler, (void*)thread_index);  
    } 

    for (int thread_index =0; thread_index<2; thread_index++){
        pthread_join(handles[thread_index], NULL);  
    }  

    int step = pow(2,1);
    int* point_values = (int*) malloc ( sizeof(int) * N/step );
    int* sorted_int_values = (int*) malloc ( sizeof(int) * N/step );

    int* point_index = (int*) malloc ( sizeof(int) * N/step );
    int* sorted_int_index = (int*) malloc ( sizeof(int) * N/step );

    bool* point_A = (bool*) malloc ( sizeof(bool) * N/step );
    bool* sorted_int_A = (bool*) malloc ( sizeof(bool) * N/step );

    for (int i = 0; i < N/(2*step); i++){
        point_values[2*i] = values[i*step];
        point_index[2*i] = i*step;
        point_A[2*i] = true;

        point_values[2*i+1] = values[(i*step)+N/2];
        point_index[2*i+1] = (i*step)+N/2;
        point_A[2*i+1]= false;

    }
    printf("Hellllloooooo---------------\n");
    for(int i = 0; i < N/pow(2,1); i++){
        printf("index =%d, value=%d \n", point_index[i], point_values[i]);
    }
    struct_merge_sort(point_values, 0,  2*N/pow(2,1)-1, sorted_int_values, point_index, sorted_int_index, point_A, sorted_int_A);

    printf("Hellllloooooo---------------\n");
    for(int i = 0; i < N/pow(2,1); i++){
        printf("index =%d, value=%d \n", point_index[i], point_values[i]);
    }
}

int compare_int(const int a , const int b) {
    if (a <= b) {
        return -1;
    } else if (a > b) {
        return 1;
    }
}

void* sort_thread_handler(void* thread_index){
    int index = (int) thread_index;
    merge_sort(value, index*n/4, (index+1)*n/4-1, sort);
    return NULL;
}

void* merge_thread_handler(void* thread_index){
    int index = (int) thread_index;
    merge(value, index*n/2, index*n/2+n/4-1, index*n/2+n/2-1, sort);
    return NULL;
}

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

void fillArray( int** values, unsigned int N )
{
    srand(time(0));
	unsigned int i;
    for (i=0; i<N; i++)
        (*values)[i] = N-i;
}

void struct_merge_sort(int* value, int start_index, int end_index, int* sorted, int* point_index, int* sorted_int_index, bool* point_A,bool* sorted_int_A){
    if (end_index <= start_index){
        return;
    }

    int mid_index = (start_index+end_index)/2;

    struct_merge_sort(value, mid_index+1, end_index, sorted, point_index, sorted_int_index, point_A, sorted_int_A);
    struct_merge_sort(value, start_index, mid_index, sorted, point_index, sorted_int_index, point_A, sorted_int_A);

    struct_merge(value, start_index, mid_index, end_index, sorted, point_index, sorted_int_index, point_A, sorted_int_A);
}

void struct_merge(int* value, int start, int mid, int end, int* test_sorted, int* point_index, int* sorted_int_index, bool* point_A,bool* sorted_int_A){
    int left_pointer = start;
    int right_pointer = mid+1;

    int total_array_size = end - start + 1;
    
    for(int merge_index = start; merge_index <= total_array_size+start; merge_index++){
        if (left_pointer >= mid +1){
            test_sorted[merge_index] = value[right_pointer];
            sorted_int_index[merge_index] = point_index[right_pointer];
            sorted_int_A[merge_index] = point_A[right_pointer];
            right_pointer++;            
        }
        else if (right_pointer >= end + 1) {
            test_sorted[merge_index] = value[left_pointer];
            sorted_int_index[merge_index] = point_index[left_pointer];
            sorted_int_A[merge_index] = point_A[left_pointer];
            left_pointer++;     
        }
        else if (value[left_pointer] < value[right_pointer]){
            test_sorted[merge_index] = value[left_pointer];
            sorted_int_index[merge_index] = point_index[left_pointer];
            sorted_int_A[merge_index] = point_A[left_pointer];
            left_pointer++;     
        }
        else {
            test_sorted[merge_index] = value[right_pointer];
            sorted_int_index[merge_index] = point_index[right_pointer];
            sorted_int_A[merge_index] = point_A[right_pointer];
            right_pointer++;     
        }
        printf("for value: %d index: %d , right_pointer: %d, left_pointer: %d\n",  test_sorted[merge_index], sorted_int_index[merge_index], right_pointer, left_pointer);
    }

    for(int merge_index = 0; merge_index <= total_array_size; merge_index++){
        value[start+merge_index] = test_sorted[merge_index+start];
        point_index[start+merge_index] = sorted_int_index[merge_index+start];
        point_A[start+merge_index] = sorted_int_A[merge_index+start];
    }

}

int main ( int argc, char* argv[] )
{
    unsigned int N = pow( 2, 5);
    
    int* values = (int*) malloc ( sizeof(int) * N );
	fillArray ( &values, N );

    printf("\n start \n \n");
    for( int i = N - 50; i< N ; i++){
        printf("%d \n", values[i]);
    }
    printf("\n");

	int* sorted = (int*) malloc ( sizeof(int) * N );

    mergeSortParallel(values, N, sorted);
    printf("\n final sorted \n \n");
        for( int i = 0; i< N ; i++){
        printf("%d \n", sorted[i]);
    }
    printf("\n");
	//Sorted array
}