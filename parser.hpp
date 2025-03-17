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
    std::stack<std::string> paren;
    std::stack<std::string> bracket;
    bool debug;
    unsigned line;
    bool valid;
    bool rvalue;
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

public:
    Parser(bool debug, std::string &file);
    void parse();
};

#endif
