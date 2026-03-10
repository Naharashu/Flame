#pragma once

#include "ast.h"
#include "common.h"
#include "lexer.h"
#include "parser.h"
#include <fstream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

/*
if (x->kind == ast_type::MODULE) {
        auto n = static_cast<ModuleNode *>(x.get());
        lexer lex;
        std::string code;
        std::string line;
        std::ifstream mod(n->name.c_str());
        while (std::getline(mod, line)) {
          code += line + '\n';
        }
        mod.close();
        std::vector<token> toks = lex.lex(code);
        parser parser_(toks);
        std::vector<astptr> res = parser_.parse();
        for (auto &y : res) {
          std::string yc = genCode(y);
          if (yc.empty())
            continue;
          if (y->kind != ast_type::MODULE && y->kind != ast_type::BLOCK &&
              y->kind != ast_type::IF)
            cpp_code << yc + ';';
          else
            cpp_code << yc;
          cpp_code << '\n';
        }
        continue;
      }
*/

class generator {
private:
  int indent = 0;

  std::string pad() { return std::string(indent * 4, ' '); }

public:
  std::ostringstream cpp_code;
  std::string header;
  std::string genCode(astptr &node) {
    switch (node->kind) {
    case ast_type::JUSTNODE: {
      auto n = static_cast<Node *>(node.get());
      if (n->tok.type == BYTE)
        return std::to_string((char)variant2int<long long>(n->tok.value));
      if (n->tok.type == WORD)
        return std::to_string((short)variant2int<long long>(n->tok.value));
      if (n->tok.type == INT)
        return std::to_string((int)variant2int<long long>(n->tok.value));
      if (n->tok.type == LONG)
        return std::to_string(variant2int<long long>(n->tok.value));
      if (n->tok.type == FLOAT)
        return std::to_string((float)variant2double(n->tok.value));
      if (n->tok.type == DOUBLE)
        return std::to_string(variant2double(n->tok.value));
      if (n->tok.type == STRING)
        return "\"" + variant2string(n->tok.value) + "\"";
      if (n->tok.type == TRUE || n->tok.type == FALSE)
        return std::to_string(variant2bool(n->tok.value));
      if (n->tok.type == NULL_)
        return variant2string(n->tok.value);
      if (n->tok.type == ID)
        return variant2string(n->tok.value);
      break;
    }
    case ast_type::BINARY: {
      auto n = static_cast<BinaryNode *>(node.get());
      std::string op;
      if (n->op == PLUS)
        op = " + ";
      if (n->op == MINUS)
        op = " - ";
      if (n->op == STAR)
        op = " * ";
      if (n->op == SLASH)
        op = " / ";
      return '(' + genCode(n->left) + op + genCode(n->right) + ')';
    }
    case ast_type::ASSIGN: {
      auto n = static_cast<AssignmentNode *>(node.get());
      std::string value = n->val ? genCode(n->val) : "";
      return " " + variant2string(n->id.value) +
             (value.empty() ? "" : "=" + value);
    }
    case ast_type::DEFINEVAR: {
      auto n = static_cast<AssignmentNodeExpr *>(node.get());
      std::string type;
      std::string nullval = "0";
      if (n->type_ == BYTE_TYPE)
        type = " int8_t ";
      if (n->type_ == WORD_TYPE)
        type = " int16_t ";
      if (n->type_ == INT_TYPE)
        type = " int32_t ";
      if (n->type_ == LONG_TYPE)
        type = " int64_t ";
      if (n->type_ == FLOAT_TYPE)
        type = " float ";
      if (n->type_ == DOUBLE_TYPE)
        type = " double ";
      if (n->type_ == STRING_TYPE) {
        type = " std::string ";
        nullval = "\"\"";
      }
      if (n->type_ == BOOL_TYPE)
        type = " bool ";
      if (n->type_ == UNSIGNED_8_TYPE)
        type = " uint8_t ";
      if (n->type_ == UNSIGNED_16_TYPE)
        type = " uint16_t ";
      if (n->type_ == UNSIGNED_32_TYPE)
        type = " uint32_t ";
      if (n->type_ == UNSIGNED_64_TYPE)
        type = " uint64_t ";
      std::string val = n->val ? genCode(n->val) : "";
      return type + variant2string(n->id.value) +
             (val.empty() ? "=" + nullval : "=" + val);
    }
    case ast_type::UNARY: {
      auto n = static_cast<UnaryNode *>(node.get());
      if (n->sign == MINUS)
        return "-" + genCode(n->left);
      if (n->sign == NOT)
        return "!" + genCode(n->left);
      return genCode(n->left);
    }
    case ast_type::FuncCall: {
      auto n = static_cast<FuncCallNode *>(node.get());
      std::string args;
      std::string name = variant2string(n->id.value);
      if (name == "print") {
        args = "std::cout << ";
        for (auto &x : n->args) {
          args += genCode(x);
          args += " << ";
        }
        args += "std::endl";
        return args;
      }
      if (name == "input") {
        args = "std::cin ";
        args += " >> ";
        args += genCode(n->args.at(0));
        return args;
      }
      for (u64 i = 0; i < n->args.size(); i++) {
        args += genCode(n->args[i]);
        if (i + 1 < n->args.size())
          args += ", ";
      }
      return variant2string(n->id.value) + '(' + args + ')';
    }
    case ast_type::COND: {
      auto n = static_cast<CondNode *>(node.get());
      std::string op;
      if (n->op == LESS)
        op = " < ";
      if (n->op == BIGGER)
        op = " > ";
      if (n->op == LEQUAL)
        op = " <= ";
      if (n->op == BEQUAL)
        op = " >= ";
      if (n->op == EQUAL)
        op = " == ";
      if (n->op == NEQUAL)
        op = " != ";
      return '(' + genCode(n->left) + op + genCode(n->right) + ')';
    }
    case ast_type::MODULE: {
      return "";
    }
    case ast_type::BLOCK: {
      auto n = static_cast<BlockNode *>(node.get());
      std::ostringstream code;
      code << " {\n";
      indent++;
      for (auto &x : n->stmts) {
        code << pad();
        code << genCode(x);
        code << ";\n";
      }
      indent--;
      code << pad() + "}";
      return code.str();
    }
    case ast_type::FUNC: {
      auto n = static_cast<FuncNode *>(node.get());
      std::ostringstream code;
      code << type_in_cpp(n->type);
      code << variant2string(n->id.value);
      code << '(';
      for (u64 i = 0; i < n->args.size(); i++) {
        code << genCode(n->args[i]);
        if (i + 1 < n->args.size())
          code << ", ";
      }
      code << ") ";
      code << genCode(n->block);
      code << '\n';
      header += code.str();
      return "";
    }
    case ast_type::FUNC_ARG: {
      auto n = static_cast<ArgumentNode *>(node.get());
      std::string type = type_in_cpp(n->type);
      return type + variant2string(n->id.value);
    }
    case ast_type::RETURN: {
      auto n = static_cast<ReturnNode *>(node.get());
      return "return (" + genCode(n->value) + ')';
    }
    case ast_type::IF: {
      std::string code;
      auto n = static_cast<IfNode *>(node.get());
      if (n->cond) {
        code += (n->type == IF ? "if(" : "else if(") + genCode(n->cond) + ")" +
                genCode(n->block);
      } else
        code += " else " + genCode(n->block);
      if (n->next)
        code += " " + genCode(n->next);
      return code;
    }
    case ast_type::LOOP: {
      auto n = static_cast<LoopNode *>(node.get());
      std::string code;
      code += " while(";
      code += genCode(n->cond);
      code += ") ";
      code += genCode(n->block);
      return code;
    }
    case ast_type::BREAK: {
      return "break";
    }
    case ast_type::CONTINUE: {
      return "continue";
    }
    case ast_type::VAR_INC_DEC: {
      auto n = static_cast<IncDecVarNode *>(node.get());
      if (n->type == 0) {
        return variant2string(n->id) + "++";
      } else
        return variant2string(n->id) + "--";
    }
    case ast_type::FOR: {
        auto n = static_cast<ForNode*>(node.get());
        std::string code = " for(";
        code += genCode(n->var) + ';';
        code += genCode(n->cond) + ';';
        code += genCode(n->thing);
        code += ')';
        code += genCode(n->block);
        return code;
    }
    case ast_type::REASSIGNVAR: {
      auto n = static_cast<ReAssignmentNodeExpr*>(node.get());
      std::string op;
      switch (n->type_) {
        case PLUS: {
          op = " += ";
          break;
        }
        case MINUS: {
          op = " -= ";
          break;
        }
        case STAR: {
          op = " *= ";
          break;
        }
        case SLASH: {
          op = " /= ";
          break;
        }
        default: break;
      }
      return variant2string(n->id.value) + op + genCode(n->val);
    }
    case ast_type::ARRAY: {
    	auto n = static_cast<ArrayNode*>(node.get());
    	std::string code;
    	code += type_in_cpp(n->type);
    	std::string values_ = "{";
    	if(!n->values.empty()) {
    		for(u64 i=0;i<n->values.size();i++) {
    			values_ += genCode(n->values[i]);
    			if(i+1<n->values.size()) values_ += ", ";
    		}
    		values_ += "}";
    	}
    	else {
    		values_ = "";
    	}
    	if(n->size!=-1) {
    		if(!n->values.empty()) return n->id + "[" + std::to_string(n->size) + "] = " + values_; 
    		return n->id + "[" + std::to_string(n->size) + "]";
    	}
    	else {
    		if(!n->values.empty()) return n->id + "[] = " + values_;
    		return n->id + "[]";
    	}
    }
    case ast_type::ARRAY_ACCESS: {
    	auto n = static_cast<ArrayAccessNode*>(node.get());
    	return variant2string(n->id.value) + "[" + genCode(n->index) + "]";
    }
    default:
      throw std::runtime_error("Unknown AST node");
    }
    return "";
  }

  std::string generate(std::vector<astptr> &nodes) {
    header = "#include <iostream>\n"
             "#include <cstdint>\n";
    for (auto &x : nodes) {
      std::string c = genCode(x);
      if (c.empty())
        continue;

      if (x->kind != ast_type::MODULE && x->kind != ast_type::BLOCK &&
          x->kind != ast_type::IF)
        cpp_code << c + ';';

      else
        cpp_code << c;
      cpp_code << '\n';
    }
    return header + cpp_code.str();
  }
};
