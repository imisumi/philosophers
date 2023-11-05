/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 21:15:55 by ichiro            #+#    #+#             */
/*   Updated: 2023/11/05 05:49:38 by imisumi-wsl      ###   ########.fr       */
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

// # define P_EAT "\x1b[1;33mis eating\x1b[0m"
// # define P_SLEEP "\x1b[1;34mis sleeping\x1b[0m"
// # define P_THINK "\x1b[1;35mis thinking\x1b[0m"
// # define P_FORK "\x1b[1;32mhas taken a fork\x1b[0m"
// # define P_DIED "\x1b[1;31mdied\x1b[0m"

# define P_EAT "is eating"
# define P_SLEEP "is sleeping"
# define P_THINK "is thinking"
# define P_FORK "has taken a fork"
# define P_DIED "died"

// enum e_action
// {
// 	FORK,
// 	EATING,
// 	SLEEPING,
// 	THINKING
// };

// typedef struct s_philo
// {
// 	int				id;

// 	int				meal_count;
// 	pthread_mutex_t	m_meal_count;

// 	int64_t			last_meal;
// 	pthread_mutex_t	m_meal_time;

// 	pthread_t		thread;
// }	t_philo;

// typedef struct s_seat
// {
// 	bool			head;

// 	t_philo			philo;

// 	pthread_mutex_t	fork;

// 	struct s_data	*data;
// 	struct s_seat	*next;
// 	struct s_seat	*prev;
// }	t_seat;

// typedef struct s_data
// {
// 	int64_t			philo_count;
// 	int64_t			time_to_die;
// 	int64_t			time_to_eat;
// 	int64_t			time_to_sleep;
// 	int64_t			times_to_eat;
// 	int64_t			start_time;
// 	t_seat			*seats;
// 	bool			dead;
// 	pthread_mutex_t	m_state;
// }	t_data;

typedef struct s_data	t_data;

typedef struct s_philo
{
	uint32_t	id;
	
	uint32_t	left_fork;
	uint32_t	right_fork;
	
	uint64_t	meal_count;
	int64_t		last_meal;
	
	t_data		*data;
}	t_philo;

typedef struct s_data
{
	bool		s_dead;

	int64_t	philo_count;
	int64_t	time_to_die;
	int64_t	time_to_eat;
	int64_t	time_to_sleep;
	int64_t	s_times_to_eat;
	
	int64_t		start_time;

	pthread_mutex_t	m_dead;

	pthread_mutex_t	*m_forks;
	pthread_mutex_t	*m_last_meal;
	pthread_mutex_t	*m_eat;

	pthread_mutex_t	m_print;

	t_philo			*philos;

	pthread_t		*threads;
	
}	t_data;

#endif