#include <stdlib.h>
#include <stdio.h>

int a[10];

void c (int b0, int b1) {
    a[b1] = b0 + a[b1];
}
  
int main () {
    c(3, 4);
    c(1, 2);
    for (int i = 0; i< 10; i++){
        printf("%d\n",a[i]);
    }
}
  