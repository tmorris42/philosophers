/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/02 13:25:38 by tmorris           #+#    #+#             */
/*   Updated: 2021/10/01 16:20:52 by tmorris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
			data->playing = 0;
			rejoin_threads(data->philos, i);
			return (-1);
		}
		i += inc;
		if (i >= data->num_of_philos && i % inc != (inc - 1))
		{
			i = (i % inc) + 1;
			if (data->time_to_die > data->time_to_eat)
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

void	kill_all_philos(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_of_philos)
	{
		philo_set_alive(data->philos[i], 0);
		++i;
	}
}

int	check_philo_status(t_philo *philo, t_data *data)
{
	long int	time_since_meal;
	long int	delta_time;
	int			times_eaten;

	times_eaten = -1;
	pthread_mutex_lock(&philo->lock);
	time_since_meal = (ft_now() - philo->time_of_last_meal);
	if (time_since_meal > (long int)philo->data->time_to_die)
		philo->alive = 0;
	else
		times_eaten = philo->times_eaten;
	pthread_mutex_unlock(&philo->lock);
	if (time_since_meal > (long int)philo->data->time_to_die)
	{
		delta_time = ft_now() - philo->data->start_time;
		pthread_mutex_lock(&(philo->data->log_lock));
		kill_all_philos(data);
		printf("%.11ld", delta_time);
		printf(" %d died\n", philo->id + 1);
		data->playing = 0;
		pthread_mutex_unlock(&(philo->data->log_lock));
	}
	return (times_eaten);
}

int	check_end_conditions(t_data *data)
{
	int	i;
	int	done;
	int	times_eaten;

	i = 0;
	done = 0;
	while (i < data->num_of_philos && data->playing)
	{
		times_eaten = check_philo_status(data->philos[i], data);
		if (data->number_eats > -1 && times_eaten >= data->number_eats)
			done += 1;
		++i;
	}
	if (done == data->num_of_philos)
		data->playing = 0;
	return (0);
}
