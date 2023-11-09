/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 11:54:34 by imisumi           #+#    #+#             */
/*   Updated: 2023/11/09 19:02:08 by imisumi-wsl      ###   ########.fr       */
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
		pthread_mutex_destroy(&current->philo.m_meal_time);
		temp = current;
		current = current->next;
		free(temp);
	}
	pthread_mutex_destroy(&current->fork);
	pthread_mutex_destroy(&current->philo.m_meal_time);
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
}

bool	philo_can_continue(t_seat *seat)
{
	if (philo_is_alive(seat) == false)
		return (false);
	pthread_mutex_lock(&seat->philo.m_meal_time);
	if ((seat->philo.meal_count > 0 || seat->philo.meal_count == -1))
	{
		pthread_mutex_unlock(&seat->philo.m_meal_time);
		return (true);
	}
	pthread_mutex_unlock(&seat->philo.m_meal_time);
	return (false);
}

static void	monitoring(t_data *data)
{
	int64_t	last;
	t_seat	*current;

	current = data->seats;
	while (current)
	{
		pthread_mutex_lock(&current->philo.m_meal_time);
		last = current->philo.last_meal;
		if (current->philo.meal_count == 0)
			return ((void)pthread_mutex_unlock(&current->philo.m_meal_time));
		pthread_mutex_unlock(&current->philo.m_meal_time);
		if (current_time() - last >= data->time_to_die)
		{
			pthread_mutex_lock(&data->m_state);
			data->dead = true;
			printf("%ld	%s%d died%s\n", current_time() - \
				current->data->start_time, RED, current->philo.id, RESET);
			pthread_mutex_unlock(&data->m_state);
			break ;
		}
		current = current->next;
		usleep(1);
	}
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
	monitoring(&data);
	finalize(&data, data.philo_count);
	return (EXIT_SUCCESS);
}
