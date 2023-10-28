/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 21:15:55 by ichiro            #+#    #+#             */
/*   Updated: 2023/10/28 14:19:26 by imisumi-wsl      ###   ########.fr       */
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

# define AVAILABLE ture

enum e_state
{
	DEAD = 0,
	THINKING,
	EATING,
	SLEEPING
};

typedef struct s_philo
{
	int			id;
	pthread_t	thread;
	int			meal_count;
	int64_t		last_meal;
	pthread_mutex_t	meal_mutex;
	pthread_mutex_t	m_meal_count;
	pthread_mutex_t	state_mutex;
	enum e_state	state;
}	t_philo;

typedef struct s_seat
{
	bool	head;
	t_philo	philo;
	// bool	fork;
	pthread_mutex_t	fork;

	struct s_data *data;
	// struct s_seat *left;
	// struct s_seat *right;
	struct s_seat *next;
	struct s_seat *prev;
}	t_seat;

typedef struct s_data
{
	int64_t				philo_count;
	int64_t				time_to_die;
	int64_t				time_to_eat;
	int64_t				time_to_sleep;
	int64_t				times_to_eat;
	int64_t			start_time;
	// pthread_mutex_t	*forks;
	t_seat			*seats;
}	t_data;


#endif