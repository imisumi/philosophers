/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 11:54:34 by imisumi           #+#    #+#             */
/*   Updated: 2023/11/15 03:45:52 by imisumi-wsl      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static bool	philo_think(t_seat	*seat)
{
	int64_t	eat_time;
	int64_t	sleep_time;
	int64_t	think_time;

	if (print_state(seat, THINKING) == false)
		return (false);
	if (seat->data->philo_count % 2 == 0)
		return (true);
	eat_time = seat->data->time_to_eat;
	sleep_time = seat->data->time_to_sleep;
	think_time = (eat_time * 2) - (eat_time / 2) - sleep_time;
	if (eat_time >= sleep_time)
		ft_usleep(seat, think_time);
	return (true);
}

static void	separate_philos(t_seat *seat)
{
	int64_t	think_time;

	print_state(seat, THINKING);
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

static bool	sleep_time(t_seat *seat)
{
	if (print_state(seat, SLEEPING) == false)
		return (false);
	ft_usleep(seat, seat->data->time_to_sleep);
	return (true);
}

static void	sync_philos(t_seat *seat)
{
	pthread_mutex_lock(&seat->data->m_state);
	pthread_mutex_unlock(&seat->data->m_state);
	seat->philo.last_meal = seat->data->start_time;
	if (mutex_inc_int64(&seat->data->m_philo, &seat->data->sitting_philos) \
							== seat->data->philo_count)
		mutex_set_bool(&seat->data->m_monitor, &seat->data->monitoring, true);
}

void	*routine(void *arg)
{
	t_seat	*seat;

	seat = arg;
	sync_philos(seat);
	separate_philos(seat);
	while (philo_can_continue(seat) == true)
	{
		if (meal_time(seat) == false)
			return (NULL);
		if (sleep_time(seat) == false)
			return (NULL);
		philo_think(seat);
	}
	return (NULL);
}
