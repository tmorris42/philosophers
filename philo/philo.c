#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "philo.h"

void	*start_philo(void *p)
{
	t_philo *philo;

	philo = (t_philo *)p;

	printf("Philosopher #%d wants to take forks\n", philo->id);
	pthread_mutex_lock(philo->taking_forks);
	printf("Philosopher #%d is taking forks\n", philo->id);
	printf("Philosopher #%d is about to release the forks\n", philo->id);
	pthread_mutex_unlock(philo->taking_forks);
	printf("Philosopher #%d has already released the forks\n", philo->id);
	pthread_exit(p);
}

t_philo	*create_philo(int id, pthread_mutex_t *forks, t_data *data)
{
	t_philo	*philo;

	if (!data)
		return (NULL);
	if (!forks)
		printf("No forks!\n");

	philo = (t_philo *)malloc(sizeof(*philo));
	if (!philo)
		return (NULL);

	philo->id = id;
	philo->left_fork = NULL;
	philo->right_fork = NULL;
	philo->taking_forks = &(data->taking_forks);
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
		printf("Arg #%d = \"%s\"\n", i, argv[i]);
		value = parse_argument(argv[i]);
		if (value < 0)
		{
			printf("\tInvalid argument\n");
			return (NULL);
		}
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

t_data	*init_data()
{
	t_data	*data;

	data = (t_data *)malloc(sizeof(*data));
	if (!data)
		return (NULL);
	data->settings = NULL;
	data->forks = NULL; //should be an array of mutex entries for the forks
	if (pthread_mutex_init(&data->taking_forks, NULL))
	{
		free(data);
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
	t_philo *philo_one;
	t_philo *philo_two;

	data = init_data();
	if (!data)
		return (-1); //cleanup first
	data->settings = get_settings(argc, argv, &settings[0]);
	if (!(data->settings))
		return (-1); //cleanup first

	i = 0;
	while (i < 5)
	{
		printf("Setting %d = %d\n", i + 1, settings[i]);
		++i;
	}

	philo_one = create_philo(1, NULL, data);
	if (!philo_one)
		return (-1); //cleanup first
	err = pthread_create(&philo_one->tid, NULL, start_philo, philo_one);
	if (err)
	{
		printf("pthread_create failed : %s\n", strerror(err));
		return (-1);
	}
	printf("Created thread with id=%d\n", philo_one->id);
	
	philo_two = create_philo(2, NULL, data);
	if (!philo_two)
		return (-1); //cleanup first
	err = pthread_create(&philo_two->tid, NULL, start_philo, philo_two);
	if (err)
	{
		printf("pthread_create failed : %s\n", strerror(err));
		return (-1);
	}
	printf("Created thread with id=%d\n", philo_two->id);
	
	pthread_join(philo_one->tid, (void **)&ptr);
	pthread_join(philo_two->tid, (void **)&ptr);

	printf("%i\n", *ptr);

	return (0);
}

int	main(int argc, char **argv)
{
	return (run(argc, argv));
}
