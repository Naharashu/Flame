#include "include/lexer.h"
#include "include/common.h"
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>
#define u64 long long

token lexer::create_token(token_type a, token_value b) { return token{a, b}; }

bool lexer::is_int(char c) { return c >= '0' && c <= '9'; }

bool lexer::is_letter(char c) {
  return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

std::vector<token> lexer::lex(std::string src) {
  std::vector<token> lexed;
  size_t i = 0;
  while (i < src.size()) {
    char c = src[i];
    char next = (i + 1 < src.size()) ? src[i + 1] : '\0';
    if (c == ' ' || c == '\n' || c == '\t' || c == '\r') {
      i++;
      continue;
    }
    switch (c) {
    case '+': {
      lexed.push_back(create_token(PLUS, nothing{}));
      break;
    }
    case '-': {
      lexed.push_back(create_token(MINUS, nothing{}));
      break;
    }
    case '*': {
      lexed.push_back(create_token(STAR, nothing{}));
      break;
    }
    case '/': {
      lexed.push_back(create_token(SLASH, nothing{}));
      break;
    }
    case '%': {
      lexed.push_back(create_token(MOD, nothing{}));
      break;
    }
    case '(': {
      lexed.push_back(create_token(L_BRACKET, nothing{}));
      break;
    }
    case ')': {
      lexed.push_back(create_token(R_BRACKET, nothing{}));
      break;
    }
    case '[': {
      lexed.push_back(create_token(L_SQ_BRACKET, nothing{}));
      break;
    }
    case ']': {
      lexed.push_back(create_token(R_SQ_BRACKET, nothing{}));
      break;
    }
    case '{': {
      lexed.push_back(create_token(L_BRACES, nothing{}));
      break;
    }
    case '}': {
      lexed.push_back(create_token(R_BRACES, nothing{}));
      break;
    }
    case '=': {
      if (next == '=') {
        lexed.push_back(create_token(EQUAL, nothing{}));
        i++;
        break;
      }
      lexed.push_back(create_token(EQ, nothing{}));
      break;
    }
    case '>': {
      if (next == '=') {
        lexed.push_back(create_token(BEQUAL, nothing{}));
        i++;
        break;
      } else if (next == '>') {
        lexed.push_back(create_token(SHIFT_R, nothing{}));
        i++;
        break;
      }
      lexed.push_back(create_token(BIGGER, nothing{}));
      break;
    }
    case '<': {
      if (next == '=') {
        lexed.push_back(create_token(LEQUAL, nothing{}));
        i++;
        break;
      } else if (next == '<') {
        lexed.push_back(create_token(SHIFT_L, nothing{}));
        i++;
        break;
      }
      lexed.push_back(create_token(LESS, nothing{}));
      break;
    }
    case '!': {
      if (next == '=') {
        lexed.push_back(create_token(NEQUAL, nothing{}));
        i++;
        break;
      }
      lexed.push_back(create_token(NOT, nothing{}));
      break;
    }
    case '&': {
      if (next == '&') {
        lexed.push_back(create_token(AND, nothing{}));
        i++;
        break;
      }
      lexed.push_back(create_token(AND_B, nothing{}));
      break;
    }
    case '|': {
      if (next == '|') {
        lexed.push_back(create_token(OR, nothing{}));
        i++;
        break;
      }
      lexed.push_back(create_token(OR_B, nothing{}));
      break;
    }
    case ';': {
      lexed.push_back(create_token(SEMI, nothing{}));
      break;
    }
    case ',': {
      lexed.push_back(create_token(COMA, nothing{}));
      break;
    }
    case '.': {
      lexed.push_back(create_token(DOT, nothing{}));
      break;
    }
    case ':': {
      lexed.push_back(create_token(TWODOTS, nothing{}));
      break;
    }
    }
    if (is_letter(c)) {
      std::string id;
      while (i < src.size() &&
             (is_letter(src[i]) || is_int(src[i]) || src[i] == '_')) {
        id.push_back(src[i]);
        i++;
      }
      if (id == "if")
        lexed.push_back(create_token(IF, nothing{}));
      else if (id == "else")
        lexed.push_back(create_token(ELSE, nothing{}));
      else if (id == "elif")
        lexed.push_back(create_token(ELIF, nothing{}));
      else if (id == "while")
        lexed.push_back(create_token(WHILE, nothing{}));
      else if (id == "break")
        lexed.push_back(create_token(BREAK, nothing{}));
      else if (id == "continue")
        lexed.push_back(create_token(CONTINUE, nothing{}));
      else if (id == "for")
        lexed.push_back(create_token(FOR, nothing{}));
      else if (id == "func")
        lexed.push_back(create_token(FUNC, nothing{}));
      else if (id == "true")
        lexed.push_back(create_token(TRUE, true));
      else if (id == "false")
        lexed.push_back(create_token(FALSE, false));
      else if (id == "i8")
        lexed.push_back(create_token(BYTE_TYPE, nothing{}));
      else if (id == "u8")
        lexed.push_back(create_token(UNSIGNED_8_TYPE, nothing{}));
      else if (id == "i16")
        lexed.push_back(create_token(WORD_TYPE, nothing{}));
      else if (id == "u16")
        lexed.push_back(create_token(UNSIGNED_16_TYPE, nothing{}));
      else if (id == "i32")
        lexed.push_back(create_token(INT_TYPE, nothing{}));
      else if (id == "u32")
        lexed.push_back(create_token(UNSIGNED_32_TYPE, nothing{}));
      else if (id == "i64")
        lexed.push_back(create_token(LONG_TYPE, nothing{}));
      else if (id == "u64")
        lexed.push_back(create_token(UNSIGNED_64_TYPE, nothing{}));
      else if (id == "f32")
        lexed.push_back(create_token(FLOAT_TYPE, nothing{}));
      else if (id == "f64")
        lexed.push_back(create_token(DOUBLE_TYPE, nothing{}));
      else if (id == "string")
        lexed.push_back(create_token(STRING_TYPE, nothing{}));
      else if (id == "bool")
        lexed.push_back(create_token(BOOL_TYPE, nothing{}));
      else if (id == "null")
        lexed.push_back(create_token(NULL_, std::string{"NULL"}));
      else if (id == "void")
        lexed.push_back(create_token(VOID_TYPE, nothing{}));
      else if (id == "auto")
        lexed.push_back(create_token(AUTO_TYPE, nothing{}));
      else if (id == "use")
        lexed.push_back(create_token(USE, nothing{}));
      else if (id == "return")
        lexed.push_back(create_token(RETURN, nothing{}));
      else
        lexed.push_back(create_token(ID, id));

      continue;
    }
    if (c == '"') {
      i++;
      std::string str;
      while (i < src.size() && src[i] != '"') {
        if (src[i] == '\\' && i + 1 < src.size()) {
          i++;
          switch (src[i]) {
          case 'n':
            str += '\n';
            break;
          case 't':
            str += '\t';
            break;
          case '"':
            str += '"';
            break;
          case '\\':
            str += '\\';
            break;
          default:
            str += src[i];
            break;
          }
        } else {
          str += src[i];
        }
        i++;
      }
      if (i == src.size()) {
        std::cerr << "Error: unterminated string literal\n";
        exit(1);
      }
      i++;
      lexed.push_back(create_token(STRING, str));
      continue;
    }
    if (is_int(c)) {
      std::string number;
      while ((i < src.size() && is_int(src[i])) || src[i] == '.') {
        number += src[i++];
      }
      char *endptr;
      if (number.find(".") != std::string::npos) {
        lexed.push_back(create_token(DOUBLE, strtod(number.c_str(), &endptr)));
      } else {
        auto val_ = strtoll(number.c_str(), &endptr, 10);
        token_type type = INT;
        if (fits<int8_t>(val_))
          type = BYTE;
        else if (fits<int16_t>(val_))
          type = WORD;
        else if (fits<int32_t>(val_))
          type = INT;
        else
          type = LONG;
        lexed.push_back(create_token(type, static_cast<token_value>(val_)));
      }
      continue;
    }
    i++;
  }
  lexed.push_back(create_token(EOF_, nothing{}));
  return lexed;
}


std::string disassemble_tok_type(token_type type) {
  switch (type) {
    case BYTE_TYPE: {
      return "i8";
    }
    case WORD_TYPE: {
      return "i16";
    }
    case INT_TYPE: {
      return "i32";
    }
    case LONG_TYPE: {
      return "i64";
    }
    case AUTO_TYPE: {
      return "auto";
    }
    case UNSIGNED_8_TYPE: {
      return "u8";
    }
    case UNSIGNED_16_TYPE: {
      return "u16";
    }
    case UNSIGNED_32_TYPE: {
      return "u32";
    }
    case UNSIGNED_64_TYPE: {
      return "u64";
    }
    case FLOAT_TYPE: {
      return "f32";
    }
    case DOUBLE_TYPE: {
      return "f64";
    }
    case BOOL_TYPE: {
      return "bool";
    }
    case STRING_TYPE: {
      return "string";
    }
    case SEMI: {
      return ";";
    }
    case ID: {
      return "ID";
    }
    default:
      return std::to_string(type);
  }
}