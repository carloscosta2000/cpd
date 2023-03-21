#include <stdio.h>


int main(int argc, char *argv[]){
    int finished = 0;
    int i = 0;
    while(finished == 0){
        if(i == 5){
            finished = 1;
            continue;
        }
        printf("Olá %d \n", i);
        i++;
    }
}