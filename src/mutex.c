/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 11:54:34 by imisumi           #+#    #+#             */
/*   Updated: 2023/11/17 02:56:11 by imisumi-wsl      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static bool	mutex_error(t_data *data)
{
	error_msg("Mutex init error\n");
	cleanup(data);
	return (false);
}

static bool	allocate_mutex(t_data *data)
{
	data->m_forks = malloc(sizeof(t_mutex) * data->philo_count);
	if (data->m_forks == NULL)
	{
		cleanup(data);
		return (false);
	}
	memset(data->m_forks, 0, sizeof(t_mutex) * data->philo_count);
	data->m_meals = malloc(sizeof(t_mutex) * data->philo_count);
	if (data->m_meals == NULL)
	{
		cleanup(data);
		return (false);
	}
	memset(data->m_meals, 0, sizeof(t_mutex) * data->philo_count);
	return (true);
}

bool	init_mutex(t_data *data)
{
	int	i;

	if (allocate_mutex(data) == false)
		return (false);
	i = 0;
	while (i < data->philo_count)
	{
		if (pthread_mutex_init(&data->m_forks[i], NULL) != 0)
			return (mutex_error(data));
		if (pthread_mutex_init(&data->m_meals[i], NULL) != 0)
			return (mutex_error(data));
		i++;
	}
	if (pthread_mutex_init(&data->m_state, NULL) != 0)
		return (mutex_error(data));
	if (pthread_mutex_init(&data->m_monitor, NULL) != 0)
		return (mutex_error(data));
	return (true);
}
