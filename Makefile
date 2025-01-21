NAME := pipex
BONUS_NAME := pipex_bonus
OBJECT_DIR := obj
LIBFT_DIR := libft
INCLUDE_DIR := includes
SRC_DIR := srcs
BONUS_DIR := bonus
FILES := main.c pipex.c pipe.c utils.c args_parser.c
SRCS := $(addprefix $(SRC_DIR)/, $(FILES))
BONUS_FILES := main_bonus.c \
			bonus_utils.c
CFLAGS := -Wall -Wextra -Werror -g
IFLAGS := -I$(LIBFT_DIR) -I$(INCLUDE_DIR)
LFLAGS := -L$(LIBFT_DIR) -lft
OBJECTS := $(addprefix $(OBJECT_DIR)/,$(FILES:.c=.o))
OBJS_WITHOUT_MAIN := $(filter-out $(OBJECT_DIR)/main.o, $(OBJECTS))
BONUS_OBJ :=  $(addprefix $(BONUS_DIR)/,$(BONUS_FILES:.c=.o))
CC := cc
UNAME := $(shell uname)
ifeq ($(UNAME),Darwin)
	LEAK_TOOL := leaks -atExit --
else
	LEAK_TOOL := valgrind --leak-check=full
endif

.PHONY: all 
all: $(NAME)

$(NAME): $(OBJECTS) $(LIBFT_DIR)/libft.a
	$(CC) $(CFLAGS) $(OBJECTS) $(LFLAGS) -o $(NAME)

$(LIBFT_DIR)/libft.a:
	$(MAKE) -C $(LIBFT_DIR)

$(OBJECT_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJECT_DIR)
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@ 

$(BONUS_DIR)/%.o: $(BONUS_DIR)/%.c
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@


.PHONY: bonus
bonus: $(BONUS_NAME)

$(BONUS_NAME): $(BONUS_OBJ) $(OBJECTS) $(LIBFT_DIR)/libft.a
	$(CC) $(CFLAGS) $(BONUS_OBJ) $(OBJS_WITHOUT_MAIN) $(LFLAGS) -o $(BONUS_NAME) 

.PHONY: re
re: fclean all

.PHONY: clean
clean:
	rm -rf $(OBJECT_DIR)
	rm -f $(BONUS_OBJ)
	$(MAKE) -C $(LIBFT_DIR) clean

.PHONY: fclean
fclean: clean
	rm -f $(NAME) $(BONUS_NAME) /tmp/out.txt
	$(MAKE) -C $(LIBFT_DIR) fclean

.PHONY: leak
leak: $(NAME)
	$(LEAK_TOOL) ./$(NAME) ./srcs/main.c "cat" "wc -l" /tmp/out.txt
