#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <vector>
#include <string>
#include <utility>
#include <unordered_map>
#include <unordered_set>

class Scanner{
    std::unordered_map<std::string, std::string> symTable;
    std::unordered_map<std::string, std::string> invSymTable;
    std::vector<std::string> source;
    std::vector<std::string> tokens;
    std::vector<std::string> lexemes;
    std::string file;
    std::string errorMsg;//Change this to flag numbers and have error handling function, change errorMsg to custErrno

    bool debug;

    void readFile();

    // After Reading Lexical Analysis
    size_t tokIter;
    std::string current;
    int token;
    int nextToken;
    std::string lexeme;
    std::string lastIdent;
    std::vector<std::string> numValues;

    int lookup(char c);
    bool isValidNum(const std::string &);
    bool isValidIden(const std::string &);
    
    void tokenize2();

    // Project 2
    bool varDecl;
    std::unordered_set<std::string> validVars;
    
public:
    Scanner(){}
    Scanner(std::string file, bool debug);

    std::string next() {current = tokens.at(tokIter++); return current;}
    std::string getCurrent() {return current;}
    std::string lookahead() {return (tokIter <= tokens.size() - 1) ?tokens.at(tokIter) : "eof" ;}
    std::string getCurrentLexeme() {return lexemes.at(tokIter);}
    void setSymVal(std::string &val){symTable[lastIdent] = val;}
    void printIdent();
    void printTokens();
};

#endif