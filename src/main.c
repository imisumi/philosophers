/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 11:54:34 by imisumi           #+#    #+#             */
/*   Updated: 2023/11/16 03:51:13 by imisumi-wsl      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	destory_forks(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		pthread_mutex_destroy(&data->m_forks[i]);
		i++;
	}
	free(data->m_forks);
}

void	destory_meals(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		pthread_mutex_destroy(&data->m_meals[i]);
		i++;
	}
	free(data->m_meals);
}

int64_t	current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec) * 1000 + (tv.tv_usec) / 1000);
}

bool	create_threads(t_data *data)
{
	int	i;

	i = 0;
	data->threads = malloc(sizeof(pthread_t) * data->philo_count);
	if (data->threads == NULL)
		return (error_msg("Malloc error\n"), false);
	pthread_mutex_lock(&data->m_state);
	data->dead = false;
	data->start_time = current_time();
	while (i < data->philo_count)
	{
		// data->philos[i].last_meal = data->start_time;
		data->philos[i].data = data;
		data->philos[i].full = false;
		if (pthread_create(&data->threads[i], NULL, &routine, &data->philos[i]) != 0)
		{
			error_msg("Thread create error\n");
			return (false);
		}
		i++;
	}
	data->start_time = current_time();
	pthread_mutex_unlock(&data->m_state);
	return (true);
}

bool	create_philos(t_data *data)
{
	int	i;

	data->philos = malloc(sizeof(t_philo) * data->philo_count);
	if (data->philos == NULL)
		return (error_msg("Malloc error\n"), false);
	i = 0;
	while (i < data->philo_count)
	{
		data->philos[i].id = i + 1;
		data->philos[i].meal_count = 0;
		data->philos[i].m_meal = &data->m_meals[i];
		if (data->philos[i].id % 2)
		{
			data->philos[i].m_fork_1 = &data->m_forks[i];
			data->philos[i].m_fork_2 = &data->m_forks[(i + 1) % data->philo_count];
		}
		else
		{
			data->philos[i].m_fork_1 = &data->m_forks[(i + 1) % data->philo_count];
			data->philos[i].m_fork_2 = &data->m_forks[i];
		}
		i++;
	}
	return (true);
}


void	cleanup(t_data *data)
{
	destory_forks(data);
	destory_meals(data);
	free(data->philos);
	free(data->threads);
}

bool	init_mutexes(t_data *data)
{
	int	i;

	data->m_forks = malloc(sizeof(t_mutex) * data->philo_count);
	if (data->m_forks == NULL)
		return (error_msg("Malloc error\n"), false);
	memset(data->m_forks, 0, sizeof(t_mutex) * data->philo_count);
	data->m_meals = malloc(sizeof(t_mutex) * data->philo_count);
	if (data->m_meals == NULL)
		return (error_msg("Malloc error\n"), false);
	memset(data->m_meals, 0, sizeof(t_mutex) * data->philo_count);
	i = 0;
	while (i < data->philo_count)
	{
		if (pthread_mutex_init(&data->m_forks[i], NULL) != 0)
		{
			error_msg("Mutex init error\n");
			destory_forks(data);
			return (false);
		}
		if (pthread_mutex_init(&data->m_meals[i], NULL) != 0)
		{
			error_msg("Mutex init error\n");
			destory_forks(data);
			return (false);
		}
		i++;
	}
	pthread_mutex_init(&data->m_state, NULL);
	return (true);
}

// void	monitor_philos(t_data *data)
// {
// 	int		i;
// 	int		j;
// 	int64_t	meal_count;
// 	int64_t	last_meal;
// 	int64_t	full_philos;

// 	i = 0;
// 	full_philos = 0;
// 	while (full_philos != data->philo_count)
// 	{
// 		i = 0;
// 		while (i < data->philo_count)
// 		{
// 			// if (data->philos[i].full == false)
// 			// {
// 			// 	pthread_mutex_lock(&data->m_state);
// 			// 	last_meal = current_time() - data->philos[i].last_meal;
// 			// 	meal_count = data->philos[i].meal_count;
// 			// 	if (meal_count == data->times_to_eat)
// 			// 	{
// 			// 		data->philos[i].full = true;
// 			// 		full_philos++;
// 			// 		if (full_philos == data->philo_count)
// 			// 		{
// 			// 			pthread_mutex_unlock(&data->m_state);
// 			// 			return ;
// 			// 		}
// 			// 	}
// 			// 	else if (last_meal >= data->time_to_die)
// 			// 	{
// 			// 		printf("time to die = %ld\n", data->time_to_die);
// 			// 		printf("last meal = %ld\n", last_meal);
// 			// 		data->dead = true;
// 			// 		printf("%ld	%d died\n", current_time() - \
// 			// 			data->start_time, data->philos[i].id);
// 			// 		pthread_mutex_unlock(&data->m_state);
// 			// 		printf("full_philos = %ld\n", meal_count);
// 			// 		printf("full_philos = %ld\n", data->times_to_eat);
// 			// 		return ;
// 			// 	}
// 			// 	pthread_mutex_unlock(&data->m_state);
// 			// }
// 			i++;
// 		}
// 		usleep(1);
// 	}
// }

void	monitor_philos(t_data *data)
{
	int		i;
	int		j;
	int64_t	meal_count;
	int64_t	last_meal;
	int64_t	full_philos;

	i = 0;
	full_philos = 0;
	int x = 0;
	while (full_philos != data->philo_count)
	{
		i = 0;
		while (i < data->philo_count)
		{
			if (data->philos[i].full == false)
			{
				pthread_mutex_lock(&data->m_state);
				pthread_mutex_lock(data->philos[i].m_meal);
				last_meal = current_time() - data->philos[i].last_meal;
				meal_count = data->philos[i].meal_count;
				if (meal_count == data->times_to_eat)
				{
					data->philos[i].full = true;
					full_philos++;
					if (full_philos == data->philo_count)
					{
						pthread_mutex_unlock(data->philos[i].m_meal);
						pthread_mutex_unlock(&data->m_state);
						return ;
					}
				}
				else if (last_meal >= data->time_to_die)
				{
					
					// printf("time to die = %ld\n", data->time_to_die);
					// printf("last meal = %ld\n", last_meal);
					data->dead = true;
					printf("%ld	%d died\n", current_time() - \
						data->start_time, data->philos[i].id);
					// printf("full_philos = %ld\n", meal_count);
					// printf("full_philos = %ld\n", data->times_to_eat);
					pthread_mutex_unlock(data->philos[i].m_meal);
					pthread_mutex_unlock(&data->m_state);
					return ;
				}
				pthread_mutex_unlock(data->philos[i].m_meal);
				pthread_mutex_unlock(&data->m_state);
			}
			i++;
		}
		usleep(1);
		// if (x == 1000000)
		// 	break ;
		// x++;
	}
}

int64_t	mutex_get_int64(t_mutex *mutex, int64_t *value)
{
	int64_t	tmp;

	pthread_mutex_lock(mutex);
	tmp = *value;
	pthread_mutex_unlock(mutex);
	return (tmp);
}

//? do I print sleeping and thinking after the last meal?
int	main(int argc, char *argv[])
{
	t_data	data;

	pthread_mutex_init(&data.m_monitor, NULL);
	data.monitoring = 0;

	// memset(&data, 0, sizeof(t_data));
	if (valid_input(argc, argv, &data) == false)
		return (EXIT_FAILURE);
	if (init_mutexes(&data) == false)
		return (EXIT_FAILURE);
	if (create_philos(&data) == false)
		return (EXIT_FAILURE);
	if (create_threads(&data) == false)
		return (EXIT_FAILURE);

	while (mutex_get_int64(&data.m_monitor, &data.monitoring) != data.philo_count)
		usleep(10);

	
	// printf("monitoring = %ld\n", data.monitoring);
	monitor_philos(&data);

	int i = 0;
	while (i < data.philo_count)
	{
		pthread_join(data.threads[i], NULL);
		i++;
	}


	// free(data.m_forks);
	cleanup(&data);
	return (EXIT_SUCCESS);
}
