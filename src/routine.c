/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi <imisumi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 11:54:34 by imisumi           #+#    #+#             */
/*   Updated: 2023/11/20 15:16:58 by imisumi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

bool	print_state(t_philo *philo, t_action action)
{
	int64_t		time_stamp;
	const char	*table[] = {
		"has taken a fork",
		"is eating",
		"is sleeping",
		"is thinking"};

	pthread_mutex_lock(&philo->data->m_state);
	if (philo->data->dead == true)
	{
		pthread_mutex_unlock(&philo->data->m_state);
		return (false);
	}
	time_stamp = current_time() - philo->data->start_time;
	printf("%ld	%d %s\n", time_stamp, philo->id, table[action]);
	pthread_mutex_unlock(&philo->data->m_state);
	return (true);
}

static bool	desync_philos(t_philo	*philo)
{
	if (philo->data->philo_count % 2 == 0)
	{
		if (philo->id % 2 == 0)
		{
			if (print_state(philo, THINKING) == false)
				return (false);
			ft_usleep(philo, philo->data->time_to_eat / 2);
		}
	}
	else
	{
		if (philo->id % 2)
		{
			if (print_state(philo, THINKING) == false)
				return (false);
			thinking(philo);
		}
	}
	return (true);
}

static void	*handle_single_philo(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(philo->m_fork_1);
	if (print_state(philo, FORK) == false)
		return (NULL);
	while (mutex_get_bool(&data->m_state, &data->dead) == false)
		usleep(10);
	pthread_mutex_unlock(philo->m_fork_1);
	return (NULL);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->data->m_state);
	pthread_mutex_unlock(&philo->data->m_state);
	philo->last_meal = philo->data->start_time;
	pthread_mutex_lock(&philo->data->m_monitor);
	philo->data->monitoring++;
	pthread_mutex_unlock(&philo->data->m_monitor);
	if (philo->data->philo_count == 1)
		return (handle_single_philo(philo));
	if (desync_philos(philo) == false)
		return (NULL);
	while (true)
	{
		if (philo_eat(philo) == false)
			return (NULL);
		if (philo_sleep(philo) == false)
			return (NULL);
		if (philo_think(philo) == false)
			return (NULL);
	}
	return (NULL);
}
