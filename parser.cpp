#include "include/parser.h"
#include "include/ast.h"
#include "include/common.h"
#include "include/lexer.h"
#include <cstdlib>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <fstream>
#include <sstream>

astptr parser::parse_factor() {
  token tok = consume();
  if (tok.type == token_type::BYTE || tok.type == token_type::WORD ||
      tok.type == token_type::INT || tok.type == token_type::LONG ||
      tok.type == token_type::NULL_ || tok.type == token_type::FLOAT ||
      tok.type == token_type::DOUBLE || tok.type == token_type::TRUE ||
      tok.type == token_type::FALSE || tok.type == token_type::VOID_TYPE ||
      tok.type == token_type::STRING) {
       return std::make_unique<Node>(tok);
      } 
  else if (tok.type == token_type::ID) {
    if (peek().type == L_BRACKET) {
      consume();
      std::vector<astptr> args_;

      while (peek().type != R_BRACKET) {
        args_.push_back(parse_or());
        if (peek().type == COMA)
          consume();
      }
      consume(R_BRACKET);
      return std::make_unique<FuncCallNode>(tok, std::move(args_));
    }
    if(peek().type==L_SQ_BRACKET) {
    	consume();
    	astptr i=parse_expr();
    	consume(R_SQ_BRACKET);
    	return std::make_unique<ArrayAccessNode>(tok, std::move(i));
    }
    return std::make_unique<Node>(tok);
  } else if (tok.type == token_type::L_BRACKET) {
    astptr node = parse_expr();

    token close = consume(token_type::R_BRACKET);

    return node;

  } else {
    std::cerr << "Error in parsing factor -> " + std::to_string(tok.type) +
                     " at " + std::to_string(indx - 1) + '\n';
    exit(EXIT_FAILURE);
  }
}

astptr parser::parse_unary() {
  token op = peek();
  if (op.type == token_type::PLUS || op.type == token_type::MINUS ||
      op.type == token_type::NOT) {
    consume();
    astptr unary = parse_unary();
    return std::make_unique<UnaryNode>(std::move(unary), op.type);
  }
  return parse_factor();
}

astptr parser::parse_term() {
  astptr node = parse_unary();

  while (true) {
    token op = peek();
    if (op.type != token_type::STAR && op.type != token_type::SLASH)
      break;

    consume();
    astptr rhs = parse_unary();
    node =
        std::make_unique<BinaryNode>(std::move(node), std::move(rhs), op.type);
  }

  return node;
}

astptr parser::parse_expr() {
  astptr node = parse_term();

  while (true) {
    token op = peek();
    if (op.type != token_type::PLUS && op.type != token_type::MINUS)
      break;
    consume();
    astptr rhs = parse_term();
    node =
        std::make_unique<BinaryNode>(std::move(node), std::move(rhs), op.type);
  }

  return node;
}

astptr parser::parse_use() {
  consume(USE);
  std::string name = variant2string(consume(ID).value) + "." + "flame";
  consume(SEMI);
  std::ifstream file(name);
  std::ostringstream oss;
  if(!file) {
    std::cerr << "Cannot open file " << name << std::endl;
    file.close();
    exit(1);
  }
  oss << file.rdbuf();
  std::string code = oss.str();
  file.close();
  lexer l;
  std::vector<token> toks = l.lex(code);
  parser p(toks);
  std::vector<astptr> module = p.parse();
  return std::make_unique<ModuleNode>(std::move(module));
}

astptr parser::parse_comparison() {
  astptr node = parse_expr();
  while (true) {
    token op = peek();
    if (op.type != LESS && op.type != BIGGER && op.type != LEQUAL &&
        op.type != BEQUAL)
      break;
    consume();
    astptr rhs = parse_expr();
    node = std::make_unique<CondNode>(std::move(node), std::move(rhs), op.type);
  }
  return node;
}

astptr parser::parse_equality() {
  astptr node = parse_comparison();
  while (true) {
    token op = peek();
    if (op.type != EQUAL && op.type != NEQUAL)
      break;
    consume();
    astptr rhs = parse_comparison();
    node = std::make_unique<CondNode>(std::move(node), std::move(rhs), op.type);
  }
  return node;
}

astptr parser::parse_and() {
  astptr node = parse_equality();
  while (peek().type == AND) {
    consume();
    astptr rhs = parse_equality();
    node = std::make_unique<CondNode>(std::move(node), std::move(rhs), AND);
  }
  return node;
}

