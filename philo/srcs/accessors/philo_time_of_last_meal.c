/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_time_of_last_meal.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/25 00:30:02 by tmorris           #+#    #+#             */
/*   Updated: 2021/09/30 11:42:57 by tmorris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_set_time_of_last_meal(t_philo *philo, long int time)
{
	int	ate;

	ate = 0;
	pthread_mutex_lock(&philo->lock);
	ate = philo->alive;
	if (philo->alive)
		philo->time_of_last_meal = time;
	pthread_mutex_unlock(&philo->lock);
	return (ate);
}

long int	philo_get_time_since_meal(t_philo *philo)
{
	long int	time_since_meal;

	pthread_mutex_lock(&philo->lock);
	time_since_meal = (ft_now() - philo->time_of_last_meal);
	pthread_mutex_unlock(&philo->lock);
	return (time_since_meal);
}
