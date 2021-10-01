/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/02 13:25:19 by tmorris           #+#    #+#             */
/*   Updated: 2021/10/01 13:40:46 by tmorris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	data_free(t_data *data)
{
	int		i;

	if (!data)
		return (0);
	if (data->locks_ready)
		pthread_mutex_destroy(&data->log_lock);
	i = 0;
	while (data && i < data->num_of_philos)
	{
		if (data->philos && data->philos[i])
		{
			pthread_mutex_destroy(&data->philos[i]->lock);
			free(data->philos[i]);
			data->philos[i] = NULL;
		}
		if (data->forks && data->locks_ready > i + 1)
			pthread_mutex_destroy(&data->forks[i]);
		++i;
	}
	free(data->philos);
	free(data->forks);
	free(data);
	return (0);
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
	data->locks_ready = 0;
	if (pthread_mutex_init(&data->log_lock, NULL))
	{
		data_free(data);
		return (NULL);
	}
	++data->locks_ready;
	return (data);
}
