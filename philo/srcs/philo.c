/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/02 13:25:04 by tmorris           #+#    #+#             */
/*   Updated: 2021/10/02 17:33:56 by tmorris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_eat(t_philo *philo)
{
	long int		now;
	long int		delta_time;

	now = ft_now();
	pthread_mutex_lock(&(philo->data->log_lock));
	pthread_mutex_lock(&(philo->lock));
	if (philo->alive)
	{
		philo->time_of_last_meal = now;
		delta_time = now - philo->data->start_time;
		ft_write_log(delta_time, philo->id, "is eating");
		++philo->times_eaten;
	}
	pthread_mutex_unlock(&(philo->lock));
	pthread_mutex_unlock(&(philo->data->log_lock));
	ft_usleep(philo, philo->data->time_to_eat);
}

void	philo_sleep(t_philo *philo)
{
	if (ft_log(philo, "is sleeping"))
		ft_usleep(philo, philo->data->time_to_sleep);
}

void	*philo_start(void *ptr)
{
	t_philo		*philo;

	philo = (t_philo *)ptr;
	while (philo)
	{
		if (!philo_get_alive(philo))
			break ;
		if (try_to_take_forks(philo) < 0)
			continue ;
		philo_eat(philo);
		drop_forks(philo);
		philo_sleep(philo);
		ft_log(philo, "is thinking");
		if (philo->data->num_of_philos % 2)
			ft_usleep(philo, 2 * philo->data->time_to_eat
				- philo->data->time_to_sleep);
		else
			ft_usleep(philo, philo->data->time_to_eat
				- philo->data->time_to_sleep);
	}
	return (ptr);
}

t_philo	*philo_create(int id, t_data *data)
{
	t_philo	*philo;

	if (!data || !data->forks)
		return (NULL);
	philo = (t_philo *)malloc(sizeof(*philo));
	if (!philo)
		return (NULL);
	if (pthread_mutex_init(&(philo->lock), NULL))
	{
		free(philo);
		return (NULL);
	}
	if (id)
		philo->left_fork = &data->forks[id - 1];
	else
		philo->left_fork = &data->forks[id];
	if (id)
		philo->right_fork = &data->forks[id];
	else
		philo->right_fork = &data->forks[data->num_of_philos - 1];
	philo->id = id;
	philo->alive = 1;
	philo->data = data;
	philo->times_eaten = 0;
	return (philo);
}
