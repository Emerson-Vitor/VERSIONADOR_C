#define _DEFAULT_SOURCE
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include "../database/database.h"
#include <sys/stat.h>
#include <unistd.h>
#include "lista.h"

struct Raiz {
    char nome[100];
    char initVersion[100];
    char initParent[100];
};

/**
 * @brief Imprime uma mensagem de erro.
 * @param message Mensagem de erro a ser exibida.
 *
 * Imprime uma mensagem de erro formatada, precedida pela tag [ERROR].
 */
void printError(const char* message) {
    printf("[ERROR] %s\n", message);
}

/**
 * @brief Imprime uma mensagem de informação.
 * @param message Mensagem de informação a ser exibida.
 *
 * Imprime uma mensagem de informação formatada, precedida pela tag [INFO].
 */
void printInfo(const char* message) {
    printf("[INFO] %s\n", message);
}

/**
 * @brief Cria um diretório.
 * @param path Caminho do diretório a ser criado.
 *
 * Cria um diretório utilizando a função `mkdir` com as permissões 0777.
 * Se a criação falhar, imprime uma mensagem de erro e encerra o programa.
 */
void createDirectory(const char* path) {
    int result = mkdir(path, 0777);
    if (result != 0) {
        printError("Failed to create directory.");
        exit(1);
    }
}

/**
 * @brief Conta o número de linhas em um arquivo.
 * @param path Caminho do arquivo a ser contado.
 * @return O número de linhas no arquivo.
 *
 * Abre o arquivo especificado em modo leitura e conta o número de quebras de linha
 * para determinar o número de linhas no arquivo. Retorna o total de linhas encontradas.
 */
int contLines(char* path){
     // nome e local do arquivo que será aberto para
  // obtermos a quantidade de linhas
  FILE *arquivo = fopen(path, "r");
  
  
  int quant_linhas = 0;
  int caractere, existe_linhas = 0;
   
  while((caractere = fgetc(arquivo)) != EOF){
    existe_linhas = 1; // há conteúdo no arquivo
     
    if(caractere == '\n'){ // é uma quebra de linha?
      // vamos incrementar a quantidade de linhas
      quant_linhas++;             
    } 
  }
 
  // se não houver uma quebra de linha na última linha
  // a contagem será sempre um a menos. Assim, é melhor
  // incrementar quant_linhas mais uma vez
  if(existe_linhas)
    quant_linhas++;

    return quant_linhas;
}

/**
 * @brief Concatena dois caminhos em uma única string.
 * @param path1 Primeiro caminho.
 * @param path2 Segundo caminho.
 * @return O caminho combinado resultante da concatenação.
 *
 * Calcula o tamanho necessário para a nova string combinada, aloca memória suficiente
 * e concatena os dois caminhos fornecidos, adicionando uma barra "/" entre eles.
 * Retorna o caminho combinado resultante.
 */
char* concatenatePaths(const char* path1, const char* path2) {
    size_t len1 = strlen(path1);
    size_t len2 = strlen(path2);
    char* combined = (char*)malloc(len1 + len2 + 2);
    strcpy(combined, path1);
    strcat(combined, "/");
    strcat(combined, path2);
    return combined;
}

/**
 * @brief Lê o conteúdo de um arquivo de texto.
 * @param name Nome do arquivo a ser lido.
 * @return O conteúdo do arquivo lido como uma string.
 *
 * Lê o conteúdo de um arquivo de texto com o nome especificado.
 * Primeiro, armazena o nome do arquivo em uma variável local.
 * Em seguida, tenta abrir o arquivo em modo leitura.
 * Se a abertura falhar, imprime uma mensagem de erro e retorna NULL.
 * Calcula o tamanho do arquivo, aloca memória suficiente e lê o conteúdo do arquivo para a string.
 * Adiciona um caractere nulo ao final da string e fecha o arquivo.
 * Retorna o conteúdo do arquivo como uma string.
 */
char* readTextFile(const char* name) {
    char filename[100];
    int i = 0;
    int j = 0;
   
   while (name[i] != '\0') {
            filename[j] = name[i]; // Armazena o caractere na palavra
            j++;
            i++;
    }
    filename[j] = '\0';

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printError("Failed to open file for reading.");
        return NULL;
    }
    
    fseek(file, 0, SEEK_END);
    long fileLength = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* content = (char*)malloc(fileLength + 1);
    fread(content, 1, fileLength, file);
    content[fileLength] = '\0';

    fclose(file);
    return content;
    
}

