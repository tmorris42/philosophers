/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/02 13:25:46 by tmorris           #+#    #+#             */
/*   Updated: 2021/10/02 17:44:38 by tmorris          ###   ########.fr       */
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
	if (finish < 0)
		return ;
	while (philo_get_alive(philo) && ft_now() < finish)
	{
		if (finish - ft_now() > 5)
			usleep(SLEEP_INT);
	}
}

void	ft_putnbr(long int i)
{
	if (i < 10)
	{
		i += '0';
		write(1, &i, 1);
	}
	else
	{
		ft_putnbr(i / 10);
		ft_putnbr(i % 10);
	}
}

void	ft_putstr(const char *msg)
{
	int	i;

	if (!msg)
		return ;
	i = 0;
	while (msg[i])
		++i;
	write(1, msg, i);
}

void	ft_write_log(long int delta_time, int id, char *msg)
{
	ft_putnbr(delta_time);
	ft_putstr("ms ");
	ft_putnbr(id + 1);
	ft_putstr(" ");
	ft_putstr(msg);
	ft_putstr("\n");
}

int	ft_log(t_philo *philo, char *msg)
{
	long int		now;
	long int		delta_time;
	int				printed;

	printed = 0;
	now = ft_now();
	pthread_mutex_lock(&(philo->data->log_lock));
	pthread_mutex_lock(&(philo->lock));
	if (philo->alive)
	{
		delta_time = now - philo->data->start_time;
		ft_write_log(delta_time, philo->id, msg);
		printed = 1;
	}
	pthread_mutex_unlock(&(philo->lock));
	pthread_mutex_unlock(&(philo->data->log_lock));
	return (printed);
}
