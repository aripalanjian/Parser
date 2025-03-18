#ifndef PARSER_HPP
#define PARSER_HPP

#include "scanner.hpp"
#include <stack>

//tokens add : undef_var, var_dec, type

class Parser{
    std::vector<std::string> tokens;
    std::string file;

    bool error;
    std::string errorMsg;
    bool debug;
    unsigned line;
    bool valid;
    bool lvalue, rvalue;
    std::string lookahead;
    Scanner scan;
    
    // void program(); //beg_tok stmtSeq end_tok
    void stmtSeq(); // statement { ; statement } ;
    void statement(); // assign | return | condition | loop
    // void assign(); // ident assign_op expression
    // void condition(); // 
    void expression(); // term { ( add_op | sub_op ) term}
    void term(); // factor { ( mult_op | div_op ) factor}
    void factor(); // ident | num_val | lParen expression rParen

    void setError(std::string const &errStr){error = true; errorMsg = errStr;}
    bool recursiveDescent(std::string const &);

    void generate();
    
    std::stack<std::string> values;
    std::stack<std::string> operators;
    std::stack<std::string> parens;
    std::string currentLVal;
    std::vector<std::string> currentRVal;
    std::vector<std::string> rpnCode;
    std::unordered_set<std::string> opers{"+","-","*","/","%"};

public:
    Parser(bool debug, std::string &file);
    void parse();
};

#endif
