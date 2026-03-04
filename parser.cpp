#include "include/parser.h"
#include "include/ast.h"
#include "include/common.h"
#include "include/lexer.h"
#include <cstdlib>
#include <memory>
#include <utility>

astptr parser::parse_factor() {
    token tok = consume();
    if(tok.type == token_type::INT) {
        return std::make_unique<Node>(tok);
    }
    else if(tok.type == token_type::L_BRACKET) {
        astptr node = parse_expr();

        token close = consume(token_type::R_BRACKET);

        return node;

    }
    else {
        std::cerr << "Error in parsing factor\n";
        exit(EXIT_FAILURE); 
    }
}

astptr parser::parse_unary() {
    token op = peek(1)[0];
    if (op.type == token_type::PLUS || op.type == token_type::MINUS) {
        consume();
        astptr unary = parse_unary();
        return std::make_unique<UnaryNode>(std::move(unary), op.type);
    }
    return parse_factor();
}

astptr parser::parse_term() {
    astptr node = parse_unary();

    while (true) {
        token op = peek(1)[0];
        if (op.type != token_type::STAR && op.type != token_type::SLASH)
            break; 

        consume();
        astptr rhs = parse_unary();
        node = std::make_unique<BinaryNode>(std::move(node), std::move(rhs), op.type);
    }

    return node;
}


astptr parser::parse_expr() {
    astptr node = parse_term();

    while (true) {
        token op = peek(1)[0]; 
        if (op.type != token_type::PLUS && op.type != token_type::MINUS)
            break; 
        consume();
        astptr rhs = parse_term();
        node = std::make_unique<BinaryNode>(std::move(node), std::move(rhs), op.type);
    }

    return node;
}

astptr parser::parse_statement() {
    token tok = peek(1)[0];
    /*
    switch(tok.type) {
        case token_type::IF:
            return parse_if_statement();
        case token_type::WHILE:
            return parse_while_statement();
        case token_type::FOR: 
            return parse_for_statement();
        case token_type::FUNC:
            return parse_func_statement();
        case token_type::ID: 
            return parse_assignment();
        default:
            return parse_expr();  
    }
    */
    return parse_expr();
}

std::vector<std::unique_ptr<ASTNode>> parser::parse() {
    std::vector<std::unique_ptr<ASTNode>> parsed;
    while(indx<src.size() && src[indx].type != token_type::EOF_) {
        parsed.push_back(parse_statement());
    }
    return parsed;
}