/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 11:54:34 by imisumi           #+#    #+#             */
/*   Updated: 2023/11/15 02:57:35 by imisumi-wsl      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int64_t	mutex_get_int64(t_mutex *mutex, int64_t *value)
{
	int64_t	ret;

	pthread_mutex_lock(mutex);
	ret = *value;
	pthread_mutex_unlock(mutex);
	return (ret);
}

int64_t	mutex_inc_int64(t_mutex *mutex, int64_t *value)
{
	int64_t	ret;

	pthread_mutex_lock(mutex);
	(*value)++;
	ret = *value;
	pthread_mutex_unlock(mutex);
	return (ret);
}

void	mutex_dec_int64(t_mutex *mutex, int64_t *value)
{
	pthread_mutex_lock(mutex);
	(*value)--;
	pthread_mutex_unlock(mutex);
}

bool	mutex_get_bool(t_mutex *mutex, bool *value)
{
	bool	ret;

	pthread_mutex_lock(mutex);
	ret = *value;
	pthread_mutex_unlock(mutex);
	return (ret);
}

void	mutex_set_bool(t_mutex *mutex, bool *value, bool new_value)
{
	pthread_mutex_lock(mutex);
	*value = new_value;
	pthread_mutex_unlock(mutex);
}
