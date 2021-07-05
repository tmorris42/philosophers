#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include "philo.h"

int	starving(t_philo *philo);

long int	timeval_to_long_int(struct timeval time)
{
	return ((long int)time.tv_sec * 1000 + (long int)time.tv_usec / 1000);
}

long int	now_int(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (timeval_to_long_int(time));
}

void	ft_usleep(long int finish)
{
	while (now_int() < finish)
	{
		usleep(SLEEP_INT);
	}
}

void	ft_log(t_philo *philo, char *msg)
{
	long int		delta_time;
	struct timeval	time;

	pthread_mutex_lock(&(philo->data->log_lock));
	if (philo->data->playing && philo->alive)
	{
		gettimeofday(&time, NULL);
		delta_time = timeval_to_long_int(time);
		delta_time -= philo->start_time;
		printf("%.11ld", delta_time);
		printf(" %d %s\n", philo->id + 1, msg);
	}
	pthread_mutex_unlock(&(philo->data->log_lock));
}

void	kill_philo(t_data *data, t_philo *philo)
{
	long int		delta_time;
	struct timeval	time;

	pthread_mutex_lock(&(data->log_lock));
	if (philo->data->playing && philo->alive)
	{
		data->playing = 0;
		philo->alive = 0;
		gettimeofday(&time, NULL);
		delta_time = timeval_to_long_int(time);
		delta_time -= philo->start_time;
		printf("%.11ld", delta_time);
		printf(" %d died\n", philo->id + 1);
	}
	pthread_mutex_unlock(&(philo->data->log_lock));
}

int	starving(t_philo *philo)
{
	long int	time_since_meal;

	time_since_meal = (now_int() - philo->time_of_last_meal);
	return (time_since_meal > (long int)philo->data->time_to_die);
}

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
	pthread_mutex_lock(&(philo->lock));
	pthread_mutex_lock(fork);
	if (!(philo->alive))
	{
		pthread_mutex_unlock(fork);
		return (0);
	}
	ft_log(philo, "has taken a fork");
	pthread_mutex_unlock(&(philo->lock));
	return (1);
}

int	try_to_take_fork(t_philo *philo)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	if (philo->id % 2)
	{
		first_fork = philo->left_fork;
		second_fork = philo->right_fork;
	}
	else
	{
		first_fork = philo->right_fork;
		second_fork = philo->left_fork;
	}
	take_fork(philo, first_fork);
	if (philo->left_fork == philo->right_fork)
		while (philo->alive)
			usleep(1);
	else if (philo->alive)
	{
		take_fork(philo, second_fork);
		return (0);
	}
	drop_fork(first_fork);
	return (-1);
}

void	philo_eat(t_philo *philo)
{
	long int	timer;

	pthread_mutex_lock(&(philo->lock));
	if (philo->data->playing && philo->alive && !starving(philo))
	{
		philo->time_of_last_meal = now_int();
		ft_log(philo, "is eating");
		pthread_mutex_unlock(&(philo->lock));
		timer = now_int() + philo->data->time_to_eat;
		ft_usleep(timer);
		philo->times_eaten += 1;
	}
	else
		pthread_mutex_unlock(&(philo->lock));
}

void	philo_sleep(t_philo *philo)
{
	long int	timer;

	pthread_mutex_lock(&(philo->lock));
	if (philo->data->playing && philo->alive)
	{
		ft_log(philo, "is sleeping");
		pthread_mutex_unlock(&(philo->lock));
		timer = now_int() + philo->data->time_to_sleep;
		ft_usleep(timer);
	}
	else
		pthread_mutex_unlock(&(philo->lock));
}

void	*start_philo(void *p)
{
	t_philo		*philo;

	philo = (t_philo *)p;
	philo->start_time = now_int();
	philo->time_of_last_meal = philo->start_time;
	while (philo && philo->alive && philo->data->playing)
	{
		if (try_to_take_fork(philo) < 0)
			continue ;
		philo_eat(philo);
		drop_forks(philo);
		philo_sleep(philo);
	}
	return (p);
}

t_philo	*create_philo(int id, t_data *data)
{
	t_philo	*philo;

	if (!data)
		return (NULL);
	if (!data->forks)
		printf("No forks!\n");
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
	philo->start_time = now_int();
	philo->time_of_last_meal = philo->start_time;
	return (philo);
}

