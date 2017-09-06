TARGET=generacion1
LEXER_SRC=lexer.cpp
PARSER_SRC=parser.cpp
SRC_FILES= $(PARSER_SRC) $(LEXER_SRC) main.cpp ast.cpp
OBJ_FILES= $(SRC_FILES:.cpp=.o)

.PHONY: clean

$(TARGET): $(OBJ_FILES)
	g++ -std=c++11 -o $@ $^

$(PARSER_SRC): parser.y
	bison --defines=tokens.h -rall -o $@ $<

$(LEXER_SRC): lexer.l
	flex -o $@ $<

%.o: %.cpp tokens.h
	g++ -c -std=c++11 -o $@ $<

run: $(TARGET)
	./$< < input.txt

clean:
	rm -f *.o
	rm -f $(LEXER_SRC)
	rm -f $(PARSER_SRC)
	rm -f $(TARGET)
	rm -f tokens.h
	rm -f *.output