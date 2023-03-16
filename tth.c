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
        for(int i = length; i < topad+length; i++){
            if(is_first_pad == 1){
                chaine[i] = 32;
            }
            else{
                chaine[i] = 0;
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
    int * chaine = (int *) malloc(l * sizeof(int));

    encodage_msg(argv[1], chaine);
    
    printf("%d\n", chaine[7]);

    int message_length = padding(chaine, strlen(argv[1]));

    printf("%d\n", chaine[7]);
    printf("%d\n", message_length);

    return 0;
}