astptr parser::parse_or() {
  astptr node = parse_and();
  while (peek().type == OR) {
    consume();
    astptr rhs = parse_and();
    node = std::make_unique<CondNode>(std::move(node), std::move(rhs), OR);
  }
  return node;
}

astptr parser::parse_return() {
  consume(RETURN);
  astptr node = parse_or();
  consume(SEMI);
  return std::make_unique<ReturnNode>(std::move(node));
}

astptr parser::parse_block() {
  consume(L_BRACES);
  std::vector<astptr> stmts;
  while (peek().type != R_BRACES) {
    stmts.push_back(parse_statement());
  }
  consume(R_BRACES);
  return std::make_unique<BlockNode>(std::move(stmts));
}

astptr parser::parse_func_statement() {
  consume(FUNC);
  token id = consume(ID);
  consume(L_BRACKET);
  std::vector<astptr> args_;
  while (peek().type != R_BRACKET) {
    astptr argument = std::make_unique<ArgumentNode>(consume(), consume(ID));
    args_.push_back(std::move(argument));
    if (peek().type == COMA)
      consume();
  }
  consume(R_BRACKET);
  token return_type = consume();
  if (!is_it_type(return_type) && return_type.type != VOID_TYPE) {
    std::cerr << "Unknown return type in " + variant2string(id.value) << '\n';
    exit(EXIT_FAILURE);
  }
  astptr block = parse_block();
  return std::make_unique<FuncNode>(id, return_type, std::move(args_),
                                    std::move(block));
}

astptr parser::parse_if_statement() {
  consume(IF);
  consume(L_BRACKET);
  astptr cond = parse_or();
  consume(R_BRACKET);
  astptr block = parse_block();
  astptr node =
      std::make_unique<IfNode>(std::move(cond), std::move(block), astptr{}, IF);
  IfNode *current = static_cast<IfNode *>(node.get());
  while (peek().type == ELIF || peek().type == ELSE) {
    token a = consume(); // ELIF or ELSE
    if (a.type == ELIF) {
      consume(L_BRACKET);
      astptr cond_elif = parse_or();
      consume(R_BRACKET);
      astptr block_elif = parse_block();
      current->next = std::make_unique<IfNode>(
          std::move(cond_elif), std::move(block_elif), astptr{}, ELIF);
      current = static_cast<IfNode *>(current->next.get());
    }
    if (a.type == ELSE) {
      astptr block_else = parse_block();
      current->next = std::make_unique<IfNode>(astptr{}, std::move(block_else),
                                               astptr{}, ELSE);
      break;
    }
  }
  return node;
}

astptr parser::parse_while_statement() {
  consume(WHILE);
  consume(L_BRACKET);
  astptr cond = parse_or();
  consume(R_BRACKET);
  astptr block = parse_block();
  return std::make_unique<LoopNode>(std::move(cond), std::move(block), WHILE);
}

astptr parser::parse_for_statement() {
  consume(FOR);
  consume(L_BRACKET);
  astptr var = parse_assignment();
  astptr cond = parse_or();
  consume(SEMI);
  astptr thing = parse_assignment();
  consume(R_BRACKET);
  astptr block = parse_block();
  return std::make_unique<ForNode>(std::move(cond), std::move(block),
                                   std::move(var), std::move(thing));
}

astptr parser::parse_break_continue() {
  token a = consume();
  if (a.type == BREAK) {
    consume(SEMI);
    return std::make_unique<BreakNode>();
  } else if (a.type == CONTINUE) {
    consume(SEMI);
    return std::make_unique<ContinueNode>();
  } else {
    std::cerr
        << "Error in parsing break or continue, expected break or continue\n";
    exit(EXIT_FAILURE);
  }
}

