/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/02 13:25:25 by tmorris           #+#    #+#             */
/*   Updated: 2021/09/25 00:27:27 by tmorris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	drop_fork(pthread_mutex_t *fork)
{
	pthread_mutex_unlock(fork);
	return (1);
}

int	drop_forks(t_philo *philo)
{
	drop_fork(philo->left_fork);
	drop_fork(philo->right_fork);
	return (1);
}

int	take_fork(t_philo *philo, pthread_mutex_t *fork)
{
	pthread_mutex_lock(fork);
	if (!(philo_get_alive(philo)))
	{
		pthread_mutex_unlock(fork);
		return (0);
	}
	ft_log(philo, "has taken a fork");
	return (1);
}

int	try_to_take_forks(t_philo *philo)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	if (philo->id % 2 && philo->id != philo->data->num_of_philos)
	{
		first_fork = philo->left_fork;
		second_fork = philo->right_fork;
	}
	else
	{
		first_fork = philo->right_fork;
		second_fork = philo->left_fork;
	}
	if (!take_fork(philo, first_fork))
		return (-1);
	if (philo->left_fork == philo->right_fork)
		while (philo_get_alive(philo))
			usleep(1);
	else if (philo_get_alive(philo))
	{
		if (take_fork(philo, second_fork))
			return (0);
	}
	drop_fork(first_fork);
	return (-1);
}

int	create_forks(t_data *data)
{
	int		i;
	size_t	size;

	size = sizeof(*data->forks) * data->num_of_philos;
	data->forks = (pthread_mutex_t *)malloc(size);
	if (!data->forks)
	{
		data_free(&data);
		return (-1);
	}
	i = 0;
	while (i < data->num_of_philos)
	{
		if (pthread_mutex_init(&(data->forks[i]), NULL))
		{
			data_free(&data);
			return (-1);
		}
		++i;
	}
	return (0);
}
