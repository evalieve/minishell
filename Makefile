NAME = minishell

HEADER = include/minishell.h

LIBFT = libft/libft.a

SRCS =  src/builtins/cd.c \
		src/builtins/echo.c \
		src/builtins/env.c \
		src/builtins/exit.c \
		src/builtins/export_utils.c \
		src/builtins/export_utils2.c \
		src/builtins/export.c \
		src/builtins/pwd.c \
		src/builtins/unset.c \
		src/enviroment/envp.c \
		src/error/free/free_utils.c \
		src/error/free/free.c \
		src/error/fatal.c \
		src/error/syntax_error.c \
		src/error/wrap_func.c \
		src/error/wrap_func2.c \
		src/executor/exec_builtin.c \
		src/executor/exec_utils.c \
		src/executor/exec.c \
		src/parser/expander/expander_util.c \
		src/parser/expander/expander.c \
		src/redirections/redir_utils.c \
		src/redirections/redir.c \
		src/tokenizer/id_tokens.c \
		src/tokenizer/merge_add.c \
		src/tokenizer/nodes_redir_utils.c \
		src/tokenizer/parser.c \
		src/tokenizer/quotes.c \
		src/tokenizer/split_utils.c \
		src/tokenizer/tokenizer.c \
		src/tokenizer/tokens.c \
		src/tokenizer/value_check.c \
		src/signals/sig.c \
		src/utils/utils.c \
		main.c

SRC_DIR = src/
OBJ_DIR = obj/
INCL_DIR = ./include/

OBJ = $(subst $(SRC_DIR),$(OBJ_DIR),$(SRCS:.c=.o))

CC = gcc
CFLAGS = -Wall -Werror -Wextra

RM = rm -rf
INCL = -I $(INCL_DIR)

ifdef DEBUG
    CFLAGS += -g
endif

ifdef FSAN
    CFLAGS += -fsanitize=address,undefined
endif

all : $(LIBFT) $(NAME)


$(NAME) : $(OBJ)
	$(CC) $(INCL) $(CFLAGS) -o $@ $^ $(LIBFT) -lreadline -L/opt/homebrew/opt/readline/lib

$(OBJ_DIR)%.o : $(SRC_DIR)%.c
	@mkdir -p $(@D)
	@$(CC) $(INCL) $(CFLAGS) -c $< -o $@  -I/opt/homebrew/opt/readline/include 

$(LIBFT) :
	$(MAKE) -C libft

clean :
	$(RM) $(OBJ_DIR) libft/*.o main.o

fclean : clean
	$(RM) $(NAME) $(LIBFT)

re : fclean all

debug :
	$(MAKE) DEBUG=1

rebug : fclean debug

fsan :
	$(MAKE) FSAN=1 DEBUG=1
    
refsan : fclean fsan

.PHONY : all clean fclean re
