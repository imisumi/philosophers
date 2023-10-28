/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 21:15:55 by ichiro            #+#    #+#             */
/*   Updated: 2023/10/28 17:23:35 by imisumi-wsl      ###   ########.fr       */
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

typedef struct s_philo
{
	int			id;
	pthread_t	thread;
	int			meal_count;
	int64_t		last_meal;
	pthread_mutex_t	m_meal_time;
	pthread_mutex_t	m_meal_count;
	pthread_mutex_t	state_mutex;
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
	int64_t			philo_count;
	int64_t			time_to_die;
	int64_t			time_to_eat;
	int64_t			time_to_sleep;
	int64_t			times_to_eat;
	int64_t			start_time;
	t_seat			*seats;
	bool			dead;
	// pthread_mutex_t	m_print;
	pthread_mutex_t	m_state;
}	t_data;


#endif