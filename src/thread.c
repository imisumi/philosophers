/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 11:54:34 by imisumi           #+#    #+#             */
/*   Updated: 2023/11/09 19:09:48 by imisumi-wsl      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

bool	pickup_fork(t_seat *seat, pthread_mutex_t *m1, pthread_mutex_t *m2)
{
	pthread_mutex_lock(m1);
	if (print_state(seat, FORK) == false)
	{
		pthread_mutex_unlock(m1);
		return (false);
	}
	if (seat->data->philo_count == 1)
	{
		pthread_mutex_unlock(m1);
		return (false);
	}
	pthread_mutex_lock(m2);
	if (print_state(seat, FORK) == false)
	{
		pthread_mutex_unlock(m2);
		pthread_mutex_unlock(m1);
		return (false);
	}
	return (true);
}

static bool	meal_time(t_seat *seat)
{
	if (seat->philo.id % 2)
	{
		ft_usleep(seat, 1);
		if (pickup_fork(seat, &seat->next->fork, &seat->fork) == false)
			return (false);
	}
	else
	{
		if (pickup_fork(seat, &seat->fork, &seat->next->fork) == false)
			return (false);
	}
	pthread_mutex_lock(&seat->philo.m_meal_time);
	seat->philo.last_meal = current_time();
	if (seat->philo.meal_count != -1)
		seat->philo.meal_count--;
	pthread_mutex_unlock(&seat->philo.m_meal_time);
	print_state(seat, EATING);
	ft_usleep(seat, seat->data->time_to_eat);
	pthread_mutex_unlock(&seat->next->fork);
	pthread_mutex_unlock(&seat->fork);
	return (true);
}

static void	*routine(void *arg)
{
	t_seat	*seat;

	seat = arg;
	pthread_mutex_lock(&seat->data->m_state);
	pthread_mutex_unlock(&seat->data->m_state);
	if (seat->philo.id % 2)
		ft_usleep(seat, seat->data->time_to_eat / 2);
	while (philo_can_continue(seat) == true)
	{
		if (meal_time(seat) == false)
			return (NULL);
		pthread_mutex_lock(&seat->philo.m_meal_time);
		if (seat->philo.meal_count == 0)
			return (pthread_mutex_unlock(&seat->philo.m_meal_time), NULL);
		pthread_mutex_unlock(&seat->philo.m_meal_time);
		print_state(seat, SLEEPING);
		ft_usleep(seat, seat->data->time_to_sleep);
		print_state(seat, THINKING);
	}
	return (NULL);
}

bool	create_threads(t_data *data)
{
	int		i;
	t_seat	*current;

	i = 0;
	current = data->seats;
	pthread_mutex_lock(&data->m_state);
	data->start_time = current_time();
	while (i < data->philo_count)
	{
		pthread_mutex_lock(&current->philo.m_meal_time);
		current->philo.last_meal = data->start_time;
		pthread_mutex_unlock(&current->philo.m_meal_time);
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
	return (pthread_mutex_unlock(&data->m_state), true);
}
