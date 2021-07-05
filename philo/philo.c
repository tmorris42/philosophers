#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include "philo.h"

# ifndef SLEEP_INT
#  define SLEEP_INT 1
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

//	pthread_mutex_lock(&(philo->lock));
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
//	pthread_mutex_unlock(&(philo->lock));
}

void	kill_philo(t_data *data, t_philo *philo)
{
	long int		delta_time;
	struct timeval	time;

//	pthread_mutex_lock(&(philo->lock));
	
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
//	pthread_mutex_unlock(&(philo->lock));
}

int	starving(t_philo *philo)
{
	int	dead;

	dead = (now_int() - philo->time_of_last_meal > (long int)philo->data->settings->time_to_die);
//	if (philo->alive && dead)
	{
//		philo->alive = 0;
//		philo->data->playing = 0;
	}
	return (dead);
}

int	drop_fork(t_fork *fork)
{
	fork->available = 1;
	pthread_mutex_unlock(&(fork->lock));
	return (1);
}

int	drop_forks(t_philo *philo)
{
	drop_fork(philo->left_fork);
	drop_fork(philo->right_fork);
	return (1);
}

int	take_fork(t_philo *philo, t_fork *fork)
{
	pthread_mutex_lock(&(philo->lock));
	pthread_mutex_lock(&(fork->lock));
	if (!(philo->alive))
	{
		pthread_mutex_unlock(&(fork->lock));
		return (0);
	}
	fork->available = 0;
	ft_log(philo, "has taken a fork");
	pthread_mutex_unlock(&(philo->lock));
	return (1);
}

void	*start_philo(void *p)
{
	t_philo		*philo;
	long int	timer;

	philo = (t_philo *)p;
	philo->start_time = now_int();
	philo->time_of_last_meal = philo->start_time;
	while (philo && philo->alive && philo->data->playing)
	{
//		if (philo->data->playing && starving(philo))
//			philo->alive = 0;
		if (philo->left_fork == philo->right_fork)
		{
			take_fork(philo, philo->left_fork);
			while (philo->alive)
				usleep(1);
			drop_fork(philo->left_fork);
			continue ;
		}
//			continue ;
//		pthread_mutex_lock(&(philo->data->taking_forks));
//		if (philo->left_fork->available && philo->right_fork->available)
//		{
		if (philo->id % 2)
		{
			take_fork(philo, philo->left_fork);
			if (philo->alive)
				take_fork(philo, philo->right_fork);
			else
			{
				drop_fork(philo->left_fork);
				continue ;
			}
		}
		else
		{
			take_fork(philo, philo->right_fork);
			if (philo->alive)
				take_fork(philo, philo->left_fork);
			else
			{
				drop_fork(philo->right_fork);
				continue ;
			}
		}
//			pthread_mutex_unlock(&(philo->data->taking_forks));
//		}
//		else
//		{
//			pthread_mutex_unlock(&(philo->data->taking_forks));
//			usleep(SLEEP_INT);
//			continue ;
//		}
/*		if (philo->id % 2)
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
		*/
//		if (philo->data->playing && !starving(philo))
		pthread_mutex_lock(&(philo->lock));
		if (philo->data->playing && philo->alive && !starving(philo))
		{
			philo->time_of_last_meal = now_int();
			ft_log(philo, "is eating");
			pthread_mutex_unlock(&(philo->lock));
			//eating here
			timer = now_int() + philo->data->settings->time_to_eat;
			ft_usleep(timer);
//			while (philo->data->playing && !starving(philo) && now_int() < timer)
//			{
//				ft_usleep(SLEEP_INT);
//			}
			philo->times_eaten += 1;
//			ft_log(philo, "is done eating");
		}
		else
			pthread_mutex_unlock(&(philo->lock));

		drop_forks(philo);
//		ft_log(philo, "dropped forks");
//		pthread_mutex_unlock(&(philo->left_fork->lock));
//		pthread_mutex_unlock(&(philo->right_fork->lock));

		pthread_mutex_lock(&(philo->lock));
		if (philo->data->playing && philo->alive)
		{
			// Sleeping
			ft_log(philo, "is sleeping");
			pthread_mutex_unlock(&(philo->lock));
			timer = now_int() + philo->data->settings->time_to_sleep;
			ft_usleep(timer);
//			while (philo->data->playing && !starving(philo) && now_int() < timer)
//			{
//				usleep(SLEEP_INT);
//			}
		}
		else
			pthread_mutex_unlock(&(philo->lock));
//		ft_log(philo, "is done sleeping");
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
		philo->right_fork = &data->forks[data->settings->number_of_philosophers - 1];
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
			pthread_mutex_destroy(&data->philos[i]->lock);
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
	int		done;
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
		write(2, "Usage: ./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n", 120);
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
	data->start_time = now_int();
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
		done = 0;
		while (i < data->settings->number_of_philosophers) //check also for everyone is full
		{
			pthread_mutex_lock(&(data->philos[i]->lock));
			if (starving(data->philos[i]))
			{
				//kill philo
//				pthread_detach(data->philos[i]->tid);
				kill_philo(data, data->philos[i]);
			}
			pthread_mutex_unlock(&(data->philos[i]->lock));
			if (data->settings->number_of_times_each_philosopher_must_eat > -1 && data->philos[i]->times_eaten >= data->settings->number_of_times_each_philosopher_must_eat)
				done += 1;
			++i;
		}
		if (done == data->settings->number_of_philosophers)
		{
			data->playing = 0;
//			printf("Everyone has eaten enough!!\n"); //
		}
		usleep(SLEEP_INT);
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
	return (ret);
}