int	parse_argument(char *arg)
{
	int			i;
	long int	value;

	value = 0;
	i = 0;
	while (arg && arg[i])
	{
		if (arg[i] < '0' || arg[i] > '9')
			return (-1);
		value = (value * 10) + (arg[i] - '0');
		++i;
	}
	if (value > 2147483647)
		return (-1);
	return (value);
}

int	get_settings(int argc, char **argv, t_data *data)
{
	int			i;
	int			settings_array[5];

	if (argc < 5 || argc > 6)
		return (-1);
	settings_array[4] = -1;
	i = 1;
	while (argc > 1 && i < argc)
	{
		settings_array[i - 1] = parse_argument(argv[i]);
		if (settings_array[i - 1] < 0)
			return (-1);
		++i;
	}
	data->num_of_philos = settings_array[0];
	data->time_to_die = settings_array[1];
	data->time_to_eat = settings_array[2];
	data->time_to_sleep = settings_array[3];
	data->number_eats = settings_array[4];
	return (0);
}

void	free_data(t_data **data_ptr)
{
	t_data	*data;
	int		i;

	data = (*data_ptr);
	pthread_mutex_destroy(&data->log_lock);
	if (data->philos && data->num_of_philos)
	{
		i = 0;
		while (i < data->num_of_philos)
		{
			pthread_mutex_destroy(&data->philos[i]->lock);
			free(data->philos[i]);
			data->philos[i] = NULL;
			pthread_mutex_destroy(&data->forks[i]);
			++i;
		}
	}
	free(data->philos);
	free(data->forks);
	free(data);
	(*data_ptr) = NULL;
}

t_data	*init_data(void)
{
	t_data	*data;

	data = (t_data *)malloc(sizeof(*data));
	if (!data)
		return (NULL);
	data->playing = 1;
	data->philos = NULL;
	data->forks = NULL; //should be an array of mutex entries for the forks
	data->start_time = now_int();
	if (pthread_mutex_init(&data->log_lock, NULL))
	{
		free_data(&data);
		return (NULL);
	}
	return (data);
}

int	create_threads(t_data *data)
{
	int		i;
	int		*ptr;
	t_philo	*philo;

	i = 0;
	while (i < data->num_of_philos)
	{
		philo = data->philos[i];
		if (pthread_create(&((philo)->tid), NULL, start_philo, philo))
		{
			data->playing = 0;
			--i;
			while (i >= 0)
			{
				pthread_join(philo->tid, (void **)&ptr);
				--i;
			}
			free_data(&data);
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
		free_data(&data);
		return (-1);
	}
	memset(data->philos, 0, data->num_of_philos * sizeof(*(data->philos)));
	i = 0;
	data->start_time = now_int();
	while (i < data->num_of_philos)
	{
		data->philos[i] = create_philo(i, data);
		if (!(data->philos[i]))
		{
			free_data(&data);
			return (-1);
		}
		++i;
	}
	return (0);
}

int	create_forks(t_data *data)
{
	int	i;
	size_t	size;

	size = sizeof(*data->forks) * data->num_of_philos;
	data->forks = (pthread_mutex_t *)malloc(size);
	if (!data->forks)
	{
		free_data(&data);
		return (-1);
	}
	i = 0;
	while (i < data->num_of_philos)
	{
		if (pthread_mutex_init(&(data->forks[i]), NULL))
		{
			free_data(&data);
			return (-1);
		}
		++i;
	}
	return (0);
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
		if (starving(data->philos[i]))
			kill_philo(data, data->philos[i]);
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

int	usage(t_data **data)
{
	int	i;

	i = 50;
	write(2, "Usage: ./philo number_of_philosophers time_to_die ", i);
	i = 26;
	write(2, "time_to_eat time_to_sleep ", i);
	i = 44;
	write(2, "[number_of_times_each_philosopher_must_eat]\n", i);
	free_data(data);
	return (-1);
}

int	run(int argc, char **argv)
{
	t_data	*data;

	data = init_data();
	if (!data)
		return (-1);
	if (get_settings(argc, argv, data) < 0)
		return (usage(&data));
	if (create_forks(data) < 0)
		return (-1);
	if (create_philos(data) < 0)
		return (-1);
	if (create_threads(data) < 0)
		return (-1);
	while (data->playing)
		check_end_conditions(data);
	rejoin_threads(data);
	free_data(&data);
	return (0);
}

int	main(int argc, char **argv)
{
	int	ret;

	ret = run(argc, argv);
	return (ret);
}
