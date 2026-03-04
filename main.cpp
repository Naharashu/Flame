#include "include/ast.h"
#include "include/lexer.h"
#include "include/parser.h"

int main() { 
    lexer lex;
    std::string code = "1+3-5";
    parser parser_(lex.lex(code));
    for(auto x : lex.lex(code)) {
        std::cout << x.type << ' ';
    }
    std::cout << '\n';
    std::vector<astptr> res = parser_.parse();
    for(auto &x : res) {
        x->print();
    }
    return 0; 
}
