#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EMPREINTE 5
#define VAL_PAR_BLOC (EMPREINTE*EMPREINTE)
#define VAL_PAR_CASE 64
#define VAL_PADDING 32

void encodage_msg(char* chaine, int* new_chaine){
    for(int i = 0; i < strlen(chaine); i++){
        new_chaine[i] = chaine[i]%VAL_PAR_CASE;
    }
}

int padding(int* chaine, int length){
    int topad = length%(VAL_PAR_BLOC);
    topad = VAL_PAR_BLOC - topad;
    if(topad != 0){    //bourrage/padding
        int is_first_pad = 1;
        for(int i = length; i < (topad+length); i++){
            if(is_first_pad == 1){
                chaine[i] = VAL_PADDING;
                is_first_pad = 0;
            }
            else{
                chaine[i] = 0;
            }
        }
    }

    return topad+length;
}

int creer_matrices(int*** matrices, int* chaine, int nb_blocs){
    for(int i = 0; i < nb_blocs; i++){ //pour chaque bloc
        for(int j = 0; j < VAL_PAR_BLOC; j++){  //pour chaque element du bloc
            matrices[i][j/EMPREINTE%EMPREINTE][j%EMPREINTE] = chaine[(i*VAL_PAR_BLOC)+j];
        }
    }
}

