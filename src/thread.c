/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 11:54:34 by imisumi           #+#    #+#             */
/*   Updated: 2023/11/14 20:54:51 by imisumi-wsl      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

bool	create_threads(t_data *data)
{
	int		i;
	t_seat	*current;

	i = 0;
	current = data->seats;
	pthread_mutex_lock(&data->m_state);
	data->start_time = current_time();
	data->monitoring = false;
	data->sitting_philos = 0;
	while (i < data->philo_count)
	{
		current->philo.full = false;
		current->philo.last_meal = data->start_time;
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
	// mutex_inc_int64(&data->m_philo, &data->sitting_philos);
	// printf("sitting philos %ld\n", data->sitting_philos);
	while (mutex_get_bool(&data->m_monitor, &data->monitoring) == false)
	{
		usleep(100);
	}
	// usleep(1000);
	// printf("sitting philos %ld\n", data->sitting_philos);
	return (true);
}
