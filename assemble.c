#include "assemble.h"

int findSymTabLen(FILE *inputFile){
   int count=0;
   size_t lineSize;
   char *line;
   line=(char *)malloc(72);
   while(getline(&line,&lineSize,inputFile) != -1){
      if(line[0] == ' ' || line[0] == '\t'
        || line[0] == '\n' || line[0] == '#');
      else
         count++;
   }
   rewind(inputFile);
   free(line);
   return count;
}

int fillSymTab(struct symbolTable *symT,FILE *inputFile){
   int lineNo=0;
   size_t lineSize;
   char *line;
   int i=0;
   char *token;
   line=(char *)malloc(72);
   while(getline(&line, &lineSize, inputFile) != -1){
      if(line[0] == ' ' || line[0] == '\t');
      else if (line[0] == '\n' || line[0] == '#') {
          lineNo--;
      }
      else {
          token=strtok(line,"\t, ");
          // check if this label is already in symbol symbolTable
          for (int j=0; j<i; j++) {
              if (strcmp(symT[j].symbol, token) == 0) {
                  printf("label \"%s\" is defined more than once.\n", symT[j].symbol);
                  exit(EXIT_FAILIURE);
                  return EXIT_FAILIURE;
              }
          }
          strcpy(symT[i].symbol,token);
          symT[i].value=lineNo;
          i++;
      }
      lineNo++;
   }
   rewind(inputFile);
   free(line);
   return EXIT_SUCCESS;
}


int isRType(int index) {
    if (index >= 0 && index < 5)
        return 1;
    return 0;
}
int isIType(int index) {
    if (index > 4 && index < 13)
        return 1;
    return 0;
}
int isJType(int index) {
    if (index > 12 && index < 15)
        return 1;
    return 0;
}

int atoiImproved(char *str, struct symbolTable* symT, int symTabLen) {
    int state = symTabLen >= 0? 1:0;
    // if state is zero it means the function is called without symbol symbolTable
    // and that is's been called for a register,
    // o.w it's been called to calculate an immidiate or label
    switch (state) {
    case 1:
        if (!isNumeric(str)) {
            for (int k=0; k<symTabLen; k++) {
                if (strcmp(symT[k].symbol, str) == 0) {
                    return symT[k].value;
                }
            }
            printf("invalid label. label \"%s\" not found in symbol table.\n", str);
            exit(EXIT_FAILIURE);
            return EXIT_FAILIURE;
        }
        return atoi(str);
    break;

    case 0:
        if (!isNumeric(str) || atoi(str) < 0 || atoi(str) > 15) {
            printf("invalid register. register \"%s\" is invalid.\n", str);
            exit(EXIT_FAILIURE);
            return EXIT_FAILIURE;
        }
        return atoi(str);
    break;
    }

}

void formInst(struct instruction* inst) {
    char lower[5];
    char tmpHex[] = "0000000000";
    switch (inst->instType) {
    case RTYPE:
        intToHex(lower, inst->rd);
        memcpy(&tmpHex[3], lower, strlen(lower));

        intToHex(lower, inst->rt);
        memcpy(&tmpHex[2], lower, strlen(lower));

        intToHex(lower, inst->rs);
        memcpy(&tmpHex[1], lower, strlen(lower));

        intToHex(lower, inst->intInst);
        memcpy(&tmpHex[0], lower, strlen(lower));

        strcpy(inst->hexInst, tmpHex);

    break;
    case ITYPE:
        if (strcmp(inst->inst, "jalr") == 0) {
            intToHex(lower, inst->rt);
            memcpy(&tmpHex[2], lower, strlen(lower));

            intToHex(lower, inst->rs);
            memcpy(&tmpHex[1], lower, strlen(lower));

            intToHex(lower, inst->intInst);
            memcpy(&tmpHex[0], lower, strlen(lower));
        }
        else if (strcmp(inst->inst, "lui") == 0) {
            intToHex(lower, inst->imm);
            memcpy(&tmpHex[6], lower, strlen(lower));

            intToHex(lower, inst->rt);
            memcpy(&tmpHex[2], lower, strlen(lower));
        }
        else {
            intToHex(lower, inst->imm);
            memcpy(&tmpHex[6], lower, strlen(lower));

            if (strcmp(inst->inst, "beq") == 0) {
                intToHex(lower, inst->rs);
                memcpy(&tmpHex[2], lower, strlen(lower));

                intToHex(lower, inst->rt);
                memcpy(&tmpHex[1], lower, strlen(lower));

            }
            else {
                intToHex(lower, inst->rt);
                memcpy(&tmpHex[2], lower, strlen(lower));

                intToHex(lower, inst->rs);
                memcpy(&tmpHex[1], lower, strlen(lower));

            }

            intToHex(lower, inst->intInst);
            memcpy(&tmpHex[0], lower, strlen(lower));
        }
        strcpy(inst->hexInst, tmpHex);

    break;
    case JTYPE:
        intToHex(lower, inst->imm);
        memcpy(&tmpHex[6], lower, strlen(lower));

        intToHex(lower, inst->intInst);
        memcpy(&tmpHex[0], lower, strlen(lower));

        strcpy(inst->hexInst, tmpHex);

    break;
    }

}

