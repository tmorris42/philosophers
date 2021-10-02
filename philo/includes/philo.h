/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/02 13:26:40 by tmorris           #+#    #+#             */
/*   Updated: 2021/10/02 17:18:08 by tmorris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>
# include <pthread.h>
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
	int				locks_ready;
}	t_data;

int			get_settings(int argc, char **argv, t_data *data);
int			usage(t_data *data);
int			data_free(t_data *data_ptr);
int			is_starving(t_philo *philo);
void		philo_eat(t_philo *philo);
void		ft_usleep(t_philo *philo, long int delay);
void		ft_write_log(long int delta_time, int id, char *msg);
int			ft_log(t_philo *philo, char *msg);
int			try_to_take_forks(t_philo *philo);
void		*philo_start(void *ptr);
long int	ft_now(void);
int			drop_forks(t_philo *philo);
t_philo		*philo_create(int id, t_data *data);
t_data		*data_init(void);
int			create_forks(t_data *data);
int			check_end_conditions(t_data *data);
void		rejoin_threads(t_philo *philos[], int i);
int			create_threads(t_data *data, int increment);
int			create_philos(t_data *data);
int			philo_get_alive(t_philo *philo);
int			philo_set_time_of_last_meal(t_philo *philo, long int time);
long int	philo_get_time_since_meal(t_philo *philo);
void		philo_add_times_eaten(t_philo *philo, int times);
int			philo_get_times_eaten(t_philo *philo);
int			philo_get_alive(t_philo *philo);
void		philo_set_alive(t_philo *philo, int alive);
#endif
