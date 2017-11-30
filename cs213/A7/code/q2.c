#include <stdlib.h>
#include <stdio.h>

int x[] = {1,2,3,0xffffffff,0xfffffffe,0,184,340057058};
int y[8];

int f(int n){
    int count = 0;
    int temp;
    while (n != 0){
        temp = n & 0x80000000;
        if (temp != 0){
            count++;
        }
        n = n*2;
    }
    return count;
}

int main() {
    for (int i = 7; i >= 0; i--){
        y[i] = f(x[i]);
    }
    for (int i = 0; i < 8; i++){
        printf("%d\n",x[i]);
    }
    for (int i = 0; i < 8; i++){
        printf("%d\n",y[i]);
    }
}