/**
 * @brief Escreve o conteúdo em um arquivo de texto na próxima linha.
 * @param name Nome do arquivo onde o conteúdo será escrito.
 * @param text Conteúdo a ser escrito no arquivo.
 *
 * Escreve o conteúdo fornecido em um arquivo de texto com o nome especificado.
 * Primeiro, armazena o nome do arquivo em uma variável local.
 * Em seguida, tenta abrir o arquivo em modo anexo (append).
 * Se a abertura falhar, imprime uma mensagem de erro e retorna.
 * Insere uma quebra de linha no arquivo e escreve o texto fornecido.
 * Fecha o arquivo após a escrita.
 */
void writeTextFileNextLine(const char* name, const char* text){
    char filename[100];
    int i = 0;
    int j = 0;
   
   while (name[i] != '\0') {
            filename[j] = name[i]; // Armazena o caractere na palavra
            j++;
            i++;
    }
    filename[j] = '\0';
    printInfo(filename);

    FILE* file = fopen(filename, "a");
    if (file == NULL) {
        printError("Failed to open file for reading.");
        return;
    }
    fputc('\n', file);
    fputs(text, file);
    fclose(file);
}

/**
 * @brief Escreve o conteúdo em um arquivo de texto.
 * @param filename Nome do arquivo onde o conteúdo será escrito.
 * @param content Conteúdo a ser escrito no arquivo.
 *
 * Escreve o conteúdo fornecido em um arquivo de texto com o nome especificado.
 * Primeiro, copia o nome do arquivo para uma nova variável local.
 * Em seguida, tenta abrir o arquivo em modo escrita.
 * Se a abertura falhar, imprime uma mensagem de erro e retorna.
 * Escreve o conteúdo no arquivo e fecha-o após a escrita.
 */

void writeTextFile(const char* filename, const char* content) {
    
    char fileName[100];
    strcpy(fileName, filename);

    FILE* file = fopen(fileName, "w");
    if (file == NULL) {
        printError("Failed to open file for writing.");
        return;
    }

    fputs(content, file);
    fclose(file);
}

/**
 * @brief Verifica se um arquivo existe.
 * @param filename Nome do arquivo a ser verificado.
 * @return Retorna 1 se o arquivo existe, caso contrário retorna 0.
 *
 * Verifica se um arquivo com o nome especificado existe.
 * Tenta abrir o arquivo em modo leitura.
 * Se o arquivo for aberto com sucesso, fecha-o e retorna 1 indicando que o arquivo existe.
 * Caso contrário, retorna 0 indicando que o arquivo não existe.
 */

int fileExists(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file != NULL) {
        fclose(file);
        return 1;
    }
    return 0;
}

/**
 * @brief Gera um identificador único.
 * @return Retorna o identificador único gerado.
 *
 * Gera um identificador único baseado na data e hora atual.
 * Aloca memória suficiente para armazenar o identificador.
 * Obtém a data e hora atual e formata a string do identificador no formato "%Y%m%d%H%M%S".
 * Retorna o identificador único gerado.
 */

char* generateUniqueIdentifier() {
    char* identifier = (char*)malloc((MAX_IDENTIFIER_LENGTH + 1) * sizeof(char));

    // Gerar um identificador único baseado na data e hora atual
    time_t currentTime = time(NULL);
    struct tm* timeInfo = localtime(&currentTime);
    strftime(identifier, MAX_IDENTIFIER_LENGTH, "%Y%m%d%H%M%S", timeInfo);

    return identifier;
}

/**
 * @brief Obtém o identificador da versão correspondente ao índice fornecido.
 * @param versionIndex O índice da versão.
 * @return Retorna o identificador da versão correspondente, ou NULL se o índice for inválido.
 *
 * Verifica se o índice da versão fornecido é válido.
 * Se o índice estiver fora dos limites do array de versões, retorna NULL.
 * Caso contrário, aloca memória para armazenar o identificador da versão.
 * Copia o identificador da versão correspondente para a variável "identifier".
 * Retorna o identificador da versão.
 */

