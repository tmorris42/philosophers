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
	int				playing;
	pthread_mutex_t	taking_forks;
	pthread_mutex_t	*forks;
	t_philo			**philos;
	long int		start_time;
	pthread_mutex_t	log_lock;
	int				num_of_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				number_eats;
}	t_data;
#endif
