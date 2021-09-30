/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/25 00:27:50 by tmorris           #+#    #+#             */
/*   Updated: 2021/09/27 10:40:04 by tmorris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	get_playing(t_data *data)
{
	int	playing;

	pthread_mutex_lock(&data->playing_lock);
	playing = data->playing;
	pthread_mutex_unlock(&data->playing_lock);
	return (playing);
}

int	set_playing(t_data *data, int value)
{
	int	success;

	success = 1;
	pthread_mutex_lock(&data->playing_lock);
	if (data->playing == value)
		success = 0;
	data->playing = value;
	pthread_mutex_unlock(&data->playing_lock);
	return (success);
}
