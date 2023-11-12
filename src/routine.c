/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 11:54:34 by imisumi           #+#    #+#             */
/*   Updated: 2023/11/12 20:06:18 by imisumi-wsl      ###   ########.fr       */
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

static bool	meal_time(t_seat *seat)
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
	if (seat->philo.meal_count != -1)
		seat->philo.meal_count--;
	pthread_mutex_unlock(&seat->philo.m_meal);
	print_state(seat, EATING);
	ft_usleep(seat, seat->data->time_to_eat);
	pthread_mutex_unlock(&seat->next->fork);
	pthread_mutex_unlock(&seat->fork);
	return (true);
}

static bool	philo_think(t_seat	*seat)
{
	int64_t	think_time;

	if (print_state(seat, THINKING) == false)
		return (false);
	if (seat->data->philo_count % 2 == 0)
		return (true);
	think_time = (seat->data->time_to_eat * 2) - seat->data->time_to_sleep;
	if (think_time > seat->data->time_to_eat)
		ft_usleep(seat, seat->data->time_to_eat / 2);
	else if (think_time >= 0)
		ft_usleep(seat, think_time);
	return (true);
}

static void	separate_philos(t_seat *seat)
{
	int64_t	think_time;

	think_time = (seat->data->time_to_eat * 2) - seat->data->time_to_sleep;
	if (seat->data->philo_count % 2 == 0)
	{
		if (seat->philo.id % 2 == 0)
			ft_usleep(seat, seat->data->time_to_eat / 2);
	}
	else
	{
		if (seat->philo.id % 2 == 0)
		{
			if (think_time >= seat->data->time_to_eat)
				ft_usleep(seat, seat->data->time_to_eat / 2);
			else if (think_time >= 0)
				ft_usleep(seat, think_time);
		}
	}
}

void	*routine(void *arg)
{
	t_seat	*seat;

	seat = arg;
	pthread_mutex_lock(&seat->data->m_state);
	pthread_mutex_unlock(&seat->data->m_state);
	separate_philos(seat);
	while (philo_can_continue(seat) == true)
	{
		if (meal_time(seat) == false)
			return (NULL);
		pthread_mutex_lock(&seat->philo.m_meal);
		if (seat->philo.meal_count == 0)
			return (pthread_mutex_unlock(&seat->philo.m_meal), NULL);
		pthread_mutex_unlock(&seat->philo.m_meal);
		print_state(seat, SLEEPING);
		ft_usleep(seat, seat->data->time_to_sleep);
		philo_think(seat);
	}
	return (NULL);
}
