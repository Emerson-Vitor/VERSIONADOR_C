#define _DEFAULT_SOURCE
#include "../database/database.h"
#include "../utils/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h> 
#include <string.h>
#include "../utils/lista.h"
#include "../utils/arvore.h"


Version versions[MAX_NUM_VERSIONS];
int numVersions = 0;

/**
 * @brief Cria o banco de dados necessário para o versionador.
 */
void createDatabase() {
    createDirectory(".versionador");
    createDirectory(".versionador/content");
    createDirectory(".versionador/content/principal");
    writeTextFile(".versionador/content/dados.txt", "");
    fillNode("principal", "Nenhum", "Nenhum", ".versionador/content/dados.txt");
    writeTextFile(".versionador/atual.txt", "<INICIO>\nprincipal\nNenhum\n<FIM>");
    createDirectory(".versionador/snapshots");
}

/**
 * @brief Verifica se um ramo existe no banco de dados.
 * @param name Nome do ramo.
 * @return 1 se o ramo existe, 0 caso contrário.
 */
int branchExists(char* name){
    printInfo("ola");
   return branchExistsInTree(name);
}

/**
 * @brief Cria um novo ramo no banco de dados.
 * @param name Nome do ramo.
 * @param branchName Nome do ramo de origem.
 * @param versionName Nome da versão de origem.
 */

void createBranchInDatabase(char* name, char *branchName, char *versionName){
    char contentAtual[200];
    char concatAtual[200];
    char concatDirectory[200];

    sprintf(concatDirectory, ".versionador/content/%s", name);
    createDirectory(concatDirectory);

    fillNode(name, versionName, branchName, ".versionador/content/dados.txt");

    sprintf(contentAtual, "<INICIO>\n%s\nNenhum\n<FIM>", name);
    writeTextFile(".versionador/atual.txt", contentAtual);

    sprintf(concatAtual, ".versionador/versions%s.txt", name);
    writeTextFile(concatAtual, "");
}

/**
 * @brief Renomeia um ramo no banco de dados.
 * @param branchName Nome do ramo a ser renomeado.
 * @param newBranchName Novo nome do ramo.
 */

void renameBranchInDatabase(char* branchName, char* newBranchName) {
    // Verificar se o ramo existe no banco de dados
    if (!branchExists(branchName)) {
        printf("Branch '%s' não existe.\n", branchName);
        return;
    }

    // Verificar se o novo nome do ramo já existe no banco de dados
    if (branchExists(newBranchName)) {
        printf("O novo nome do ramo '%s' já existe no banco de dados.\n", newBranchName);
        return;
    }

    // Renomear o ramo no arquivo da árvore de versões
    renameBranchFromFile(branchName, newBranchName);

    // Renomear o diretório do ramo
    char branchPath[100];
    sprintf(branchPath, ".versionador/content/%s", branchName);

    char newBranchPath[100];
    sprintf(newBranchPath, ".versionador/content/%s", newBranchName);

    if (rename(branchPath, newBranchPath) != 0) {
        printf("Erro ao renomear o diretório do ramo.\n");
        return;
    }

    // Renomear o arquivo de versões do ramo
    char versionsPath[100];
    sprintf(versionsPath, ".versionador/versions%s.txt", branchName);

    char newVersionsPath[100];
    sprintf(newVersionsPath, ".versionador/versions%s.txt", newBranchName);

    if (rename(versionsPath, newVersionsPath) != 0) {
        printf("Erro ao renomear o arquivo de versões do ramo.\n");
        return;
    }

    FILE* file = fopen(".versionador/atual.txt", "r");

    if (file != NULL) {
        char linha[50];
        char branchName[50];
        char versionName[50];
        char concatContent[200];

        while (fgets(linha, sizeof(linha), file) != NULL) {
            if (strcmp(linha, "<INICIO>\n") == 0) {
                fgets(branchName, 50, file);
                fgets(versionName, 50, file);
                // Removendo o caractere de nova linha (\n) no final de cada string
                branchName[strcspn(branchName, "\n")] = '\0';
                versionName[strcspn(versionName, "\n")] = '\0';
            }
        }

        sprintf(concatContent, "<INICIO>\n%s\n%s\n<FIM>", newBranchName, versionName);
        // printf(concatContent);
        // remove(".versionador/atual.txt");
        writeTextFile(".versionador/atual.txt", concatContent);

        fclose(file);
    } else {
        printf("Erro ao abrir o arquivo para ler as structs.\n");
    }

    printf("Ramo '%s' renomeado para '%s' com sucesso.\n", branchName, newBranchName);
}

