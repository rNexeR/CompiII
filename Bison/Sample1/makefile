TARGET=gramatica
C_SRCFILES = main.c expression_parser.c expr_lexer.c
OBJ_FILES = $(C_SRCFILES: .c=.o)
.PHONY: clean

$(TARGET): $(OBJ_FILES)
	gcc -o $@ $(OBJ_FILES)

expr_lexer.c: gramatica.l
	flex -o $@ $^

expression_parser.c: expression.y
	bison --defines=token.h -o $@ $<

%.o: %.c token.h
	gcc -c -o $@ $<
	# -c para hacer object files
	# -d para debug

run: $(TARGET)
	./$(TARGET) < input1.txt

clean: 
	rm -f gramatica.c expr_lexer.c
	rm -f $(TARGET)
	rm -f *.o
	rm -f token.h