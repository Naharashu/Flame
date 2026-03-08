#pragma once

#include "ast.h"
#include "lexer.h"
#include <memory>
#include <string>
#include <vector>
#include "common.h"

class generator {
    public:
    std::string cpp_code;
    std::string header;
    std::string genCode(astptr &node) {
        switch(node->kind) {
            case ast_type::JUSTNODE: {
                auto n = static_cast<Node*>(node.get());
                if(n->tok.type == BYTE) return std::to_string((char)variant2int<long long>(n->tok.value));
                if(n->tok.type == WORD) return std::to_string((short)variant2int<long long>(n->tok.value));
                if(n->tok.type == INT) return std::to_string((int)variant2int<long long>(n->tok.value));
                if(n->tok.type == LONG) return std::to_string(variant2int<long long>(n->tok.value));
                if(n->tok.type == FLOAT) return std::to_string((float)variant2double(n->tok.value));
                if(n->tok.type == DOUBLE) return std::to_string(variant2double(n->tok.value));
                if(n->tok.type == STRING) return "\"" + variant2string(n->tok.value) + "\"";
                if(n->tok.type == TRUE || n->tok.type == FALSE) return std::to_string(variant2bool(n->tok.value));
                if(n->tok.type == NULL_) return variant2string(n->tok.value);
                if(n->tok.type == ID) return variant2string(n->tok.value);
                break;
            }
            case ast_type::BINARY: {
                auto n = static_cast<BinaryNode*>(node.get());
                std::string op;
                if(n->op==PLUS) op = " + ";
                if(n->op==MINUS) op = " - ";
                if(n->op==STAR) op = " * ";
                if(n->op==SLASH) op = " / ";
                return '(' + genCode(n->left) + op + genCode(n->right) + ')';
            }
            case ast_type::ASSIGN: {
                auto n = static_cast<AssignmentNode*>(node.get());
                std::string value = n->val ? genCode(n->val) : "";
                return " " + variant2string(n->id.value) + (value.empty() ? "" : "=" + value);
            }
            case ast_type::DEFINEVAR: {
                auto n = static_cast<AssignmentNodeExpr*>(node.get());
                std::string type;
                std::string nullval = "0";
                if(n->type_==BYTE_TYPE) type = " int8_t ";
                if(n->type_==WORD_TYPE) type = " int16_t ";
                if(n->type_==INT_TYPE) type = " int32_t ";
                if(n->type_==LONG_TYPE) type = " int64_t ";
                if(n->type_==FLOAT_TYPE) type = " float ";
                if(n->type_==DOUBLE_TYPE) type = " double ";
                if(n->type_==STRING_TYPE) {
                    type = " std::string ";
                    nullval = "\"\"";
                }
                if(n->type_==BOOL_TYPE) type = " bool ";
                if(n->type_==UNSIGNED_8_TYPE) type = " uint8_t ";
                if(n->type_==UNSIGNED_16_TYPE) type = " uint16_t ";
                if(n->type_==UNSIGNED_32_TYPE) type = " uint32_t ";
                if(n->type_==UNSIGNED_64_TYPE) type = " uint64_t ";
                std::string val = n->val ? genCode(n->val) : "";
                return type + variant2string(n->id.value) + (val.empty() ? "=" + nullval : "=" + val);
            }
            case ast_type::UNARY: {
                auto n = static_cast<UnaryNode*>(node.get());
                if(n->sign == MINUS) return "-" + genCode(n->left);
                if(n->sign == NOT) return "!" + genCode(n->left);
                return genCode(n->left);
            }
            case ast_type::FuncCall: {
                auto n = static_cast<FuncCallNode*>(node.get());
                std::string args;
                if(variant2string(n->id.value) == "print") {
                    args = "std::cout << ";
                    for(auto &x : n->args) {
                        args += genCode(x);
                        args += " << ";
                    }
                    args += "std::endl";
                    return args;
                }
                if(variant2string(n->id.value) == "input") {
                    args = "std::cin ";
                    args += " >> ";
                    args += genCode(n->args.at(0));
                    return args;
                }
                for(auto &x : n->args) {
                    args += genCode(x);
                    args += " , ";
                }
                if(args.size()>1) args.erase(args.length() - 2);
                return variant2string(n->id.value) + '(' + args + ')';
            }
            case ast_type::COND: {
                auto n = static_cast<CondNode*>(node.get());
                std::string op;
                if(n->op==LESS) op = " < ";
                if(n->op==BIGGER) op = " > ";
                if(n->op==LEQUAL) op = " <= ";
                if(n->op==BEQUAL) op = " >= ";
                if(n->op==EQUAL) op = " == ";
                if(n->op==NEQUAL) op = " != ";
                return '(' + genCode(n->left) + op + genCode(n->right) + ')';
            }
            case ast_type::MODULE: {
                auto n = static_cast<ModuleNode*>(node.get());
                return "#include \"" + n->name + "\"";
            }
            case ast_type::BLOCK: {
                auto n = static_cast<BlockNode*>(node.get());
                std::string code = " {\n";
                for(auto &x : n->stmts) {
                    code += genCode(x);
                    code += ';';
                }
                code += "\n }";
                return code;
            }
            case ast_type::FUNC: {
                auto n = static_cast<FuncNode*>(node.get());
                std::string code;
                code.reserve(64);
                code += type_in_cpp(n->type);
                code += ' ';
                code += variant2string(n->id.value);
                code += '(';
                for(auto &x : n->args) {
                    code += genCode(x);
                    code += ", ";
                }
                code.erase(code.length()-2);
                code += ") ";
                code += genCode(n->block);
                code += '\n';
                header += code;
                return "";
            }
            case ast_type::FUNC_ARG: {
                auto n = static_cast<ArgumentNode*>(node.get());
                std::string type;
                if(n->type.type==BYTE_TYPE) type = " int8_t ";
                if(n->type.type==WORD_TYPE) type = " int16_t ";
                if(n->type.type==INT_TYPE) type = " int32_t ";
                if(n->type.type==LONG_TYPE) type = " int64_t ";
                if(n->type.type==FLOAT_TYPE) type = " float ";
                if(n->type.type==DOUBLE_TYPE) type = " double ";
                if(n->type.type==STRING_TYPE) {
                    type = " std::string ";
                }
                if(n->type.type==BOOL_TYPE) type = " bool ";
                if(n->type.type==UNSIGNED_8_TYPE) type = " uint8_t ";
                if(n->type.type==UNSIGNED_16_TYPE) type = " uint16_t ";
                if(n->type.type==UNSIGNED_32_TYPE) type = " uint32_t ";
                if(n->type.type==UNSIGNED_64_TYPE) type = " uint64_t ";
                if(n->type.type==VOID_TYPE) type = " void ";
                return type + variant2string(n->id.value);
            }
            case ast_type::RETURN: {
                auto n = static_cast<ReturnNode*>(node.get());
                return "  return (" + genCode(n->value) + ')';
            }
            case ast_type::IF: {
                std::string code;
                auto n = static_cast<IfNode*>(node.get());
                if(n->cond) {
                    code += (n->type == IF ? "if(" : "else if(") + genCode(n->cond) + ")" + genCode(n->block);
                }
                else code += " else " + genCode(n->block);
                if(n->next) code += " " + genCode(n->next);
                return code;
            }
            default:
                return "";
        }
        return "";
    } 

    std::string generate(std::vector<astptr> &nodes) {
        header = "#include <iostream>\n"
                        "#include <cstdint>\n"
                        "using namespace std;\n";
        cpp_code = "int main() {\n";
        for(auto &x : nodes) {
            if(x->kind != ast_type::MODULE && x->kind != ast_type::BLOCK && x->kind != ast_type::IF) cpp_code += genCode(x) + ';';
            else cpp_code += genCode(x);
            cpp_code += '\n';
        }
        cpp_code += "return 0;\n}\n";
        return header + cpp_code;
    }
};
