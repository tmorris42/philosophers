/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_log.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/02 18:13:50 by tmorris           #+#    #+#             */
/*   Updated: 2021/10/04 10:36:18 by tmorris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	pthread_mutex_lock(&(philo->data->log_lock));
	pthread_mutex_lock(&(philo->lock));
	if (philo->alive)
	{
		now = ft_now();
		delta_time = now - philo->data->start_time;
		ft_write_log(delta_time, philo->id, msg);
		printed = 1;
	}
	pthread_mutex_unlock(&(philo->lock));
	pthread_mutex_unlock(&(philo->data->log_lock));
	return (printed);
}
