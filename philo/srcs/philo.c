#include "philo.h"

void	philo_kill(t_data *data, t_philo *philo)
{
	long int		delta_time;

	pthread_mutex_lock(&(data->log_lock));
	if (philo->data->playing && philo->alive)
	{
		data->playing = 0;
		philo->alive = 0;
		delta_time = ft_now() - philo->start_time;
		printf("%.11ld", delta_time);
		printf(" %d died\n", philo->id + 1);
	}
	pthread_mutex_unlock(&(philo->data->log_lock));
}

void	philo_eat(t_philo *philo)
{
	long int	stop_time;

	pthread_mutex_lock(&(philo->lock));
	if (philo->data->playing && philo->alive && !is_starving(philo))
	{
		philo->time_of_last_meal = ft_now();
		ft_log(philo, "is eating");
		pthread_mutex_unlock(&(philo->lock));
		stop_time = ft_now() + philo->data->time_to_eat;
		ft_usleep(stop_time);
		philo->times_eaten += 1;
	}
	else
		pthread_mutex_unlock(&(philo->lock));
}

void	philo_sleep(t_philo *philo)
{
	long int	stop_time;

	pthread_mutex_lock(&(philo->lock));
	if (philo->data->playing && philo->alive)
	{
		ft_log(philo, "is sleeping");
		pthread_mutex_unlock(&(philo->lock));
		stop_time = ft_now() + philo->data->time_to_sleep;
		ft_usleep(stop_time);
	}
	else
		pthread_mutex_unlock(&(philo->lock));
}

void	*philo_start(void *ptr)
{
	t_philo		*philo;

	philo = (t_philo *)ptr;
	while (philo && philo->alive && philo->data->playing)
	{
		if (try_to_take_forks(philo) < 0)
			continue ;
		philo_eat(philo);
		drop_forks(philo);
		philo_sleep(philo);
		pthread_mutex_lock(&(philo->lock));
		ft_log(philo, "is thinking");
		pthread_mutex_unlock(&(philo->lock));
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
