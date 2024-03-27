#include"global.h"

// helper function to check if a string is present in tokenizedQuery or not?
// if present returns the index at which present else returns -1
int isPresent(std::string str)
{
    for(int i=0; i<tokenizedQuery.size(); ++i)
    {
        if(tokenizedQuery[i] == str)
            return i;
    }
    return -1;
}

/**
 * @brief File contains method to process SORT commands.
 * 
 * syntax:
 * SORT <table_name> BY <column_name1, column_name2,..., column_namek> IN
 *  <ASC|DESC, ASC|DESC,..., ASC|DESC>
 */
bool syntacticParseSORT(){
    logger.log("syntacticParseSORT");
    
    bool isValid = true;
    if(tokenizedQuery.size() <6){
        isValid = false;
    }

    // ensuring BY and IN are present, and equal (>0) number of column name and sortingOrder are there
    int ind1 = isPresent("BY");
    int ind2 = isPresent("IN");

    if(ind1 == -1 || ind2 == -1)
    {
        isValid = false;
    }

    int numColumns = ind2 - ind1 -1;
    int numSortingOrder = tokenizedQuery.size() - ind2 -1;

    if( (numColumns<1) || (numColumns != numSortingOrder))
        isValid = false;

    if(!isValid)
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }

    parsedQuery.queryType = SORT;
    parsedQuery.sortRelationName = tokenizedQuery[1];
    
    // storing the column name on which ordering is to be done
    for(int i=ind1+1; i <= ind1 + numColumns; ++i)
    {
        parsedQuery.sortColumnName.push_back(tokenizedQuery[i]);
    }

    // storing the order in which these columns have to be sorted
    // 0 -> Descending
    // 1 -> Ascending
    for(int i=ind2+1; i <= ind2 + numSortingOrder; ++i)
    {
        if(tokenizedQuery[i] == "ASC")
            parsedQuery.sortOrder.push_back(1);
        else if(tokenizedQuery[i] == "DESC")
            parsedQuery.sortOrder.push_back(0);
        else
        {
            cout << "SYNTAX ERROR" << endl;
            return false;
        }
    }
    return true;
}

bool semanticParseSORT(){
    logger.log("semanticParseSORT");

    if(!tableCatalogue.isTable(parsedQuery.sortRelationName)){
        cout<<"SEMANTIC ERROR: Relation doesn't exist"<<endl;
        return false;
    }

    // checking if all column names are valid or not?
    for (auto columnName : parsedQuery.sortColumnName)
    {
        if(!tableCatalogue.isColumnFromTable(columnName, parsedQuery.sortRelationName)){
            cout<<"SEMANTIC ERROR: Column doesn't exist in relation"<<endl;
            return false;
        }
    }
    return true;
}

void executeSORT(){
    logger.log("executeSORT");
    Table* table = tableCatalogue.getTable(parsedQuery.sortRelationName);
    table->sort(parsedQuery.sortColumnName, parsedQuery.sortOrder, table->tableName);
    return;
}