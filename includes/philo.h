/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi <imisumi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 21:15:55 by ichiro            #+#    #+#             */
/*   Updated: 2023/11/17 14:17:47 by imisumi          ###   ########.fr       */
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
# include <string.h>

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

typedef pthread_mutex_t	t_mutex;
typedef struct s_data	t_data;

typedef enum e_action
{
	FORK,
	EATING,
	SLEEPING,
	THINKING
}	t_action;

typedef struct s_philo
{
	int			id;

	int64_t		meal_count;

	int64_t		last_meal;
	t_mutex		*m_meal;

	t_mutex		*m_fork_1;
	t_mutex		*m_fork_2;

	bool		full;

	t_data		*data;
}	t_philo;

typedef struct s_data
{
	bool		dead;

	int64_t		philo_count;
	int64_t		time_to_die;
	int64_t		time_to_eat;
	int64_t		time_to_sleep;
	int64_t		times_to_eat;
	int64_t		start_time;

	int64_t		monitoring;

	t_philo		*philos;

	t_mutex		*m_forks;
	t_mutex		*m_meals;
	t_mutex		m_monitor;
	t_mutex		m_state;

	pthread_t	*threads;
}	t_data;

// INPUT.C
bool	valid_input(int argc, char *argv[], t_data *data);

// THREAD.C
bool	create_threads(t_data *data);

// ROUTINE.C
void	*routine(void *arg);

bool	print_state(t_philo *philo, t_action action);

// PHILO_ACTIONS.C
bool	philo_eat(t_philo	*philo);
bool	philo_sleep(t_philo	*philo);
bool	philo_think(t_philo	*philo);
void	thinking(t_philo *philo);

// PHILO_UTILS_ACTIONS.C
bool	pickup_forks(t_philo	*philo);
void	drop_forks(t_philo	*philo);

// UTILS.C
int64_t	current_time(void);
int64_t	mutex_get_int64(t_mutex *mutex, int64_t *value);
void	error_msg(char *str);
size_t	ft_strlen(char *str);
bool	ft_usleep(t_philo *philo, int64_t time_to);
bool	mutex_get_bool(t_mutex *mutex, bool *value);

// MONITORING.C
void	monitor_philos(t_data *data);

// MUTEX.C
bool	init_mutex(t_data *data);

// MAIN.C
void	cleanup(t_data *data);
void	join_threads(t_data *data, int max);

#endif