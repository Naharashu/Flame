#include "include/table.h"
#include "include/lexer.h"
#include <ranges>
#include <stdexcept>
#include <utility>

std::vector<std::unordered_map<std::string, std::pair<token_type, token_value>>> table;

token_value search_value(std::string &name) {
    bool found = false;
    for(auto &scope : std::ranges::reverse_view(table)) {
        if(scope.contains(name)) {
            return scope.at(name).second;
        }
    }
    return nothing{};
}

token_type search_type(std::string &name) {
    bool found = false;
    for(auto &scope : std::ranges::reverse_view(table)) {
        if(scope.contains(name)) {
            return scope.at(name).first;
        }
    }
    return EOF_;
}

void insert(std::string &name, token_type &type, token_value &val) {
    if(search_type(name)!=FUNC) {
        table[table.back()].insert_or_assign(name, std::make_pair(type, val));
    }
    throw std::runtime_error("Trying to assign function " + name + "\n");
}