astptr parser::parse_array() {
  if(peek().type == ID && peek(1).type == L_SQ_BRACKET) return parse_factor();
  if (!is_it_type(peek()) || !(peek(1).type == L_SQ_BRACKET)) {
    std::cerr << "Error in parsing array, expected syntax like this: Type[]";
    exit(1);
  }
  token type = consume();
  consume(L_SQ_BRACKET);
  token size;
  bool size_defined = false;
  if (peek().type != R_SQ_BRACKET) {
    size = consume();
    size_defined = true;
  }
  consume(R_SQ_BRACKET);
  std::string id = variant2string(consume(ID).value);
  if (peek().type == SEMI) {
    if (size_defined)
      return std::make_unique<ArrayNode>(type, std::vector<astptr>{}, id,
                                         variant2int<long long>(size.value));
    else
      return std::make_unique<ArrayNode>(type, std::vector<astptr>{}, id, -1);
  }
  consume(EQ);
  consume(L_SQ_BRACKET);
  std::vector<astptr> values;
  while (peek().type != R_SQ_BRACKET) {
    astptr value = parse_factor();
    if (peek().type == COMA)
        consume();
    values.push_back(std::move(value));
  }
  consume(R_SQ_BRACKET);
  consume(SEMI);
  if (size_defined)
    return std::make_unique<ArrayNode>(type, std::move(values), id,
                                       variant2int<long long>(size.value));
  else
    return std::make_unique<ArrayNode>(type, std::move(values), id, -1);
}

astptr parser::parse_assignment() {
  if (peek().type == ID && (peek(1).type == L_BRACKET||peek(1).type==L_SQ_BRACKET)) {
    astptr node = parse_factor();
    consume(SEMI);
    return node;
  } else if ((peek().type == ID && peek(1).type == PLUS &&
              peek(2).type == PLUS) ||
             (peek().type == ID && peek(1).type == MINUS &&
              peek(2).type == MINUS)) {
    token_value id = consume(ID).value;
    token a = consume();
    consume();
    if (a.type == PLUS)
      return std::make_unique<IncDecVarNode>(0, id);
    else
      return std::make_unique<IncDecVarNode>(1, id);
  }
  if (is_it_type(peek()) && peek(1).type == L_SQ_BRACKET)
      return parse_array();
  token type = consume();
  if (type.type == ID) {
    if (peek().type == PLUS || peek().type == MINUS || peek().type == STAR ||
        peek().type == SLASH) {
      token_type op = consume().type;
      consume(EQ);
      astptr value = parse_or();
      if (peek().type == SEMI)
        consume(SEMI);
      return std::make_unique<ReAssignmentNodeExpr>(op, type, std::move(value));
    }
    consume(EQ);
    astptr value = parse_or();
    consume(SEMI);
    return std::make_unique<AssignmentNode>(type, std::move(value));
  }
  if (!is_it_type(type)) {
    std::cerr << "Error: unexpected type " + std::to_string(type.type) + '\n';
    exit(1);
  }
  token id = consume(token_type::ID);
  if (peek().type == SEMI) {
    consume();
    return std::make_unique<AssignmentNodeExpr>(type.type, id, astptr{});
  }
  consume(token_type::EQ);
  astptr value = parse_or();
  consume(SEMI);
  return std::make_unique<AssignmentNodeExpr>(type.type, id, std::move(value));
}

astptr parser::parse_statement() {
  token tok = peek();
  switch (tok.type) {
  case token_type::IF:
    return parse_if_statement();
  case token_type::WHILE:
    return parse_while_statement();
  case token_type::FOR:
    return parse_for_statement();
  case token_type::L_BRACES:
    return parse_block();
  case token_type::FUNC:
    return parse_func_statement();
  case token_type::USE:
    return parse_use();
  case token_type::RETURN:
    return parse_return();
  case token_type::BREAK:
  case token_type::CONTINUE:
    return parse_break_continue();
  case token_type::BYTE_TYPE:
  case token_type::WORD_TYPE:
  case token_type::INT_TYPE:
  case token_type::LONG_TYPE:
  case token_type::BOOL_TYPE:
  case token_type::FLOAT_TYPE:
  case token_type::DOUBLE_TYPE:
  case token_type::STRING_TYPE:
  case token_type::UNSIGNED_8_TYPE:
  case token_type::UNSIGNED_16_TYPE:
  case token_type::UNSIGNED_32_TYPE:
  case token_type::UNSIGNED_64_TYPE:
  case token_type::AUTO_TYPE:
  case token_type::ID:
    return parse_assignment();
  default:
    astptr node = parse_expr();
    consume(SEMI);
    return node;
  }
  return parse_expr();
}

std::vector<std::unique_ptr<ASTNode>> parser::parse() {
  std::vector<std::unique_ptr<ASTNode>> parsed;
  while (indx < src.size() && src[indx].type != token_type::EOF_) {
    parsed.push_back(parse_statement());
  }
  return parsed;
}
