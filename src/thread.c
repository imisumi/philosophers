/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 11:54:34 by imisumi           #+#    #+#             */
/*   Updated: 2023/11/15 03:42:23 by imisumi-wsl      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

bool	create_threads(t_data *data)
{
	int		i;
	t_seat	*current;

	i = 0;
	current = data->seats;
	data->monitoring = false;
	data->sitting_philos = 0;
	pthread_mutex_lock(&data->m_state);
	while (i < data->philo_count)
	{
		current->philo.full = false;
		if (pthread_create(&current->philo.thread, NULL, \
				&routine, current) != 0)
		{
			data->dead = true;
			pthread_mutex_unlock(&data->m_state);
			finalize(data, i);
			return (false);
		}
		current = current->next;
		i++;
	}
	data->start_time = current_time();
	pthread_mutex_unlock(&data->m_state);
	return (true);
}
