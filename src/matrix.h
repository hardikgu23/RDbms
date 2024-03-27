#ifndef MATRIX_H
#define MATRIX_H

#include "cursor.h"

/**
 * @brief The Matrix class holds all information related to a loaded Matrix. It
 * also implements methods that interact with the parsers, executors, cursors
 * and the buffer manager. There are typically 2 ways a table object gets
 * created through the course of the workflow - the first is by using the LOAD
 * command and the second is to use assignment statements (SELECT, PROJECT,
 * JOIN, SORT, CROSS and DISTINCT). 
 *
 */


class Matrix
{

public:
    string sourceFileName = "";
    string MatrixName = "";
    int dimension = 0; // square matrix of dimension x dimension
    int blockCount = 0; // total number of blocks used to store this matrix will be blockCount x blockCount
    
    Matrix();
    Matrix(string MatrixName);
    bool blockify();
    bool load();
    // TODO[pending]: implement unload
    void unload() {};
    void print();
    bool isPermanent();
    void makePermanent();
    bool isSymmetric();
    void transpose();
    void compute();
    void rename(string newMatrixName);
};

#endif