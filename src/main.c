/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 11:54:34 by imisumi           #+#    #+#             */
/*   Updated: 2023/11/15 03:47:44 by imisumi-wsl      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void	free_seats(t_seat *head)
{
	t_seat	*temp;
	t_seat	*current;

	current = head;
	while (current != NULL && current->next != head)
	{
		pthread_mutex_destroy(&current->fork);
		pthread_mutex_destroy(&current->philo.m_meal);
		temp = current;
		current = current->next;
		free(temp);
	}
	pthread_mutex_destroy(&current->fork);
	pthread_mutex_destroy(&current->philo.m_meal);
	free(current);
	head = NULL;
}

void	finalize(t_data *data, int index)
{
	int		i;
	t_seat	*current;

	i = 0;
	current = data->seats;
	while (i < index)
	{
		pthread_join(current->philo.thread, NULL);
		current = current->next;
		i++;
	}
	free_seats(data->seats);
	pthread_mutex_destroy(&data->m_state);
	pthread_mutex_destroy(&data->m_print);
	pthread_mutex_destroy(&data->m_philo);
	pthread_mutex_destroy(&data->m_monitor);
}

bool	philo_can_continue(t_seat *seat)
{
	if (is_philo_dead(seat) == true)
		return (false);
	pthread_mutex_lock(&seat->philo.m_meal);
	if ((seat->philo.meal_count > 0 || seat->philo.meal_count == -1))
	{
		pthread_mutex_unlock(&seat->philo.m_meal);
		return (true);
	}
	pthread_mutex_unlock(&seat->philo.m_meal);
	return (false);
}

//? do I print sleeping and thinking after the last meal?
int	main(int argc, char *argv[])
{
	t_data	data;

	if (valid_input(argc, argv, &data) == false)
		return (EXIT_FAILURE);
	if (setup_table(&data) == false)
		return (EXIT_FAILURE);
	if (create_threads(&data) == false)
		return (EXIT_FAILURE);
	while (mutex_get_bool(&data.m_monitor, &data.monitoring) == false)
		usleep(100);
	monitoring(&data);
	finalize(&data, data.philo_count);
	return (EXIT_SUCCESS);
}
