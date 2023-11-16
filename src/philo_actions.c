/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 11:54:34 by imisumi           #+#    #+#             */
/*   Updated: 2023/11/16 03:07:05 by imisumi-wsl      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

bool	philo_eat(t_philo	*philo)
{
	bool	full;

	full = false;
	if (pickup_forks(philo) == false)
		return (false);
	if (print_state(philo, EATING) == false)
		return (false);
	pthread_mutex_lock(philo->m_meal);
	philo->last_meal = current_time();
	pthread_mutex_unlock(philo->m_meal);
	ft_usleep(philo, philo->data->time_to_eat);

	drop_forks(philo);
	
	pthread_mutex_lock(philo->m_meal);
	// philo->last_meal = current_time();
	philo->meal_count++;
	if (philo->meal_count == philo->data->times_to_eat)
		full = true;
	pthread_mutex_unlock(philo->m_meal);

	if (full == true)
		return (false);
	return (true);
}

bool	philo_sleep(t_philo	*philo)
{
	if (print_state(philo, SLEEPING) == false)
		return (false);
	ft_usleep(philo, philo->data->time_to_sleep);
	return (true);
}

void	thinking(t_philo *philo)
{
	int64_t	t_eat;
	int64_t	t_sleep;
	int64_t	t_think;

	if (philo->data->philo_count % 2 == 0)
		return ;
	t_eat = philo->data->time_to_eat;
	t_sleep = philo->data->time_to_sleep;
	t_think = (t_eat * 2) - t_sleep;
	if (t_think < 0)
		t_think = 0;
	ft_usleep(philo, t_think * 0.42);
}

bool	philo_think(t_philo	*philo)
{
	if (print_state(philo, THINKING) == false)
		return (false);
	thinking(philo);
	return (true);
}
