CXX = g++

test:
	$(CXX) main.cpp lexer.cpp parser.cpp -O2 -o main -Wall -Wextra

clean:
	rm -f main
