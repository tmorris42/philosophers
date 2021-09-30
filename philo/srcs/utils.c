/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/02 13:25:46 by tmorris           #+#    #+#             */
/*   Updated: 2021/09/30 14:06:30 by tmorris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long int	timeval_to_long_int(struct timeval time)
{
	return ((long int)time.tv_sec * 1000 + (long int)time.tv_usec / 1000);
}

long int	ft_now(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (timeval_to_long_int(time));
}

void	ft_usleep(t_philo *philo, long int delay)
{
	long int	finish;

	finish = ft_now() + delay;
	while (get_playing(philo->data) && ft_now() < finish)
	{
		if (finish - ft_now() > 2)
			usleep(SLEEP_INT);
	}
}

void	ft_log(t_philo *philo, char *msg)
{
	long int		delta_time;

	pthread_mutex_lock(&(philo->data->log_lock));
	if (get_playing(philo->data) && philo_get_alive(philo))
	{
		delta_time = ft_now() - philo->data->start_time;
		printf("%.11ld", delta_time);
		printf(" %d %s\n", philo->id + 1, msg);
	}
	pthread_mutex_unlock(&(philo->data->log_lock));
}
