/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi <imisumi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 11:54:34 by imisumi           #+#    #+#             */
/*   Updated: 2023/11/16 13:47:33 by imisumi          ###   ########.fr       */
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
