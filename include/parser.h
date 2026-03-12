#pragma once 

#include "lexer.h"
#include "ast.h"
#include "common.h"
#include <vector>



class parser {
    public:
    std::vector<token> src;
    u64 indx;
    i64 line;
    parser(const std::vector<token> &a) {
        src = a;
        line = 0;
        indx = 0;
    }
    
    astptr parse_expr();
    astptr parse_unary();
    astptr parse_term();
    astptr parse_factor();
    astptr parse_statement();
    astptr parse_if_statement();
    astptr parse_while_statement();
    astptr parse_for_statement();
    astptr parse_return();
    astptr parse_block();
    astptr parse_func_statement();
    astptr parse_use();
    astptr parse_comparison();
    astptr parse_equality();
    astptr parse_and();
    astptr parse_or();
    astptr parse_break_continue();
    astptr parse_array();
    astptr parse_assignment();
    token consume() {
        if(indx >= src.size()) {
            std::cerr << "Error: (Parser) - unexpected end of input" << "\n";
        }
        while(src[indx].type == NEWLINE) {
            indx++;
            line++;
        }
        return src[indx++];
    }
    token consume(token_type expected) {
        if(indx >= src.size()) {
            std::cerr << "Error: (Parser) - unexpected end of input" << "\n";
            exit(1);
        }
        while(src[indx].type == NEWLINE) {
            indx++;
            line++;
        }
        if(src[indx].type != expected) {
            std::cerr << "Error: (Parser) - unexpected token " << disassemble_tok_type(src[indx].type) << " at line " << line << ", " << "expected " << disassemble_tok_type(expected) << '\n';
            exit(1);
        }
        return src[indx++];
    }
    inline token peek() {
        while(src.at(indx).type == NEWLINE) {
            indx++;
            line++;
        }
        return src.at(indx);
    }
    inline token peek(u8 i) {
        while(src.at(indx+i).type == NEWLINE) {
            i++;
        }
        return src.at(indx+i);
    }
    std::vector<token> peek_(int n) {
        std::vector<token> res;
        for(u64 i=indx;i<indx+n;i++) {
            while(src[indx].type == NEWLINE) {
                i++;
            }
            res.push_back(src.at(i));
        }
        return res;
    }
    std::vector<std::unique_ptr<ASTNode>>parse();
};
