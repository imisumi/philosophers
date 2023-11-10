/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 21:15:55 by ichiro            #+#    #+#             */
/*   Updated: 2023/11/10 22:15:44 by imisumi-wsl      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H

# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdint.h>
# include <fcntl.h>
# include <stddef.h>
# include <errno.h>
# include <time.h>
# include <sys/time.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <pthread.h>
# include <stdbool.h>
# include <math.h>

// # define RED "\x1b[1;31m"
// # define BLUE "\x1b[1;34m"
// # define GREEN "\x1b[1;32m"
// # define YELLOW "\x1b[1;33m"
// # define MAGENTA "\x1b[1;35m"
// # define RESET "\x1b[0m"

# define RED ""
# define BLUE ""
# define GREEN ""
# define YELLOW ""
# define MAGENTA ""
# define RESET ""

enum e_action
{
	FORK,
	EATING,
	SLEEPING,
	THINKING
};

typedef struct s_mutex_fork
{
	pthread_mutex_t	m_fork;
	bool			locked;
	pthread_mutex_t	m_lock;
}	t_mutex_fork;


typedef struct s_philo
{
	int				id;

	int				meal_count;

	int64_t			last_meal;
	pthread_mutex_t	m_meal;

	pthread_t		thread;
}	t_philo;



typedef struct s_seat
{
	bool			head;

	t_philo			philo;

	pthread_mutex_t	fork;
	t_mutex_fork	f;

	struct s_data	*data;
	struct s_seat	*next;
	struct s_seat	*prev;
}	t_seat;

typedef struct s_data
{
	int64_t			philo_count;
	int64_t			time_to_die;
	int64_t			time_to_eat;
	int64_t			time_to_sleep;
	int64_t			times_to_eat;
	int64_t			start_time;
	t_seat			*seats;
	bool			dead;
	pthread_mutex_t	m_state;
	pthread_mutex_t	m_print;
}	t_data;

// INPUT.C
bool	valid_input(int argc, char *argv[], t_data *data);

// SEAT.c
void	free_seat_fail(t_seat *head);
t_seat	*create_seat_node(t_data *data);
void	insert_seat(t_seat **head, t_seat *new);

// SETUP.C
bool	setup_table(t_data *data);

// THREAD.C
bool	create_threads(t_data *data);

// UTILS.C
size_t	ft_strlen(char *str);
int64_t	current_time(void);
bool	ft_usleep(t_seat *seat, int64_t time_to);
bool	print_state(t_seat *seat, enum e_action action);
bool	is_philo_dead(t_seat *seat);

// MAIN.C
void	finalize(t_data *data, int index);
bool	philo_can_continue(t_seat *seat);

#endif