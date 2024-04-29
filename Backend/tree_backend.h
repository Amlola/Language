#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>



#define CHECK_TREE_ERROR(tree)                           \
                    if (TreeVerify(tree) != TREE_OK)     \
                        {                                \
                        ON_DUMP                          \
                            (                            \
                            TreeDump(tree, tree->root);  \
                            )                            \
                        return tree->status;             \
                        }


#ifdef DUMP
    #define ON_DUMP(...) __VA_ARGS__
    #define TreeDump(tree_ptr, node_ptr) TreeDumpFunction(tree_ptr, node_ptr,  __FILE__, __PRETTY_FUNCTION__, __LINE__); 


#else
    #define ON_DUMP(...)
#endif

typedef char* Tree_type;


typedef int Type_error;


enum Tree_status
    {
    TREE_OK                       = 0,
    TREE_IS_NULL                  = 1 << 0,
    TREE_ROOT_IS_NULL             = 1 << 1,
    NODE_PTR_IS_NULL              = 1 << 2,  
    TREE_CANT_HAVE_THIS_CHILD     = 1 << 3,
    TREE_SIZE_LESS_THAN_ZERO      = 1 << 4,
    TREE_LINKING_ERROR            = 1 << 5,
    NUM_OF_ERROR                  = 6
    };



const int MAX_COMMAND_LENGTH = 125;

const size_t WORD_MAX_LEN = 100;



struct Token
    {
    int type;

    union NodeType
        {
        int key_w;
        double num;
        char id[WORD_MAX_LEN];
        }form;
    };

struct Node_t 
    {
    Node_t* left;
    Node_t* right;
    Node_t* parent;
    Token kind;
    };


struct Tree 
    {
    Node_t* root;
    size_t size;
    Type_error status;
    };


struct Text
    {
    char* Buf;
    long long BufSize;
    };


enum Child
    {
    L_CHILD = 0,
    R_CHILD = 1,
    };


enum Types
    {
    NUM_TYPE       = 1,
    ID_TYPE        = 2,
    KEYW_TYPE      = 3,
    FUNC_DEF_TYPE  = 4,
    PARAM_TYPE     = 5,
    VAR_DECL_TYPE  = 6,
    CALL_TYPE      = 7
    };


static FILE* graph_file1 = NULL;

static FILE* logfile = nullptr;

#define print(...) fprintf(graph_file1, __VA_ARGS__)

#define SPECIFICATOR_TYPE "%s "


struct TREE_STATUS
    {
    Tree_status CodeError;
    const char* NameError;
    };


const TREE_STATUS ErrorMas[] = 
    {
    {TREE_OK,                     "NO ERROR"},
    {TREE_IS_NULL,                "TREE_IS_NUL"},
    {TREE_ROOT_IS_NULL,           "TREE_ROOT_IS_NULL"},
    {NODE_PTR_IS_NULL,            "NODE_PTR_IS_NULL"},
    {TREE_CANT_HAVE_THIS_CHILD,   "TREE_CANT_HAVE_THIS_CHILD"},
    {TREE_SIZE_LESS_THAN_ZERO,    "TREE_SIZE_LESS_THAN_ZERO"},
    {TREE_LINKING_ERROR,          "TREE_LINKING_ERROR"}
    };


void TreeCtor(Tree* tree);

Type_error TreeDelete(Tree* tree, Node_t* node);

Type_error TreePrefixPrint(Tree* tree, Node_t* node, FILE* file);

bool TreeVerify(Tree* tree);

Type_error TreeDtor(Tree* tree); 

void TreeDumpFunction(Tree* tree, Node_t* node, const char* path, const char* signature, unsigned line); 

void NodeDump(Node_t* node, size_t* number_of_node, Child child, const char* color);

void PrintGraphEdge(size_t from, size_t to, Child child, const char* color); 

void PrintGraphNode(Node_t* node, size_t* number_of_node, Child child, const char* color); 

Type_error GetString(Tree* tree, Text* data, char** string, size_t* i);

Node_t* PrefixReadTree(Tree* tree, Text* data, size_t* i);

Node_t* CreateNode(Token token, Types type, Node_t* left, Node_t* right);

Type_error TreeRead(Tree* tree, Text* data);

Type_error CheckTreeLinks(Tree* tree, Node_t* node);

int SkipSpaces(Text* data, size_t i);

long long GetFileSize(FILE* file);

void FileInput(FILE* file, Text* data);

Node_t* CreateNodeFromBrackets(Tree* tree, Text* data, size_t* i);



