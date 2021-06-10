#ifndef PHILO_H
# define PHILO_H

typedef struct	s_settings
{
	int	number_of_philosophers;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	number_of_times_each_philosopher_must_eat;
}	t_settings;

typedef struct	s_data
{
	int playing;
	pthread_mutex_t taking_forks;
	pthread_mutex_t	*forks;
	t_settings	*settings;
}	t_data;

typedef struct	s_philo
{
	int	id;
	pthread_t tid;
	pthread_mutex_t *left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*taking_forks;
}	t_philo;

#endif
