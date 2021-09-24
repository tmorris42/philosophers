/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/25 00:27:50 by tmorris           #+#    #+#             */
/*   Updated: 2021/09/25 00:33:08 by tmorris          ###   ########.fr       */
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

void	set_playing(t_data *data, int value)
{
	pthread_mutex_lock(&data->playing_lock);
	data->playing = value;
	pthread_mutex_unlock(&data->playing_lock);
}
