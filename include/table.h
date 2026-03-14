#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include "common.h"
#include "lexer.h"

extern std::vector<std::unordered_map<std::string, std::pair<token_type, token_value>>> table;

token_value search_value(const std::string &name);

token_type search_type(const std::string &name);

token_type search_type_scope(const std::string &name, unsigned int lvl);


void insert(const std::string &name,token_type type, token_value val);
void insert_top(const std::string &name,token_type type, token_value val);

bool exist(const std::string &name);

bool exist_in_scope(const std::string &name, unsigned int lvl);