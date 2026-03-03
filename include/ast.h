#pragma once
#include <memory>
#include <vector>
#include <string>
#include "lexer.h"

class ASTNode;

using astptr = std::unique_ptr<ASTNode>;

class ASTNode {
public:
    virtual ~ASTNode() = default;
};


class BinaryNode : public ASTNode {
	public:
	token op;
	astptr left;
	astptr right;
	BinaryNode(astptr left_, astptr  right_, const token &op_) 
	            : left(std::move(left_)), right(std::move(right_)), op(op_) {};
};
