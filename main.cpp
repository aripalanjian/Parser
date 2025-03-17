#include "scanner.hpp"
#include "parser.hpp"
#include <iostream>

int main(int argC, char** argV){
    std::string file;
    bool debug = false;

    if (argC > 1){
        file = argV[1];
        if (argC > 2){
            for (int i = 2; i < argC; i++){
                if (std::string("-d").compare(argV[i]) == 0){
                    std::cout << "Debug Mode\n\n";
                    debug = true;
                }
            }
        }
    }

    Parser parser(debug, file);
    parser.parse();
}