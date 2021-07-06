#ifndef PHILO_H
# define PHILO_H

# ifndef SLEEP_INT
#  define SLEEP_INT 1
# endif

typedef struct s_philo
{
	int				id;
	pthread_t		tid;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	int				alive;
	struct s_data	*data;
	long int		start_time;
	long int		time_of_last_meal;
	int				times_eaten;
	pthread_mutex_t	lock;
}	t_philo;

typedef struct s_data
{
	pthread_mutex_t	log_lock;
	pthread_mutex_t	*forks;
	t_philo			**philos;
	int				playing;
	long int		start_time;
	int				num_of_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				number_eats;
}	t_data;

int	starving(t_philo *philo);

#endif
