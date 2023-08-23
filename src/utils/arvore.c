#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "lista.h"

// Definição da estrutura do nó da árvore
typedef struct TreeNode {
    char name[50];
    char version[50];
    char initVersion[50];
    char parentName[50];
    struct TreeNode* parent;
    struct TreeNode* firstChild;
    struct TreeNode* nextSibling;
} TreeNode;

// Definição da estrutura da lista dinâmica
typedef struct ListNode {
    TreeNode* node;
    struct ListNode* next;
} ListNode;

typedef enum boolean {
    FALSE = 0,
    TRUE,
}Boolean;

// Função para criar um novo nó da árvore
TreeNode* createNode(const char* name, const char* initVersion, const char* version, const char* parentName) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    strcpy(newNode->name, name);
    strcpy(newNode->initVersion, initVersion);
    strcpy(newNode->version, version);
    strcpy(newNode->parentName, parentName);
    newNode->parent = NULL;
    newNode->firstChild = NULL;
    newNode->nextSibling = NULL;
    return newNode;
}

// Função para criar um novo nó da lista dinâmica
ListNode* createListNode(TreeNode* node) {
    ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
    newNode->node = node;
    newNode->next = NULL;
    return newNode;
}

// Função para adicionar um nó à lista dinâmica
void addListNode(ListNode** head, TreeNode* node) {
    ListNode* newNode = createListNode(node);
    if (*head == NULL) {
        *head = newNode;
    } else {
        ListNode* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}
TreeNode*  findNodeInListByInitVersion(ListNode* head, const char* version){
    ListNode* current = head;
    while (current != NULL) {
        if (strcmp(current->node->initVersion, version) == 0) {
            return current->node;
        }
        current = current->next;
    }
    return NULL;
}
// Função para buscar um nó na lista dinâmica
TreeNode* findNodeInList(ListNode* head, const char* name) {
    ListNode* current = head;
    while (current != NULL) {
        if (strcmp(current->node->name, name) == 0) {
            return current->node;
        }
        current = current->next;
    }
    return NULL;
}

// Função para adicionar um nó filho a um nó pai na árvore
void addChild(TreeNode* parent, TreeNode* child) {
    child->parent = parent;
    if (parent->firstChild == NULL)
        parent->firstChild = child;
    else {
        TreeNode* sibling = parent->firstChild;
        while (sibling->nextSibling != NULL)
            sibling = sibling->nextSibling;
        sibling->nextSibling = child;
    }
}
void printTreeContent(TreeNode* node, int depth){
    if (node == NULL)
        return;
    for (int i = 0; i < depth; i++)
        printf("  ");
    if(!(strcmp(node->initVersion, "") == 0)){
        printf("No da Branch %s (%s)\n", node->name, node->initVersion);
    }else{
        char line[200];
        printf("Branch %s -> vercao %s\n", node->name, node->version);
        sprintf(line, ".versionador/content/%s/%s", node->name, node->version);

    Lista *header = lst_cria();
    searchDirectoryFiles(line, header);
    ListaNo* ptr = lst_returnNodeValid(header);

    
    if(ptr == NULL){
    return;
    }else{
    

    char* fileOrigin = lst_infoValid(ptr);
    char copySec[200];
    while (fileOrigin != NULL)
    {   
        printf("\n");
        for (int i = 0; i < depth; i++)
        printf("  ");
        sprintf(copySec, ".versionador/content/%s/%s/%s", node->name, node->version, fileOrigin);
        printf("conteudo: %s\n", readTextFile(copySec));
        ptr = lst_nextNode(ptr);
        fileOrigin = lst_infoValid(ptr);
    }
    free(fileOrigin);
    
    }
    }
    printTreeContent(node->firstChild, depth + 3);
    printTreeContent(node->nextSibling, depth);
}

// Função para imprimir a árvore
void printTree(TreeNode* node, int depth) {
    if (node == NULL)
        return;
    for (int i = 0; i < depth; i++)
        printf("  ");
    if(!(strcmp(node->initVersion, "") == 0)){
        printf("No da Branch %s (%s)\n", node->name, node->initVersion);
    }else{
        printf("Branch %s -> vercao %s\n", node->name, node->version);
    }
    printTree(node->firstChild, depth + 3);
    printTree(node->nextSibling, depth);
}

// Função para liberar a memória alocada pela árvore
void freeTree(TreeNode* node) {
    if (node == NULL)
        return;
    freeTree(node->firstChild);
    freeTree(node->nextSibling);
    free(node);
}
void addVersionsToList(const char* fileName, ListNode** listHead, const char* nodeName) {
    FILE* file = fopen(fileName, "r");
    if (file != NULL) {
        TreeNode* node = findNodeInList(*listHead, nodeName);
        if (node != NULL) {
            char line[200];
            char version[50];
            fgets(line, sizeof(line), file); //linha lixo
            while (fgets(line, sizeof(line), file) != NULL) {
                sscanf(line, "%s", version);
                TreeNode* newNode = createNode(nodeName, "", version, "");
                TreeNode* nodeExist = findNodeInListByInitVersion(*listHead, version);

                if (nodeExist != NULL && !strcmp(nodeExist->name, nodeName) == 0) {
                    addChild(node, newNode);
                    addChild(newNode, nodeExist);
                } else {
                    addChild(node, newNode);
                }
            }
        }
        fclose(file);
    }
}

void readMetadataFile(const char* fileName, ListNode** listHead, TreeNode** root) {
    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de metadados.\n");
        return;
    }

    char line[200];
    char name[50];
    char initVersion[50];
    char parentName[50];

    while (fgets(line, sizeof(line), file) != NULL) {
        if (strcmp(line, "<INICIO>\n") == 0) {
            fgets(line, sizeof(line), file);  // Ler o nome da branch
            sscanf(line, "%s", name);
            fgets(line, sizeof(line), file);  // Ler a versão da branch
            sscanf(line, "%s", initVersion);
            fgets(line, sizeof(line), file);  // Ler o nome do pai da branch
            sscanf(line, "%s", parentName);

            TreeNode* newNode = createNode(name, initVersion, "", parentName);
            addListNode(listHead, newNode);

            TreeNode* node = findNodeInList(*listHead, "principal");
            if (*root == NULL && node != NULL) {
                *root = newNode;
            }
        }
    }

    fclose(file);
}

