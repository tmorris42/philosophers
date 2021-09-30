/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/02 13:25:38 by tmorris           #+#    #+#             */
/*   Updated: 2021/09/30 18:21:53 by tmorris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	rejoin_threads(t_philo *philos[], int i)
{
	while (i >= 0)
	{
		pthread_join(philos[i]->tid, NULL);
		--i;
	}
}

int	create_threads(t_data *data, int inc)
{
	int		i;
	t_philo	*philo;

	i = 0;
	data->start_time = ft_now();
	while (i < data->num_of_philos)
	{
		philo = data->philos[i];
		philo->time_of_last_meal = data->start_time;
		if (pthread_create(&((philo)->tid), NULL, philo_start, philo))
		{
			set_playing(data, 0);
			rejoin_threads(data->philos, i);
			return (-1);
		}
		i += inc;
		if (i >= data->num_of_philos && i % inc != (inc - 1))
		{
			i = (i % inc) + 1;
			ft_usleep(philo, inc * data->time_to_eat / 4);
		}
	}
	return (0);
}

int	create_philos(t_data *data)
{
	int		i;
	size_t	size;

	size = sizeof(*data->philos) * data->num_of_philos;
	data->philos = (t_philo **)malloc(size);
	if (!data->philos)
		return (-1);
	memset(data->philos, 0, data->num_of_philos * sizeof(*(data->philos)));
	i = 0;
	while (i < data->num_of_philos)
	{
		data->philos[i] = philo_create(i, data);
		if (!(data->philos[i]))
			return (-1);
		++i;
	}
	return (0);
}

int	is_starving(t_philo *philo)
{
	long int	time_since_meal;
	long int	delta_time;

	pthread_mutex_lock(&philo->lock);
	time_since_meal = (ft_now() - philo->time_of_last_meal);
	if (time_since_meal > (long int)philo->data->time_to_die)
		philo->alive = 0;
	pthread_mutex_unlock(&philo->lock);
	if (time_since_meal > (long int)philo->data->time_to_die)
	{
		delta_time = ft_now() - philo->data->start_time;
		pthread_mutex_lock(&(philo->data->log_lock));
		printf("%.11ld", delta_time);
		printf(" %d died\n", philo->id + 1);
		set_playing(philo->data, 0);
		pthread_mutex_unlock(&(philo->data->log_lock));
	}
	return (time_since_meal > (long int)philo->data->time_to_die);
}

int	check_end_conditions(t_data *data)
{
	int	i;
	int	done;

	i = 0;
	done = 0;
	while (i < data->num_of_philos)
	{
		if (is_starving(data->philos[i]))
			break ;
		if (data->number_eats > -1)
			if (philo_get_times_eaten(data->philos[i]) >= data->number_eats)
				done += 1;
		++i;
	}
	if (done == data->num_of_philos)
		set_playing(data, 0);
	usleep(SLEEP_INT);
	return (0);
}
