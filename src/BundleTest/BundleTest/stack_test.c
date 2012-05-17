/*

checking how much data we could stuff into the stack 

*/

void fun(){
  int a[10000000];
  int i=0;
  for (i=0;i < 10000000; i++){
    a[i]=i;
  }
}

int main(void){
  fun();
}
