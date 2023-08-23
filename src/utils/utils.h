#ifndef UTILS_H
#define UTILS_H
#include "lista.h"

struct Raiz;

void printError(const char* message);
void printInfo(const char* message);
void createDirectory(const char* path);
char* concatenatePaths(const char* path1, const char* path2);
char* readTextFile(const char* name);
void writeTextFile(const char* filename, const char* content);
int fileExists(const char* filename);
char* generateUniqueIdentifier();
int contLines(char* path);
void removeFile(char* file);
void writeTextFileNextLine(const char* name, const char* text);
void searchDirectoryFiles(char* path, Lista* header);
void salvarStruct(const struct Raiz* raiz, const char* arquivo);
void lerStructs(const char* arquivo);
void fillNode(char* name, char* initVersion, char* initParent, char* file);

#endif  // UTILS_H
