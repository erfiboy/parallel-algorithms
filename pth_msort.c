// Include your C header files here

#include "pth_msort.h"

int* value;
unsigned int n;
int* sort;
typedef struct 
{
    int index;
    int left_last_index;
    int right_last_index;
    int right_size;
    int left_size;
} pth_msort;

void merge_sort(int* value, int start_index, int end_index, int* sorted);
void mergeSortParallel(const int* values, unsigned int N, int* sorted);
void merge(int* value, int start, int mid, int end, int* test_sorted);
void binary(const int* values, int value);
void* merge_thread_handler(void* thread_index);
void* sort_thread_handler(void* thread_index);
void* para_merge_thread_handler(void* info);
int compare_point(const int a , const int b);
void printArray(int *arr, int size);
void merge_tow_separated(int* value1, int start1, int size1, int start2, int size2, int* sorted, int index_sorted);
int binary_search(const int* values, int start, int end, int value);

void mergeSortParallel(const int* values, unsigned int N, int* sorted)
{
    value = (int*) values;
    n = N;
    sort = sorted;
    int* value = (int*) values;
    int start = 0;
    int end = N-1;
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

    int step = pow(2,n/8);
    int section_left = step;
    int section_right= N/2+1 + step;

    int last_element_left=0;
    int last_element_right=N/2;

    for (int thread_index =0; thread_index<4; thread_index++){
        int temp = binary_search(value, N/2, N-1, value[thread_index*n/8]);
        pth_msort* thraed_index = (pth_msort*) malloc(sizeof(pth_msort));
        thraed_index->index = thread_index;
        thraed_index->left_last_index = thread_index*n/8;
        thraed_index->left_size=thread_index*n/4;
        thraed_index->right_last_index=0;
        // merge_tow_separated(value, thread_index*n/8, n/8, n/2 + thread_index*n/8, n/8, sort, thread_index*n/4);
        pthread_create(&handles[thread_index], NULL, para_merge_thread_handler, (void*)thraed_index);  
    } 
    for (int thread_index =0; thread_index<4; thread_index++){
        pthread_join(handles[thread_index], NULL);  
    } 

    merge_tow_separated(value,0,N/2,0,N/2,sort,0);
}

void printArray(int *arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%d \n", arr[i]);
    }
    printf("\n");
    
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

void* para_merge_thread_handler(void* thread_index){
    pth_msort* index = (pth_msort*) thread_index;
    merge_tow_separated(value, index->index*n/8, n/8, n/2 + index->index*n/8, n/8, sort, index->index*n/4);
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

int binary_search(const int* values, int start, int end, int value)
{
    if ( end - start < 2 &&  values[end] != value && values[start] != value){
        return end;
    }
    if (value < values[start])
    {
        return start;
    }

    if (value == values[start] || value < values[start]){
        return start;
    }
    else if(value == values[end] || value > values[end]){
        return end;
    }
    else if(value == values[(start+end)/2]){
        return (start+end)/2;
    }
    else if (value < values[(start+end)/2]){
        return binary_search(values, start, (start+end)/2, value);
    }
    else{
        return binary_search(values, (start+end)/2+1, end, value);
    }
}

void merge_tow_separated(int* value, int start1, int size1, int start2, int size2, int* sorted, int index_sorted)
{
    int left_pointer = start1;
    int right_pointer = start2;
    int total_array_size = size1+size2;
    merge(value, left_pointer, right_pointer, total_array_size, sorted);
    
    for(int merge_index = index_sorted; merge_index < total_array_size+index_sorted; merge_index++){
        if (left_pointer >= start1+size1){
            sorted[merge_index] = merge_index;
            right_pointer++;            
        }
        else if (right_pointer >= start2 + size2) {
            sorted[merge_index] = merge_index;
            left_pointer++; 
        }
        else if (value[left_pointer] <= value[right_pointer]){
            sorted[merge_index] =  merge_index;
            left_pointer ++;
        }
        else {
            sorted[merge_index] = merge_index;
            right_pointer ++;
        }

    }
    for(int merge_index = index_sorted; merge_index < total_array_size+index_sorted; merge_index++){
        value[merge_index]=sorted[merge_index];
    }
}