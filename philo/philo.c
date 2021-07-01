#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include "philo.h"

# ifndef SLEEP_INT
#  define SLEEP_INT 0
# endif

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

void	ft_log(t_philo *philo, char *msg)
{
	long int		delta_time;
	struct timeval	time;

	pthread_mutex_lock(&(philo->data->log_lock));
	if (philo->data->playing && philo->alive)
	{
		gettimeofday(&time, NULL);
		delta_time = timeval_to_long_int(time);
		delta_time -= philo->data->start_time;
		printf("%ldms", delta_time);
		printf(" %d %s\n", philo->id + 1, msg);
	}
	pthread_mutex_unlock(&(philo->data->log_lock));
}

int	starving(t_philo *philo)
{
	int	dead;

	dead = (now_int() - philo->time_of_last_meal >= (long int)philo->data->settings->time_to_die);
	if (philo->alive && dead)
	{
		ft_log(philo, "has died");
		philo->alive = 0;
		philo->data->playing = 0;
	}
	return (dead);
}

int	take_fork(t_philo *philo, t_fork *fork)
{
	pthread_mutex_lock(&(fork->lock));
	if (starving(philo))
	{
		pthread_mutex_unlock(&(fork->lock));
		return (0);
	}
	fork->available = 0;
	ft_log(philo, "has taken a fork");
	return (1);
}

void	*start_philo(void *p)
{
	t_philo		*philo;
	long int	timer;

	philo = (t_philo *)p;

	while (philo && philo->alive && philo->data->playing)
	{
		if (philo->data->playing && starving(philo))
			philo->alive = 0;
		if (philo->left_fork == philo->right_fork)
			continue ;
		if (philo->id % 2)
		{
			if (!philo->data->playing || !(take_fork(philo, philo->left_fork)))
				break ;
			if (!philo->data->playing || !(take_fork(philo, philo->right_fork)))
			{
				pthread_mutex_unlock(&(philo->left_fork->lock));
				break ;
			}
		}
		else
		{
			if (!philo->data->playing || !(take_fork(philo, philo->right_fork)))
				break ;
			if (!philo->data->playing || !(take_fork(philo, philo->left_fork)))
			{
				pthread_mutex_unlock(&(philo->right_fork->lock));
				break ;
			}
		}
		if (philo->data->playing && !starving(philo))
		{
			philo->time_of_last_meal = now_int();
			ft_log(philo, "is eating");
			//eating here
			timer = now_int() + philo->data->settings->time_to_eat;
			while (philo->data->playing && !starving(philo) && now_int() < timer)
			{
				usleep(SLEEP_INT);
			}
		}

		pthread_mutex_unlock(&(philo->left_fork->lock));
		pthread_mutex_unlock(&(philo->right_fork->lock));

		if (philo->data->playing && !starving(philo) && philo->alive)
		{
			// Sleeping
			ft_log(philo, "is sleeping");
			timer = now_int() + philo->data->settings->time_to_sleep;
			while (philo->data->playing && !starving(philo) && now_int() < timer)
			{
				usleep(SLEEP_INT);
			}
		}
	}
	pthread_exit(p);
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
	philo->left_fork = &data->forks[id];
	if (id)
		philo->right_fork = &data->forks[id - 1];
	else
		philo->right_fork = &data->forks[data->settings->number_of_philosophers - 1];
	philo->alive = 1;
	philo->data = data;
	philo->time_of_last_meal = philo->data->start_time;
	return (philo);
}

int	parse_argument(char *arg)
{
	int	i;
	int	value;

	value = 0;
	i = 0;
	while (arg && arg[i])
	{
		if (arg[i] < '0' || arg[i] > '9')
			return (-1);
		value = (value * 10) + (arg[i] - '0');
		++i;
	}
	return (value);
}

