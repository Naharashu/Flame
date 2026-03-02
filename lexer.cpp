#include "include/lexer.h"
#define u64 long long

token lexer::create_token(token_type a, token_value b) { return token{a, b}; }

bool lexer::is_int(char c) { return c >= '0' && c <= '9'; }

bool lexer::is_letter(char c) {
  return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

std::vector<token> lexer::lex(std::string src) {
  std::vector<token> lexed;
  u64 i = 0;
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
    case '.': {
      lexed.push_back(create_token(DOT, nothing{}));
      break;
    }
    case ':': {
      lexed.push_back(create_token(TWODOTS, nothing{}));
      break;
    }
    case 'i': {
      if(next=='f') {
        lexed.push_back(create_token(IF, nothing{}));
      }
      break;
    }
    case 'e': {
      if(next=='l') {
        if(src[i+2]=='i') {
          lexed.push_back(create_token(ELIF, nothing{}));
        }
        else if(src[i+2]=='e') {
          lexed.push_back(create_token(ELSE, nothing{}));
        }
      }
      break;
    }
    case 'f': {
      if(next=='u') {
        lexed.push_back(create_token(FUNC, nothing{}));
      }
      else if(next=='o') {
        lexed.push_back(create_token(FOR, nothing{}));
      }
      break;
    }
    case 'w': {
      if(next=='h') {
        lexed.push_back(create_token(WHILE, nothing{}));
      }
    }
    }
    
    if (is_letter(c)) {
      std::string id;
      while (i < src.size() &&
             (is_letter(src[i]) || is_int(src[i]) || src[i] == '_')) {
        id.push_back(src[i]);
        i++;
      }
      lexed.push_back(create_token(ID, id));
      continue;
    }
    if (c == '"') {
      std::string string;
      while (i < src.size() && (!(c == '"'))) {
        string.push_back(src[i]);
        i++;
      }
      lexed.push_back(create_token(STRING, string));
      continue;
    }
    if (is_int(c)) {
      std::string number;
      while (i < src.size() && is_int(src[i])) {
        if(src[i+1]=='.') {
          number += src[i++];
          while(i<src.size()&& is_int(src[i])) {
            number += src[i++];
          }
          char *endptr;
          lexed.push_back(create_token(
          DOUBLE, static_cast<token_value>(strtod(number.c_str(), &endptr))));
        }
        number += src[i];
        i++;
      }
      char *endptr;
      lexed.push_back(create_token(
          INT, static_cast<token_value>(strtoll(number.c_str(), &endptr, 10))));
    }
    i++;
  }
  lexed.push_back(create_token(EOF_, nothing{}));
  return lexed;
}


/* 
class lexer {
private:
  token create_token(token_type a, token_value b) { return token{a, b}; }
  bool is_int(char c) {
    switch (c) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      return true;
    }
    return false;
  }

  bool is_letter(char c) {
    bool result = false;

    if ((c >= 'A') && (c <= 'Z'))
      result = true;
    if ((c >= 'a') && (c <= 'z'))
      result = true;

    return result;
  }

public:
  std::vector<token> lex(std::string src) {
    std::vector<token> lexed;
    u64 i = 0;
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
        lexed.push_back(create_token(ID, id));
        continue;
      }
      if (c == '"') {
        std::string string;
        while (i < src.size() && (!(c == '"'))) {
          string.push_back(src[i]);
          i++;
        }
        lexed.push_back(create_token(STRING, string));
        continue;
      }
      if (is_int(c)) {
        std::string number;
        while (i < src.size() && is_int(src[i])) {
          number += src[i];
          i++;
        }
        char *endptr;
        lexed.push_back(create_token(INT, static_cast<token_value>(strtoll(
                                              number.c_str(), &endptr, 10))));
      }
      i++;
    }
    return lexed;
  }
};
*/