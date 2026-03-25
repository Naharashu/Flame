#pragma once
#include "common.h"
#include "lexer.h"
#include <memory>
#include <string>
#include <vector>

class ASTNode;
class generator;

using ast_type = enum class ast_type : uint8_t {
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
  NAMESPACE,
};

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
  explicit Node(const token& t) : tok(t) { kind = ast_type::JUSTNODE; line=tok.line; column=tok.column;};
  std::string gen(generator &g) override;
};

class ReturnNode : public ASTNode {
public:
  astptr value;
  explicit ReturnNode(astptr node) : value(std::move(node)) { kind = ast_type::RETURN; };
  void print() const override {}
  std::string gen(generator &g) override;
};

class ArgumentNode : public ASTNode {
public:
  token type;
  token id;
  bool is_array;
  u64 size_if_array=0;
  ArgumentNode(const token& t, const token& id_, bool isarr, u64 s) : type(t), id(id_), is_array(isarr), size_if_array(s) {
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
  bool is_const;
  AssignmentNode(const token &id_, astptr val_, bool isconst=false)
      : id(id_), val(std::move(val_)), is_const(isconst) {
    kind = ast_type::ASSIGN;
  };

  std::string gen(generator &g) override;
};

class AssignmentNodeExpr : public ASTNode {
public:
  token_type type_;
  token id;
  astptr val;
  bool is_const;
  AssignmentNodeExpr(const token_type &t_, const token &id_, astptr val_, bool isconst=false)
      : type_(t_), id(id_), val(std::move(val_)), is_const(isconst) {
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
  bool is_const;
  ReAssignmentNodeExpr(const token_type &t_, const token &id_, astptr val_, bool isconst=false)
      : type_(t_), id(id_), val(std::move(val_)), is_const(isconst) {
    kind = ast_type::REASSIGNVAR;
  };

  void print() const override { std::cout << id.type << '\n'; }
  std::string gen(generator &g) override;
};

class FuncCallNode : public ASTNode {
public:
  std::string id;
  std::vector<astptr> args;
  std::string want_get="";
  FuncCallNode(const std::string &id_, std::vector<astptr> args_,const std::string &wg)
      : id(id_), args(std::move(args_)), want_get(wg) {
    kind = ast_type::FuncCall;
  };

  void print() const override {}
  std::string gen(generator &g) override;
};

class BlockNode : public ASTNode {
public:
  std::vector<astptr> stmts;
  explicit BlockNode(std::vector<astptr> stmts_) : stmts(std::move(stmts_)) {
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
  bool is_return_type_array=false;
  u64 size;
  FuncNode(const token &id_, const token &type_, std::vector<astptr> args_,
           astptr block_,const bool &irta, const u64 &s)
      : id(id_), type(type_), args(std::move(args_)), block(std::move(block_)), is_return_type_array(irta), size(s) {
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
  std::string mname;
  ModuleNode(std::vector<astptr> m,const std::string &name) : module(std::move(m)), mname(name) { kind = ast_type::MODULE; }

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
  IncDecVarNode(u8 t,const token_value &n) : type(t), id(n) {}
  std::string gen(generator &g) override;
};

class ArrayNode : public ASTNode {
public:
  token type;
  std::vector<astptr> values;
  std::string id;
  unsigned long long size;
  bool is_init=false;
  ArrayNode(const token &t, std::vector<astptr> v, const std::string &i,unsigned long long s, bool ii=false)
      : type(t), values(std::move(v)), id(i), size(s), is_init(ii) {
    kind = ast_type::ARRAY;
  }
  std::string gen(generator &g) override;
};

class ArrayAccessNode : public ASTNode {
public:
  token id;
  astptr index;
  ArrayAccessNode(const token &id_, astptr i_) : id(id_), index(std::move(i_)) {
    kind = ast_type::ARRAY_ACCESS;
  }
  std::string gen(generator &g) override;
};

class ArrayChangeNode : public ASTNode {
public:
  token id;
  astptr index;
  astptr value;
  ArrayChangeNode(const token &id_, astptr i_, astptr v) : id(id_), index(std::move(i_)), value(std::move(v)) {
    kind = ast_type::ARRAY_CHANGE;
  }
  std::string gen(generator &g) override;
};


class NamespaceAccessNode : public ASTNode {
  public:
  std::string namespace_="";
  astptr node;
  NamespaceAccessNode(const std::string &name, astptr n) : namespace_(name), node(std::move(n)) {
    kind = ast_type::NAMESPACE;
  }
  std::string gen(generator &g) override;
};

class MethodNode : public ASTNode {
  public:
  std::vector<astptr> children;
  token_type type;
  std::string parent;
  MethodNode(std::vector<astptr> m,const std::string &name, const token_type &t) : children(std::move(m)), parent(name), type(t) { //kind = ast_type::MODULE;
     }

  void print() const override {}
  std::string gen(generator &g) override;
};

