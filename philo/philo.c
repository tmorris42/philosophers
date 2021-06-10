#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "philo.h"

void	ft_log(int id, char *msg)
{
	printf("TIMESTAMP %d %s\n", id + 1, msg);
}

void	*start_philo(void *p)
{
	t_philo *philo;

	philo = (t_philo *)p;

	while (philo && philo->alive && philo->data->playing)
	{
		pthread_mutex_lock(philo->taking_forks);
		if (philo->data->playing && philo->left_fork->available && philo->right_fork->available)
		{
			pthread_mutex_lock(&(philo->left_fork->lock)); //don't need to lock forks... maybw
			printf("Philosopher #%d is taking Fork #%d\n", philo->id, philo->left_fork->id);
			philo->left_fork->available = 0;
			ft_log(philo->id, "has taken a fork");
			pthread_mutex_lock(&(philo->right_fork->lock));
			printf("Philosopher #%d is taking Fork #%d\n", philo->id, philo->right_fork->id);
			philo->right_fork->available = 0;
			ft_log(philo->id, "has taken a fork");
			pthread_mutex_unlock(philo->taking_forks);
			ft_log(philo->id, "is eating");

			//eating here
			usleep(philo->data->settings->time_to_eat * 1000);

			printf("Philosopher #%d is about to release the forks\n", philo->id);
			philo->left_fork->available = 1;
			philo->right_fork->available = 1;
			pthread_mutex_unlock(&(philo->left_fork->lock));
			pthread_mutex_unlock(&(philo->right_fork->lock));
			printf("Philosopher #%d has already released the forks\n", philo->id);

			// Sleeping
			ft_log(philo->id, "is sleeping");

			// Dying because other death conditions not set yet and I don't want infinite
			philo->alive = 0;
		}
		else
			pthread_mutex_unlock(philo->taking_forks);
	}
	if (philo && philo->data && philo->data->playing && (!philo->alive))
	{
		philo->data->playing = 0;
		ft_log(philo->id, "has died");
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
	philo->taking_forks = &(data->taking_forks);
	philo->alive = 1;
	philo->data = data;
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
	int	i;
	int	value;
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
	int	i;

	data = (*data_ptr);
	pthread_mutex_destroy(&data->taking_forks);
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
	free(data->philos);
/*	if (data->forks && data->settings)
	{
		i = 0;
		while (i < data->settings->number_of_philosophers)
		{
			free(data->forks[i]);
			data->forks[i] = NULL;
			++i;
		}
	} */
	free(data->forks);
	free(data->settings);
	free(data);
	(*data_ptr) = NULL;
}

t_data	*init_data()
{
	t_data	*data;

	data = (t_data *)malloc(sizeof(*data));
	if (!data)
		return (NULL);
	data->playing = 1;
	data->settings = NULL;
	data->philos = NULL;
	data->forks = NULL; //should be an array of mutex entries for the forks
	if (pthread_mutex_init(&data->taking_forks, NULL))
	{
		free_data(&data);
		return (NULL);
	}
	return (data);	
}

int	run(int argc, char **argv)
{
	int	i;
	int	settings[5];
	t_data *data;
	int *ptr;
	int	err;

	data = init_data();
	if (!data)
		return (-1); //cleanup first
	data->settings = get_settings(argc, argv, &settings[0]);
	if (!(data->settings))
	{
		printf("Invalid arguments!\n");
		free_data(&data);
		return (-1); //cleanup first
	}

	// Printing settings for debugging only
/*	i = 0;
	while (i < 5)
	{
		printf("Setting %d = %d\n", i + 1, settings[i]);
		++i;
	}
	printf("\n"); */

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
	
	i = 0;
	while (i < data->settings->number_of_philosophers)
	{
		pthread_join(data->philos[i]->tid, (void **)&ptr);
		++i;
	}
	
	free_data(&data);
	return (0);
}

int	main(int argc, char **argv)
{
	return (run(argc, argv));
}
