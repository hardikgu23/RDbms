#ifndef GLOBAL_H
#define GLOBAL_H

#include"executor.h"

extern float BLOCK_SIZE;
extern uint BLOCK_COUNT;
extern uint PRINT_COUNT;
extern vector<string> tokenizedQuery;
extern ParsedQuery parsedQuery;
extern TableCatalogue tableCatalogue;
extern MatrixCatalogue matrixCatalogue;
extern BufferManager bufferManager;
extern int MatrixPage_Dimension;
extern int BLOCKS_READ;
extern int BLOCKS_WRITTEN;
extern int BUFFER_SIZE;

#endif