/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 11:54:34 by imisumi           #+#    #+#             */
/*   Updated: 2023/11/05 21:21:26 by imisumi-wsl      ###   ########.fr       */
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
		pthread_mutex_destroy(&current->philo.m_meal_count);
		temp = current;
		current = current->next;
		free(temp);
	}
	pthread_mutex_destroy(&current->fork);
	pthread_mutex_destroy(&current->philo.m_meal_time);
	pthread_mutex_destroy(&current->philo.m_meal_count);
	free(current);
	head = NULL;
}

static void	finalize(t_data *data)
{
	int		i;
	t_seat	*current;

	i = 0;
	current = data->seats;
	while (i < data->philo_count)
	{
		pthread_join(current->philo.thread, NULL);
		current = current->next;
		i++;
	}
	free_seats(data->seats);
	pthread_mutex_destroy(&data->m_state);
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
		pthread_mutex_unlock(&current->philo.m_meal_time);
		pthread_mutex_lock(&current->philo.m_meal_count);
		if (current->philo.meal_count == 0)
			return ((void)pthread_mutex_unlock(&current->philo.m_meal_count));
		pthread_mutex_unlock(&current->philo.m_meal_count);
		if (current_time() - last >= data->time_to_die)
		{
			pthread_mutex_lock(&data->m_state);
			data->dead = true;
			printf("%ldms	%s%d died%s\n", current_time() - \
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
	create_threads(&data);
	monitoring(&data);
	finalize(&data);
	return (EXIT_SUCCESS);
}
