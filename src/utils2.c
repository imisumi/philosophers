/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi <imisumi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 11:54:34 by imisumi           #+#    #+#             */
/*   Updated: 2023/11/20 15:18:10 by imisumi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int64_t	mutex_get_int64(t_mutex *mutex, int64_t *value)
{
	int64_t	tmp;

	pthread_mutex_lock(mutex);
	tmp = *value;
	pthread_mutex_unlock(mutex);
	return (tmp);
}

bool	mutex_get_bool(t_mutex *mutex, bool *value)
{
	bool	tmp;

	pthread_mutex_lock(mutex);
	tmp = *value;
	pthread_mutex_unlock(mutex);
	return (tmp);
}
