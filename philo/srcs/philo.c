/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/02 13:25:04 by tmorris           #+#    #+#             */
/*   Updated: 2021/09/27 12:21:18 by tmorris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_eat(t_philo *philo)
{
	long int	stop_time;

	if (get_playing(philo->data) && philo_get_alive(philo))
	{
		philo_set_time_of_last_meal(philo, ft_now());
		ft_log(philo, "is eating");
		stop_time = ft_now() + philo->data->time_to_eat;
		ft_usleep(stop_time);
		philo_add_times_eaten(philo, 1);
	}
}

void	philo_sleep(t_philo *philo)
{
	long int	stop_time;

	if (get_playing(philo->data) && philo_get_alive(philo))
	{
		ft_log(philo, "is sleeping");
		stop_time = ft_now() + philo->data->time_to_sleep;
		ft_usleep(stop_time);
	}
}

void	*philo_start(void *ptr)
{
	t_philo		*philo;

	philo = (t_philo *)ptr;
	while (philo)
	{
		if (!philo_get_alive(philo))
			break ;
		if (!get_playing(philo->data))
			break ;
		if (try_to_take_forks(philo) < 0)
			continue ;
		philo_eat(philo);
		drop_forks(philo);
		philo_sleep(philo);
		ft_log(philo, "is thinking");
		if (philo->time_of_last_meal != philo->start_time)
			usleep(10);
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
	philo->id = id;
	pthread_mutex_init(&(philo->lock), NULL);
	philo->left_fork = &data->forks[id];
	if (id)
		philo->right_fork = &data->forks[id - 1];
	else
		philo->right_fork = &data->forks[data->num_of_philos - 1];
	philo->alive = 1;
	philo->data = data;
	philo->times_eaten = 0;
	philo->start_time = data->start_time;
	philo->time_of_last_meal = philo->start_time;
	return (philo);
}
