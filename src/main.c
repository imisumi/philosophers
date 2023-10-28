/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 11:54:34 by imisumi           #+#    #+#             */
/*   Updated: 2023/10/28 17:27:16 by imisumi-wsl      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

bool	setup(t_data *data, char **argv)
{
	
	return true;
}

bool	str_num_only(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (false);
		i++;
	}
	return (true);
}

// 1. number_of_philosophers
// 2. time_to_die
// 3. time_to_eat
// 4. time_to_sleep
// 5. [number_of_times_each_philosopher_must_eat]

size_t	ft_strlen(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	error_msg(char *str)
{
	write(STDERR_FILENO, str, ft_strlen(str));
}


//! positive number only
int	ft_atoi(char *str)
{
	int	i;
	int	num;

	if (str == NULL)
		return (-1);
	if (str_num_only(str) == false)
		return (error_msg("Numeric argument only\n"), -1);
	if (ft_strlen(str) > 9)
		return (error_msg("Number too big\n"), -1);
	i = 0;
	num = 0;
	while (str[i])
	{
		num = num * 10 + str[i] - 48;
		i++;
	}
	return (num);
}

bool	valid_input(int argc, char *argv[], t_data *data)
{
	if (argc < 5 || argc > 6)
		return (error_msg("Wrong number of arguments\n"), false);
	data->philo_count = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
	{
		data->times_to_eat = ft_atoi(argv[5]);
		if (data->times_to_eat < 1)
			return (false);
	}
	else
		data->times_to_eat = -1;
	if (data->philo_count < 1 || data->time_to_die < 1 || data->time_to_eat < 1
		|| data->time_to_sleep < 1)
		return (false);
	return (true);
}

bool	insert_seat(t_seat *head, t_seat *new)
{
	
}

t_seat	*create_seat_node(t_data *data)
{
	t_seat	*new;

	new = malloc(sizeof(t_seat));
	if (new == NULL)
		return (NULL);
	if (data->seats == NULL)
	{
		new->head = true;
		new->philo.id = 1;
		new->philo.meal_count = 0;
		new->data = data;
		new->next = new;
		new->prev = new;
		return (new);
	}
	new->head = false;
	new->philo.id = data->seats->prev->philo.id + 1;
	new->philo.meal_count = 0;
	new->data = data;
	return (new);
}

void	seat_add_back(t_seat **head, t_seat *new)
{
	t_seat	*current;

	if (*head == NULL)
	{
		*head = new;
		new->head = true;
		new->next = *head;
		new->prev = *head;
		return ;
	}
	current = *head;
	while (current->next->head != true)
	{
		current = current->next;
	}
	current->next = new;
	new->prev = current;
	new->next = *head;
	(*head)->prev = new;
}

// returns the current time in microseconds
int64_t	current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec) * 1000 + (tv.tv_usec) / 1000);
}

void	ft_usleep(int64_t time_to)
{
	int64_t	start;

	start = current_time();
	while (current_time() - start <= time_to)
		usleep(50);
}

bool	philo_is_alive(t_seat *seat)
{
	pthread_mutex_lock(&seat->data->m_state);
	if (seat->data->dead == true)
	{
		pthread_mutex_unlock(&seat->data->m_state);
		return (false);
	}
	pthread_mutex_unlock(&seat->data->m_state);
	return (true);
}

bool	meal_time(t_seat *seat)
{
	pthread_mutex_lock(&seat->next->fork);
	if (philo_is_alive(seat) == false)
	{
		pthread_mutex_unlock(&seat->next->fork);
		return (false);
	}
	printf("%ldms	%d Took a fork\n", current_time() - seat->data->start_time, seat->philo.id);
	pthread_mutex_lock(&seat->fork);
	if (philo_is_alive(seat) == false)
	{
		pthread_mutex_unlock(&seat->fork);
		pthread_mutex_unlock(&seat->next->fork);
		return (false);
	}
	printf("%ldms	%d Took a fork\n", current_time() - seat->data->start_time, seat->philo.id);

	pthread_mutex_lock(&seat->philo.m_meal_time);
	seat->philo.last_meal = current_time();
	pthread_mutex_unlock(&seat->philo.m_meal_time);

	
	
	printf("%ldms	%d is eating\n", current_time() - seat->data->start_time, seat->philo.id);
	// ft_usleep(seat->data->time_to_eat);
	usleep(seat->data->time_to_eat * 1000);
	
	pthread_mutex_unlock(&seat->fork);
	pthread_mutex_unlock(&seat->next->fork);

	if (seat->data->times_to_eat != -1)
	{
		pthread_mutex_lock(&seat->philo.m_meal_count);
		seat->philo.meal_count--;
		pthread_mutex_unlock(&seat->philo.m_meal_count);
	}
	return (true);
}

// bool	philo_is_alive(t_seat *seat)
// {
// 	pthread_mutex_lock(&seat->data->m_state);
// 	if (seat->data->dead == true)
// 	{
// 		pthread_mutex_unlock(&seat->data->m_state);
// 		return (false);
// 	}
// 	pthread_mutex_unlock(&seat->data->m_state);
// 	return (true);
// }

bool	philo_can_continue(t_seat *seat)
{
	if (philo_is_alive(seat) == false)
		return (false);
	
	pthread_mutex_lock(&seat->philo.m_meal_count);
	if ((seat->philo.meal_count > 0 || seat->philo.meal_count == -1))
	{
		pthread_mutex_unlock(&seat->philo.m_meal_count);
		return (true);
	}
	pthread_mutex_unlock(&seat->philo.m_meal_count);
	// return (true);
	return (false);
}



void	*routine(void *arg)
{
	t_seat	*seat;

	seat = arg;
	if (seat->philo.id % 2)
		usleep(15000);
	while(philo_can_continue(seat) == true)
	{
		// printf("%ldms	%d ENTERING LOOP=====================\n", current_time() - seat->data->start_time, seat->philo.id);
		if (meal_time(seat) == false)
			return (NULL);
		pthread_mutex_lock(&seat->philo.m_meal_count);
		if (seat->philo.meal_count == 0)
		{
			pthread_mutex_unlock(&seat->philo.m_meal_count);
			return (NULL);
		}
		pthread_mutex_unlock(&seat->philo.m_meal_count);

		// if (philo_is_alive(seat) == false)
		// 	return (NULL);

		pthread_mutex_lock(&seat->data->m_state);
		if (seat->data->dead == true)
		{
			pthread_mutex_unlock(&seat->data->m_state);
			return (NULL);
		}
		printf("%ldms	%d is sleeping\n", current_time() - seat->data->start_time, seat->philo.id);
		pthread_mutex_unlock(&seat->data->m_state);
		usleep(seat->data->time_to_sleep * 1000);
		// if (philo_is_alive(seat) == false)
		// 	return (NULL);

		pthread_mutex_lock(&seat->data->m_state);
		if (seat->data->dead == true)
		{
			pthread_mutex_unlock(&seat->data->m_state);
			return (NULL);
		}
		printf("%ldms	%d is thinking\n", current_time() - seat->data->start_time, seat->philo.id);
		pthread_mutex_unlock(&seat->data->m_state);
	}
}


bool	create_philo(t_data *data)
{
	int	i;
	t_seat	*new;

	data->seats = NULL;
	pthread_mutex_init(&data->m_state, NULL);
	// pthread_mutex_init(&data->m_print, NULL);
	i = 0;
	// data->philo_count = 4;
	// data->seats->philo.meal_count = -1;
	while (i < data->philo_count)
	{
		new = create_seat_node(data);
		if (new == NULL)
			return (false);
		seat_add_back(&data->seats, new);
		i++;
	}

	t_seat	*current;
	current = data->seats;
	i = 0;
	while (i < data->philo_count)
	{
		current->philo.meal_count = data->times_to_eat;
		//TODO: protect mutex
		int p = pthread_mutex_init(&current->fork, NULL);
		pthread_mutex_init(&current->philo.m_meal_time, NULL);
		pthread_mutex_init(&current->philo.m_meal_count, NULL);
		// printf("fork: %d\n", p);
		current = current->next;
		i++;
	}
	data->start_time = current_time();


	i = 0;
	current = data->seats;
	while (i < data->philo_count)
	{
		// printf("philo thread\n");
		current->philo.last_meal = current_time();
		// data->start_time = current_time();
		pthread_create(&current->philo.thread, NULL, &routine, current);
		current = current->next;
		i++;
	}


	current = data->seats;
	while (current)
	{
		pthread_mutex_lock(&current->philo.m_meal_time);
		int64_t last = current->philo.last_meal;
		pthread_mutex_unlock(&current->philo.m_meal_time);

		pthread_mutex_lock(&current->philo.m_meal_count);
		if (current->philo.meal_count == 0)
		{
			pthread_mutex_unlock(&current->philo.m_meal_count);
			break ;
		}
		pthread_mutex_unlock(&current->philo.m_meal_count);

		if (current_time() - last > data->time_to_die)
		{
			pthread_mutex_lock(&data->m_state);
			data->dead = true;
			printf("%ldms	\x1b[1;31m%d died\x1b[0m\n", current_time() - current->data->start_time, current->philo.id);
			pthread_mutex_unlock(&data->m_state);
			// printf("%d died\n", current->philo.id);
			
			// printf("%ldms	\x1b[1;31m%d died\x1b[0m\n", current_time() - current->data->start_time, current->philo.id);


			// exit(0);
			break ;
		}
		
		current = current->next;
	}



	i = 0;
	current = data->seats;
	while (i < data->philo_count)
	{
		pthread_join(current->philo.thread, NULL);
		current = current->next;
		i++;
	}




	// printf("%ld\n", current_time());

	
	// current = data->seats;
	// while (current->next->head != true)
	// {
	// 	printf("%d\n", current->philo.id);
	// 	current = current->next;
	// }

	// current = data->seats;
	// for (int i = 0; i < 10; i++)
	// {
	// 	printf("%d\n", current->head);
	// 	current = current->next;
	// }

	

	return (true);
}

int	main(int argc, char *argv[])
{
	t_data	data;

	if (valid_input(argc, argv, &data) == false)
		return (EXIT_FAILURE);
	if (create_philo(&data) == false)
		return (EXIT_FAILURE);
	
	// printf("%d\n", data.philo_count);
	// printf("%d\n", data.time_to_die);
	// printf("%d\n", data.time_to_eat);
	// printf("%d\n", data.time_to_sleep);
	// printf("%d\n", data.times_to_eat);

	// printf("argc: %d\n", argc);
	// printf("argv[0]: %d\n", ft_atoi(argv[1]));
	// printf("argv[1]: %d\n", ft_atoi(argv[2]));
	// printf("argv[2]: %d\n", ft_atoi(argv[3]));
	// if (argc != 5 && argc != 6)
	// 	return (EXIT_FAILURE);
	// if (setup(&data, argv) == false)
	// 	return (EXIT_FAILURE);
	// printf("%ld\n", data.start_time);
	// printf("%ld\n", data.seats->philo.last_meal);
	return (EXIT_SUCCESS);
}
