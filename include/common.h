#pragma once

#include "lexer.h"
#include <limits>

typedef unsigned long long u64;
typedef long long i64;
typedef double f64;
typedef unsigned char u8;

inline std::string variant2string(const token_value &val) {
  if (const std::string *s = std::get_if<std::string>(&val)) {
    return *s;
  }
  return "error";
}

template <typename T> inline long long variant2int(token_value &val) {
  return std::get<T>(val);
}

inline long long variant2double(token_value &val) {
  return std::get<double>(val);
}

inline long long variant2bool(token_value &val) { return std::get<bool>(val); }

template <typename T> bool fits(long long v) {
  return v >= std::numeric_limits<T>::min() &&
         v <= std::numeric_limits<T>::max();
}

inline bool is_it_value(token a) {
  if (a.type >= 13 && a.type <= 22)
    return true;
  return false;
}

inline bool is_it_type(token a) {
  switch (a.type) {
  case BYTE_TYPE:
  case WORD_TYPE:
  case INT_TYPE:
  case LONG_TYPE:
  case FLOAT_TYPE:
  case DOUBLE_TYPE:
  case STRING_TYPE:
  case BOOL_TYPE:
  case UNSIGNED_8_TYPE:
  case UNSIGNED_16_TYPE:
  case UNSIGNED_32_TYPE:
  case UNSIGNED_64_TYPE:
    return true;
  default:
    return false;
  }
}

inline bool is_it_logical(token_type a) {
  if (a != BIGGER && a != BEQUAL && a != LEQUAL && a != LESS && a != EQUAL &&
      a != NEQUAL && a != AND && a != OR)
    return false;
  return true;
}

inline bool is_literal(token_type t) {
  switch (t) {
  case BYTE:
  case WORD:
  case INT:
  case LONG:
  case FLOAT:
  case DOUBLE:
  case TRUE:
  case FALSE:
  case STRING:
    return true;
  default:
    return false;
  }
}

inline std::string type_in_cpp(token a) {
  std::string type;
  if (a.type == BYTE_TYPE)
    type = "int8_t ";
  if (a.type == WORD_TYPE)
    type = "int16_t ";
  if (a.type == INT_TYPE)
    type = "int32_t ";
  if (a.type == LONG_TYPE)
    type = "int64_t ";
  if (a.type == FLOAT_TYPE)
    type = "float ";
  if (a.type == DOUBLE_TYPE)
    type = "double ";
  if (a.type == STRING_TYPE) {
    type = "std::string ";
  }
  if (a.type == BOOL_TYPE)
    type = "bool ";
  if (a.type == UNSIGNED_8_TYPE)
    type = "uint8_t ";
  if (a.type == UNSIGNED_16_TYPE)
    type = "uint16_t ";
  if (a.type == UNSIGNED_32_TYPE)
    type = "uint32_t ";
  if (a.type == UNSIGNED_64_TYPE)
    type = "uint64_t ";
  if (a.type == VOID_TYPE)
    type = "void ";
  return type;
}