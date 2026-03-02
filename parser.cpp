#include "include/parser.h"
#include "include/ast.h"
#include "include/common.h"
#include "include/lexer.h"
#include <memory>

astptr parser::parse_factor(token tok) {
    if(tok.type == token_type::INT) {
        auto a = std::make_unique<ASTNode>(NULL, NULL, tok);
    }
}

astptr parser::parse_term(token tok) {
    if(tok.type==token_type::STAR) {
        token a = consume();
        astptr fac = parse_factor(a);
        auto term = std::make_unique<ASTNode>();
    }
}


astptr parser::parse_expr(token tok) {
    astptr binnode = nullptr;
    if(tok.type == INT) {
        token a = consume();
        astptr term = parse_term(a);
    }
}

astptr parser::parse_statement(token tok) {
    astptr node = nullptr;
    if(tok.type == token_type::IF) {

    }

}

std::vector<std::unique_ptr<ASTNode>> parser::parse() {
    std::vector<std::unique_ptr<ASTNode>> parsed;
    while(indx<src.size()) {
        parse_statement(consume());
    }
    return parsed;
}