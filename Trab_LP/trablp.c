#include <stdio.h>
#include <stdlib.h>

typedef struct {
    void* end;  // Endereço alocado
    int cont;   // Contagem de ponteiros apontando para o endereço
} ContatorReferencia;

static ContatorReferencia* ref = NULL;    // Vetor para rastrear as alocações
static int ContatorRefTam = 0;            // Tamanho do vetor


void* malloc2(size_t size) {	// Função para alocar memória com rastreamento de referências
    void* end = malloc(size);   // Aloca memória usando o malloc tradicional

    if (end == NULL) {
        fprintf(stderr, "Erro: Falha na alocação de memória.\n");
        exit(1);
    }

    // Incrementa a contagem de referências
    ContatorReferencia* temp = realloc(ref, sizeof(ContatorReferencia) * (ContatorRefTam + 1));
    if (temp == NULL) {
        fprintf(stderr, "Erro: Falha na alocação de memória.\n");
        exit(1);
    }

    ref = temp;
    ref[ContatorRefTam].end = end;
    ref[ContatorRefTam].cont = 1;
    ContatorRefTam++;

    return end;
}

void atrib2(void** dest, void* src) {	// Função para atribuir um novo valor a um ponteiro, atualizando as contagens de referências
    if (dest == NULL) {
        fprintf(stderr, "Erro: O destino é um ponteiro nulo.\n");
        exit(1);
    }

    if (*dest != NULL) {	// Decrementa a contagem de referências do ponteiro de destino atual
        int i;
        for (i = 0; i < ContatorRefTam; i++) {
            if (ref[i].end == *dest) {
                ref[i].cont--;

                
                if (ref[i].cont == 0) { 	// Verifica se a contagem de referências chegou a zero
                    free(*dest);
                    ref[i] = ref[ContatorRefTam - 1];    // Substitui pelo último elemento
                    ContatorRefTam--;
                    ref = realloc(ref, sizeof(ContatorReferencia) * ContatorRefTam);    // Reduz o tamanho do vetor
                }

                break;
            }
        }
    }

    *dest = src;	// Atribui o novo ponteiro de origem

    if (src != NULL) {	// Incrementa a contagem de referências do novo ponteiro de origem
        int i;
        for (i = 0; i < ContatorRefTam; i++) {
            if (ref[i].end == src) {
                ref[i].cont++;
                break;
            }
        }
    }
}


void Exibecont() { 	// Função para exibir a contagem de referências de cada endereço alocado
    for (int i = 0; i < ContatorRefTam; i++) {
        printf("Contador de referencias: %p = %d\n", ref[i].end, ref[i].cont);
    }
}

int main() {
    // Alocação de memória 
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
    Exibecont();    // Exibe o contador de referências

    return 0;
}

