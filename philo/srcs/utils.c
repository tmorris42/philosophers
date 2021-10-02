/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/02 13:25:46 by tmorris           #+#    #+#             */
/*   Updated: 2021/10/02 18:29:19 by tmorris          ###   ########.fr       */
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

void	ft_usleep_until(t_philo *philo, long int finish)
{
	int	remaining;

	remaining = finish - ft_now();
	while (philo_get_alive(philo) && remaining > 0)
	{
		if (remaining > SLEEP_INT)
			usleep(remaining / 2);
		else
			usleep(1);
		remaining = finish - ft_now();
	}
}

void	ft_usleep(t_philo *philo, long int delay)
{
	long int	finish;

	if (delay < 1)
		return ;
	finish = ft_now() + delay;
	ft_usleep_until(philo, finish);
}
