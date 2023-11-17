/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 11:54:34 by imisumi           #+#    #+#             */
/*   Updated: 2023/11/17 03:47:37 by imisumi-wsl      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

bool	allocate_philos(t_data *data)
{
	data->philos = malloc(sizeof(t_philo) * data->philo_count);
	if (data->philos == NULL)
	{
		cleanup(data);
		return (error_msg("Malloc error\n"), false);
	}
	return (true);
}

bool	create_philos(t_data *data)
{
	int		i;
	int64_t	philo_count;

	i = 0;
	philo_count = data->philo_count;
	if (allocate_philos(data) == false)
		return (false);
	while (i < philo_count)
	{
		data->philos[i].id = i + 1;
		data->philos[i].meal_count = 0;
		data->philos[i].m_meal = &data->m_meals[i];
		if (data->philos[i].id % 2)
		{
			data->philos[i].m_fork_1 = &data->m_forks[i];
			data->philos[i].m_fork_2 = &data->m_forks[(i + 1) % philo_count];
		}
		else
		{
			data->philos[i].m_fork_1 = &data->m_forks[(i + 1) % philo_count];
			data->philos[i].m_fork_2 = &data->m_forks[i];
		}
		i++;
	}
	return (true);
}

void	cleanup(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		if (data->m_forks != NULL)
			pthread_mutex_destroy(&data->m_forks[i]);
		if (data->m_meals != NULL)
			pthread_mutex_destroy(&data->m_meals[i]);
		i++;
	}
	free(data->m_forks);
	free(data->m_meals);
	free(data->philos);
	free(data->threads);
}

static void	join_threads(t_data *data, int max)
{
	int	i;

	i = 0;
	while (i < max)
	{
		pthread_join(data->threads[i], NULL);
		i++;
	}
}

//? do I print sleeping and thinking after the last meal?
int	main(int argc, char *argv[])
{
	t_data	data;

	memset(&data, 0, sizeof(t_data));
	if (valid_input(argc, argv, &data) == false)
		return (EXIT_FAILURE);
	if (init_mutex(&data) == false)
		return (EXIT_FAILURE);
	if (create_philos(&data) == false)
		return (EXIT_FAILURE);
	if (create_threads(&data) == false)
		return (EXIT_FAILURE);
	while (mutex_get_int64(&data.m_monitor, &data.monitoring) != data.philo_count)
		usleep(10);
	monitor_philos(&data);
	join_threads(&data, data.philo_count);
	cleanup(&data);
	return (EXIT_SUCCESS);
}
