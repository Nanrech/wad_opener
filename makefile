CC = gcc

SRC_DIR = src
INC_DIR = include
OBJ_DIR = bin/obj

EXE = bin/wad_ripper
CFLAGS = -I$(INC_DIR) -Wall -Wfatal-errors -Wextra -Werror -MMD -MP

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)


$(EXE): $(OBJ)
		$(CC) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
		$(CC) $(CFLAGS) -c $< -o $@

clean:
		rm $(OBJ_DIR)/*.o $(OBJ_DIR)/*.d
