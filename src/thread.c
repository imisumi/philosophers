/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi <imisumi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 11:54:34 by imisumi           #+#    #+#             */
/*   Updated: 2023/11/16 13:42:56 by imisumi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

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
