#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include "common.h"
#include "lexer.h"

using symbol = struct symbol {
    token_type type=EOF_;
    token_value value;
    bool is_const=false;
    u64 size=0;
    bool is_array=false;
    bool comptime=false;
};

using fsymbol = struct fsymbol {
    token_type type=EOF_;
    std::vector<symbol> args;
    bool comptime=false;
};


extern std::vector<std::unordered_map<std::string, symbol>> table;
extern std::unordered_map<std::string, fsymbol> ftable;

token_value search_value(const std::string &name);

symbol search(const std::string &name);

token_type search_type(const std::string &name);

token_type search_type_scope(const std::string &name, unsigned int lvl);


void insert(const std::string &name,token_type type, token_value val, bool is_const=false, u64 size=1, bool is_array=false, bool comptime=false);
void insert_top(const std::string &name,token_type type, token_value val,bool is_const=false, u64 size=1, bool is_array=false, bool comptime=false);

bool exist(const std::string &name);

bool exist_in_scope(const std::string &name, unsigned int lvl);

inline fsymbol* fsearch(const std::string &name) {
    auto it = ftable.find(name);
    if (it != ftable.end()) {
        return &it->second;
    }
    return nullptr;
}

inline bool fexist(const std::string &name) {
    if(fsearch(name)->type==EOF_) return false;
    return true;
}

inline void finsert(const std::string &name, const token_type &type, const std::vector<symbol> &args) {
    ftable.insert_or_assign(name, fsymbol{type, args, false});
}

inline void finsert_arg(const std::string &name, const symbol &arg) {
    fsymbol* func = fsearch(name);
    if(func) {
        func->args.emplace_back(arg);
    }
}