void freeList(ListNode* listHead) {
    ListNode* current = listHead;
    while (current != NULL) {
        ListNode* next = current->next;
        free(current);
        current = next;
    }
}


void addVersionsToListAndConcatenate(ListNode** head, const char* name) {
    char filename[200];
    sprintf(filename, ".versionador/versions%s.txt", name);
    addVersionsToList(filename, head, name);
}


TreeNode* createTree(const char* fileName){
    ListNode* listHead = NULL;
    TreeNode* root = NULL;

    readMetadataFile(fileName, &listHead, &root);

    ListNode* current = listHead;
    while (current != NULL) {
        addVersionsToListAndConcatenate(&listHead, current->node->name);
        current = current->next;
    }

    freeList(listHead);
    return root;
}

void searchVersion(TreeNode* node, const char* version,  char* name, char* versionName, char* file) {
    if (node == NULL)
        return;

    if (strcmp(node->version, version) == 0) {
        printf("Versão encontrada!\n");
        printf("Nome do nó: %s\n", node->name);
        printf("Versão: %s\n", node->version);
        strcpy(name, node->name);
        strcpy(versionName, node->version);
        

        char line[200];
    sprintf(line, ".versionador/content/%s/%s", node->name, node->version);
    Lista *header = lst_cria();
    searchDirectoryFiles(line, header);
    ListaNo* ptr = lst_returnNodeValid(header);

    
    if(ptr == NULL){
    return;
    }else{
    

    char* fileOrigin = lst_infoValid(ptr);
    printInfo(fileOrigin);
    strcpy(file, fileOrigin);

    free(fileOrigin);
    }
    }


    // Recursivamente buscar na subárvore do primeiro filho
    searchVersion(node->firstChild, version, name,  versionName, file );

    // Recursivamente buscar na subárvore do próximo irmão
    searchVersion(node->nextSibling, version ,name,  versionName, file);
}

void printBranchByName(TreeNode* node, const char* name, const char* versionStop) {

    if (node == NULL)
        return;
    
    if (strcmp(node->name, name) == 0) {
        // Imprimir o nó atual e seus descendentes
        if(versionStop != NULL){
        if((strcmp(node->version, versionStop ) == 0)){
            printf("%s %s\n", node->name, node->version);
            return;
        }
        }

        if (!(strcmp(node->version, "") == 0))
        printf("%s %s\n", node->name, node->version);
        // printTree(node->firstChild, depth + 1);
    }

    // Buscar no próximo irmão
    printBranchByName(node->firstChild, name, versionStop);
    printBranchByName(node->nextSibling, name, versionStop);
}
void treeBranchRecursiv(char* branch, TreeNode* root, char* proxBranch, ListNode* listnodeRoot, char* versionStop){
    ListNode* listnodeRootCopy = listnodeRoot;
        
        if(proxBranch == NULL){
            printf("branch inexistente");
            return;
        }

        

        if(strcmp(proxBranch, "Nenhum") == 0){
            return;
        }

        while(!(strcmp(listnodeRootCopy->node->name, proxBranch) == 0)){
            listnodeRootCopy = listnodeRootCopy->next;
        }

        treeBranchRecursiv(branch, root, listnodeRootCopy->node->parentName, listnodeRoot, listnodeRootCopy->node->initVersion);

        if(strcmp(proxBranch, branch) == 0){
            printBranchByName(root, branch , NULL);
        }else{
            printBranchByName(root, proxBranch , versionStop);
        }
}
void treeBranch(char* branch, TreeNode* root){
    ListNode* listnodeRoot =NULL;
    TreeNode* nodeRoot = NULL;
    readMetadataFile("dados.txt",&listnodeRoot, &nodeRoot);
    char* proxBranch = (char*)malloc(sizeof(char));
    strcpy(proxBranch, branch);

    char* versionStop = (char*)malloc(sizeof(char));
    treeBranchRecursiv(branch, root, proxBranch, listnodeRoot, versionStop);
    
    freeList(listnodeRoot);
    freeTree(nodeRoot);
    free(proxBranch);
    free(versionStop);
        
}

