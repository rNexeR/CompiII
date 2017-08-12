TARGET=gramatica
EXPR_PARSER_SRC=expression_parser.cpp
EXPR_LEXER_SRC=expression_lexer.cpp
SRCFILES=$(EXPR_PARSER_SRC) $(EXPR_LEXER_SRC) ast.cpp main.cpp
OBJ_FILES=$(SRCFILES: .cpp=.o)
.PHONY: clean

$(TARGET): $(OBJ_FILES)
	g++ -o $@ $(OBJ_FILES)

$(EXPR_LEXER_SRC): gramatica.l
	flex -o $@ $^

$(EXPR_PARSER_SRC): expression.y
	bison -rall --defines=token.h -o $@ $<

%.o: %.cpp token.h
	g++ -c -o $@ $<
	# -c para hacer object files
	# -d para debug

run: $(TARGET)
	./$(TARGET) input1.txt

clean: 
	rm -f expression_parser.cpp expression_lexer.cpp
	rm -f $(TARGET)
	rm -f *.o
	rm -f token.h