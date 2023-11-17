/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi <imisumi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 11:54:34 by imisumi           #+#    #+#             */
/*   Updated: 2023/11/17 16:42:57 by imisumi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	monitor_philos(t_data *data)
{
	int		i;
	int64_t	meal_count;
	int64_t	last_meal;
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
				pthread_mutex_lock(&data->m_state);
				pthread_mutex_lock(data->philos[i].m_meal);
				last_meal = current_time() - data->philos[i].last_meal;
				meal_count = data->philos[i].meal_count;
				if (meal_count == data->times_to_eat)
				{
					data->philos[i].full = true;
					full_philos++;
					// printf("%ld ---------------------------------- %d	%ld	%ld\n", current_time() - data->start_time, i, meal_count, data->times_to_eat);
					if (full_philos == data->philo_count)
					{
						pthread_mutex_unlock(data->philos[i].m_meal);
						pthread_mutex_unlock(&data->m_state);
						// printf("All philosophers are full\n");
						return ;
						// break ;
					}
				}
				else if (last_meal >= data->time_to_die)
				{
					data->dead = true;
					printf("%ld	%d died\n", current_time() - \
						data->start_time, data->philos[i].id);
					pthread_mutex_unlock(data->philos[i].m_meal);
					pthread_mutex_unlock(&data->m_state);
					return ;
				}
				pthread_mutex_unlock(data->philos[i].m_meal);
				pthread_mutex_unlock(&data->m_state);
			}
			i++;
		}
		usleep(50);
	}
}
