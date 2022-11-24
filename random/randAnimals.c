#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char **argv){
    
    int n = atoi(argv[1]);
    int l = atoi(argv[2]);
    int c = atoi(argv[3]);
    printf("%d\n", n);
    srand(time(0));
    int position[n];
    int type[n];
    for(int i = 0; i < n; i++){
        int number = rand() % (c * l);
        int error = 0;
        for(int j = 0; j < i; j++){
            if(position[j] == number){
                error = 1;
                break;
            }
        }
        if(error){
            i--;
            continue;
        }
        position[i] = number;
        type[i] = rand() % 3;
    }
    for(int i = 0 ; i < n; i++){
        switch(type[i]){
            case 0:
                printf("ROCHA ");
            break;
            case 1:
                printf("COELHO ");
            break;
            case 2:
                printf("RAPOSA ");
            break;
        }
        printf("%d %d\n", position[i]/c , position[i] % c);    
    }
    return 0;
}