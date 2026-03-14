#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include "lexer.h"

extern std::vector<std::unordered_map<std::string, std::pair<token_type, token_value>>> table;