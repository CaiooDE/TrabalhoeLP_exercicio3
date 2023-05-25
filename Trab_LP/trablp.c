#include <stdio.h>
#include <stdlib.h>

typedef struct {
    void* end;  // Endere�o alocado
    int cont;   // Contagem de ponteiros apontando para o endere�o
} ContatorReferencia;

static ContatorReferencia* ref = NULL;    // Vetor para rastrear as aloca��es
static int ContatorRefTam = 0;            // Tamanho do vetor


void* malloc2(size_t size) {	// Fun��o para alocar mem�ria com rastreamento de refer�ncias
    void* end = malloc(size);   // Aloca mem�ria usando o malloc tradicional

    if (end == NULL) {
        fprintf(stderr, "Erro: Falha na aloca��o de mem�ria.\n");
        exit(1);
    }

    // Incrementa a contagem de refer�ncias
    ContatorReferencia* temp = realloc(ref, sizeof(ContatorReferencia) * (ContatorRefTam + 1));
    if (temp == NULL) {
        fprintf(stderr, "Erro: Falha na aloca��o de mem�ria.\n");
        exit(1);
    }

    ref = temp;
    ref[ContatorRefTam].end = end;
    ref[ContatorRefTam].cont = 1;
    ContatorRefTam++;

    return end;
}

void atrib2(void** dest, void* src) {	// Fun��o para atribuir um novo valor a um ponteiro, atualizando as contagens de refer�ncias
    if (dest == NULL) {
        fprintf(stderr, "Erro: O destino � um ponteiro nulo.\n");
        exit(1);
    }

    if (*dest != NULL) {	// Decrementa a contagem de refer�ncias do ponteiro de destino atual
        int i;
        for (i = 0; i < ContatorRefTam; i++) {
            if (ref[i].end == *dest) {
                ref[i].cont--;

                
                if (ref[i].cont == 0) { 	// Verifica se a contagem de refer�ncias chegou a zero
                    free(*dest);
                    ref[i] = ref[ContatorRefTam - 1];    // Substitui pelo �ltimo elemento
                    ContatorRefTam--;
                    ref = realloc(ref, sizeof(ContatorReferencia) * ContatorRefTam);    // Reduz o tamanho do vetor
                }

                break;
            }
        }
    }

    *dest = src;	// Atribui o novo ponteiro de origem

    if (src != NULL) {	// Incrementa a contagem de refer�ncias do novo ponteiro de origem
        int i;
        for (i = 0; i < ContatorRefTam; i++) {
            if (ref[i].end == src) {
                ref[i].cont++;
                break;
            }
        }
    }
}


void Exibecont() { 	// Fun��o para exibir a contagem de refer�ncias de cada endere�o alocado
    for (int i = 0; i < ContatorRefTam; i++) {
        printf("Contador de referencias: %p = %d\n", ref[i].end, ref[i].cont);
    }
}

int main() {
    // Aloca��o de mem�ria 
    int* ptr1 = (int*)malloc2(sizeof(int));
    *ptr1 = 10;

    int* ptr2 = (int*)malloc2(sizeof(int));
    *ptr2 = 40;

    int* ptr3 = (int*)malloc2(sizeof(int));
    *ptr3 = 50;
	
	//Atribuicao de ponteiros 
    int* ptr4 = NULL;
    atrib2((void**)&ptr4, ptr1);

    int* ptr5 = NULL;
    atrib2((void**)&ptr5, ptr1);

    int* ptr6 = NULL;
    atrib2((void**)&ptr6, ptr1);

    int* ptr7 = NULL;
    atrib2((void**)&ptr7, ptr1);

    atrib2((void**)&ptr2, ptr1);

    printf("Coletor automatico de lixo:\n");
    printf("ptr1: %d\n", *ptr1);
    printf("ptr2: %d\n", *ptr2);
    printf("ptr3: %d\n", *ptr3);
    printf("ptr4: %d\n", *ptr4);
    printf("ptr5: %d\n", *ptr5);
    printf("ptr6: %d\n", *ptr6);
    printf("ptr7: %d\n", *ptr7);
    Exibecont();    // Exibe o contador de refer�ncias

    return 0;
}

