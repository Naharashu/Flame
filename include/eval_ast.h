#pragma once
#include "ast.h"
#include "ast.h"
#include "common.h"
#include "lexer.h"
#include "table.h"
#include <cstddef>

class eval_ast {
    public:
    std::string eval_string(astptr &node) {
        auto n = static_cast<Node*>(node.get());
        return variant2string(search(variant2string(n->tok.value)).value);
    }
    template<typename T>
    T eval(astptr &node) {
        switch (node->kind) {
            case ast_type::JUSTNODE: {
                auto n = static_cast<Node*>(node.get());
                if(n->tok.type==ID) {
                    return variant2int<T>(search(variant2string(n->tok.value)).value);
                }
                return variant2int<T>(n->tok.value);
            }
            case ast_type::BINARY: {
                auto n = static_cast<BinaryNode*>(node.get());
                switch(n->op) {
                    case PLUS: {
                        return eval<T>(n->left) + eval<T>(n->right);
                    }
                    case MINUS: {
                        return eval<T>(n->left) - eval<T>(n->right);
                    }
                    case STAR: {
                        return eval<T>(n->left) * eval<T>(n->right);
                    }
                    case SLASH: {
                        return eval<T>(n->left) / eval<T>(n->right);
                    }
                    case MOD: {
                        return eval<T>(n->left) % eval<T>(n->right);
                    }
                    default:
                        break;
                }
            }
            default:
                return 0;
        }
    }
};
