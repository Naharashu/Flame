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
    
    astptr parse_expr(token tok);
    astptr parse_term(token tok);
    astptr parse_factor(token tok);
    astptr parse_statement(token tok);
    token consume() {
        if(indx > src.size()) {
            std::cerr << "Error: (Parser) - unexpected end of input" << "\n";
        }
        return src[indx++];
    }
    token consume(token expected) {
        if(indx > src.size()) {
            std::cerr << "Error: (Parser) - unexpected end of input" << "\n";
        }
        if(src[indx].type != expected.type) {
            std::cerr << "Error: (Parser) - unexpected token " << src[indx].type << "\n";
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
