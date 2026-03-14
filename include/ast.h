#pragma once
#include "common.h"
#include "lexer.h"
#include <memory>
#include <string>
#include <vector>

class ASTNode;
class generator;

typedef enum class ast_type {
  JUSTNODE,
  BINARY,
  DEFINEVAR,
  ASSIGN,
  UNARY,
  FuncCall,
  COND,
  MODULE,
  BLOCK,
  FUNC,
  FUNC_ARG,
  RETURN,
  IF,
  LOOP,
  BREAK,
  CONTINUE,
  VAR_INC_DEC,
  FOR,
  REASSIGNVAR,
  ARRAY,
  ARRAY_ACCESS,
  ARRAY_CHANGE,
} ast_type;

using astptr = std::unique_ptr<ASTNode>;

class ASTNode {
public:
  ast_type kind;
  virtual ~ASTNode() = default;
  u64 line=0;
  u64 column=0;
  virtual std::string gen(generator &g) {
    (void)g;
	  return "";
  }
  virtual void print() const {}
};

class Node : public ASTNode {
public:
  token tok;
  Node(const token t) : tok(t) { kind = ast_type::JUSTNODE; line=tok.line; column=tok.column;};
  std::string gen(generator &g) override;
};

class ReturnNode : public ASTNode {
public:
  astptr value;
  ReturnNode(astptr node) : value(std::move(node)) { kind = ast_type::RETURN; };
  void print() const override {}
  std::string gen(generator &g) override;
};

class ArgumentNode : public ASTNode {
public:
  token type;
  token id;
  ArgumentNode(const token t, const token id_) : type(t), id(id_) {
    kind = ast_type::FUNC_ARG;
  };
  void print() const override {}
  std::string gen(generator &g) override;
};

class BinaryNode : public ASTNode {
public:
  astptr left;
  astptr right;
  token_type op;
  BinaryNode(astptr left_, astptr right_, const token_type &op_)
      : left(std::move(left_)), right(std::move(right_)), op(op_) {
    kind = ast_type::BINARY;
  };
  std::string gen(generator &g) override;
};

class CondNode : public ASTNode {
public:
  astptr left;
  astptr right;
  token_type op;
  CondNode(astptr left_, astptr right_, const token_type &op_)
      : left(std::move(left_)), right(std::move(right_)), op(op_) {
    kind = ast_type::COND;
  };
  void print() const override {
    left->print();
    std::cout << op << '\n';
    right->print();
  }

  std::string gen(generator &g) override;
};

class UnaryNode : public ASTNode {
public:
  astptr left;
  token_type sign;
  UnaryNode(astptr left_, const token_type &sign_)
      : left(std::move(left_)), sign(sign_) {
    kind = ast_type::UNARY;
  };
  void print() const override {
    std::cout << sign << '\n';
    left->print();
  }
  std::string gen(generator &g) override;
};

class AssignmentNode : public ASTNode {
public:
  token id;
  astptr val;
  AssignmentNode(const token &id_, astptr val_)
      : id(id_), val(std::move(val_)) {
    kind = ast_type::ASSIGN;
  };

  std::string gen(generator &g) override;
};

class AssignmentNodeExpr : public ASTNode {
public:
  token_type type_;
  token id;
  astptr val;
  AssignmentNodeExpr(const token_type &t_, const token &id_, astptr val_)
      : type_(t_), id(id_), val(std::move(val_)) {
    kind = ast_type::DEFINEVAR;
  };

  void print() const override { std::cout << id.type << '\n'; }
  std::string gen(generator &g) override;
};

class ReAssignmentNodeExpr : public ASTNode {
public:
  token_type type_;
  token id;
  astptr val;
  ReAssignmentNodeExpr(const token_type &t_, const token &id_, astptr val_)
      : type_(t_), id(id_), val(std::move(val_)) {
    kind = ast_type::REASSIGNVAR;
  };

  void print() const override { std::cout << id.type << '\n'; }
  std::string gen(generator &g) override;
};

