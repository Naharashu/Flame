CXX = g++
CXX_CLANG = clang++

clangpp: clangpp_main

all: main

main:
	$(CXX) main.cpp lexer.cpp parser.cpp -pipe -O2 -o flame -Wall -Wextra 

clangpp_main:
	$(CXX_CLANG) main.cpp lexer.cpp parser.cpp -pipe -O2 -o flame -Wall -Wextra

clean:
	rm -f flame
	rm -f temp_flame.cpp
