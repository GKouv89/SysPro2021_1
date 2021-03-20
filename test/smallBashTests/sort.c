#include <stdio.h>

void swap(int* a, int* b) 
{ 
  int t = *a; 
  *a = *b; 
  *b = t; 
} 
 
/* This function takes last element as pivot, places 
the pivot element at its correct position in sorted 
array, and places all smaller (smaller than pivot) 
to left of pivot and all greater elements to right 
of pivot */
int partition (int arr[], int low, int high) 
{ 
  int pivot = arr[high]; // pivot 
  int i = (low - 1); // Index of smaller element and indicates the right position of pivot found so far

  for (int j = low; j <= high - 1; j++) 
  { 
      // If current element is smaller than the pivot 
      if (arr[j] < pivot) 
      { 
          i++; // increment index of smaller element 
          swap(&arr[i], &arr[j]); 
      } 
  } 
  swap(&arr[i + 1], &arr[high]); 
  return (i + 1); 
} 
 
/* The main function that implements QuickSort 
arr[] --> Array to be sorted, 
low --> Starting index, 
high --> Ending index */
void quickSort(int arr[], int low, int high) 
{ 
  if (low < high) 
  { 
      /* pi is partitioning index, arr[p] is now 
      at right place */
      int pi = partition(arr, low, high); 

      // Separately sort elements before 
      // partition and after partition 
      quickSort(arr, low, pi - 1); 
      quickSort(arr, pi + 1, high); 
  } 
} 

int main(){
  FILE *fp = fopen("ids.txt", "r");
  int ids[10000];
  for(int i = 0; i < 10000; i++){
    ids[i] = 0;
  }
  int linesRead = 0;
  while(!feof(fp)){
    fscanf(fp, "%d", &ids[linesRead]);
    linesRead++;
  }
  fclose(fp);
  quickSort(ids, 0, 9999);
  fp = fopen("ids.txt", "w");
  for(int i = 0; i < 10000; i++){
    fprintf(fp, "%d\n", ids[i]);
  }
  fclose(fp);
}