/**
 * @brief Remove um ramo do banco de dados.
 * @param name Nome do ramo a ser removido.
 */

void removeBranchInDatabase(char* branchName) {
    
    //Verificar se o ramo existe no banco de dados
    if (!branchExists(branchName)) {
        printf("Branch '%s' não existe.\n", branchName);
        return;
    }

    // Remover o ramo da árvore de versões
    Lista* root = lst_cria();
    removeBranchFromFileReturnChildren(branchName, root);

    // Remover o diretório do ramo
    
    ListaNo* ptr = lst_returnNodeValid(root);
    char* fileOrigin = lst_infoValid(ptr);
    
    while (fileOrigin != NULL)
    {   
        char branchPath[100];
        sprintf(branchPath, ".versionador/content/%s", fileOrigin);
        removeDirectory(branchPath);
    
        // Remover o arquivo de versões do ramo
        char versionsPath[100];
        sprintf(versionsPath, ".versionador/versions%s.txt", fileOrigin);
        if (remove(versionsPath) != 0) {
            printf("Failed to remove versions file for branch '%s'.\n", fileOrigin);
        }
        
        ptr = lst_nextNode(ptr);
        fileOrigin = lst_infoValid(ptr);
    }
    free(fileOrigin);
    lst_libera(root);

    char branchPath[100];
    sprintf(branchPath, ".versionador/content/%s", branchName);
    removeDirectory(branchPath);
 
    // Remover o arquivo de versões do ramo
    char versionsPath[100];
    sprintf(versionsPath, ".versionador/versions%s.txt", branchName);
    if (remove(versionsPath) != 0) {
        printf("Failed to remove versions file for branch '%s'.\n", branchName);
    }

    printf("Branch '%s' removida com sucesso.\n", branchName);
}




/**
 * @brief Adiciona um arquivo ao próximo snapshot.
 * @param filename Nome do arquivo a ser adicionado.
 */
void addFileToSnapshot(const char* filename) {
     // Marcar o arquivo para ser adicionado ao próximo snapshot
    char* snapshotPath = concatenatePaths(".versionador/snapshots", "next_snapshot.txt");
    size_t len1 = strlen("\n");
    size_t len2 = strlen(filename);
    char* combined = (char*)malloc(len1 + len2 + 1);
    strcpy(combined, "\n");
    strcat(combined, filename);
     
    FILE* snapshotFile = fopen(snapshotPath, "w");
    if (snapshotFile == NULL) {
        printError("Failed to open snapshot file for appending.");
        free(snapshotPath);
       
        free(combined);
        return;
    }

    fputs(combined, snapshotFile);
    fclose(snapshotFile);
    free(combined);
    free(snapshotPath);

}

/**
 * @brief Registra um snapshot no banco de dados.
 * @param identifier Identificador do snapshot.
 * @param commit Texto do commit associado ao snapshot.
 */
void registerSnapshot(const char* identifier, const char* commit) {
    // Criar um snapshot dos arquivos marcados e registrar no banco de dados
    char* snapshotPath = concatenatePaths(".versionador/snapshots", identifier);
    writeTextFile(snapshotPath, commit);
   
    free(snapshotPath);
}
/**
 * @brief Define o caminho para o identificador do próximo snapshot.
 * @param identifier Identificador do próximo snapshot.
 */
