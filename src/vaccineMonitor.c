#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
  if(argc != 6){
    printf("Flags:\n");
    printf("-c file_name: records will be read from the file named file_name.\n");
    printf("-b bloomSize: the size of the bloom filters in *kilobytes*\n");
    printf("It is necessary you use them both and provide an argument for each one,\n");
    printf("but the order in which you use them doesn't matter.\n");
    exit(1);
  }
}