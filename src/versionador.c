#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "commands/commands.h"
#include "utils/utils.h"
#include "database/database.h"
#include "utils/lista.h"

// Protótipos das funções
void iniciar();
void adiciona(int argc, char *argv[]);
void registra(char *texto);
void exibirlog();
void logComConteudo();
void mostrar(char *identificador);
void mudar(char *identificador);
void mudarAtual();
void criarRamo(char* name);
void removeRamo(char* name);
void alteraRamo(char* nameAtual, char* novoNome);

int main(int argc, char *argv[]) {
    // Verifica o número de argumentos
    if (argc < 2) {
        printf("Uso: versionador.exe <comando> [argumentos]\n");
        return 1;
    }

    // Obtém o comando a ser executado
    char *comando = argv[1];

    // Chama a função correspondente ao comando
    if (strcmp(comando, "iniciar") == 0) {
        iniciar();
        
    } else if (strcmp(comando, "adiciona") == 0) {
        adiciona(argc, argv);
    }  else if (strcmp(comando, "ramo") == 0) {
        
        switch (argc)
        {
        case 3:
            criarRamo(argv[2]);
            break;
        case 4:
            if (strcmp(argv[2], "--remover") == 0) {
                removeRamo(argv[3]);
            }else{
                printError("comando invalido sugestoes:\nversionador.exe ramo <nome_ramo>\nversionador.exe ramo –remover <nome_ramo>\nversionador.exe ramo --alterar <nome_ramo> <novo_nome>");
            }
            break;
        case 5:
            if(strcmp(argv[2], "--alterar") == 0){
                alteraRamo(argv[3], argv[4]);
            }else{
                printError("comando invalido sugestoes:\nversionador.exe ramo <nome_ramo>\nversionador.exe ramo –remover <nome_ramo>\nversionador.exe ramo --alterar <nome_ramo> <novo_nome>");
            }
            break;
        
        default:
            printError("comando invalido sugestoes:\nversionador.exe ramo <nome_ramo>\nversionador.exe ramo –remover <nome_ramo>\nversionador.exe ramo --alterar <nome_ramo> <novo_nome>");
            break;
        }
    } else if (strcmp(comando, "registra") == 0) {
        // Verifica se foi fornecido o texto do snapshot
        if (argc < 3) {
            printf("Uso: versionador.exe registra \"Texto do snapshot\"\n");
            return 1;
        }
        char *texto = argv[2];
        registra(texto);
    } else if (strcmp(comando, "log") == 0) {
        exibirlog();
        
    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "--conteudo") == 0) {
            logComConteudo();
        }
    }
        
    } else if (strcmp(comando, "mostrar") == 0) {
        // Verifica se foi fornecido o identificador da versão
        if (argc < 3) {
            printf("Uso: versionador.exe mostrar <identificador>\n");
            return 1;
        }
        char *identificador = argv[2];
        mostrar(identificador);
    } else if (strcmp(comando, "mudar") == 0) {
        // Verifica se foi fornecido o identificador da versão
        if (argc < 3) {
            printf("Uso: versionador.exe mudar <identificador>/<branch>\n");
            return 1;
        }else{for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "--atual") == 0) {
            mudarAtual();
        }else{
            char *identificador = argv[2];
            mudar(identificador);
        }
        }
        }
        
     }else {
        printf("Comando inválido.\n");
        return 1;
    }

    return 0;
}

// Implementação das funções
/**
 * @brief Inicia o versionador chamando a função iniciarVersionador().
 */
void iniciar() {
    iniciarVersionador();
}

/**
 * @brief Adiciona arquivos ao versionador.
 * 
 * @param argc O número de argumentos.
 * @param argv Os argumentos.
 */

void adiciona(int argc, char* argv[]) {
    for (int i = 2; i < argc; i++) {
        if (fileExists(argv[i])) {
            addFileToSnapshot(argv[i]);
            printInfo("Arquivo adicionado com sucesso.");
        } else {
            printf("Arquivo não encontrado: %s", argv[i]);
        }
    }
}

/**
 * @brief Registra um snapshot com o texto especificado.
 * 
 * @param texto O texto do snapshot.
 */

void registra(char* texto) {
    char* id = registrarSnapshot(texto);
    saveSnapshotFilesInContent(id);
    free(id);
}

/**
 * @brief Exibe o log das versões.
 */

void exibirlog() {
    exibirLogInCommands();
}

/**
 * @brief Exibe o log das versões com o conteúdo dos arquivos.
 */

void logComConteudo() {
    getLogsContent();
    
}

void mostrar(char* indentfier){
    mostrarLogInCommands(indentfier);
}

/**
 * @brief Muda para uma versão específica.
 * 
 * @param identificador O identificador da versão.
 */

void mudar(char* identificador) {
    if (versionExists(identificador)) {
        changeVersion(identificador);
    }else{
        printf("A versão %s não existe.", identificador);
    }
}

void criarRamo(char* name){
    printInfo(name);
    if(!branchExists(name)){
        createBranch(name);
    } else {
        printf("A Branch %s já existe.", name);
    }
}

void removeRamo(char* name){
    if(branchExists(name)){
        removeBranch(name);
    } else {
        printf("A Branch %s não existe.", name);
    }
}

void alteraRamo(char* nameAtual, char* novoNome){
     if(branchExists(nameAtual)){
        if(!branchExists(novoNome)){
            renameBranch(nameAtual, novoNome);
        } else {
            printf("A Branch %s já existe.", novoNome);
        }
    } else {
        printf("A Branch %s não existe.", nameAtual);
    }
}

/**
 * @brief Muda para a versão atual.
 */
void mudarAtual() {
    restoreCurrentVersion();
    printInfo("Versão atual revertida com sucesso.");
}

