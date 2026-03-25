#include "include/table.h"
#include "include/lexer.h"
#include <ranges>
#include <string>
#include <utility>

std::vector<std::unordered_map<std::string, symbol>> table;
std::unordered_map<std::string, fsymbol> ftable;


token_value search_value(const std::string &name) {
    for(auto &scope : std::ranges::reverse_view(table)) {
        if(scope.contains(name)) {
            return scope.at(name).value;
        }
    }
    return nothing{};
}

symbol search(const std::string &name) {
    for(auto &scope : std::ranges::reverse_view(table)) {
        if(scope.contains(name)) {
            return scope.at(name);
        }
    }
    return symbol{};
}

token_type search_type(const std::string &name) {
    for(auto &scope : std::ranges::reverse_view(table)) {
        if(scope.contains(name)) {
            return scope.at(name).type;
        }
    }
    return EOF_;
}

token_type search_type_scope(const std::string &name, unsigned int lvl) {
    if(table.at(lvl).contains(name)) return table.at(lvl).at(name).type;
    return EOF_;
}

void insert(const std::string &name,token_type type, token_value val, bool is_const, u64 size, bool is_array, bool comptime) {
    if(table.empty()) return;
    table[table.size()-1].insert_or_assign(name, symbol{type, std::move(val), is_const, size, is_array, comptime});
}

void insert_top(const std::string &name,token_type type,token_value val, bool is_const, u64 size,bool is_array, bool comptime) {
    if(table.empty()) return;
    table[0].insert_or_assign(name, symbol{type, std::move(val), is_const, size, is_array, comptime});
}

bool exist(const std::string &name) {
    if(search(name).type==EOF_) return false;
    return true;
}

bool exist_in_scope(const std::string &name, unsigned int lvl) {
    if(search_type_scope(name, lvl)==EOF_) return false;
    return true;
}


