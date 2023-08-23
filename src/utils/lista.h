#ifndef LISTA_H_
#define LISTA_H_

typedef struct lista_no ListaNo;
typedef struct lista Lista;

// Interface das funções
Lista* lst_cria(void);
void lst_insere(Lista *header, char* text);
void lst_imprime(Lista *header);
int pertence (Lista *header, char* text);
void lst_retira(Lista *header, char* text);
int lst_vazia(Lista *header);
void lst_libera(Lista *header);
void lst_populateList(char* path, Lista* header);
ListaNo* lst_returnNodeValid(Lista *header);
ListaNo* lst_nextNode(ListaNo *ptr);
char* lst_infoValid(ListaNo *ptr);

#endif