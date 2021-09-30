/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/02 13:25:19 by tmorris           #+#    #+#             */
/*   Updated: 2021/09/30 14:07:13 by tmorris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	data_free(t_data **data_ptr)
{
	t_data	*data;
	int		i;

	if (!data_ptr || ! (*data_ptr))
		return ;
	data = (*data_ptr);
	pthread_mutex_destroy(&data->log_lock);
	pthread_mutex_destroy(&data->playing_lock);
	if (data->philos && data->num_of_philos)
	{
		i = 0;
		while (i < data->num_of_philos)
		{
			pthread_mutex_destroy(&data->philos[i]->lock);
			free(data->philos[i]);
			data->philos[i] = NULL;
			if (data->forks)
				pthread_mutex_destroy(&data->forks[i]);
			++i;
		}
	}
	free(data->philos);
	free(data->forks);
	free(data);
	(*data_ptr) = NULL;
}

t_data	*data_init(void)
{
	t_data	*data;

	data = (t_data *)malloc(sizeof(*data));
	if (!data)
		return (NULL);
	data->playing = 1;
	data->philos = NULL;
	data->forks = NULL;
	data->start_time = 0;
	if (pthread_mutex_init(&data->log_lock, NULL))
	{
		data_free(&data);
		return (NULL);
	}
	if (pthread_mutex_init(&data->playing_lock, NULL))
	{
		data_free(&data);
		return (NULL);
	}
	return (data);
}
