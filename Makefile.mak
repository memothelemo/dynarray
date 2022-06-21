CC		= g++
EXT		= .cpp

CFLAGS  = -Wall -O2
#CFLAGS	= -Wall -g -fPIC -pthread

SRC		= $(shell find src -name *$(EXT))
OBJ		= $(SRC:$(EXT)=.o)

EXEC	= ./build
RM 		= rm -rf

ifeq ($(OS), Windows_NT)
	EXEC	:= $(EXEC).exe
else
	EXEC	:= $(EXEC).out
endif

%.o: %$(EXT)
	$(CC) -o $@ -c $< $(CFLAGS)

test: clean exec
	$(EXEC)

exec: $(OBJ)
	$(CC) -o $(EXEC) $^

clean:
	$(RM) $(EXEC) $(OBJ)
