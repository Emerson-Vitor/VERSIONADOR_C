#ifndef ARVORE_H
#define ARVORE_H

// STRUCTS
typedef struct TreeNode TreeNode;
typedef struct ListNode ListNode;
typedef enum boolean Boolean;

// FUNÇÕES

TreeNode* createNode(const char* name, const char* initVersion, const char* version, const char* parentName);
ListNode* createListNode(TreeNode* node);
void addListNode(ListNode** head, TreeNode* node);
TreeNode*  findNodeInListByInitVersion(ListNode* head, const char* version);
TreeNode* findNodeInList(ListNode* head, const char* name);
void addChild(TreeNode* parent, TreeNode* child);
void printTree(TreeNode* node, int depth);
void freeTree(TreeNode* node);
void addVersionsToList(const char* fileName, ListNode** listHead, const char* nodeName);
void readMetadataFile(const char* fileName, ListNode** listHead, TreeNode** root);
void freeList(ListNode* listHead);
void addVersionsToListAndConcatenate(ListNode** head, const char* name);
TreeNode* createTree(const char* fileName);
void searchVersion(TreeNode* node, const char* version,  char* name, char* versionName, char* file);
void printBranchByName(TreeNode* node, const char* name, const char* versionStop);
void treeBranchRecursiv(char* branch, TreeNode* root, char* proxBranch, ListNode* listnodeRoot, char* versionStop);
void treeBranch(char* branch, TreeNode* root);
void removeBranchFromFileReturnChildren(const char* branchName, Lista* children);
void renameBranchFromFile( const char* currentBranchName, const char* newBranchName);
int branchExistsInTree(char* branch);
void printTreeContent(TreeNode* node, int depth);

#endif