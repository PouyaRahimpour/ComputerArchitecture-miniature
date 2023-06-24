#include "utils.h"

int hexToInt(char* hex) {
    int result=0;
    while ((*hex)!='\0') {
        if (('0'<=(*hex))&&((*hex)<='9'))
            result = result*16 + (*hex) -'0';
        else if (('a'<=(*hex))&&((*hex)<='f'))
            result = result*16 + (*hex) -'a'+10;
        else if (('A'<=(*hex))&&((*hex)<='F'))
            result = result*16 + (*hex) -'A'+10;
        hex++;
    }
    return(result);
}
void intToHex(char* lower, int a) {
    sprintf(lower,"%X",a);
    if(a <0x10){
	lower[4]='\0';
	lower[3]=lower[0];
	lower[2]='0';
	lower[1]='0';
	lower[0]='0';
    }
    else if(a <0x100){
	lower[4]='\0';
	lower[3]=lower[1];
	lower[2]=lower[0];
	lower[1]='0';
	lower[0]='0';
    }
    else if(a <0x1000){
	lower[4]='\0';
	lower[3]=lower[2];
	lower[2]=lower[1];
	lower[1]=lower[0];
	lower[0]='0';
    }
}
int isNumeric(char *str) {
    int numeric = 1;
    if (str[0] == '-') str++;
    while (*str != '\0') {
        if (!isdigit(*str)) {
            numeric = 0;
            break;
        }
        str++;
    }
    return numeric;
}

