#pragma once

#include "ast.h"
#include "common.h"
#include "lexer.h"
#include "parser.h"
#include <cstddef>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

class TranspileTimeError : public std::exception {
  private:
  std::string message;
  public:
  explicit TranspileTimeError(const std::string &msg) : message(msg) {}

  const char * what() const noexcept override {
    return message.c_str();
  }
};

class generator {
public:
  int indent = 0;
  u64 line=1;
  u64 column=0;

  std::string pad() { return std::string(static_cast<int>(indent * 4), ' '); }

  std::ostringstream cpp_code;
  std::string header;
  std::string gencode(astptr &node) {
    return node->gen(*this);
  }

  std::string generate(std::vector<astptr> &nodes) {
    if(header.find("<iostream>\n")==std::string::npos) { 
      header += "#include <iostream>\n";
    }
    if(header.find("<cstdint>")==std::string::npos) {
      header += "#include <cstdint>\n";
    }
    for (auto &x : nodes) {
      if(!x) continue;
      std::string c = gencode(x);
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
