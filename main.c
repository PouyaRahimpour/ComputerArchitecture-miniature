#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "assemble.h"
#include "cpu.h"

char hexTable[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
char *instructions[] = {"add","sub","slt","or","nand",
    "addi","slti","ori","lui","lw","sw","beq","jalr",
    "j","halt"};
char * directives[] = {".fill", ".space"};
struct instruction mainMem[16*1024];
//int dataMem[16*1024];
int instCnt=0;

// eaxh register is a 32 bit integer and we have 16 of them and are initially set to zero
int R[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


#define ADD 0
#define SUB 1
#define OR 2
#define NAND 3
#define SHIFTLEFT 4

// control unit signals
int RegDst,
    Jump,
    Branch,
    MemRead,
    MemtoReg,
    ALUop,
    MemWrite,
    ALUSrc,
    RegWrite,
    Less,
    Jalr;

void assemble(FILE* assp, FILE* machp) {
    struct symbolTable *pSymTab;
    size_t symTabLen;
    int found,noInsts;
    // create and fill symbol table (first scan)
    symTabLen = findSymTabLen(assp);
    pSymTab = (struct symbolTable *) malloc(symTabLen * sizeof(struct symbolTable));
    fillSymTab(pSymTab, assp);
    for (int x = 0 ; x<symTabLen; x++) {
        printf("%s\t%d\n", pSymTab[x].symbol, pSymTab[x].value);
    }

    // find and form instructions (second scan)

    size_t lineSize;
    int PC = 0;
    char *line;
    char *token;
    int lineNo = 0;
    char instructParts[6][10];
    struct instruction *currInst;
    currInst = (struct instruction *) malloc(sizeof(struct instruction));
    line = (char *)malloc(72);
    while(getline(&line, &lineSize, assp) != -1) {
        lineNo++;
        if(line[0] == '\n' || line[0] == '#') continue;
        printf("%s", line);
        int i = 0, found = 0;
        token = strtok(line ,"\t, \n");
        while (token != NULL) {
            if (token[0] == '#') break;
            strcpy(instructParts[i++], token);
            token = strtok(NULL, "\t, \n");
        }

        int pos, instNo = 15, dirNo = 2;
        // check wether or not it is a directive
        for (pos=0; pos<2 && found!=1; pos++) {
            for (dirNo=0; dirNo<2; dirNo++) {
                if (strcmp(instructParts[pos], directives[dirNo]) == 0) {
                    found = 1;
                    pos--;
                    break;
                }
            }
        }
        // check wether first or second word is an instruction
        if (found!=1)
        for (pos=0; pos<2 && found!=1; pos++) {
            for (instNo=0; instNo<15; instNo++) {
                if (strcmp(instructParts[pos], instructions[instNo]) == 0) {
                    found = 1;
                    pos--;
                    break;
                }
            }
        }
        int isDir = dirNo<2, isInst = instNo<15;
        //printf("%d, %d\n", isDir, isInst);

        currInst->PC = PC;
        if (isDir) {
            int immidiate = atoiImproved(instructParts[++pos], pSymTab, symTabLen);
            struct instruction tmp;
            tmp.intInst = 15;
            switch(dirNo) {
            case 0:
                tmp.imm = immidiate;
                fprintf(machp, "%d\n", immidiate);
                mainMem[PC] = tmp;

            break;

            case 1:
                // allocates space in memory.
                tmp.imm = 0;
                fprintf(machp, "%d\n", 0);
                mainMem[PC] = tmp;

            break;
            default:
                printf("invalid dirNo!\n");
                exit(EXIT_FAILIURE);

            }
            PC++;
            instCnt++;
        }
        else if (isInst) {
            currInst->intInst = instNo;

            strcpy(currInst->inst, instructions[instNo]);

            if (isRType(instNo)) {
                currInst->instType = RTYPE;
                currInst->rd = atoiImproved(instructParts[++pos], pSymTab, -1);
                currInst->rs = atoiImproved(instructParts[++pos], pSymTab, -1);
                currInst->rt = atoiImproved(instructParts[++pos], pSymTab, -1);
                sprintf(currInst->mnemonic, "%s\t%d,%d,%d",
                        instructions[currInst->intInst],
                        currInst->rd,
                        currInst->rs,
                        currInst->rt);
                formInst(currInst);
                fprintf(machp, "%d\n", hexToInt(currInst->hexInst));
                mainMem[instCnt] = *currInst;

            }
            else if (isIType(instNo)) {
                currInst->instType = ITYPE;
                currInst->rt = atoiImproved(instructParts[++pos], pSymTab, -1);
                if (instructions[instNo] == "jalr") {
                    currInst->rs = atoiImproved(instructParts[++pos], pSymTab, -1);
                    sprintf(currInst->mnemonic, "%s\t%d,%d",
                            instructions[currInst->intInst],
                            currInst->rt,
                            currInst->rs);

                }
                else if (instructions[instNo] == "lui") {
                    currInst->imm = atoiImproved(instructParts[++pos], pSymTab, symTabLen);
                    sprintf(currInst->mnemonic, "%s\t%d,%d",
                            instructions[currInst->intInst],
                            currInst->rt,
                            currInst->imm);

                }
                else {
                    currInst->rs = atoiImproved(instructParts[++pos], pSymTab, -1);
                    currInst->imm = atoiImproved(instructParts[++pos], pSymTab, symTabLen);
                    if (strcmp(instructions[instNo], "beq") == 0) currInst->imm -= (PC+1);// relative addressing for beq
                    if (currInst->imm > 1<<16) {
                        printf("offset bigger than 2^16 in line %d.\n", lineNo);
                        exit(EXIT_FAILIURE);
                    }

                    sprintf(currInst->mnemonic, "%s\t%d,%d,%d",
                            instructions[currInst->intInst],
                            currInst->rt,
                            currInst->rs,
                            currInst->imm);
                }
                formInst(currInst);
                fprintf(machp, "%d\n", hexToInt(currInst->hexInst));
                mainMem[instCnt] = *currInst;

            }
            else if (isJType(instNo)) {
                currInst->instType = JTYPE;
                if (strcmp(instructions[instNo], "j") == 0) {
                    currInst->imm = atoiImproved(instructParts[++pos], pSymTab, symTabLen);
                    sprintf(currInst->mnemonic, "j\t%d", currInst->imm);
                }
                else if (strcmp(instructions[instNo], "halt") == 0) {
                    currInst->imm = 0;
                    strcpy(currInst->mnemonic, "halt");
                }

                formInst(currInst);
                fprintf(machp, "%d\n", hexToInt(currInst->hexInst));
                mainMem[instCnt] = *currInst;

            }
            PC++;
            instCnt++;
        }
        else {
        // instruction does not exist
            printf("invalid instruction in line %d.\n", lineNo);
            exit(EXIT_FAILIURE);

        }

        printf("hex instruction: %s\n", currInst->hexInst);
        printf("PC value: %d\n", currInst->PC);

        printf("----------------------------------------\n");
    }

    free(line);
    free(currInst);
    free(pSymTab);
    printf("______________________________\n|  asseble done succesfully  |\n|____________________________|\n");
}

void controlUnit(struct instruction currInst) {
    // RegDst
    RegDst = (currInst.instType == RTYPE);

    // Jump
    Jump = (currInst.intInst==13); // j

    // Branch
    Branch = (currInst.intInst==11); // beq

    // MemRead
    MemRead = (currInst.intInst==9); // lw

    // MemtoReg
    MemtoReg = (currInst.intInst==9); // lw


    // ALUop
    // 14 halt??

    int add = (currInst.intInst==0)|| // add
            (currInst.intInst==5)|| // addi
            (currInst.intInst==9)|| // lw
            (currInst.intInst==10); // sw

    int sub = (currInst.intInst==1)|| // sub
            (currInst.intInst==2)||  // slt
            (currInst.intInst==6); // slti

    int or = (currInst.intInst==3)|| // or
            (currInst.intInst==7); // ori

    int nand = (currInst.intInst==4); // nand

    int shift = (currInst.intInst==8); // lui


    if (add) ALUop = ADD;
    if (sub) ALUop = SUB;
    if (or) ALUop = OR;
    if (nand) ALUop = NAND;
    if (shift) ALUop = SHIFTLEFT;

    // MemWrite
    MemWrite = (currInst.intInst==10); //sw

    // ALUSrc
    ALUSrc = (currInst.instType==ITYPE && currInst.intInst!=11); // all ITypes except for beq

    // RegWrite
    RegWrite = (currInst.instType==RTYPE)||
                (currInst.intInst==5)|| // addi
                (currInst.intInst==7)|| // ori
                (currInst.intInst==6)|| // slti
                (currInst.intInst==9)|| // lw
                (currInst.intInst==12)|| // jalr
                (currInst.intInst==8); // lui
    // Less
    Less = (currInst.intInst==2)|| // slt
            (currInst.intInst==6); // slti
    // Jalr
    Jalr = (currInst.intInst==12); // jalr

}
void printSignals() {
    printf("------------------------\n");
    printf("control signals:\n");
    printf("RegDst: %d\n", RegDst);
    printf("Jump: %d\n", Jump);
    printf("Branch: %d\n", Branch);
    printf("MemRead: %d\n", MemRead);
    printf("MemtoReg: %d\n", MemtoReg);
    printf("ALUop: %d\n", ALUop);
    printf("MemWrite: %d\n", MemWrite);
    printf("ALUSrc: %d\n", ALUSrc);
    printf("RegWrite: %d\n", RegWrite);
    printf("Less: %d\n", Less);
    printf("Jalr: %d\n", Jalr);
    printf("\n");
}
int ALUSrcMux(int rt, int imm) {
    if (ALUSrc) return imm;
    else return rt;
}
int RegDstMux(int rt, int rd) {
    if (RegDst) return rd;
    else return rt;
}
int MemtoRegMux(int ALUout, int memOut) {
    if (MemtoReg) return memOut;
    else return ALUout;
}
int BranchMux(int inp, int jalOut, int zero) {
    if (Branch && zero) return inp;
    else return jalOut;
}
int JumpMux(int branchOut,int imm) {
    if (Jump) return imm;
    else return branchOut;
}
int LessMux(int ALUout, int less) {
    if (Less) return less;
    else return ALUout;
}
int JalrMux(int busA, int inp) {
    if (Jalr) return busA;
    else return inp;
}
int JalrWritebackMux(int memToRegMuxOut, int inp) {
    if (Jalr) return inp;
    else return memToRegMuxOut;
}
int ALU(int srcA, int srcB, int *zero, int* less) {
    int ALUout;
    switch(ALUop) {
        case ADD:
            ALUout = srcA+srcB;
            break;
        case SUB:
            ALUout = srcA-srcB;
            break;
        case OR:
            ALUout = srcA|srcB;
            break;
        case NAND:
            ALUout = ~(srcA&srcB);
            break;
        case SHIFTLEFT:
            ALUout = srcB << 16;
            break;
    }
    *zero = (srcA-srcB==0)? 1: 0;
    *less = (srcA-srcB<0)? 1: 0;
    return ALUout;
}
void registerFileDecode(int rs, int rt, int *busA, int *busB) {
    *busA = R[rs];
    *busB = R[rt];
}
int mem(int addr, int writeData) {
    struct instruction tmp;
    if (MemWrite) {
        tmp.imm = writeData;
        tmp.intInst = 15;
        mainMem[addr] = tmp;
    }
    if (MemRead) {
        tmp = mainMem[addr];
        if (tmp.intInst<15) return mainMem[addr].PC;
        else return mainMem[addr].imm;
    }
    return 0;
}
void registerFileWriteback(int writeData, int writeRegister) {
    if (RegWrite) R[writeRegister] = writeData;
}
void printRegisters() {
    for (int i=0; i<16; i++) {
        printf("Reg[%d]: %u\n", i, R[i]);
    }
    printf("\n");
}
void cpuCycle(int PC) {
    if (PC >= instCnt) {
        printf("the end:)\n");
        return;
    }

    // instruction fetch
    struct instruction currInst = mainMem[PC];
    if (currInst.intInst==14) {
        printf("halt\n");
        return;
    }
    else if (currInst.intInst==15) {
        // means it's just data, not an instruction
        cpuCycle(PC+1);
        return;
    }

    controlUnit(currInst);
    printf("%s\n", currInst.mnemonic);
    printf("PC: %d\n", PC);
    //printSignals();

    // decode: register file
    int busA, busB;
    registerFileDecode(currInst.rs, currInst.rt, &busA, &busB);
    printf("%d, %d\n", busA, busB);

    // exe: alu
    int zero, less;
    int ALUout = ALU(busA, ALUSrcMux(busB, currInst.imm), &zero, &less);
    ALUout = LessMux(ALUout, less);

    // memory
    int memOut = mem(ALUout, busB);

    // writeback: register file
    int memToRegMuxOut = MemtoRegMux(ALUout, memOut);
    int jalrWritebackOut = JalrWritebackMux(memToRegMuxOut, PC+1);
    int writeRegister = RegDstMux(currInst.rt, currInst.rd);
    registerFileWriteback(jalrWritebackOut, writeRegister);

    // calculate next PC state
    int jalOut = JalrMux(busA, PC+1);
    int branchOut = BranchMux(currInst.imm+PC+1, jalOut, zero);
    PC = JumpMux(branchOut, currInst.imm);

    printRegisters();
    // next cycle
    cpuCycle(PC);
}


int main(int argc, char **argv) {
    FILE *assp,*machp;
    if(argc < 3){
      printf("***** Please run this program as follows:\n");
      printf("***** %s assprog.as machprog.m\n",argv[0]);
      printf("***** where assprog.as is your assembly program\n");
      printf("***** and machprog.m will be your machine code.\n");
      exit(EXIT_FAILIURE);
    }
    if((assp=fopen(argv[1],"r")) == NULL){
      printf("%s cannot be opened\n",argv[1]);
      exit(EXIT_FAILIURE);
    }
    if((machp=fopen(argv[2],"w+")) == NULL){
      printf("%s cannot be opened\n",argv[2]);
      exit(EXIT_FAILIURE);
    }

    assemble(assp, machp);
    int PC = 0;
    cpuCycle(PC);

    fclose(assp);
    fclose(machp);
    return 0;
}


