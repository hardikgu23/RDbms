#include "global.h"

void MatrixCatalogue::insertMatrix(Matrix* matrix)
{
    logger.log("MatrixCatalogue::insertMatrix"); 
    this->matrices[matrix->MatrixName] = matrix;
}
void MatrixCatalogue::deleteMatrix(const string &matrixName)
{
    logger.log("MatrixCatalogue::deleteMatrix"); 
    // Todo[Pending]: Implement unload function for matrix
    this->matrices[matrixName]->unload();
    delete this->matrices[matrixName];
    this->matrices.erase(matrixName);
}
void MatrixCatalogue::deleteMatrixFromCatalogue(const string &matrixName)
{
    logger.log("MatrixCatalogue::deleteMatrixFromCatalogue"); 
    this->matrices.erase(matrixName);
}
Matrix* MatrixCatalogue::getMatrix(const string &matrixName)
{
    logger.log("MatrixCatalogue::getMatrix"); 
    Matrix *matrix = this->matrices[matrixName];
    return matrix;
}
bool MatrixCatalogue::isMatrix(const string &matrixName)
{
    logger.log("MatrixCatalogue::isMatrix"); 
    if (this->matrices.count(matrixName))
        return true;
    return false;
}

void MatrixCatalogue::print()
{
    logger.log("MatrixCatalogue::print"); 
    cout << "\nMATRICES" << endl;

    int rowCount = 0;
    for (auto rel : this->matrices)
    {
        cout << rel.first << endl;
        rowCount++;
    }
    printRowCount(rowCount);
}

MatrixCatalogue::~MatrixCatalogue(){
    logger.log("MatrixCatalogue::~MatrixCatalogue"); 
    for(auto matrix: this->matrices){
        matrix.second->unload();
        delete matrix.second;
    }
}
