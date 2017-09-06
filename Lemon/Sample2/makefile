TARGET=exe
LEXER_SRC=lexer.cpp
PARSER_SRC=parser.cpp
SRC_FILES=$(PARSER_SRC) $(LEXER_SRC) main.cpp
OBJ_FILES=${SRC_FILES:.cpp=.o}

.PHONY: clean

$(TARGET): $(OBJ_FILES)
	g++ -o $@ $^

$(PARSER_SRC): grammar.y
	lemon -o$@ -dtokens.h -Tlempar.c $^

$(LEXER_SRC): lexer.l
	flex -o $@ $^

%.o : %.cpp
	g++ -std=c++11 -c -o $@ $^

clean:
	rm -f $(PARSER_SRC) 
	rm -f $(LEXER_SRC)
	rm -f tokens.h
	rm -f $(TARGET)