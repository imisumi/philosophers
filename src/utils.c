/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 11:54:34 by imisumi           #+#    #+#             */
/*   Updated: 2023/11/14 20:00:20 by imisumi-wsl      ###   ########.fr       */
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

// bool	ft_usleep(t_seat *seat, int64_t time_to)
// {
// 	int64_t	start;

// 	start = current_time();
// 	while (current_time() - start < time_to)
// 	{
// 		if (is_philo_dead(seat) == true)
// 			return (false);
// 		usleep(300);
// 	}
// 	return (true);
// }

typedef struct timeval	t_time;

long	compare_time(t_time *time1, t_time *time2)
{
	gettimeofday(time1, NULL);
	return (((time1->tv_usec - time2->tv_usec) / 1000) + \
			(time1->tv_sec - time2->tv_sec) * 1000);
}	
bool	ft_usleep(t_seat *seat, int64_t time_to)
{
	t_time	start;
	t_time	current;
	long	time_passed;

	time_passed = 0;
	gettimeofday(&start, NULL);
	while (1)
	{
		if (time_to - time_passed > 5)
			usleep(5000);
		else if (time_to - time_passed > 0)
			usleep((time_to - time_passed) * 1000);
		time_passed = compare_time(&current, &start);
		if (is_philo_dead(seat) == true)
			return (false);
		else if (time_passed >= time_to)
			return (0);
	}
}

bool	is_philo_dead(t_seat *seat)
{
	bool	dead;

	pthread_mutex_lock(&seat->data->m_state);
	dead = seat->data->dead;
	pthread_mutex_unlock(&seat->data->m_state);
	return (dead);
}

bool	print_state(t_seat *seat, enum e_action action)
{
	int64_t		time_stamp;
	const char	*table[] = {
		"has taken a fork",
		"is eating",
		"is sleeping",
		"is thinking"};

	pthread_mutex_lock(&seat->data->m_print);
	if (is_philo_dead(seat) == true)
	{
		pthread_mutex_unlock(&seat->data->m_print);
		return (false);
	}
	// if (seat->data->dead == true)
	// {
	// 	pthread_mutex_unlock(&seat->data->m_print);
	// 	return (false);
	// }
	time_stamp = current_time() - seat->data->start_time;
	printf("%ld	%d %s\n", time_stamp, seat->philo.id, table[action]);
	pthread_mutex_unlock(&seat->data->m_print);
	return (true);
}
