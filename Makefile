CC	=	gcc

CFLAGS	= -g -Wall -Iinclude -Llib -lbt -lreadline

RM	=	rm -f

BIN_FOLDER = out

SRCS	=	src/main.c

OBJS	=	$(SRCS:.c=.o)

ifeq ($(OS),Windows_NT)
	NAME	=	main.exe
else
	NAME	=	main
endif

all: $(BIN_FOLDER)/$(NAME)

$(BIN_FOLDER)/$(NAME):$(OBJS)
	$(CC) -o $(BIN_FOLDER)/$(NAME) ${SRCS} $(CFLAGS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(BIN_FOLDER)/$(NAME)

re: fclean all

.PHONY: all clean fclean re