t_settings	*get_settings(int argc, char **argv, int *settings)
{
	int			i;
	int			value;
	t_settings	*ss; //setting struct: rename to settings if kept

	if (argc < 5 || argc > 6)
		return (NULL);
	i = 0;
	while (i < 5)
	{
		settings[i] = -1;
		++i;
	}
	i = 1;
	while (argc > 1 && i < argc)
	{
		value = parse_argument(argv[i]);
		if (value < 0)
			return (NULL);
		else
			settings[i - 1] = value;
		++i;
	}
	ss = (t_settings *)malloc(sizeof(*ss));
	if (!ss)
		return (NULL);
	ss->number_of_philosophers = settings[0];
	ss->time_to_die = settings[1];
	ss->time_to_eat = settings[2];
	ss->time_to_sleep = settings[3];
	ss->number_of_times_each_philosopher_must_eat = settings[4];
	return (ss);
}

void	free_data(t_data **data_ptr)
{
	t_data	*data;
	int		i;

	data = (*data_ptr);
	pthread_mutex_destroy(&data->taking_forks);
	pthread_mutex_destroy(&data->log_lock);
	if (data->philos && data->settings)
	{
		i = 0;
		while (i < data->settings->number_of_philosophers)
		{
			free(data->philos[i]);
			data->philos[i] = NULL;
			++i;
		}
	}
	if (data->philos && data->settings)
	{
		i = 0;
		while (i < data->settings->number_of_philosophers)
		{
			pthread_mutex_destroy(&data->forks[i].lock);
			++i;
		}
	}
	free(data->philos);
	free(data->forks);
	free(data->settings);
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
	data->settings = NULL;
	data->philos = NULL;
	data->forks = NULL; //should be an array of mutex entries for the forks
	data->start_time = now_int();
	if (pthread_mutex_init(&data->taking_forks, NULL))
	{
		free_data(&data);
		return (NULL);
	}
	if (pthread_mutex_init(&data->log_lock, NULL))
	{
		free_data(&data);
		return (NULL);
	}
	return (data);	
}

int	run(int argc, char **argv)
{
	int		i;
	int		settings[5];
	t_data	*data;
	int		*ptr;
	int		err;

	data = init_data();
	if (!data)
		return (-1); //cleanup first
	data->settings = get_settings(argc, argv, &settings[0]);
	if (!(data->settings))
	{
		printf("Usage: ./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n");
		free_data(&data);
		return (-1); //cleanup first
	}

	data->philos = (t_philo **)malloc(sizeof(*data->philos) * data->settings->number_of_philosophers);
	if (!data->philos)
	{
		free_data(&data);
		return (-1);
	}

	data->forks = (t_fork *)malloc(sizeof(*data->forks) * data->settings->number_of_philosophers);
	if (!data->forks)
	{
		free_data(&data);
		return (-1);
	}

	i = 0;
	while (i < data->settings->number_of_philosophers)
	{
		data->philos[i] = NULL;
		++i;
	}

	// Create Forks
	i = 0;
	while (i < data->settings->number_of_philosophers)
	{
		if (pthread_mutex_init(&(data->forks[i].lock), NULL))
		{
			free_data(&data);
			return (-1);
		}
		data->forks[i].id = i;
		data->forks[i].available = 1;
		++i;
	}
	i = 0;
	while (i < data->settings->number_of_philosophers)
	{
		data->philos[i] = create_philo(i, data);
		if (!(data->philos[i]))
		{
			free_data(&data);
			return (-1);
		}
		++i;
	}
	i = 0;
	while (i < data->settings->number_of_philosophers)
	{
		err = pthread_create(&((data->philos[i])->tid), NULL, start_philo, data->philos[i]);
		if (err)
		{
			printf("pthread_create failed : %s\n", strerror(err));
			free_data(&data);
			return (-1); //clean up first, including created threads!!!
		}
		++i;
	}
	while (data->playing)
	{
		i = 0;
		while (i < data->settings->number_of_philosophers)
		{
			if (starving(data->philos[i]))
			{
				//kill philo
//				pthread_detach(data->philos[i]->tid);
				data->playing = 0;
			}
			++i;
		}
	}
	i = 0;
	while (i < data->settings->number_of_philosophers)
	{
		pthread_join(data->philos[i]->tid, (void **)&ptr); //should you detach instead?
		++i;
	}
	free_data(&data);
	return (0);
}

int	main(int argc, char **argv)
{
	int	ret;

	ret = run(argc, argv);
//	pthread_exit(&ret);
	return (ret);
}
