#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE 2

void testDummy2dArray();
void dump2dArr(int* arr, int row, int col);

int main(){
    testDummy2dArray();
    return 0;
}

void testDummy2dArray(){
    int arr[][ARRAY_SIZE]={{1,2},{3,4},{5,6},{7,8}};
    int row = sizeof(arr)/sizeof(*arr); // 32/8=4
    int col = ARRAY_SIZE; // 2
    dump2dArr(*arr, row, col);

    printf("arr %p\n", arr);
    printf("*arr %p\n", *arr);
    printf("arr[0] %p\n", arr[0]);
    printf("&(*arr)[0] %p\n", &(*arr)[0]);
    printf("&arr[0][0] %p\n", &arr[0][0]);

    /*----- Output -----
      array[0][0] = 1
      array[0][1] = 2
      array[1][0] = 3
      array[1][1] = 4
      array[2][0] = 5
      array[2][1] = 6
      array[3][0] = 7
      array[3][1] = 8
      arr 000000F6BDCFFC48
      *arr 000000F6BDCFFC48
      arr[0] 000000F6BDCFFC48
      &(*arr)[0] 000000F6BDCFFC48
      &arr[0][0] 000000F6BDCFFC48
     */
}

void dump2dArr(int* arr, int row, int col){
    int i,j;
    for(i=0; i<row; i++){
        for(j=0; j<col; j++){
            printf("array[%d][%d] = %d\n",i,j, *(arr+i*col+j));
        }
    }
}


