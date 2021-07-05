#ifndef PHILO_H
# define PHILO_H

# ifndef SLEEP_INT
#  define SLEEP_INT 1
# endif

typedef struct s_settings
{
	int	num_of_philos;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	number_eats;
}				t_settings;

typedef struct s_fork
{
	int				id;
	pthread_mutex_t	lock;
	int				available;
}				t_fork;

typedef struct s_philo
{
	int				id;
	pthread_t		tid;
	t_fork			*left_fork;
	t_fork			*right_fork;
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
	t_fork			*forks;
	t_settings		*settings;
	t_philo			**philos;
	long int		start_time;
	pthread_mutex_t	log_lock;
}	t_data;
#endif
