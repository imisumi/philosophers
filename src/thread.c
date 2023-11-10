/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 11:54:34 by imisumi           #+#    #+#             */
/*   Updated: 2023/11/10 22:14:11 by imisumi-wsl      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

bool	pickup_fork(t_seat *seat, pthread_mutex_t *m1, pthread_mutex_t *m2)
{
	pthread_mutex_lock(m1);
	if (print_state(seat, FORK) == false)
	{
		pthread_mutex_unlock(m1);
		return (false);
	}
	if (seat->data->philo_count == 1)
	{
		pthread_mutex_unlock(m1);
		return (false);
	}
	pthread_mutex_lock(m2);
	if (print_state(seat, FORK) == false)
	{
		pthread_mutex_unlock(m2);
		pthread_mutex_unlock(m1);
		return (false);
	}
	return (true);
}

static bool	meal_time(t_seat *seat)
{
	if (seat->philo.id % 2)
	{
		// ft_usleep(seat, 1);
		if (pickup_fork(seat, &seat->next->fork, &seat->fork) == false)
			return (false);
	}
	else
	{
		if (pickup_fork(seat, &seat->fork, &seat->next->fork) == false)
			return (false);
	}
	// if (pickup_fork(seat, &seat->next->fork, &seat->fork) == false)
	// 		return (false);
	pthread_mutex_lock(&seat->philo.m_meal);
	seat->philo.last_meal = current_time();
	if (seat->philo.meal_count != -1)
		seat->philo.meal_count--;
	pthread_mutex_unlock(&seat->philo.m_meal);
	print_state(seat, EATING);
	ft_usleep(seat, seat->data->time_to_eat);
	pthread_mutex_unlock(&seat->next->fork);
	pthread_mutex_unlock(&seat->fork);
	return (true);
}

static void	*routine(void *arg)
{
	t_seat	*seat;

	seat = arg;
	if (seat->philo.id % 2 == 0)
		ft_usleep(seat, seat->data->time_to_eat);
	while (philo_can_continue(seat) == true)
	{
		if (print_state(seat, THINKING) == false)
			return (NULL);
		// if (seat->philo.id % 2 == 0)
		// {
		// 	pthread_mutex_lock(&seat->f.m_lock);
		// 	pthread_mutex_lock(&seat->next->f.m_lock);
		// }
		// else
		// {
		// 	pthread_mutex_lock(&seat->next->f.m_lock);
		// 	pthread_mutex_lock(&seat->f.m_lock);
		// }
		// pthread_mutex_lock(&seat->f.m_lock);
		// pthread_mutex_lock(&seat->next->f.m_lock);
		// while (seat->f.locked == false || seat->next->f.locked == false)

		
		// while (true)
		// {
		// 	pthread_mutex_lock(&seat->f.m_lock);
		// 	pthread_mutex_lock(&seat->next->f.m_lock);
		// 	if (seat->f.locked == true && seat->next->f.locked == true)
		// 	{
		// 		pthread_mutex_unlock(&seat->next->f.m_lock);
		// 		pthread_mutex_unlock(&seat->f.m_lock);
		// 		break ;
		// 	}
			
		// 	// break ;
		// 	pthread_mutex_unlock(&seat->f.m_lock);
		// 	pthread_mutex_unlock(&seat->next->f.m_lock);
		// 	if (is_philo_dead(seat) == true)
		// 	{
		// 		return (NULL);
		// 	}
		// 	usleep(100);

		// 	pthread_mutex_lock(&seat->f.m_lock);
		// 	if (seat->f.locked == false)
		// 	{
		// 		pthread_mutex_lock(&seat->fork);
		// 		// pthread_mutex_lock(&seat->f.m_fork);
		// 		seat->f.locked = true;
		// 		print_state(seat, FORK);
		// 	}
		// 	pthread_mutex_unlock(&seat->f.m_lock);


		// 	pthread_mutex_lock(&seat->next->f.m_lock);
		// 	if (seat->next->f.locked == false)
		// 	{
		// 		pthread_mutex_lock(&seat->next->fork);
		// 		// pthread_mutex_lock(&seat->next->f.m_fork);
		// 		seat->next->f.locked = true;
		// 		print_state(seat, FORK);
		// 	}
		// 	pthread_mutex_unlock(&seat->next->f.m_lock);
		// }
		//! pthread_mutex_unlock(&seat->next->f.m_lock);
		//! pthread_mutex_unlock(&seat->f.m_lock);

		// print_state(seat, EATING);
		
		// pthread_mutex_lock(&seat->philo.m_meal);
		// seat->philo.last_meal = current_time();
		// if (seat->philo.meal_count != -1)
		// 	seat->philo.meal_count--;
		// pthread_mutex_unlock(&seat->philo.m_meal);
		
		// ft_usleep(seat, seat->data->time_to_eat);
		
		// pthread_mutex_lock(&seat->f.m_lock);
		// seat->f.locked = false;
		// pthread_mutex_unlock(&seat->f.m_lock);
		// // pthread_mutex_unlock(&seat->f.m_fork);
		// pthread_mutex_unlock(&seat->fork);

		// pthread_mutex_lock(&seat->next->f.m_lock);
		// seat->next->f.locked = false;
		// pthread_mutex_unlock(&seat->next->f.m_lock);
		// // pthread_mutex_unlock(&seat->next->f.m_fork);
		// pthread_mutex_unlock(&seat->next->fork);

		if (meal_time(seat) == false)
			return (NULL);
		pthread_mutex_lock(&seat->philo.m_meal);
		if (seat->philo.meal_count == 0)
			return (pthread_mutex_unlock(&seat->philo.m_meal), NULL);
		pthread_mutex_unlock(&seat->philo.m_meal);
		print_state(seat, SLEEPING);
		ft_usleep(seat, seat->data->time_to_sleep);
	}
	return (NULL);
}

bool	create_threads(t_data *data)
{
	int		i;
	t_seat	*current;

	i = 0;
	current = data->seats;
	// pthread_mutex_lock(&data->m_state);
	data->start_time = current_time();
	while (i < data->philo_count)
	{
		current->philo.last_meal = data->start_time;
		// current->philo.left_fork = false;
		// current->philo.right_fork = false;
		pthread_mutex_lock(&current->f.m_lock);
		current->f.locked = false;
		pthread_mutex_unlock(&current->f.m_lock);
		if (pthread_create(&current->philo.thread, NULL, &routine, current) != 0)
		{
			pthread_mutex_lock(&data->m_state);
			data->dead = true;
			pthread_mutex_unlock(&data->m_state);
			finalize(data, i);
			return (false);
		}
		current = current->next;
		i++;
	}
	return (true);
	// return (pthread_mutex_unlock(&data->m_state), true);
}
