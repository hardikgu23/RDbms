#ifndef MATRIXCATALOGUE_H
#define MATRIXCATALOGUE_H

#include "matrix.h"

/**
 * @brief The MatrixCatalogue acts like an index of tables existing in the
 * system. Everytime a matrix is added(removed) to(from) the system, it needs to
 * be added(removed) to(from) the MatrixCatalogue. 
 *
 */
class MatrixCatalogue
{

    unordered_map<string, Matrix*> matrices;

public:
    MatrixCatalogue() {}
    void insertMatrix(Matrix* matrix);
    void deleteMatrix(const string &matrixName);
    void deleteMatrixFromCatalogue(const string &matrixName);
    Matrix* getMatrix(const string &matrixName);
    bool isMatrix(const string &matrixName);
    void print();
    ~MatrixCatalogue();
};

#endif