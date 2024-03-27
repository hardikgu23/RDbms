#ifndef EXECUTOR_H
#define EXECUTOR_H

#include"semanticParser.h"

void executeCommand();

void executeCLEAR();
void executeCROSS();
void executeDISTINCT();
void executeEXPORT();
void executeEXPORTMATRIX();
void executeINDEX();
void executeJOIN();
void executeLIST();
void executeLOAD();
void executeLOADMATRIX();
void executePRINT();
void executePRINTMATRIX();
void executeCHECKSYMMETRY();
void executeRENAMEMATRIX();
void executeCOMPUTE();
void executeTRANSPOSEMATRIX();
void executePROJECTION();
void executeRENAME();
void executeSELECTION();
void executeSORT();
void executeORDER();
void executeGROUP();
void executeSOURCE();

bool evaluateBinOp(int value1, int value2, BinaryOperator binaryOperator);
void printRowCount(int rowCount);

#endif