#include"lista.h"
#include<stdlib.h>
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include "../database/database.h"
/**
 * @brief Estrutura de nó da lista.
 */
struct lista_no {
    char info[200];              /**< Informação do nó. */
    ListaNo* proximo;       /**< Ponteiro para o próximo nó. */
};

/**
 * @brief Estrutura da lista.
 */
struct lista {
    ListaNo* primeiro;          /**< Ponteiro para o primeiro nó da lista. */
};

/**
 * @brief Cria uma nova lista vazia.
 * @return Retorna um ponteiro para a lista criada.
 *
 * Aloca memória para uma nova lista e inicializa o ponteiro do primeiro nó como NULL.
 * Retorna o ponteiro para a lista criada.
 */
Lista* lst_cria(void)
{
    Lista *l = (Lista*) malloc(sizeof(Lista));
    l->primeiro = NULL;
    return l;
}

/**
 * @brief Insere um novo nó na lista.
 * @param header Ponteiro para a lista onde o nó será inserido.
 * @param text Informação a ser armazenada no novo nó.
 *
 * Cria um novo nó e atribui a informação fornecida.
 * O novo nó é inserido no início da lista.
 * O próximo ponteiro do novo nó aponta para o nó que era o primeiro na lista.
 * O ponteiro do primeiro nó da lista é atualizado para o novo nó.
 */

void lst_insere(Lista *header, char* text)
{
    ListaNo *novo = (ListaNo*) malloc(sizeof(ListaNo));
    strcpy(novo->info, text);
    novo->proximo = header->primeiro;
    header->primeiro = novo;
}

/**
 * @brief Preenche a lista com palavras do conteúdo de um arquivo.
 * @param path Caminho do arquivo a ser lido.
 * @param header Ponteiro para a lista onde as palavras serão armazenadas.
 *
 * Lê o conteúdo do arquivo especificado.
 * Percorre o conteúdo caractere por caractere e armazena palavras separadas por quebras de linha.
 * Para cada palavra encontrada, cria um novo nó na lista e atribui a palavra como informação do nó.
 * O novo nó é inserido no início da lista.
 */

void lst_populateList(char* path, Lista* header){
    char* content = readTextFile(path);


    int i = 0;
    char palavra[100]; // Tamanho máximo da palavra definido como 100
    int j = 0;
    
    while (content[i] != '\0') {
            palavra[j] = content[i]; // Armazena o caractere na palavra
            j++;
        i++;
        if (content[i] == '\n' || content[i] == '\0') {
            palavra[j] = '\0'; // Adiciona o caractere nulo para indicar o fim da palavra
            printInfo(palavra);
            ListaNo *novo = (ListaNo*) malloc(sizeof(ListaNo));
            size_t len1 = strlen(palavra);
            strcpy(novo->info, palavra+1);
            novo->proximo = header->primeiro;
            header->primeiro = novo; // Imprime a palavra completa
            j = 0; // Reseta o índice para a próxima palavra
        } 
    }
    
}

/**
 * @brief Imprime os elementos da lista.
 * @param header Ponteiro para a lista a ser impressa.
 *
 * Percorre a lista a partir do primeiro nó e imprime a informação de cada nó.
 * A função assume que a informação dos nós é do tipo string.
 * Imprime cada informação seguida de uma quebra de linha.
 */

void lst_imprime(Lista *header)
{
    ListaNo *ptr = header->primeiro;
    while(ptr != NULL)
    {
        printf("%s\n", ptr->info);
        ptr = ptr->proximo;
    }
}

/**
 * @brief Verifica se a lista contém um determinado elemento.
 * @param header Ponteiro para a lista a ser verificada.
 * @param text Elemento a ser procurado na lista.
 * @return Retorna 1 se o elemento pertence à lista, 0 caso contrário.
 *
 * Percorre a lista a partir do primeiro nó e compara a informação de cada nó com o elemento fornecido.
 * A função assume que a informação dos nós é do tipo string.
 * Se o elemento for encontrado, retorna 1; caso contrário, retorna 0.
 */