void  setPathToSnapshotIdentifier(const char* identifier){
     FILE* file = fopen(".versionador/atual.txt", "r");

    if (file != NULL) {
        char linha[50];
        char branchName[50];
        char versionName[50];
        char concatBranchPath[100];
        char concatBranchFile[100];
        char concatContent[200];

        while (fgets(linha, sizeof(linha), file) != NULL) {
            if (strcmp(linha, "<INICIO>\n") == 0) {
                fgets(branchName, 50, file);
                fgets(versionName, 50, file);
               

                // Removendo o caractere de nova linha (\n) no final de cada string
                branchName[strcspn(branchName, "\n")] = '\0';
                versionName[strcspn(versionName, "\n")] = '\0';
            }
        }
         
        sprintf(concatBranchPath, ".versionador/content/%s", branchName);
        char* path = concatenatePaths(concatBranchPath, identifier);

        sprintf(concatBranchFile, ".versionador/versions%s.txt", branchName);
        writeTextFileNextLine(concatBranchFile, identifier);

        createDirectory(path);
        printInfo("create version path");
        free(path);

        sprintf(concatContent, "<INICIO>\n%s\n%s\n<FIM>", branchName, identifier);
        // printf(concatContent);
        // remove(".versionador/atual.txt");
        writeTextFile(".versionador/atual.txt", concatContent);

        fclose(file);
    } else {
        printf("Erro ao abrir o arquivo para ler as structs.\n");
    }
}

/**
 * @brief Obtém informações sobre os snapshots registrados no banco de dados.
 * @return Número total de snapshots.
 */
int getSnapshotInfo() {
    printf("Lista de snapshots:\n");

    // Abrir a pasta de snapshots
    DIR* snapshotsDir = opendir(".versionador/snapshots");
    if (snapshotsDir == NULL) {
        printError("Failed to open snapshots directory.");
        return 0;
    }

    // Ler os arquivos na pasta de snapshots
    int numVersions = 0;
    struct dirent* entry;
    while ((entry = readdir(snapshotsDir)) != NULL) {
        if (entry->d_type == DT_REG) {  // Verificar se é um arquivo regular
            char* identifier = entry->d_name;

            // Construir o caminho completo para o arquivo do snapshot
            char* snapshotPath = concatenatePaths(".versionador/snapshots", identifier);


            // Ler o texto do snapshot
            char* snapshotText = readTextFile(snapshotPath);
            if (snapshotText != NULL) {
                printf("Identificador: %s\n", identifier);
                printf("Texto: %s\n\n", snapshotText);
                free(snapshotText);
                numVersions++;
            }

            free(snapshotPath);
        }
    }

    closedir(snapshotsDir);
    return numVersions;
}

/**
 * @brief Obtém o conteúdo de uma determinada versão.
 * @param id Identificador da versão.
 * @param showContent Flag para indicar se o conteúdo dos arquivos deve ser exibido.
 */
void getVersionContent(const char* id, int showContent) {
    char identifier[100];
    int i = 0;
    int j = 0;
   
   while (id[i] != '\0') {
            identifier[j] = id[i]; // Armazena o caractere na palavra
            j++;
            i++;
    }
    identifier[j] = '\0';
    // Obter o texto registrado para a versão com o identificador especificado
    // Obter o conteúdo dos arquivos versionados naquela versão
    // Se showContent for verdadeiro, exibir o conteúdo dos arquivos no console
    printInfo(identifier);
    // Construir o caminho completo para o arquivo do snapshot
    char* snapshotPath = concatenatePaths(".versionador/snapshots", identifier);
   

    // Ler o texto do snapshot
    char* snapshotText = readTextFile(snapshotPath);

    if (snapshotText != NULL) {
        printf("Texto registrado para a versão %s:\n", identifier);
        printf("%s\n\n", snapshotText);
        free(snapshotText);
    }

    if (showContent) {
        // Construir o caminho completo para a pasta de conteúdo
        char* contentPath = concatenatePaths(".versionador/content", identifier);

        // Abrir a pasta de conteúdo
        DIR* contentDir = opendir(contentPath);
        if (contentDir == NULL) {
            printError("Failed to open content directory.");
            free(contentPath);
            return;
        }

        printf("Conteúdo dos arquivos versionados:\n");

        // Ler os arquivos na pasta de conteúdo
        struct dirent* entry;
        while ((entry = readdir(contentDir)) != NULL) {
            if (entry->d_type == DT_REG) {  // Verificar se é um arquivo regular
                // Construir o caminho completo para o arquivo de conteúdo
                char* filePath = concatenatePaths(contentPath, entry->d_name);

                // Ler o conteúdo do arquivo
                char* fileContent = readTextFile(filePath);
              
                if (fileContent != NULL) {
                    printf("Arquivo: %s\n", entry->d_name);
                    printf("Conteúdo:\n%s\n\n", fileContent);
                    free(fileContent);
                }
                free(filePath);
            }
        }

        closedir(contentDir);
        free(contentPath);
    }

    free(snapshotPath);
    
}





