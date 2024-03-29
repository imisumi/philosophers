/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi <imisumi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 11:54:34 by imisumi           #+#    #+#             */
/*   Updated: 2023/11/20 15:18:03 by imisumi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int64_t	current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec) * 1000 + (tv.tv_usec) / 1000);
}

void	error_msg(char *str)
{
	printf("%s", str);
}

size_t	ft_strlen(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

bool	ft_usleep(t_philo *philo, int64_t time_to)
{
	int64_t	start;
	bool	dead;

	start = current_time();
	while (current_time() - start < time_to)
	{
		usleep(500);
		pthread_mutex_lock(&philo->data->m_state);
		dead = philo->data->dead;
		pthread_mutex_unlock(&philo->data->m_state);
		if (dead == true)
			return (false);
	}
	return (true);
}