int pertence(Lista *header, char* text)
{
    ListaNo *ptr = header->primeiro;
    while(ptr != NULL)
    {
        if (ptr->info == text)
            return 1;
        ptr = ptr->proximo;
    }

    return 0;
}

/**
 * @brief Remove um elemento da lista.
 * @param header Ponteiro para a lista da qual o elemento será removido.
 * @param text Elemento a ser removido da lista.
 *
 * Percorre a lista a partir do primeiro nó e procura o nó com a informação igual ao elemento fornecido.
 * Se o nó for encontrado, remove-o da lista.
 * Atualiza os ponteiros para manter a integridade da lista.
 * Libera a memória alocada para o nó removido.
 */

void lst_retira(Lista *header, char* text)
{
    ListaNo *ant = NULL;
    ListaNo *p = header->primeiro;

    while (p != NULL && p->info != text)
    {
        ant = p;
        p = p->proximo;
    }

    if (p != NULL) {
        if (ant == NULL)
            header->primeiro = p->proximo;
        else 
            ant->proximo = p->proximo;

        free(p);
    }
}

/**
 * @brief Verifica se a lista está vazia.
 * @param header Ponteiro para a lista a ser verificada.
 * @return Retorna 1 se a lista está vazia, 0 caso contrário.
 *
 * Verifica se o ponteiro para o primeiro nó da lista é nulo.
 * Se for nulo, a lista está vazia e a função retorna 1.
 * Caso contrário, a lista não está vazia e a função retorna 0.
 */

int lst_vazia(Lista *header)
{
    return (header->primeiro == NULL);
}

/**
 * @brief Libera a memória ocupada pela lista.
 * @param header Ponteiro para a lista a ser liberada.
 *
 * Percorre a lista a partir do primeiro nó e libera a memória ocupada por cada nó.
 * Libera a memória alocada para a estrutura da lista.
 */

void lst_libera(Lista *header)
{
    ListaNo *p = header->primeiro;
    while (p != NULL)
    {
        ListaNo *t  = p->proximo;
        free(p);
        p = t;
    }

    free(header);
}

/**
 * @brief Retorna o primeiro nó válido da lista.
 * @param header Ponteiro para a lista.
 * @return Retorna um ponteiro para o primeiro nó válido da lista.
 *
 * Verifica se o ponteiro para o primeiro nó da lista é nulo.
 * Se não for nulo, retorna o ponteiro para o primeiro nó.
 * Caso contrário, imprime uma mensagem de erro e retorna NULL.
 */

ListaNo* lst_returnNodeValid(Lista *header){
    if(header->primeiro != NULL)
    return header->primeiro;
    
    printError("lista vazia");

    return NULL;
}

/**
 * @brief Retorna o próximo nó válido da lista.
 * @param ptr Ponteiro para o nó atual.
 * @return Retorna um ponteiro para o próximo nó válido da lista.
 *
 * Verifica se o ponteiro para o nó atual é nulo.
 * Se não for nulo, retorna o ponteiro para o próximo nó.
 * Caso contrário, imprime uma mensagem de erro e retorna NULL.
 */

ListaNo* lst_nextNode(ListaNo *ptr){
    if(ptr != NULL)
    return ptr->proximo;

    printError("no invalido");

    return NULL;
}

/**
 * @brief Retorna a informação válida de um nó.
 * @param ptr Ponteiro para o nó.
 * @return Retorna a informação do nó.
 *
 * Verifica se o ponteiro para o nó é nulo.
 * Se não for nulo, retorna a informação do nó.
 * Caso contrário, retorna NULL.
 */

char* lst_infoValid(ListaNo *ptr)
{
        if(ptr != NULL)
        return ptr->info;
        
        return NULL;
         
}