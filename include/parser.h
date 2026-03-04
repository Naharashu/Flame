#pragma once 

#include "lexer.h"
#include "ast.h"
#include "common.h"
#include <vector>


class parser {
    public:
    std::vector<token> src;
    u64 indx = 0;
    parser(const std::vector<token> &a) {
        src = a;
    }
    
    astptr parse_expr();
    astptr parse_unary();
    astptr parse_term();
    astptr parse_factor();
    astptr parse_statement();
    astptr parse_if_statement();
    astptr parse_while_statement();
    astptr parse_for_statement();
    astptr parse_func_statement();
    astptr parse_assignment();
    token consume() {
        if(indx > src.size()) {
            std::cerr << "Error: (Parser) - unexpected end of input" << "\n";
        }
        return src[indx++];
    }
    token consume(token_type expected) {
        if(indx >= src.size()) {
            std::cerr << "Error: (Parser) - unexpected end of input" << "\n";
            exit(1);
        }
        if(src[indx].type != expected) {
            std::cerr << "Error: (Parser) - unexpected token " << src[indx].type << "\n";
            exit(1);
        }
        return src[indx++];
    }
    std::vector<token> peek(int n) {
        std::vector<token> res;
        for(u64 i=indx;i<indx+n;i++) {
            res.push_back(src.at(i));
        }
        return res;
    }
    std::vector<std::unique_ptr<ASTNode>>parse();
};
