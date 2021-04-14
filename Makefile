# Trình biên dịch được sử dụng là gcc 
CC	=	gcc

# -Iinclude thêm các file header trong thư mục include vào project 
# -Ilib thêm các file library trong thư mục lib vào project
# -lbt thêm thư viện bt vào project 
# -lreadline thêm thư viện readline vào project 
CFLAGS	= -g -Wall -Iinclude -Llib -lbt -lreadline

# Lệnh xóa 
RM	=	rm -f

# Thư mục file thực thi được tạo ra 
BIN_FOLDER = out

# Đường dẫn đến main.c
SRCS	=	src/main.c

# File .o 
OBJS	=	$(SRCS:.c=.o)

# Cho hđh Windows 
ifeq ($(OS),Windows_NT)
	NAME	=	main.exe
else
	NAME	=	main
endif

# Biên dịch ra file thực thi 
all: $(BIN_FOLDER)/$(NAME)

$(BIN_FOLDER)/$(NAME):$(OBJS)
	$(CC) -o $(BIN_FOLDER)/$(NAME) ${SRCS} $(CFLAGS)

# Xóa file .o và file thực thi 
clean:
	$(RM) $(OBJS)
	$(RM) $(BIN_FOLDER)/$(NAME)

.PHONY: all clean