/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 11:54:34 by imisumi           #+#    #+#             */
/*   Updated: 2023/11/15 03:43:48 by imisumi-wsl      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static bool	pickup_fork(t_seat *seat, pthread_mutex_t *m1, pthread_mutex_t *m2)
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

static void	drop_forks(t_seat *seat)
{
	if (seat->philo.id % 2)
	{
		pthread_mutex_unlock(&seat->fork);
		pthread_mutex_unlock(&seat->next->fork);
	}
	else
	{
		pthread_mutex_unlock(&seat->next->fork);
		pthread_mutex_unlock(&seat->fork);
	}
}

bool	meal_time(t_seat *seat)
{
	if (seat->philo.id % 2)
	{
		if (pickup_fork(seat, &seat->next->fork, &seat->fork) == false)
			return (false);
	}
	else
	{
		if (pickup_fork(seat, &seat->fork, &seat->next->fork) == false)
			return (false);
	}
	pthread_mutex_lock(&seat->philo.m_meal);
	seat->philo.last_meal = current_time();
	pthread_mutex_unlock(&seat->philo.m_meal);
	print_state(seat, EATING);
	ft_usleep(seat, seat->data->time_to_eat);
	drop_forks(seat);
	if (seat->data->times_to_eat != -1)
	{
		pthread_mutex_lock(&seat->philo.m_meal);
		seat->philo.meal_count--;
		if (seat->philo.meal_count == 0)
			return (pthread_mutex_unlock(&seat->philo.m_meal), false);
		pthread_mutex_unlock(&seat->philo.m_meal);
	}
	return (true);
}
