PROJ_NAME=ss
C_SOURCE=$(wildcard src/*.c)
H_SOURCE=$(wildcard src/*.h)
OBJ=$(C_SOURCE:.c=.o)

CC=gcc
CC_FLAGS=-c \
	-g

all: $(PROJ_NAME)

$(PROJ_NAME): $(OBJ)
	$(CC) -o $@ $^
	@ rm src/*.o

src/%.o: src/%.c src/%.h
	$(CC) -o $@ $< $(CC_FLAGS)

src/main.o: src/main.c $(H_SOURCE)
	$(CC) -o $@ $< $(CC_FLAGS)

clean:
	rm -f *.o $(PROJ_NAME) *~
