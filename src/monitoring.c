/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 11:54:34 by imisumi           #+#    #+#             */
/*   Updated: 2023/11/15 03:44:56 by imisumi-wsl      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

bool	philo_died(t_data *data, t_seat *current)
{
	int64_t	last;

	last = mutex_get_int64(&current->philo.m_meal, &current->philo.last_meal);
	if (current_time() - last >= data->time_to_die)
	{
		mutex_set_bool(&data->m_state, &data->dead, true);
		pthread_mutex_lock(&data->m_print);
		printf("%ld	%s%d died%s\n", current_time() - \
			current->data->start_time, RED, current->philo.id, RESET);
		pthread_mutex_unlock(&data->m_print);
		return (true);
	}
	return (false);
}

void	monitoring(t_data *data)
{
	int		i;
	t_seat	*current;

	i = 0;
	current = data->seats;
	while (i < data->philo_count)
	{
		if (current->philo.full == false)
		{
			if (philo_died(data, current) == true)
				return ;
			if (data->times_to_eat != -1)
			{
				if (mutex_get_int64(&current->philo.m_meal, \
						&current->philo.meal_count) == 0)
				{
					current->philo.full = true;
					i++;
				}
			}
			usleep(1);
		}
		current = current->next;
	}
}
