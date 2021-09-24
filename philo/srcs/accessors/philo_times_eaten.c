/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_times_eaten.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/25 00:30:02 by tmorris           #+#    #+#             */
/*   Updated: 2021/09/25 00:32:13 by tmorris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_add_times_eaten(t_philo *philo, int times)
{
	pthread_mutex_lock(&philo->lock);
	philo->times_eaten += times;
	pthread_mutex_unlock(&philo->lock);
}

int	philo_get_times_eaten(t_philo *philo)
{
	int	times_eaten;

	pthread_mutex_lock(&philo->lock);
	times_eaten = philo->times_eaten;
	pthread_mutex_unlock(&philo->lock);
	return (times_eaten);
}
