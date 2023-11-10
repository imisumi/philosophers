/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 11:54:34 by imisumi           #+#    #+#             */
/*   Updated: 2023/11/10 20:25:34 by imisumi-wsl      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

// void	mutex_set(pthread_mutex_t *mutex, int64_t i)
// {
// 	pthread_mutex_lock(mutex);
// 	pthread_mutex_unlock(mutex);
// }

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

bool	ft_usleep(t_seat *seat, int64_t time_to)
{
	int64_t	start;

	start = current_time();
	while (current_time() - start < time_to)
	{
		if (is_philo_dead(seat) == true)
			return (false);
		usleep(300);
	}
	return (true);
}


bool	print_state(t_seat *seat, enum e_action action)
{
	const char	*table[] = {"has taken a fork", "is eating", "is sleeping", "is thinking"};

	if (is_philo_dead(seat) == true)
		return (false);
	pthread_mutex_lock(&seat->data->m_print);
	printf("%ld	%d %s\n", current_time() - seat->data->start_time, seat->philo.id, table[action]);
	pthread_mutex_unlock(&seat->data->m_print);
	return (true);
}

bool	is_philo_dead(t_seat *seat)
{
	bool	dead;

	pthread_mutex_lock(&seat->data->m_state);
	dead = seat->data->dead;
	pthread_mutex_unlock(&seat->data->m_state);
	return (dead);
}
