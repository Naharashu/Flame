#pragma once

#include <variant>
#include <string>
#include <vector>
#include <iostream>

enum token_type {
    VOID_TOK,
    INT,
    DOUBLE,
    STRING,
    BOOL,
    NULL_,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    EQ,
    EQUAL,
    BIGGER,
    LESS,
    SHIFT_R,
    SHIFT_L,
    NOT,
    AND_B,
    AND,
    OR_B,
    OR,
    XOR,
    NEQUAL,
    BEQUAL,
    LEQUAL,
    R_BRACKET,
    L_BRACKET,
    R_SQ_BRACKET,
    L_SQ_BRACKET,
    R_BRACES,
    L_BRACES,
    DOT,
    TWODOTS,
    SEMI,
    IF,
    ELIF,
    ELSE,
    WHILE,
    FOR,
    FUNC,
    ID,
    EOF_
};

using nothing = std::monostate;
using token_value = std::variant<nothing, int, long long, double, std::string>;

struct token {
    token_type type;
    token_value value;
};

class lexer {
private:
  token create_token(token_type a, token_value b);
  bool is_int(char c);
  bool is_letter(char c);

public:
  std::vector<token> lex(std::string src);
};
