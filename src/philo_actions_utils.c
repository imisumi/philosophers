/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 11:54:34 by imisumi           #+#    #+#             */
/*   Updated: 2023/11/16 02:18:39 by imisumi-wsl      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

bool	pickup_forks(t_philo	*philo)
{
	pthread_mutex_lock(philo->m_fork_1);
	if (print_state(philo, FORK) == false)
	{
		pthread_mutex_unlock(philo->m_fork_1);
		return (false);
	}
	pthread_mutex_lock(philo->m_fork_2);
	if (print_state(philo, FORK) == false)
	{
		pthread_mutex_unlock(philo->m_fork_1);
		pthread_mutex_unlock(philo->m_fork_2);
		return (false);
	}
	return (true);
}

void	drop_forks(t_philo	*philo)
{
	pthread_mutex_unlock(philo->m_fork_1);
	pthread_mutex_unlock(philo->m_fork_2);
}

