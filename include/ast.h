#pragma once
#include <memory>
#include <vector>
#include <string>
#include "lexer.h"

class ASTNode {
public:
    virtual ~ASTNode() = default;

    token tok;
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;
    ASTNode(std::unique_ptr<ASTNode> left_, std::unique_ptr<ASTNode> right_, const token &tok_)
    : left(std::move(left_)), right(std::move(right_)), tok(tok_) {}
};

