# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/12 00:14:46 by luevange          #+#    #+#              #
#    Updated: 2025/03/12 00:14:46 by luevange         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = so_long
CC = gcc
CFLAGS = -Wall -Wextra -Werror

# Verifica se la directory minilibx-darwin esiste, altrimenti usa MLX42
ifneq ("$(wildcard ./minilibx-darwin)","")
    MLX_PATH = ./minilibx-darwin
    MLX_LIB = $(MLX_PATH)/libmlx.a
    MLX_FLAGS = -L$(MLX_PATH) -lmlx -framework OpenGL -framework AppKit
else ifneq ("$(wildcard ./MLX42)","")
    MLX_PATH = ./MLX42
    MLX_LIB = $(MLX_PATH)/build/libmlx42.a
    MLX_FLAGS = -L$(MLX_PATH)/build -lmlx42 -framework Cocoa -framework OpenGL -framework IOKit
    # Aggiungi flag per MLX42 se necessario
    CFLAGS += -DUSE_MLX42
else
    # Fallback su altre possibili directory
    ifneq ("$(wildcard ./minilibx_macos)","")
        MLX_PATH = ./minilibx_macos
        MLX_LIB = $(MLX_PATH)/libmlx.a
        MLX_FLAGS = -L$(MLX_PATH) -lmlx -framework OpenGL -framework AppKit
    else ifneq ("$(wildcard ./minilibx_mms_20191025_beta)","")
        MLX_PATH = ./minilibx_mms_20191025_beta
        MLX_LIB = $(MLX_PATH)/libmlx.a
        MLX_FLAGS = -L$(MLX_PATH) -lmlx -framework OpenGL -framework AppKit
    else
        # Se non viene trovata nessuna directory conosciuta, usa un percorso generico
        MLX_PATH = ./minilibx
        MLX_LIB = $(MLX_PATH)/libmlx.a
        MLX_FLAGS = -L$(MLX_PATH) -lmlx -framework OpenGL -framework AppKit
    endif
endif

SRCS = so_long.c map_functions.c gameplay_functions.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(MLX_LIB):
	@echo "Compiling MiniLibX..."
	@if [ -d "$(MLX_PATH)" ]; then \
		if [ "$(MLX_PATH)" = "./MLX42" ]; then \
			mkdir -p $(MLX_PATH)/build; \
			cd $(MLX_PATH)/build && cmake .. && make; \
		else \
			make -C $(MLX_PATH); \
		fi \
	else \
		echo "Error: $(MLX_PATH) directory not found. Please clone the appropriate MiniLibX repository."; \
		exit 1; \
	fi

$(NAME): $(MLX_LIB) $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(MLX_FLAGS)
	@echo "Compilation completed!"

%.o: %.c
	$(CC) $(CFLAGS) -I$(MLX_PATH) -c $< -o $@

clean:
	rm -f $(OBJS)
	@if [ -d "$(MLX_PATH)" ]; then \
		make -C $(MLX_PATH) clean; \
	fi

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re