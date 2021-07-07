#include "philo.h"

int	create_threads(t_data *data)
{
	int		i;
	int		*ptr;
	t_philo	*philo;

	i = 0;
	while (i < data->num_of_philos)
	{
		philo = data->philos[i];
		if (pthread_create(&((philo)->tid), NULL, philo_start, philo))
		{
			data->playing = 0;
			--i;
			while (i >= 0)
			{
				pthread_join(philo->tid, (void **)&ptr);
				--i;
			}
			data_free(&data);
			return (-1);
		}
		++i;
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
	{
		data_free(&data);
		return (-1);
	}
	memset(data->philos, 0, data->num_of_philos * sizeof(*(data->philos)));
	i = 0;
	data->start_time = ft_now();
	while (i < data->num_of_philos)
	{
		data->philos[i] = philo_create(i, data);
		if (!(data->philos[i]))
		{
			data_free(&data);
			return (-1);
		}
		++i;
	}
	return (0);
}

int	is_starving(t_philo *philo)
{
	long int	time_since_meal;

	time_since_meal = (ft_now() - philo->time_of_last_meal);
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
		pthread_mutex_lock(&(data->philos[i]->lock));
		if (is_starving(data->philos[i]))
			philo_kill(data, data->philos[i]);
		pthread_mutex_unlock(&(data->philos[i]->lock));
		if (data->number_eats > -1)
			if (data->philos[i]->times_eaten >= data->number_eats)
				done += 1;
		++i;
	}
	if (done == data->num_of_philos)
	{
		data->playing = 0;
	}
	usleep(SLEEP_INT);
	return (0);
}

void	rejoin_threads(t_data *data)
{
	int	i;
	int	*ptr;

	i = 0;
	while (i < data->num_of_philos)
	{
		pthread_join(data->philos[i]->tid, (void **)&ptr);
		++i;
	}
}
