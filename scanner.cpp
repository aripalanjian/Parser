#include "scanner.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cctype>

Scanner::Scanner(std::string file, bool debug){
    this->file = file;
    this->debug = debug;
    tokIter = 0;
    readFile();
    tokenize2();

    if (debug) {
        printIdent();
        printTokens();
    }
}

void Scanner::readFile(){
    std::string line;
    std::fstream in(file);

    if (in.is_open()){
        while(getline(in, line)){
            line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
            source.emplace_back(line);
        }

        if (debug) std::cout << "File contains " << source.size() << " lines\n";
//loop until op then add to map
        in.close();
    } else {
        std::cerr << "Error: " << errno << ", file unable to open or is not recognized";
    }
}

bool Scanner::isValidIden(const std::string &identifier){
    bool isValid = true;
    int consecUnderScores = 0;
    for (int i = 0; i < identifier.size(); i++) {
        if (consecUnderScores > 1) {
            isValid = false;
            errorMsg = "Invalid Identifier";
        }

        if ( identifier.at(i) == '_') {
            consecUnderScores++;
            if (i == identifier.size() - 1 || i == 0){
                isValid = false;
                errorMsg = "Invalid Identifier";
            }
        }
    }

    if (isValid) {
        symTable.insert({identifier, ""});
        tokens.emplace_back("ident");
    } else {
        invSymTable.insert({identifier, ""});
        tokens.emplace_back("inval_ident");
    }
    lexemes.push_back(identifier);
    if (debug) std::cout << "Current Lexeme: " << lexeme << '\n';
    return isValid;
}

bool Scanner::isValidNum(const std::string &num){
    int numPer = 0;
    for (int i = 0; i < num.size(); i++) {
        if ( num.at(i) == '.') {
            numPer++;
        }
    }
    bool isValid = (numPer <= 1) ? true:false;
    if (isValid){ 
        numValues.emplace_back(num);
        tokens.emplace_back("num_val");
    } else {
        tokens.emplace_back("inval_num");
    }
    lexemes.push_back(num);
    if (debug) std::cout << "Current Lexeme: " << lexeme << '\n';
    return isValid;
}


void Scanner::tokenize2(){
    //Add functionality for lvalue and rvalue std::string prevToken; if (lookup(c) == '=') set flag to store next expr as prevToken val;
    std::string lastIdent = "";
    for (int i = 0; i < source.size(); i++){
        lexeme = "";
        std::string line = source.at(i);
        if (line.compare("begin") == 0){
            tokens.emplace_back("beg_tok");
            lexemes.push_back(line);
        } else if (line.compare("end.") == 0){
            tokens.emplace_back("end_tok");
            lexemes.push_back(line);
            break;
        } else {
            for(int i = 0; i < line.size(); i++){

                char c = line.at(i);
                if (isalpha(c)){
                    lexeme.push_back(c);
                    if (i < line.size() - 1){
                        c = line.at(i+1);
                        while(isalnum(c) || c == '_'){
                            i++;
                            lexeme.push_back(c);
                            if (i < line.size() - 1){
                                c = line.at(i+1);
                            } else {
                                isValidIden(lexeme);
                                break;
                            }
                        }
                    } else {
                        isValidIden(lexeme);
                        break;
                    }
                    isValidIden(lexeme);
                    
                } else if (isdigit(c)){
                    lexeme.push_back(c);
                    if (i < line.size() - 1){
                        c = line.at(i+1);
                        while(isdigit(c) || c == '.'){
                            i++;
                            lexeme.push_back(c);
                            if (i < line.size() - 1){
                                c = line.at(i+1);
                            } else {
                                isValidNum(lexeme);
                                break;
                            }
                        }
                    } else {
                        isValidNum(lexeme);
                        break;
                    }
                    isValidNum(lexeme);
                } else {
                    if (c == '~'){
                        tokens.emplace_back("comment");
                        lexemes.push_back(line);
                        break;
                    } else {
                        lookup(c);
                        // i++;
                        lexeme = "";
                    }
                    
                }
            }
        }
        tokens.emplace_back("newline");
        lexemes.emplace_back("\n");
    }
}

int Scanner::lookup(char current){
    if (current == '='){
        tokens.emplace_back("assign_op");
    } else if (current == '('){
        tokens.emplace_back("lParen");
    } else if (current == ')'){
        tokens.emplace_back("rParen");
    } else if (current == '['){
        tokens.emplace_back("lBracket");
    } else if (current == ']'){
        tokens.emplace_back("rBracket");
    } else if (current == ';'){
        tokens.emplace_back("term_op");
    } else if (current == '+'){
        tokens.emplace_back("add_op");
    } else if (current == '-'){
        tokens.emplace_back("sub_op");
    } else if (current == '*'){
        tokens.emplace_back("mult_op");
    } else if (current == '/'){
        tokens.emplace_back("div_op");
    } else if (current == '%'){
        tokens.emplace_back("mod_op");
    } else {
        return -1;
    }
    lexemes.push_back(std::string() + current);
    if (debug) std::cout << "Current Operator: " << current << '\n';
    return 0;
}

void Scanner::printIdent(){
    std::cout << "Valid Identifiers: \n";
    for(auto pair : symTable){
        std::cout << "Key: [" << pair.first << "], Value: [" << pair.second << "]\n";
    }
    std::cout << '\n';
}

void Scanner::printTokens(){
    std::cout << "Source:\n";
    for ( int i = 0; i < tokens.size(); i++){
        if (tokens.at(i).compare("newline") != 0){
            std::cout << tokens.at(i) << " : " << lexemes.at(i) << '\n';
        } else {
            std::cout << tokens.at(i) << " : \n";
        }
    }
}

/*
Tokens: comment, string literal, (valid/invalid)identifier, assignment operator, numerical literal, terminator , beg, end, lParen, rParen, lBracket, rBracket, arithmetic operator
*/