/**
 * @brief Verifica se uma determinada versão existe.
 * @param identifier Identificador da versão.
 * @return 1 se a versão existe, 0 caso contrário.
 */


int versionExists(const char* identifier) {
    // Verificar se a versão com o identificador especificado existe
    char* snapshotPath = concatenatePaths(".versionador/snapshots", identifier);
    int exists = fileExists(snapshotPath);
    free(snapshotPath);
    return exists;
}

/**
 * @brief Restaura os arquivos de uma determinada versão.
 * @param identifier Identificador da versão.
 * @param backupPath Caminho para a pasta de backup dos arquivos da versão.
 */
void restoreVersionFiles(const char* identifier, const char* backupPath) {
    // Restaurar os arquivos da versão com o identificador especificado

    // Construir o caminho completo para a pasta de conteúdo da versão
    char* versionContentPath = concatenatePaths(".versionador/content", identifier);

    // Remover a pasta de conteúdo atual
    removeDirectory(".versionador/content");

    // Copiar todos os arquivos da pasta de backup para a pasta de conteúdo
    copyDirectory(backupPath, ".versionador/content");

    // Renomear a pasta de conteúdo para a versão desejada
    renameDirectory(".versionador/content", versionContentPath);

    free(versionContentPath);
}

/**
 * @brief Verifica se um diretório existe.
 * @param path Caminho do diretório.
 * @return 1 se o diretório existe, 0 caso contrário.
 */
int directoryExists(const char* path) {
    struct stat st;
    if (stat(path, &st) == 0 && S_ISDIR(st.st_mode)) {
        return 1;  // Diretório existe
    }
    return 0;  // Diretório não existe
}
/**
 * @brief Obtém o identificador da versão atual.
 * @return Identificador da versão atual.
 */
char* getCurrentVersionIdentifier() {
    char line[200];
    char* cuerrentCopy = (char*)malloc(sizeof(char)*200);
    char cuerrent[200];

    FILE* file = fopen(".versionador/atual.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de metadados.\n");
        return NULL;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        if (strcmp(line, "<INICIO>\n") == 0) {
            fgets(line, sizeof(line), file);  // Ler o nome da branch
            fgets(cuerrent, sizeof(cuerrent), file);  // Ler a versão da branch
            cuerrent[strcspn(cuerrent, "\n")] = '\0';
        }
    }

    fclose(file);

    strcpy(cuerrentCopy, cuerrent);
    return cuerrentCopy;
}
/**
 * @brief Copia um diretório e seu conteúdo para um destino específico.
 * @param sourceDir Diretório de origem.
 * @param destDir Diretório de destino.
 */
void copyDirectory(const char* sourceDir, const char* destDir) { 

    // Abrir o diretório de origem
    DIR* dir = opendir(sourceDir);
    if (dir == NULL) {
        printf("Failed to open source directory.\n");
        return;
    }

    // Ler os arquivos no diretório de origem
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Construir os caminhos completos para o arquivo de origem e destino
        char* sourcePath = concatenatePaths(sourceDir, entry->d_name);
        char* destPath = concatenatePaths(destDir, entry->d_name);

        // Verificar se é um diretório
        struct stat st;
        if (stat(sourcePath, &st) == 0 && S_ISDIR(st.st_mode)) {
            // Se for um diretório, chamar recursivamente a função copyDirectory
            copyDirectory(sourcePath, destPath);
        } else {
            // Se for um arquivo, copiar o arquivo para o destino
            copyFile(sourcePath, destPath);
        }

        free(sourcePath);
        free(destPath);
    }

    closedir(dir);
}

/**
 * @brief Copia um arquivo de origem para um destino específico.
 * @param sourceFile Arquivo de origem.
 * @param destFile Arquivo de destino.
 */
