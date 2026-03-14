#include "include/table.h"
#include "include/lexer.h"
#include <ranges>
#include <utility>

std::vector<std::unordered_map<std::string, std::pair<token_type, token_value>>> table;

token_value search_value(const std::string &name) {
    for(auto &scope : std::ranges::reverse_view(table)) {
        if(scope.contains(name)) {
            return scope.at(name).second;
        }
    }
    return nothing{};
}

token_type search_type(const std::string &name) {
    for(auto &scope : std::ranges::reverse_view(table)) {
        if(scope.contains(name)) {
            return scope.at(name).first;
        }
    }
    return EOF_;
}

token_type search_type_scope(const std::string &name, unsigned int lvl) {
    if(table.at(lvl).contains(name)) return table.at(table.size()-1).at(name).first;
    return EOF_;
}

void insert(const std::string &name,token_type type, token_value val) {
    if(table.empty()) return;
    table[table.size()-1].insert_or_assign(name, std::make_pair(type, val));
}

void insert_top(const std::string &name,token_type type, token_value val) {
    if(table.empty()) return;
    table[0].insert_or_assign(name, std::make_pair(type, val));
}

bool exist(const std::string &name) {
    if(search_type(name)==EOF_) return false;
    return true;
}

bool exist_in_scope(const std::string &name, unsigned int lvl) {
    if(search_type_scope(name, lvl)==EOF_) return false;
    return true;
}