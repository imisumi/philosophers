/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 11:54:34 by imisumi           #+#    #+#             */
/*   Updated: 2023/11/17 03:22:56 by imisumi-wsl      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

bool	allocate_threads(t_data *data)
{
	data->threads = malloc(sizeof(pthread_t) * data->philo_count);
	if (data->threads == NULL)
	{
		cleanup(data);
		return (error_msg("Malloc error\n"), false);
	}
	return (true);
}

bool	create_threads(t_data *data)
{
	int	i;

	i = 0;
	if (allocate_threads(data) == false)
		return (false);
	pthread_mutex_lock(&data->m_state);
	while (i < data->philo_count)
	{
		data->philos[i].data = data;
		data->philos[i].full = false;
		if (pthread_create(&data->threads[i], NULL, &routine, &data->philos[i]) != 0)
		{
			data->dead = true;
			pthread_mutex_unlock(&data->m_state);
			join_threads(data, i);
			cleanup(data);
			error_msg("Thread create error\n");
			return (false);
		}
		i++;
	}
	data->start_time = current_time();
	pthread_mutex_unlock(&data->m_state);
	return (true);
}
