#ifndef SEMANTIC_PARSER_H
#define SEMANTIC_PARSER_H

#include"syntacticParser.h"

bool semanticParse();

bool semanticParseCLEAR();
bool semanticParseCROSS();
bool semanticParseDISTINCT();
bool semanticParseEXPORT();
bool semanticParseEXPORTMATRIX();
bool semanticParseINDEX();
bool semanticParseJOIN();
bool semanticParseLIST();
bool semanticParseLOAD();
bool semanticParseLOADMATRIX();
bool semanticParsePRINT();
bool semanticParsePRINTMATRIX();
bool semanticParseCHECKSYMMETRY();
bool semanticParseRENAMEMATRIX();
bool semanticParseCOMPUTE();
bool semanticParseTRANSPOSEMATRIX();
bool semanticParsePROJECTION();
bool semanticParseRENAME();
bool semanticParseSELECTION();
bool semanticParseSORT();
bool semanticParseORDER();
bool semanticParseGROUP();
bool semanticParseSOURCE();

#endif