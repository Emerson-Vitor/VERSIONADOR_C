#ifndef DATABASE_H
#define DATABASE_H

#define MAX_IDENTIFIER_LENGTH 20
#define MAX_NUM_VERSIONS 100

typedef struct {
    char identifier[MAX_IDENTIFIER_LENGTH];
    // outros campos da versão
} Version;

extern Version versions[MAX_NUM_VERSIONS];
extern int numVersions;


// Declaração das funções do banco de dados
void createDatabase();
void addFileToSnapshot(const char* filename);
void registerSnapshot(const char* identifier, const char* text);
int getSnapshotInfo();
void getVersionContent(const char* identifier, int showContent);


int versionExists(const char* identifier);

void restoreVersionFiles(const char* identifier, const char* backupPath);
int directoryExists(const char* path);
char* getCurrentVersionIdentifier();
void copyDirectory(const char* sourceDir, const char* destDir);
void copyFile(const char* sourceFile, const char* destFile);
void removeDirectory(const char* path);
void renameDirectory(const char* oldPath, const char* newPath);
void setPathToSnapshotIdentifier(const char* identifier);
void addContent(const char* identifier,const char* text);


void renameBranchInDatabase(char* branchName, char* newBranchName);
void removeBranchInDatabase(char* branchName);
void createBranchInDatabase(char* name, char *branchName, char *versionName);
int branchExists(char* name);

#endif /* DATABASE_H */
