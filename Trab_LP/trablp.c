#include <stdio.h>
#include <stdlib.h>

typedef struct {
    void* end;  // Endereco alocado
    int cont;   // Contagem de ponteiros apontando para o endereco
} ContatorReferencia;

static ContatorReferencia* ref = NULL;    // Vetor para rastrear as alocacoes
static int ContatorRefTam = 0;            // Tamanho do vetor


void* malloc2(size_t size) {	// Funcao para alocar memoria com rastreamento de referencias
    void* end = malloc(size);   // Aloca memoria usando o malloc tradicional

    if (end == NULL) {
        fprintf(stderr, "Erro: Falha na alocacao de memoria.\n");
        exit(1);
    }

    // Incrementa a contagem de referencias
    ContatorReferencia* temp = realloc(ref, sizeof(ContatorReferencia) * (ContatorRefTam + 1));
    if (temp == NULL) {
        fprintf(stderr, "Erro: Falha na alocacao de memoria.\n");
        exit(1);
    }

    ref = temp;
    ref[ContatorRefTam].end = end;
    ref[ContatorRefTam].cont = 1;
    ContatorRefTam++;

    return end;
}

void atrib2(void** dest, void* src) {	// Funcao para atribuir um novo valor a um ponteiro, atualizando as contagens de referencias
    if (dest == NULL) {
        fprintf(stderr, "Erro: O destino e um ponteiro nulo.\n");
        exit(1);
    }

    if (*dest != NULL) {	// Decrementa a contagem de referencias do ponteiro de destino atual
        int i;
        for (i = 0; i < ContatorRefTam; i++) {
            if (ref[i].end == *dest) {
                ref[i].cont--;

                
                if (ref[i].cont == 0) { 	// Verifica se a contagem de referencias chegou a zero
                    free(*dest);
                    ref[i] = ref[ContatorRefTam - 1];    // Substitui pelo ultimo elemento
                    ContatorRefTam--;
                    ref = realloc(ref, sizeof(ContatorReferencia) * ContatorRefTam);    // Reduz o tamanho do vetor
                }

                break;
            }
        }
    }

    *dest = src;	// Atribui o novo ponteiro de origem

    if (src != NULL) {	// Incrementa a contagem de referencias do novo ponteiro de origem
        int i;
        for (i = 0; i < ContatorRefTam; i++) {
            if (ref[i].end == src) {
                ref[i].cont++;
                break;
            }
        }
    }
}


void Exibecont() { 	// Funcao para exibir a contagem de referencias de cada endereco alocado
    for (int i = 0; i < ContatorRefTam; i++) {
        printf("Contador de referencias: %p = %d\n", ref[i].end, ref[i].cont);
    }
}

int main() {
    // Alocacao de memoria 
    int* ptr1 = (int*)malloc2(sizeof(int));
    *ptr1 = 1;

    int* ptr2 = (int*)malloc2(sizeof(int));
    *ptr2 = 2;

    int* ptr3 = (int*)malloc2(sizeof(int));
    *ptr3 = 3;
	
	//Atribuicao de ponteiros 
    int* ptr4 = NULL;
    atrib2((void**)&ptr4, ptr1);

    int* ptr5 = NULL;
    atrib2((void**)&ptr5, ptr2);

    int* ptr6 = NULL;
    atrib2((void**)&ptr6, ptr3);



    printf("\nColetor automatico de lixo:\n\n");
    printf("ptr4: %d\n", *ptr4);
    printf("ptr5: %d\n", *ptr5);
    printf("ptr6: %d\n\n", *ptr6);
    Exibecont();    // Exibe o contador de referencias

  	//Atribuicao de ponteiros (acrscendo e decrescendo o contador) 
    atrib2((void**)&ptr4, ptr2);
    atrib2((void**)&ptr5, ptr3);
    atrib2((void**)&ptr6, ptr3);

  
    printf("\n\nColetor automatico de lixo 2 instancia:\n\n");
    printf("ptr4: %d\n", *ptr4);
    printf("ptr5: %d\n", *ptr5);
    printf("ptr6: %d\n\n", *ptr6);
    Exibecont();    // Exibe o contador de referencias

  //Atribuicao de ponteiros (eliminando o espaco alocado no ptr1 por nao ter mais nada apontando pra la) 
    atrib2((void**)&ptr4, ptr2);
    atrib2((void**)&ptr5, ptr3);
    atrib2((void**)&ptr6, ptr3);
    atrib2((void**)&ptr1, ptr2);
    atrib2((void**)&ptr2, ptr3);

  
    printf("\n\nColetor automatico de lixo 3 instancia:\n\n");
    printf("ptr4: %d\n", *ptr4);
    printf("ptr5: %d\n", *ptr5);
    printf("ptr6: %d\n\n", *ptr6);
    Exibecont();    // Exibe o contador de referencias
  
    return 0;
}
