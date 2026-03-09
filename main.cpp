#include "include/ast.h"
#include "include/lexer.h"
#include "include/parser.h"
#include "include/generator.h"
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <string>

int main(int argc, char* argv[]) { 
	if(argc>1) {
		if(strcmp(argv[1], "-v")==0) {
			std::cout << "Flame compiler 0.3\nBy Naharashu\n";
			return 0;
		}
	}
	bool compile_into_bin = true;
	std::string out_name = "out";
	bool lexer_output = true;
	for(int i=0;i<argc;i++) {
		if(strcmp(argv[i], "-C")==0) compile_into_bin = false;
		if(strcmp(argv[i], "-o")==0) out_name = argc>i+1 ? argv[i+1] : "out";
		if(strcmp(argv[i], "-no-lexer-debug")==0) lexer_output = false;
	}
    lexer lex;
    std::string code;
    code.clear();
    if(argc<2) {
        std::cerr << "Usage: flame [file.flame] -o [DEFAULT=out]\n";
        return 1; 
    }
    std::string line;
    std::ifstream file(argv[1]);
    while(std::getline(file, line)) {
        code += line + '\n';
    }
    file.close();
    std::vector<token> toks = lex.lex(code);
    parser parser_(toks);
    u64 i = 0;
    if(lexer_output) {
	for(auto x : toks) {
        	std::cout << x.type << ": " << i << ' ';
       		 i++;
    	}
    	std::cout << '\n';
    }
    std::vector<astptr> res = parser_.parse();
    generator gen;
    std::string code_ = gen.generate(res);
    std::ofstream out("temp_flame.cpp", std::ios::out | std::ios::binary);
    if(!out.is_open()) {
	std::cerr << "Cannot open temp file to generate cpp code\n"; 
	return 1;
    }
    out.write(code_.c_str(), code_.size());
    out.close();
    if(compile_into_bin) {
    	std::string output = "g++ temp_flame.cpp -o " + out_name; //" && rm -f temp.cpp";
    	system(output.c_str());
    }
    //std::cout << code_ << std::endl;
    return 0; 
}
