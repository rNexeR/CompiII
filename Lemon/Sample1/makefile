TARGET=LemonGrammar
EXPR_PARSER_SRC=expression_parser.cpp
EXPR_LEXER_SRC=expression_lexer.cpp
SRC_FILES=$(EXPR_PARSER_SRC) $(EXPR_LEXER_SRC) ast.cpp main.cpp
OBJ_FILES=$(SRC_FILES: .cpp=.o)
.PHONY: clean

$(TARGET): $(OBJ_FILES)
	g++ -std=c++11 -o $@ $(OBJ_FILES)

$(EXPR_LEXER_SRC): lexer.l
	flex -o $@ $^

$(EXPR_PARSER_SRC): grammar.y
	./lemon -o$@ -dtokens.h -Tlempar.c $<

%.o: %.cpp tokens.h
	g++ -std=c++11 -c -o $@ $<

run: $(TARGET)
	./$(TARGET) input1.txt

clean:
	rm -f $(EXPR_PARSER_SRC) $(EXPR_LEXER_SRC)
	rm -f $(TARGET)
	rm -f *.o
	rm -f tokens.h