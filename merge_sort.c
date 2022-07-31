//C header files
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "timer.h"
#include <stdbool.h>

int* value;
unsigned int n;
int* sort;

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



int main(){
    int* a = (int*) malloc(sizeof(int) * 10);
    for(int i = 0; i < 10; i++){
        a[i] = 2*i;
    }
    int * sort = (int*) malloc(sizeof(int) * 10);
    int result = binary_search(a, 0, 9, 11);

    printf(" %d \n", result);

}