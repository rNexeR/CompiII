TARGET=gramatica
C_SRCFILES = main.c expression_parser.c gramatica.c
OBJ_FILES = $(C_SRCFILES: .c=.o)
.PHONY: clean

$(TARGET): $(OBJ_FILES)
	gcc -o $@ $(OBJ_FILES)

gramatica.c: gramatica.l
	flex -o $@ $^

%.o: %.c token.h
	gcc -c -o $@ $<
	# -c para hacer object files
	# -d para debug

run: $(TARGET)
	./$(TARGET) < input1.txt

clean: 
	rm -f gramatica.c
	rm -f gramatica