TARGET=C1Exa
EXPR_LEXER=expression_lexer.cpp
EXPR_PARSER=expression_parser.cpp

SRC_FILES=$(EXPR_LEXER) $(EXPR_PARSER)
BISON_SRC_FILES=$(SRC_FILES) Bison/ast.cpp Bison/main.cpp
LEMON_SRC_FILES=$(SRC_FILES) Lemon/ast.cpp Lemon/main.cpp

OBJ_FILES=$(SRC_FILES: .cpp=.o)
BISON_OBJ_FILES=$(BISON_SRC_FILES: .cpp=.o)
LEMON_OBJ_FILES=$(LEMON_SRC_FILES: .cpp=.o)

.PHONY: clean

Lemon: lemon_lexer $(LEMON_OBJ_FILES)
	g++ -std=c++11 -ILemon -o $(TARGET) $(LEMON_OBJ_FILES)

Bison: bison_lexer $(BISON_OBJ_FILES)
	g++ -std=c++11 -IBison -o $(TARGET) $(BISON_OBJ_FILES)

lemon_lexer: Lemon/lexer.l
	flex -o $(EXPR_LEXER) $<
	./lemon -o$(EXPR_PARSER) -Tlempar.c -dLemon/tokens.h Lemon/grammar.y 

bison_lexer: Bison/lexer.l
	flex -o $(EXPR_LEXER) $<
	bison -rall -o $(EXPR_PARSER) --defines=Bison/tokens.h Bison/grammar.y

%.o: %.cpp tokens.h
	g++ -std=c++11 -c -o $< $@

Lemon_run: Lemon run

Bison_run: Bison run

run: 
	./$(TARGET) input1.txt

clean:
	rm -rf $(TARGET)
	rm -rf *.o
	rm -rf *.output
	rm -rf tokens.h
	rm -rf $(EXPR_LEXER)
	rm -rf $(EXPR_PARSER)