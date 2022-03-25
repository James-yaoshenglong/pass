INC_DIR = ../include
SRC_DIR = ./
OBJ_DIR = ./object
LIB_DIR = ../lib

TARGET = $(OBJ_DIR)/libInsert.a
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CC = gcc
CFLAGS = -c  
AR = ar

$(TARGET): $(OBJ_DIR) $(OBJ)
	$(AR) -r $(TARGET) $(OBJ) && \
	cp $(TARGET) $(LIB_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c  # 静态模规则，规则存在多个目标，执行多次命令
	$(CC) $(CFLAGS) -I$(INC_DIR) -o $@ $^

$(OBJ_DIR):
	mkdir object

.PHONY: clean
clean:
	-rm -rf $(OBJ_DIR)