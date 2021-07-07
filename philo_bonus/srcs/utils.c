#include "philo.h"

long int	timeval_to_long_int(struct timeval time)
{
	return ((long int)time.tv_sec * 1000 + (long int)time.tv_usec / 1000);
}

long int	ft_now(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (timeval_to_long_int(time));
}

void	ft_usleep(long int finish)
{
	while (ft_now() < finish)
	{
		usleep(SLEEP_INT);
	}
}

void	ft_log(t_philo *philo, char *msg)
{
	long int		delta_time;

	pthread_mutex_lock(&(philo->data->log_lock));
	if (philo->data->playing && philo->alive)
	{
		delta_time = ft_now() - philo->start_time;
		printf("%.11ld", delta_time);
		printf(" %d %s\n", philo->id + 1, msg);
	}
	pthread_mutex_unlock(&(philo->data->log_lock));
}
