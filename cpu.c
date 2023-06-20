#include<stdio.h>
#include<stdlib.h>
#include "utils.h"
#include "assemble.h"
// eaxh register is a 32 bit integer and we have 16 of them and are initially set to zero
int R[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


void run(struct instruction* inst) {
    switch(inst->instNo) {
        case RTYPE:
            break;
        case ITYPE:
            break;
        case JTYPE:
            break;
    }

}
