/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 11:54:34 by imisumi           #+#    #+#             */
/*   Updated: 2023/10/29 02:57:14 by imisumi-wsl      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void	free_seat_fail(t_seat *head)
{
	t_seat	*temp;
	t_seat	*current;

	current = head;
	while (current != NULL && current->next != head)
	{
		temp = current;
		current = current->next;
		free(temp);
	}
	free(current);
	head = NULL;
}

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
		pthread_mutex_destroy(&current->philo.m_meal_time);
		pthread_mutex_destroy(&current->philo.m_meal_count);
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
		if (pthread_mutex_init(&current->philo.m_meal_time, NULL) != 0)
		{
			pthread_mutex_destroy(&current->fork);
			return (destroy_mutexes(data, i));
		}
		if (pthread_mutex_init(&current->philo.m_meal_count, NULL) != 0)
		{
			pthread_mutex_destroy(&current->fork);
			pthread_mutex_destroy(&current->philo.m_meal_time);
			return (destroy_mutexes(data, i));
		}
		current = current->next;
		i++;
	}
	return (pthread_mutex_init(&data->m_state, NULL), true);
}

bool	setup_table(t_data *data)
{
	if (create_table(data) == false)
		return (false);
	if (init_mutexes(data) == false)
		return (false);
	return (true);
}