class FuncCallNode : public ASTNode {
public:
  token id;
  std::vector<astptr> args;
  FuncCallNode(const token &id_, std::vector<astptr> args_)
      : id(id_), args(std::move(args_)) {
    kind = ast_type::FuncCall;
  };

  void print() const override {}
  std::string gen(generator &g) override;
};

class BlockNode : public ASTNode {
public:
  std::vector<astptr> stmts;
  BlockNode(std::vector<astptr> stmts_) : stmts(std::move(stmts_)) {
    kind = ast_type::BLOCK;
  };

  void print() const override {}
  std::string gen(generator &g) override;
};

class IfNode : public ASTNode {
public:
  astptr cond;
  astptr block;
  astptr next;
  token_type type;
  IfNode(astptr c, astptr b, astptr n, token_type t)
      : cond(std::move(c)), block(std::move(b)), next(std::move(n)), type(t) {
    kind = ast_type::IF;
  }
  void print() const override {}
  std::string gen(generator &g) override;
};

class FuncNode : public ASTNode {
public:
  token id;
  token type;
  std::vector<astptr> args;
  astptr block;
  FuncNode(const token id_, const token type_, std::vector<astptr> args_,
           astptr block_)
      : id(id_), type(type_), args(std::move(args_)), block(std::move(block_)) {
    kind = ast_type::FUNC;
  };

  void print() const override {}
  std::string gen(generator &g) override;
};

class LoopNode : public ASTNode {
public:
  astptr cond;
  astptr block;
  token_type type;
  LoopNode(astptr c, astptr b, token_type t)
      : cond(std::move(c)), block(std::move(b)), type(t) {
    kind = ast_type::LOOP;
  }
  void print() const override {}
  std::string gen(generator &g) override;
};

class ForNode : public ASTNode {
public:
  astptr cond;
  astptr block;
  astptr var;
  astptr thing;
  ForNode(astptr c, astptr b, astptr v, astptr t)
      : cond(std::move(c)), block(std::move(b)), var(std::move(v)),
        thing(std::move(t)) {
    kind = ast_type::FOR;
  }
  void print() const override {}
  std::string gen(generator &g) override;
};

class ModuleNode : public ASTNode {
public:
  std::vector<astptr> module;
  ModuleNode(std::vector<astptr> m) : module(std::move(m)) { kind = ast_type::MODULE; }

  void print() const override {}
  std::string gen(generator &g) override;
};

class BreakNode : public ASTNode {
public:
  BreakNode() { kind = ast_type::BREAK; }
  void print() const override {}
  std::string gen(generator &g) override;
};

class ContinueNode : public ASTNode {
public:
  ContinueNode() { kind = ast_type::CONTINUE; }
  void print() const override {}
  std::string gen(generator &g) override;
};

class IncDecVarNode : public ASTNode {
public:
  u8 type;
  token_value id;
  IncDecVarNode(u8 t, token_value n) : type(t), id(n) {}
  std::string gen(generator &g) override;
};

class ArrayNode : public ASTNode {
public:
  token type;
  std::vector<astptr> values;
  std::string id;
  long size;
  ArrayNode(const token &t, std::vector<astptr> v, const std::string &i, long s)
      : type(t), values(std::move(v)), id(i), size(s) {
    kind = ast_type::ARRAY;
  }
  std::string gen(generator &g) override;
};

class ArrayAccessNode : public ASTNode {
public:
  token id;
  astptr index;
  ArrayAccessNode(token id_, astptr i_) : id(id_), index(std::move(i_)) {
    kind = ast_type::ARRAY_ACCESS;
  }
  std::string gen(generator &g) override;
};

class ArrayChangeNode : public ASTNode {
public:
  token id;
  astptr index;
  astptr value;
  ArrayChangeNode(token id_, astptr i_, astptr v) : id(id_), index(std::move(i_)), value(std::move(v)) {
    kind = ast_type::ARRAY_CHANGE;
  }
  std::string gen(generator &g) override;
};


