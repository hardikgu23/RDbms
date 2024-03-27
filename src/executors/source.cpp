#include "global.h"
/**
 * @brief 
 * SYNTAX: SOURCE filename
 */
bool syntacticParseSOURCE()
{
    logger.log("syntacticParseSOURCE");
    if (tokenizedQuery.size() != 2)
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = SOURCE;
    parsedQuery.sourceFileName = tokenizedQuery[1];
    return true;
}

bool semanticParseSOURCE()
{
    logger.log("semanticParseSOURCE");
    if (!isQueryFile(parsedQuery.sourceFileName))
    {
        cout << "SEMANTIC ERROR: File doesn't exist" << endl;
        return false;
    }
    return true;
}

void executeSOURCE()
{
    logger.log("executeSOURCE");
    std::string filePath = "../data/" + parsedQuery.sourceFileName + ".ra";
    // Open the file
    std::ifstream inputFile(filePath);
    
    if (!inputFile.is_open()) {
        logger.log("Error opening file " + filePath);
        return ;
    }

    std::string command;
    // vector<string> tokenizedQuery;
    regex delim("[^\\s,]+");

    while (std::getline(inputFile, command)) {

    tokenizedQuery.clear();
    parsedQuery.clear();

    logger.log("hardik " + command);

    auto words_begin = std::sregex_iterator(command.begin(), command.end(), delim);
        auto words_end = std::sregex_iterator();
        for (std::sregex_iterator i = words_begin; i != words_end; ++i)
            tokenizedQuery.emplace_back((*i).str());

        if (tokenizedQuery.size() == 1 && tokenizedQuery.front() == "QUIT")
        {
            break;
        }

        if (tokenizedQuery.empty())
        {
            continue;
        }

        if (tokenizedQuery.size() == 1)
        {
            cout << "SYNTAX ERROR" << endl;
            continue;
        }
        logger.log("Tokenized query is "+ parsedQuery.queryType);
        if (syntacticParse() && semanticParse())
                executeCommand();

    }
    return;
}
