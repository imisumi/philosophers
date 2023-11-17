# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/28 00:51:40 by ichiro            #+#    #+#              #
#    Updated: 2023/11/17 02:12:52 by imisumi-wsl      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CFLAGS = -g -pthread
# CFLAGS += -Wall -Wextra -Werror

cc = gcc

OBJS_DIR = .objs
SRC_DIR = src

BLUE := \033[1;34m
PINK := \033[1;38;5;206m
NC := \033[0m

SRCS =	main.c \
		input.c \
		routine.c \
		philo_actions.c \
		philo_actions_utils.c \
		thread.c \
		utils.c \
		monitoring.c \
		mutex.c

OBJS = $(addprefix $(OBJS_DIR)/,$(SRCS:.c=.o))

all: $(NAME)
	@echo "$(PINK)done$(NC)"

$(OBJS_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "$(BLUE)compiling: $<$(NC)"
	@$(cc) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	@$(cc) $(CFLAGS) $^ -o $(NAME)

clean:
	@rm -rf $(OBJS_DIR)

fclean:
	@rm -rf $(OBJS_DIR)
	@rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
