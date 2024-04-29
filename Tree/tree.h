#pragma once


#include "../Front/inc/Lexem.h"


const int NUMBER_OF_KEYWORD = 31; //need to add if keyword++


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


enum Child
    {
    L_CHILD = 0,
    R_CHILD = 1,
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


struct Keyword 
    {
    size_t num;
    const char* name;
    };


const Keyword keyword_array[] = 
    {
    {11, "if"},
    {12, "while"},
    {13, "="},
    {21, "sin"},
    {22, "cos"},
    {23, "floor"},
    {24, "+"},
    {25, "-"},
    {26, "*"},
    {27, "/"},
    {28, "diff"},
    {29, "sqrt"},
    {31, "=="},
    {32, "<"},
    {33, ">"},
    {34, "<="},
    {35, ">="},
    {36, "!="},
    {37, "&&"},
    {38, "||\0"},
    {39, "!"},
    {41, ";"},
    {42, ","},
    {51, "int"},
    {61, "input"},
    {62, "print"},
    {71, "return"},
    {72, "break"},
    {73, "continue"},
    {74, "abort"},
    {5,  "else"}
    };


void TreeCtor(Tree* tree);

Type_error TreeDelete(Tree* tree, Node_t* node);

Type_error TreePrefixPrint(Tree* tree, Node_t* node, FILE* file, LangNameTableArray* table_array);

bool TreeVerify(Tree* tree);

Type_error TreeDtor(Tree* tree); 

void TreeDumpFunction(Tree* tree, Node_t* node, const char* path, const char* signature, unsigned line); 

void NodeDump(Node_t* node, size_t* number_of_node, Child child, const char* color);

void PrintGraphEdge(size_t from, size_t to, Child child, const char* color); 

void PrintGraphNode(Node_t* node, size_t* number_of_node, Child child, const char* color); 

Node_t* CreateNode(Token token, Types type, Node_t* left, Node_t* right);

Type_error CheckTreeLinks(Tree* tree, Node_t* node);