int branchExistsInTree(char* branch){
    ListNode* listnodeRoot =NULL;
    TreeNode* nodeRoot = NULL;
    readMetadataFile(".versionador/content/dados.txt",&listnodeRoot, &nodeRoot);
    while (listnodeRoot != NULL)
    {
        if(strcmp(listnodeRoot->node->name, branch)==0){
            freeList(listnodeRoot);
            freeTree(nodeRoot);
            return 1;
        }
        listnodeRoot = listnodeRoot->next;
        
    }

    freeList(listnodeRoot);
    freeTree(nodeRoot);
    return 0;
    
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void renameBranchFromFile( const char* currentBranchName, const char* newBranchName) {
    
    // Abrir o arquivo em modo de leitura
    char* fileName = ".versionador/content/dados.txt";
    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    // Criar um arquivo temporário para escrever as informações atualizadas
    FILE* tempFile = fopen(".versionador/content/temp.txt", "w");
    if (tempFile == NULL) {
        printf("Erro ao criar o arquivo temporário.\n");
        fclose(file);
        return;
    }

    char line[200];

    // Ler o arquivo linha por linha
    while (fgets(line, sizeof(line), file) != NULL) {
        // Verificar se a linha contém "<INICIO>"
        line[strcspn(line, "\n")] = '\0';
        if(strcmp(line, currentBranchName)==0)
        strcpy(line, newBranchName);
        sprintf(line, "%s\n", line);
        fputs(line, tempFile);
    }

    // Fechar os arquivos
    fclose(file);
    fclose(tempFile);

    // Remover o arquivo original
    if (remove(fileName) != 0) {
        printf("Erro ao remover o arquivo original.\n");
        return;
    }

    // Renomear o arquivo temporário para o nome original
    if (rename(".versionador/content/temp.txt", fileName) != 0) {
        printf("Erro ao renomear o arquivo temporário.\n");
        return;
    }

    printf("Renomeação da ramificação concluída com sucesso.\n");
}

void removeBranchFromFileReturnChildren(const char* branchName, Lista* root) {
    
    int num = 0;
    char* fileName = ".versionador/content/dados.txt";
    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        return;
    }

    // Criar um arquivo temporário para escrever as informações atualizadas
    writeTextFile(".versionador/content/temp.txt", "");

    FILE* tempFile = fopen(".versionador/content/temp.txt", "a");
    if (tempFile == NULL) {
        printf("Erro ao criar o arquivo temporário.\n");
        return;
    }

    char line[200];
    char line1[200];
    char line2[200];
    Boolean escreve = TRUE;
    // Ler o arquivo linha por linha
    while (fgets(line, sizeof(line), file) != NULL) {
        // Verificar se a linha contém "<INICIO>"
        if (strcmp(line, "<INICIO>\n") == 0) {
            // Ler as informações da ramificação
            fgets(line, sizeof(line), file);
            fgets(line1, sizeof(line1), file);  // Ler o nome da branch
            fgets(line2, sizeof(line2), file);
            line[strcspn(line, "\n")] = '\0';
            line1[strcspn(line1, "\n")] = '\0';
            line2[strcspn(line2, "\n")] = '\0';  // Remover a quebra de linha
            printInfo("oi");
            if(strcmp(line2, branchName) == 0){
                char lineCop[200];
                strcpy(lineCop, line);
                lst_insere(root, lineCop);
            }else if(strcmp(line, branchName) == 0){
                char concat[600];
                sprintf(concat, "<INICIO>\n%s\n%s\n<FIM>", line2, line1);
                writeTextFile(".versionador/atual.txt", concat);
            }else{
                fputs("<INICIO>\n", tempFile);
                fputs(line, tempFile);
                fputs("\n", tempFile);
                fputs(line1, tempFile);
                fputs("\n", tempFile);
                fputs(line2, tempFile);
                fputs("\n", tempFile);
                fputs("<FIM>\n", tempFile);
            }
        }

    }

    // Fechar os arquivos
    fclose(file);
    fclose(tempFile);
    
    // Remover o arquivo original
    if (remove(fileName) != 0) {
        printf("Erro ao remover o arquivo original.\n");
        return;
    }

    // Renomear o arquivo temporário para o nome original
    if (rename(".versionador/content/temp.txt", fileName) != 0) {
        printf("Erro ao renomear o arquivo temporário.\n");
        return;
    }


    printf("Remoção da ramificação concluída com sucesso.\n");
    return;
}

