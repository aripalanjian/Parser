#include "parser.hpp"
#include <iostream>
#include <algorithm>

// public
Parser::Parser(bool debug, std::string &file){
    this->file = file;
    this->debug = debug;
    line = 1;
    scan = Scanner(file, debug);
    error = false;
    
}

bool Parser::recursiveDescent(std::string const &current){
    if (debug) std::cout << "*RD*Current: " << current << ", next: " << scan.lookahead() << '\n';
    if (current.compare("end_tok") == 0 || error){
        if (error) {
            return false;
        } else {
            line++;
            return true;
        }
    } else if (current.compare("beg_tok") == 0){
        // program
        if (scan.lookahead().compare("newline") == 0){
            stmtSeq();
        } else {
            setError(std::string("expected \'\\n\', found ") + scan.lookahead());
        }
        return recursiveDescent(scan.next());
    } else if (current.compare("comment") == 0){
        //Handle comment
        if (scan.lookahead().compare("newline") == 0){
            line++;
            scan.next();
        } else {
            setError(std::string("expected \'\\n\', found ") + scan.lookahead());
        }
        return recursiveDescent(scan.next());
    } else if (current.compare("newline") == 0){
        line++;
        return recursiveDescent(scan.next());
    } else {
        setError(std::string("Unexpected token: ") + current + " \"" + scan.getCurrentLexeme() + "\"");
        return false;
    }


}

void Parser::stmtSeq(){ // statement { ; statement } ;
    if (debug) std::cout << "*StmtSeq1*Current: " << scan.getCurrent() << ", next: " << scan.lookahead() << '\n';
    while (scan.lookahead().compare("end_tok") != 0 ){
        //TODO: Need to handle eof in case no end_tok & comments
        if (error) break;
        //Expect newline
        if (scan.lookahead().compare("newline") == 0){
            line++;
            scan.next();
            statement();
            // if(scan.lookahead().compare("comment") == 0) scan.next();
            if (debug) std::cout << "*StmtSeq2*Current: " << scan.getCurrent() << ", next: " << scan.lookahead() << '\n';
        }else {
            setError(std::string("expected \'\\n\', found ") + scan.lookahead());
        }
        
    }
}

void Parser::statement(){ // assign | return | condition | loop
    if (debug) std::cout << "*Stmt1*Current: " << scan.getCurrent() << ", next: " << scan.lookahead() << '\n';
    //Expect ident
    if (scan.lookahead().compare("ident") == 0){
        //Future move to assign if 'ident'
        scan.next();
        //Expect '='
        if (scan.lookahead().compare("assign_op") == 0){
            rvalue = true;
            scan.next();
            expression();
            if (debug) std::cout << "*Stmt2*Current: " << scan.getCurrent() << ", next: " << scan.lookahead() << '\n';
            //Expect ';'
            if (scan.lookahead().compare("term_op") != 0){
                setError(std::string("expected \';\', found ") + scan.lookahead());
            } else {
                scan.next();
                if (debug) std::cout << "*Stmt3*Current: " << scan.getCurrent() << ", next: " << scan.lookahead() << '\n';
                if (scan.lookahead().compare("ident") == 0){
                    statement();
                } else if (scan.lookahead().compare("newline") != 0){
                    setError(std::string("expected {\'\\n\',identifier}, found ") + scan.lookahead());
                }
            }
        } else {
            setError(std::string("expected \'=\', found ") + scan.lookahead());
        }
    } else if (scan.lookahead().compare("end_tok") == 0){
        //Do nothing
    } else if (scan.lookahead().compare("comment") == 0){
        scan.next();
    } else {
        setError(std::string("expected \'identifier\', found ") + scan.lookahead());
    }
}

void Parser::expression(){ // term { ( add_op | sub_op ) term}
    if (debug) std::cout << "*Expr1*Current: " << scan.getCurrent() << ", next: " << scan.lookahead() << '\n';
    term(); //Current token '=' | '('
    if (debug) std::cout << "*Expr2*Current: " << scan.getCurrent() << ", next: " << scan.lookahead() << '\n';
    std::unordered_set<std::string> exprOpr{"add_op","sub_op"};
    while (exprOpr.find(scan.lookahead()) != exprOpr.end()){
        scan.next();
        if (debug) std::cout << "*Expr3*Current: " << scan.getCurrent() << ", next: " << scan.lookahead() << '\n';
        if (error) break;
        term();
    }
}

void Parser::term(){ // factor { ( mult_op | div_op ) factor}
    if (debug) std::cout << "*Term1*Current: " << scan.getCurrent() << ", next: " << scan.lookahead() << '\n';
    factor();
    if (debug) std::cout << "*Term2*Current: " << scan.getCurrent() << ", next: " << scan.lookahead() << '\n';
    std::unordered_set<std::string> termOpr{"mult_op","div_op","mod_op"};
    while (termOpr.find(scan.lookahead()) != termOpr.end()){
        if (debug) std::cout << "*Term3*Current: " << scan.getCurrent() << ", next: " << scan.lookahead() << '\n';
        if (error) break;
        scan.next(); // Current '*' | '/' | '%'
        factor();
    }

}

void Parser::factor(){ 
    if (debug) std::cout << "*Fact1*Current: " << scan.getCurrent() << ", next: " << scan.lookahead() << '\n';
    // Expect ident | num_val | lParen 
    if (scan.lookahead().compare("ident") == 0 || scan.lookahead().compare("num_val") == 0){
        scan.next(); //Current ident | num_val
    } else if (scan.lookahead().compare("lParen") == 0){
        scan.next(); //Current '('
        expression();
        if (debug) std::cout << "*Fact2*Current: " << scan.getCurrent() << ", next: " << scan.lookahead() << '\n';
        //Expect rParen
        if (scan.lookahead().compare("rParen") != 0){
            setError(std::string("expected \')\' found ") + scan.lookahead());
        } else {
            scan.next();
        }
    } else {
        setError(std::string("expected {\'identifier\',\'numerical value\',\'(\'} found ") + scan.lookahead());
    }
}

void Parser::parse(){
    valid = recursiveDescent(scan.next());

    if (valid){
        std::cout << "The program " << file << " is legal: \"success\".\n";
        scan.printIdent();
    } else {
        std::cout << "Line " << line << " contains error " << errorMsg << ".\n";
    }
}