void copyFile(const char* sourceFile, const char* destFile) {
    char destino[100];
    strcpy(destino, destFile);
    FILE* dest = fopen(destino, "w");
    FILE* source = fopen(sourceFile, "r");
    

    if (source == NULL || dest == NULL) {
        printf("Failed to open source or destination file.\n");
        return;
    }

    fseek(source, 0, SEEK_END);
    long fileLength = ftell(source);
    fseek(source, 0, SEEK_SET);
    char *buffer = (char*)malloc(fileLength);

    size_t bytesRead;

    if ((bytesRead = fread(buffer, 1, fileLength+1, source)) > 0) {
        fwrite(buffer, 1, bytesRead, dest);
    }

    fclose(source);
    fclose(dest);
}
/**
 * @brief Remove um diretório e todo o seu conteúdo.
 * @param path Caminho do diretório a ser removido.
 */
void removeDirectory(const char* path) {
    // Abrir o diretório
    DIR* dir = opendir(path);
    if (dir == NULL) {
        printf("Failed to open directory.\n");
        return;
    }

    // Ler os arquivos no diretório
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Construir o caminho completo para o arquivo/diretório
        char* filePath = concatenatePaths(path, entry->d_name);

        // Verificar se é um diretório
        struct stat st;
        if (stat(filePath, &st) == 0 && S_ISDIR(st.st_mode)) {
            // Se for um diretório, chamar recursivamente a função removeDirectory
            removeDirectory(filePath);
        } else {
            // Se for um arquivo, remover o arquivo
            if (remove(filePath) != 0) {
                printf("Failed to remove file: %s\n", filePath);
            }
        }

        free(filePath);
    }

    closedir(dir);
 
    // Remover o diretório
    if (rmdir(path) != 0) {
        printf("Failed to remove directory: %s\n", path);
    }
}

/**
 * @brief Renomeia um diretório.
 * @param oldPath Caminho do diretório antigo.
 * @param newPath Novo caminho para o diretório.
 *
 * Verifica se os caminhos são diferentes e, em seguida, utiliza a função `rename`
 * para renomear o diretório.
 */
void renameDirectory(const char* oldPath, const char* newPath) {
    // Verificar se os caminhos são diferentes
    if (strcmp(oldPath, newPath) == 0) {
        printf("The old and new paths are the same.\n");
        return;
    }

    // Renomear o diretório usando a função rename
    if (rename(oldPath, newPath) != 0) {
        printf("Failed to rename directory.\n");
        return;
    }
}

/**
 * @brief Adiciona conteúdo a um arquivo específico.
 * @param identifier Identificador do arquivo.
 * @param text Texto a ser adicionado.
 *
 * Extrai o nome do arquivo do texto fornecido e, em seguida, cria o caminho completo para
 * o arquivo dentro da pasta ".versionador/content". Em seguida, lê o conteúdo do arquivo
 * fornecido e o escreve no arquivo correspondente no local especificado.
 */
void addContent( const char* identifier,const char* text){
    char filename[100];
    int i = 0;
    int j = 0;
   
   while (text[i] != '\0') {
            filename[j] = text[i]; // Armazena o caractere na palavra
            j++;
            i++;
    }
    filename[j] = '\0';
    FILE* file = fopen(".versionador/atual.txt", "r");

    if (file != NULL) {
        char linha[50];
        char branchName[50];
        char versionName[50];
        char concatBranchPath[100];

        while (fgets(linha, sizeof(linha), file) != NULL) {
            if (strcmp(linha, "<INICIO>\n") == 0) {
                fgets(branchName, 50, file);
                fgets(versionName, 50, file);
               

                // Removendo o caractere de nova linha (\n) no final de cada string
                branchName[strcspn(branchName, "\n")] = '\0';
                versionName[strcspn(versionName, "\n")] = '\0';
            }
        }
         
        
        sprintf(concatBranchPath, ".versionador/content/%s", branchName);
        char* fileConcat = concatenatePaths(concatBranchPath, concatenatePaths(identifier, filename));
        printInfo(filename);
        writeTextFile(fileConcat, readTextFile(filename));
        free(fileConcat);
        fclose(file);

    } else {
        printf("Erro ao abrir o arquivo para ler as structs.\n");
    }
}