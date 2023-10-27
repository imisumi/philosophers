/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi <imisumi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 11:54:34 by imisumi           #+#    #+#             */
/*   Updated: 2023/10/27 16:08:54 by imisumi          ###   ########.fr       */
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

int	main(int argc, char *argv[])
{
	t_data	data;

	// printf("hey\n");
	if (valid_input(argc, argv, &data) == false)
		return (EXIT_FAILURE);

	printf("%d\n", data.philo_count);
	printf("%d\n", data.time_to_die);
	printf("%d\n", data.time_to_eat);
	printf("%d\n", data.time_to_sleep);
	printf("%d\n", data.times_to_eat);

	// printf("argc: %d\n", argc);
	// printf("argv[0]: %d\n", ft_atoi(argv[1]));
	// printf("argv[1]: %d\n", ft_atoi(argv[2]));
	// printf("argv[2]: %d\n", ft_atoi(argv[3]));
	// if (argc != 5 && argc != 6)
	// 	return (EXIT_FAILURE);
	// if (setup(&data, argv) == false)
	// 	return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
