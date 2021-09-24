/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_alive.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/25 00:29:28 by tmorris           #+#    #+#             */
/*   Updated: 2021/09/25 00:33:01 by tmorris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_get_alive(t_philo *philo)
{
	int	alive;

	pthread_mutex_lock(&philo->lock);
	alive = philo->alive;
	pthread_mutex_unlock(&philo->lock);
	return (alive);
}

void	philo_set_alive(t_philo *philo, int alive)
{
	pthread_mutex_lock(&philo->lock);
	philo->alive = alive;
	pthread_mutex_unlock(&philo->lock);
}
