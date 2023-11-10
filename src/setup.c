/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 11:54:34 by imisumi           #+#    #+#             */
/*   Updated: 2023/11/10 21:43:36 by imisumi-wsl      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static bool	create_table(t_data *data)
{
	int		i;
	t_seat	*new;

	data->seats = NULL;
	data->dead = false;
	i = 0;
	while (i < data->philo_count)
	{
		new = create_seat_node(data);
		if (new == NULL)
		{
			free_seat_fail(data->seats);
			return (false);
		}
		insert_seat(&data->seats, new);
		i++;
	}
	return (true);
}

static bool	destroy_mutexes(t_data *data, int i)
{
	t_seat	*current;

	current = data->seats;
	while (i > 0)
	{
		pthread_mutex_destroy(&current->fork);
		pthread_mutex_destroy(&current->philo.m_meal);
		current = current->next;
		i--;
	}
	return (false);
}

static bool	init_mutexes(t_data *data)
{
	int		i;
	t_seat	*current;

	i = 0;
	current = data->seats;
	while (i < data->philo_count)
	{
		current->philo.meal_count = data->times_to_eat;
		if (pthread_mutex_init(&current->fork, NULL) != 0)
			return (destroy_mutexes(data, i));
		if (pthread_mutex_init(&current->philo.m_meal, NULL) != 0)
			return (pthread_mutex_destroy(&current->fork), destroy_mutexes(data, i));


		pthread_mutex_init(&current->f.m_fork, NULL);
		pthread_mutex_init(&current->f.m_lock, NULL);

						
		current = current->next;
		i++;
	}
	if (pthread_mutex_init(&data->m_state, NULL) != 0)
		return (destroy_mutexes(data, i));
	//! TODO protect
	if (pthread_mutex_init(&data->m_print, NULL) != 0)
		return (destroy_mutexes(data, i));
	return (true);
}

bool	setup_table(t_data *data)
{
	if (create_table(data) == false)
		return (false);
	if (init_mutexes(data) == false)
		return (false);
	return (true);
}
