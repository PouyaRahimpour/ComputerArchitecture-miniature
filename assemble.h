#include "utils.h"

struct instruction{
   size_t instType; // 0 means r type, 1 means i type and 2 means j type
   size_t intInst;
   char mnemonic[50];
   char inst[10];
   char hexInst[15];
   int rs;
   int rt;
   int rd;
   int imm;
   int PC;
};


//the number of R, I and J type instructions
#define NORTYPE 5
#define NOITYPE 8
#define NOJTYPE 2
#define RTYPE 0
#define ITYPE 1
#define JTYPE 2
#define EXIT_FAILIURE -1
#define EXIT_SUCCESS 0

/* this structure is defined to hold ever entity of symbol table    *
 * which, after finding the length, will become an array of symbols */
struct symbolTable{
   int value;
   char symbol[30];
};
/* the follwing function, findSymTabLen, will scan the assembly    *
 * code and count the number of symbols and return it              */
int findSymTabLen(FILE *);
/* this function fills up the symbol table                         */
int fillSymTab(struct symbolTable *, FILE *);

int isRType(int);

int isIType(int);

int isJType(int);

int atoiImproved(char *, struct symbolTable* , int);

void formInst(struct instruction*);

