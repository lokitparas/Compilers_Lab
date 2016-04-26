#include <stdio.h>
extern void swap();
int buf[2] = {23,56};
void foo(){
    buf[0] = buf[1]+1;
}
int main(){
    foo();
    swap();
    printf("buf[0] = %d",buf[0]);
    return 0;
}