const char* getVersionIdentifier(int versionIndex) {
    // Verificar se o índice da versão é válido
    if (versionIndex < 0 || versionIndex >= numVersions) {
        return NULL;
    }
    
    // Obter o identificador da versão correspondente
    char* identifier = (char*)malloc(sizeof(char) * MAX_IDENTIFIER_LENGTH);
    if (identifier == NULL) {
        return NULL;
    }
    
    // Copiar o identificador da versão para a variável "identifier"
    strncpy(identifier, versions[versionIndex].identifier, MAX_IDENTIFIER_LENGTH);
    
    return identifier;
}

/**
 * @brief Remove um arquivo.
 * @param file Nome do arquivo a ser removido.
 *
 * Remove o arquivo com o nome especificado.
 * Verifica se o arquivo existe usando a função access().
 * Se o arquivo não existir, imprime uma mensagem de erro e retorna.
 * Caso contrário, remove o arquivo usando a função remove().
 */

void removeFile(char* file) {

    char filE[100];
    int i = 0;
    int j = 0;
   
   while (file[i] != '\0') {
            filE[j] = file[i]; // Armazena o caractere na palavra
            j++;
            i++;
    }
    filE[j] = '\0';
    // Verificar se o arquivo existe
    if (access(filE, F_OK) != 0) {
        printf("O arquivo %s não existe.\n", filE);
        return;
    }

    // Remover o arquivo
    int result = remove(filE);

}

/**
 * @brief Pesquisa os arquivos em um diretório.
 * @param path Caminho do diretório a ser pesquisado.
 * @param header Ponteiro para a lista onde os nomes dos arquivos serão armazenados.
 *
 * Abre o diretório especificado.
 * Se o diretório não puder ser aberto, imprime uma mensagem de erro e retorna.
 * Lê os arquivos no diretório um por um.
 * Para cada arquivo encontrado que não seja um diretório, insere o nome na lista especificada.
 * Fecha o diretório após a leitura.
 */

void searchDirectoryFiles(char* path, Lista* header){
        // Abrir a pasta de conteúdo
        char pathe[200];
        strcpy(pathe, path);
        
        DIR* contentDir = opendir(pathe);
        if (contentDir == NULL) {
            printError("Failed to open content directory.");
            return;
        }

        // Ler os arquivos na pasta de conteúdo
        struct dirent* entry;
        while ((entry = readdir(contentDir)) != NULL) {
            if (entry->d_type == DT_REG) {  
                printInfo(entry->d_name);
                 lst_insere(header, entry->d_name);
            }
        }

        closedir(contentDir);
}

void salvarStruct(const struct Raiz* raiz, const char* arquivo) {
    FILE* file = fopen(arquivo, "a");

    if (file != NULL) {
        fprintf(file, "<INICIO>\n");
        fprintf(file, "%s\n", raiz->nome);
        fprintf(file, "%s\n", raiz->initVersion);
        fprintf(file, "%s\n", raiz->initParent);
        fprintf(file, "<FIM>\n");

        fclose(file);
        printf("Struct salva com sucesso.\n");
    } else {
        printf("Erro ao abrir o arquivo para salvar a struct.\n");
    }
}

void lerStructs(const char* arquivo) {
    FILE* file = fopen(arquivo, "r");

    if (file != NULL) {
        char linha[100];
        struct Raiz raiz;

        while (fgets(linha, sizeof(linha), file) != NULL) {
            if (strcmp(linha, "<INICIO>\n") == 0) {
                fgets(raiz.nome, sizeof(raiz.nome), file);
                fgets(raiz.initVersion, sizeof(raiz.initVersion), file);
                fgets(raiz.initParent, sizeof(raiz.initParent), file);

                // Removendo o caractere de nova linha (\n) no final de cada string
                raiz.nome[strcspn(raiz.nome, "\n")] = '\0';
                raiz.initVersion[strcspn(raiz.initVersion, "\n")] = '\0';
                raiz.initParent[strcspn(raiz.initParent, "\n")] = '\0';

                printf("Nome: %s\n", raiz.nome);
                printf("Versão Inicial: %s\n", raiz.initVersion);
                printf("Pai Inicial: %s\n", raiz.initParent);
            }
        }

        fclose(file);
    } else {
        printf("Erro ao abrir o arquivo para ler as structs.\n");
    }
}

void fillNode(char* name, char* initVersion, char* initParent, char* file){
    struct Raiz raiz;
    strcpy(raiz.nome, name);
    strcpy(raiz.initVersion, initVersion);
    strcpy(raiz.initParent, initParent);
    salvarStruct(&raiz, file);
    }