int main(int argc, char* argv[]){
    if(argc < 2){
        printf("utilisation: %s [chaine]", argv[0]);
        return 1;
    }

    size_t l = strlen(argv[1]) + 1;
    int * chaine = (int *) malloc(12*l * sizeof(int));

    encodage_msg(argv[1], chaine);
    
/*
    //chaine exemple
    chaine[0] = 0;
    chaine[1] = 6;
    chaine[2] = 8;
    chaine[3] = 35;
    chaine[4] = 17;
    chaine[5] = 28;
    chaine[6] = 24;
    chaine[7] = 56;
    chaine[8] = 62;
    chaine[9] = 7;
    chaine[10] = 12;
    chaine[11] = 16;
    chaine[12] = 20;
    chaine[13] = 5;
    chaine[14] = 33;
    chaine[15] = 43;
    chaine[16] = 35;
    chaine[17] = 27;
    chaine[18] = 12;
    chaine[19] = 60;
    chaine[20] = 25;
    chaine[21] = 23;
    chaine[22] = 18;
    chaine[23] = 1;
    chaine[24] = 45;
    chaine[25] = 56;
    chaine[26] = 12;
    chaine[27] = 34;
    chaine[28] = 21;
    chaine[29] = 20;
    chaine[30] = 2;
    chaine[31] = 10;
    chaine[32] = 22;
    chaine[33] = 20;
    chaine[34] = 17;
    chaine[35] = 34;
    chaine[36] = 1;
*/

    //print
    printf("encodage:\n");
    for(int i = 0; i < strlen(argv[1]); i++){
        printf("%d ", chaine[i]);
    }
    printf("\n\n");

    int message_length = padding(chaine, strlen(argv[1]));

    //print
    printf("padding:\n");
    for(int i = 0; i < message_length; i++){
        printf("%d ", chaine[i]);
    }
    printf("\n%d\n\n", message_length);

    int nb_blocs = message_length/VAL_PAR_BLOC%VAL_PAR_BLOC;

    //creation matrices
    int matrices[nb_blocs][EMPREINTE][EMPREINTE];
    int (* pointer_matrices)[nb_blocs][EMPREINTE][EMPREINTE] = &matrices;
    for(int i = 0; i < nb_blocs; i++){ //pour chaque bloc
        for(int j = 0; j < VAL_PAR_BLOC; j++){  //pour chaque element du bloc
            matrices[i][j/EMPREINTE%EMPREINTE][j%EMPREINTE] = chaine[(i*VAL_PAR_BLOC)+j];
        }
    }

    //print
    printf("matrices:\n");
    for(int i = 0; i < nb_blocs; i++){ //pour chaque bloc
        for(int j = 0; j < VAL_PAR_BLOC; j++){  //pour chaque element du bloc
            if(j%EMPREINTE == 0) printf("\n");
            printf("%d ", matrices[i][j/EMPREINTE%EMPREINTE][j%EMPREINTE]);
        }
        printf("\n\n");
    }

    //initialisation de l'empreinte
    int empreinte[EMPREINTE];
    for(int j = 0; j < EMPREINTE; j++){ //pour chaque colonne
        empreinte[j] = 0;
    }

    //CALCUL EMPREINTE
    for(int i = 0; i < nb_blocs; i++){  //pour chaque bloc
        printf("CALCUL EMPREINTE BLOC n.%d\n", i);
        //calcul empreinte etape 1  (SOMME ET MODULO)
        for(int j = 0; j < EMPREINTE; j++){ //pour chaque colonne
            for(int k = 0; k < EMPREINTE; k++){ //pour chaque case de la colonne (somme)
                empreinte[j] += matrices[i][k][j];
            }
            empreinte[j] = empreinte[j]%VAL_PAR_CASE;
        }

        //print
        printf("empreinte etape 1:\n");
        for(int j = 0; j < EMPREINTE; j++){ //pour chaque colonne
            printf("%d ", empreinte[j]);
        }
        printf("\n\n");

        //calcul empreinte etape 2 (DECALAGE)
        int new_matrice[EMPREINTE][EMPREINTE];
        int decalage, ajouter;
        int retenues[EMPREINTE];
        for(int j = 0; j < EMPREINTE; j++){ //pour chaque ligne //0
            decalage = j;
            ajouter = 0;
            for(int l = 0; l < EMPREINTE; l++){
                retenues[l] = 0;
            }
            for(int k = 0; k < EMPREINTE; k++){ //pour chaque case de la ligne (decalage)   //1
                if(j == 0){
                    new_matrice[j][k] = matrices[i][j][k];
                }
                else if((EMPREINTE-k) <= decalage){
                    new_matrice[j][ajouter] = matrices[i][j][k];
                    ajouter++;
                    //new_matrice[j][k] = matrices[i][j][ajouter];
                }
            }
            if(j != 0){
                while(ajouter < EMPREINTE){
                    new_matrice[j][ajouter] = matrices[i][j][ajouter-j];
                    ajouter++;
                }
            }
        }

        printf("matrice etape 2:\n");
        for(int j = 0; j < VAL_PAR_BLOC; j++){  //pour chaque element du bloc
            if(j%EMPREINTE == 0) printf("\n");
            printf("%d ", new_matrice[j/EMPREINTE%EMPREINTE][j%EMPREINTE]);
        }
        printf("\n\n");

        //calcul empreinte etape 3 (SOMME ET MODULO)
        for(int j = 0; j < EMPREINTE; j++){ //pour chaque colonne
            for(int k = 0; k < EMPREINTE; k++){ //pour chaque case de la colonne (somme)
                empreinte[j] += new_matrice[k][j];
            }
            //printf("%d ", empreinte[i][j]);
            empreinte[j] = empreinte[j]%VAL_PAR_CASE;
        }
        //printf("\n\n");

        printf("empreinte etape 3:\n");
        for(int j = 0; j < EMPREINTE; j++){ //pour chaque colonne
            printf("%d ", empreinte[j]);
        }
        printf("\n\n");
        
    }

    printf("##################################################################\n\n");
    printf("TTH^%dv%d(\"%s\") = ", EMPREINTE, VAL_PAR_CASE, argv[1]);
    for(int i = 0; i < EMPREINTE; i++){ //pour chaque colonne
        printf("[%d]", empreinte[i]);
    }
    printf("\n\n");
    printf("##################################################################\n\n");

    printf("L'empreinte du message: \n\n%s\n \navec l'algorithme Toy-Hash-Telegraph^%dv%d (travaillant sur des valeurs numérique modulo %d et dont l'empreinte résultante est constituée de %d valeurs) =\n\n", argv[1], EMPREINTE, VAL_PAR_CASE, VAL_PAR_CASE, EMPREINTE);
    for(int i = 0; i < EMPREINTE; i++){ //pour chaque colonne
        printf("[%d]", empreinte[i]);
    }
    printf("\n\n");

    return 0;
}