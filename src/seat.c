/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   seat.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi <imisumi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 11:54:34 by imisumi           #+#    #+#             */
/*   Updated: 2023/11/09 13:56:48 by imisumi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	free_seat_fail(t_seat *head)
{
	t_seat	*temp;
	t_seat	*current;

	current = head;
	while (current != NULL && current->next != head)
	{
		temp = current;
		current = current->next;
		free(temp);
	}
	free(current);
	head = NULL;
}

void	insert_seat(t_seat **head, t_seat *new)
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
		current = current->next;
	current->next = new;
	new->prev = current;
	new->next = *head;
	(*head)->prev = new;
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
