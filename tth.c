#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VAL_PAR_BLOC 25
#define VAL_PAR_CASE 64

void encodage_msg(char* chaine, int* new_chaine){
    for(int i = 0; i < strlen(chaine); i++){
        new_chaine[i] = chaine[i]%VAL_PAR_CASE;
    }
}

int padding(int* chaine, int length){
    int topad = length%VAL_PAR_BLOC;
    topad = VAL_PAR_BLOC - topad;
    if(topad != 0){    //bourrage/padding
        int is_first_pad = 1;
        for(int i = length; i < (topad+length); i++){
            if(is_first_pad == 1){
                chaine[i] = 32;
                is_first_pad = 0;
            }
            else{
                chaine[i] = 0;
                chaine[13] = 0;
            }
        }
    }

    return topad+length;
}



int main(int argc, char* argv[]){
    if(argc < 2){
        printf("utilisation: %s [chaine]", argv[0]);
        return 1;
    }

    size_t l = strlen(argv[1]) + 1;
    int * chaine = (int *) malloc(12*l * sizeof(int));

    encodage_msg(argv[1], chaine);
    
    printf("encodage:\n");
    for(int i = 0; i < strlen(argv[1]); i++){
        printf("%d ", chaine[i]);
    }
    printf("\n\n");

    int message_length = padding(chaine, strlen(argv[1]));

    printf("padding:\n");
    for(int i = 0; i < message_length; i++){
        printf("%d ", chaine[i]);
    }
    printf("\n%d\n\n", message_length);
/*
    //creation matrices
    int matrices[message_length/25%25][25];
    for(int i = 0; i < message_length/25%25; i++){ //pour chaque bloc
        for(int j = 0; j < 25; j++){  //pour chaque element du bloc
            matrices[i][j] = chaine[(i*25)+j];
        }
    }

    printf("matrices:\n");
    for(int i = 0; i < message_length/25%25; i++){ //pour chaque bloc
        for(int j = 0; j < 25; j++){  //pour chaque element du bloc
            printf("%d ", matrices[i][j]);
        }
        printf("\n");
    }
*/

    //creation matrices
    int matrices[message_length/25%25][5][5];
    for(int i = 0; i < message_length/25%25; i++){ //pour chaque bloc
        for(int j = 0; j < 25; j++){  //pour chaque element du bloc
            matrices[i][j/5%5][j%5] = chaine[(i*25)+j];
        }
    }

    printf("matrices:\n");
    for(int i = 0; i < message_length/25%25; i++){ //pour chaque bloc
        for(int j = 0; j < 25; j++){  //pour chaque element du bloc
            if(j%5 == 0) printf("\n");
            printf("%d ", matrices[i][j/5%5][j%5]);
        }
        printf("\n\n");
    }

    return 0;
}