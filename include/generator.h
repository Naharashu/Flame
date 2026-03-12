#pragma once

#include "ast.h"
#include "common.h"
#include "lexer.h"
#include "parser.h"
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
  TranspileTimeError(const char* msg) : message(msg) {}

  const char * what() const noexcept override {
    return message.c_str();
  }
};

class generator {
public:
  int indent = 0;

  std::string pad() { return std::string(indent * 4, ' '); }

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
      std::string c;
      try {
        c = gencode(x);
      } catch (const TranspileTimeError& e) {
        std::cout << e.what();
        nodes.clear();
        throw e;
      }
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
