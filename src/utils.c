/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 11:54:34 by imisumi           #+#    #+#             */
/*   Updated: 2023/10/29 02:51:07 by imisumi-wsl      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

size_t	ft_strlen(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

// returns the current time in microseconds
int64_t	current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec) * 1000 + (tv.tv_usec) / 1000);
}

void	ft_usleep(t_seat *seat, int64_t time_to)
{
	int64_t	start;

	start = current_time();
	while (current_time() - start < time_to)
	{
		if (philo_is_alive(seat) == false)
			return ;
		usleep(100);
	}
}

void	print_state(t_seat *seat, enum e_action action)
{
	pthread_mutex_lock(&seat->data->m_state);
	if (seat->data->dead == true)
		return ((void)pthread_mutex_unlock(&seat->data->m_state));
	if (action == FORK)
	{
		printf("%ldms	%s%d has taken a fork%s\n", current_time() - \
			seat->data->start_time, GREEN, seat->philo.id, RESET);
	}
	else if (action == EATING)
	{
		printf("%ldms	%s%d is eating%s\n", current_time() - \
			seat->data->start_time, YELLOW, seat->philo.id, RESET);
	}
	else if (action == SLEEPING)
	{
		printf("%ldms	%s%d is sleeping%s\n", current_time() - \
			seat->data->start_time, BLUE, seat->philo.id, RESET);
	}
	else if (action == THINKING)
	{
		printf("%ldms	%s%d is thinking%s\n", current_time() - \
			seat->data->start_time, MAGENTA, seat->philo.id, RESET);
	}
	pthread_mutex_unlock(&seat->data->m_state);
}

bool	philo_is_alive(t_seat *seat)
{
	pthread_mutex_lock(&seat->data->m_state);
	if (seat->data->dead == true)
	{
		pthread_mutex_unlock(&seat->data->m_state);
		return (false);
	}
	pthread_mutex_unlock(&seat->data->m_state);
	return (true);
}
