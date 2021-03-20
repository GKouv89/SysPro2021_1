#include <stdio.h>
#include <assert.h>

int main(){
  FILE *fp = fopen("ids.txt", "r");
  int prevnumber = -1;
  int currnumber;
  while(!feof(fp)){
    if(fscanf(fp, "%d", &currnumber) != 1){
      break;
    }
    // printf("currnumber = %d, prevnumber = %d\n", currnumber, prevnumber);
    assert(currnumber == prevnumber + 1);
    prevnumber = currnumber;
  }
  fclose(fp);
  return 0;
}