/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi <imisumi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 11:54:34 by imisumi           #+#    #+#             */
/*   Updated: 2023/11/20 15:13:34 by imisumi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void	lock_monitoring(t_data *data, int i)
{
	pthread_mutex_lock(&data->m_state);
	pthread_mutex_lock(data->philos[i].m_meal);
}

static void	unlock_monitoring(t_data *data, int i)
{
	pthread_mutex_unlock(data->philos[i].m_meal);
	pthread_mutex_unlock(&data->m_state);
}

static void	philo_died(t_data *data, int i)
{
	data->dead = true;
	printf("%ld	%d died\n", current_time() - \
		data->start_time, data->philos[i].id);
	unlock_monitoring(data, i);
}

static bool	check_if_philos_full(t_data *data, int i, int64_t *full_philos)
{
	data->philos[i].full = true;
	(*full_philos)++;
	if (*full_philos == data->philo_count)
	{
		unlock_monitoring(data, i);
		printf("%ld ALL PHILOS ARE FULL!\n", current_time() - data->start_time);
		return (true);
	}
	return (false);
}

void	monitor_philos(t_data *data)
{
	int		i;
	int64_t	full_philos;

	i = 0;
	full_philos = 0;
	while (full_philos != data->philo_count)
	{
		i = 0;
		while (i < data->philo_count)
		{
			if (data->philos[i].full == false)
			{
				lock_monitoring(data, i);
				if (data->philos[i].meal_count == data->times_to_eat)
					if (check_if_philos_full(data, i, &full_philos) == true)
						return ;
				if (current_time() - data->philos[i].last_meal \
						>= data->time_to_die)
					return (philo_died(data, i));
				unlock_monitoring(data, i);
			}
			i++;
		}
		usleep(50);
	}
}
