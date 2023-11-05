/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 11:54:34 by imisumi           #+#    #+#             */
/*   Updated: 2023/11/05 06:00:14 by imisumi-wsl      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int64_t	current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec) * 1000 + (tv.tv_usec) / 1000);
}

size_t	ft_strlen(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

static void	error_msg(char *str)
{
	write(STDERR_FILENO, str, ft_strlen(str));
}

static bool	str_num_only(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (false);
		i++;
	}
	return (true);
}

//! positive number only
static int64_t	ft_atoi(char *str)
{
	int64_t	i;
	int64_t	num;

	if (str == NULL)
		return (-1);
	if (str_num_only(str) == false)
		return (error_msg("Numeric argument only\n"), -1);
	if (ft_strlen(str) > 9)
		return (error_msg("Number too big\n"), -1);
	i = 0;
	num = 0;
	while (str[i])
	{
		num = num * 10 + str[i] - 48;
		i++;
	}
	return (num);
}

bool	valid_input(int argc, char *argv[], t_data *data)
{
	if (argc < 5 || argc > 6)
		return (error_msg("Wrong number of arguments\n"), false);
	data->philo_count = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
	{
		data->s_times_to_eat = ft_atoi(argv[5]);
		if (data->s_times_to_eat < 1)
			return (false);
	}
	else
		data->s_times_to_eat = -1;
	if (data->philo_count < 1 || data->time_to_die < 1 || data->time_to_eat < 1
		|| data->time_to_sleep < 1)
		return (false);
	return (true);
}



void	free_data(t_data *data)
{
	free(data->philos);
	free(data->m_forks);
	free(data->m_eat);
}

void	destroy_fork_mutex(t_data *data, int i)
{
	while (i >= 0)
	{
		pthread_mutex_destroy(&data->m_forks[i]);
		printf("destroy_mutexes %d\n", i);
		i--;
	}
}

void	destroy_eat_mutex(t_data *data, int i)
{
	while (i >= 0)
	{
		pthread_mutex_destroy(&data->m_eat[i]);
		printf("destroy_mutexes %d\n", i);
		i--;
	}
}

bool	init_mutexes(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		printf("init_mutexes %d\n", i);
		if (pthread_mutex_init(&data->m_forks[i], NULL) != 0)
		{
			destroy_fork_mutex(data, i - 1);
			destroy_eat_mutex(data, i - 1);
			return (false);
		}
		if (pthread_mutex_init(&data->m_eat[i], NULL) != 0)
		{
			destroy_fork_mutex(data, i);
			destroy_eat_mutex(data, i - 1);
			return (false);
		}
		if (pthread_mutex_init(&data->m_last_meal[i], NULL) != 0)
		{
			destroy_fork_mutex(data, i);
			destroy_eat_mutex(data, i);
			return (false);
		}
		i++;
	}
	if (pthread_mutex_init(&data->m_dead, NULL) != 0)
	{
		destroy_fork_mutex(data, i - 1);
		destroy_eat_mutex(data, i - 1);
		return (false);
	}
	pthread_mutex_init(&data->m_print, NULL);
	return (true);
}

// void	mutex_set_time(t_data *data, pthread_mutex_t *mutex, int64_t time)
// {
// 	pthread_mutex_lock(mutex);
// 	*mutex = time;
// 	pthread_mutex_unlock(mutex);
// }

void	ft_usleep(t_data *data, int64_t time_to)
{
	int64_t	start;

	start = current_time();
	while (current_time() - start < time_to)
	{
		// if (philo_is_alive(seat) == false)
		// 	return ;
		usleep(256);
	}
}

void	print_state(t_philo *philo, char *str)
{
	pthread_mutex_lock(&philo->data->m_print);
	printf("%ld	%d	%s\n", current_time() - philo->data->start_time, philo->id, str);
	pthread_mutex_unlock(&philo->data->m_print);
}

void	pickup_fork(t_philo *philo, uint32_t fork)
{
	pthread_mutex_lock(&philo->data->m_forks[fork]);
	print_state(philo, P_FORK);
}

void	putdown_fork(t_philo *philo, uint32_t fork)
{
	pthread_mutex_unlock(&philo->data->m_forks[fork]);
}

bool	meal_time(t_philo *philo)
{
	if (philo->id % 2 == 1)
	{
		pickup_fork(philo, philo->right_fork);
		pickup_fork(philo, philo->left_fork);
	}
	else
	{
		pickup_fork(philo, philo->left_fork);
		pickup_fork(philo, philo->right_fork);
	}


	pthread_mutex_lock(&philo->data->m_last_meal[philo->id - 1]);
	philo->last_meal = current_time();
	pthread_mutex_unlock(&philo->data->m_last_meal[philo->id - 1]);

	print_state(philo, P_EAT);
	// ft_usleep(philo->data, philo->data->time_to_eat);

	if (philo->id % 2 == 1)
	{
		putdown_fork(philo, philo->left_fork);
		putdown_fork(philo, philo->right_fork);
	}
	else
	{
		putdown_fork(philo, philo->right_fork);
		putdown_fork(philo, philo->left_fork);
	}
	return (true);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = arg;
	// printf("%d		%d	left = %d, right = %d\n", philo->id, philo->id % 2, philo->left_fork, philo->right_fork);
	// if (philo->id % 2 == 1)
	// 	usleep(256);
	
	while (true)
	{
		if (meal_time(philo) == false)
			return (NULL);
		print_state(philo, P_SLEEP);
		ft_usleep(philo->data, philo->data->time_to_sleep);
		print_state(philo, P_THINK);
	}
	return (NULL);
}

void	set_philo_params(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		data->philos[i].id = i + 1;
		data->philos[i].left_fork = i;
		data->philos[i].right_fork = (i + 1) % data->philo_count;
		data->philos[i].data = data;
		i++;
	}
}

void	monitoring(t_data *data)
{
	int	i;

	i = 0;
	// printf("monitoring\n");
	// exit(0);
	while (true)
	{
		i = 0;
		while (i < data->philo_count)
		{
			pthread_mutex_lock(&data->m_last_meal[i]);
			if (current_time() - data->philos[i].last_meal >= data->time_to_die)
			{
				pthread_mutex_lock(&data->m_print);
				printf("%ld	%d	%s\n", current_time() - data->start_time, data->philos[i].id, P_DIED);
				pthread_mutex_unlock(&data->m_print);
				exit(0);
				return ;
			}
			pthread_mutex_unlock(&data->m_last_meal[i]);
			i++;
		}
		usleep(1);
	}
}

void	create_threads(t_data *data)
{
	int	i;

	set_philo_params(data);
	// i = 0;
	// while (i < data->philo_count)
	// {
	// 	// data->philos[i].data = data;
	// 	data->philos[i].id = i + 1;
	// 	i++;
	// }
	i = 0;
	data->start_time = current_time();
	// memset(data->threads, 0, sizeof(pthread_t) * data->philo_count);
	while (i < data->philo_count)
	{
		// printf("create_threads %d\n", i);
		pthread_mutex_lock(&data->m_last_meal[i]);
		data->philos[i].last_meal = data->start_time;
		pthread_mutex_unlock(&data->m_last_meal[i]);
		
		pthread_create(&data->threads[i], NULL, &routine, &data->philos[i]);
		// usleep(10);
		// usleep(1000);
		i++;
	}

	monitoring(data);

	i = 0;
	while (i < data->philo_count)
	{
		pthread_join(data->threads[i], NULL);
		i++;
	}
}

bool	init_philos(t_data *data)
{
	data->philos = malloc(sizeof(t_philo) * data->philo_count);
	if (data->philos == NULL)
		return (false);
	data->m_forks = malloc(sizeof(pthread_mutex_t) * data->philo_count);
	if (data->m_forks == NULL)
		return (free(data->philos), false);
	data->m_eat = malloc(sizeof(pthread_mutex_t) * data->philo_count);
	if (data->m_eat == NULL)
		return (free(data->philos), free(data->m_forks), false);
	data->m_last_meal = malloc(sizeof(pthread_mutex_t) * data->philo_count);
	if (data->m_last_meal == NULL)
		return (free(data->philos), free(data->m_forks), free(data->m_eat), false);
	data->threads = malloc(sizeof(pthread_t) * data->philo_count);
	if (data->threads == NULL)
		return (free(data->philos), free(data->m_forks), free(data->m_eat), free(data->m_last_meal), false);
	if (init_mutexes(data) == false)
		return (free_data(data), false);
	return (true);
}

//? do I print sleeping and thinking after the last meal?
int	main(int argc, char *argv[])
{
	t_data	data;

	if (valid_input(argc, argv, &data) == false)
		return (EXIT_FAILURE);
	if (init_philos(&data) == false)
		return (EXIT_FAILURE);
	create_threads(&data);
	// monitoring(&data);
	// finalize(&data);
	// pthread_mutex_destroy(&data.m_state);
	return (EXIT_SUCCESS);
}
