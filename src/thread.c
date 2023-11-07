/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi <imisumi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 11:54:34 by imisumi           #+#    #+#             */
/*   Updated: 2023/11/07 17:22:47 by imisumi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void	update_times_ate(t_seat *seat)
{
	if (seat->data->times_to_eat != -1)
	{
		pthread_mutex_lock(&seat->philo.m_meal_count);
		seat->philo.meal_count--;
		pthread_mutex_unlock(&seat->philo.m_meal_count);
	}
}

bool	pickup_fork(t_seat *seat, pthread_mutex_t *first, pthread_mutex_t *second)
{
	pthread_mutex_lock(first);
	print_state(seat, FORK);
	if (philo_is_alive(seat) == false)
	{
		pthread_mutex_unlock(first);
		return (false);
	}
	if (seat->data->philo_count == 1)
	{
		pthread_mutex_unlock(first);
		return (false);
	}
	pthread_mutex_lock(second);
	print_state(seat, FORK);
	if (philo_is_alive(seat) == false)
	{
		pthread_mutex_unlock(second);
		pthread_mutex_unlock(first);
		return (false);
	}
	return (true);
}

void	putdown_fork(pthread_mutex_t *fork)
{
	pthread_mutex_unlock(fork);
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
	pthread_mutex_unlock(&seat->philo.m_meal_time);
	print_state(seat, EATING);
	ft_usleep(seat, seat->data->time_to_eat);
	putdown_fork(&seat->next->fork);
	putdown_fork(&seat->fork);
	update_times_ate(seat);
	return (true);
}

static bool	philo_can_continue(t_seat *seat)
{
	if (philo_is_alive(seat) == false)
		return (false);
	pthread_mutex_lock(&seat->philo.m_meal_count);
	if ((seat->philo.meal_count > 0 || seat->philo.meal_count == -1))
	{
		pthread_mutex_unlock(&seat->philo.m_meal_count);
		return (true);
	}
	pthread_mutex_unlock(&seat->philo.m_meal_count);
	return (false);
}

static void	*routine(void *arg)
{
	t_seat	*seat;

	seat = arg;
	while (philo_can_continue(seat) == true)
	{
		if (meal_time(seat) == false)
			return (NULL);
		pthread_mutex_lock(&seat->philo.m_meal_count);
		if (seat->philo.meal_count == 0)
			return (pthread_mutex_unlock(&seat->philo.m_meal_count), NULL);
		pthread_mutex_unlock(&seat->philo.m_meal_count);
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
	data->start_time = current_time();
	while (i < data->philo_count)
	{
		pthread_mutex_lock(&current->philo.m_meal_time);
		current->philo.last_meal = data->start_time;
		pthread_mutex_unlock(&current->philo.m_meal_time);
		if (pthread_create(&current->philo.thread, NULL, &routine, current) != 0)
		{
			pthread_mutex_lock(&data->m_state);
			data->dead = true;
			pthread_mutex_unlock(&data->m_state);
			finalize(data, i - 1);
			return (false);
		}
		current = current->next;
		i++;
	}
	return (true);
}