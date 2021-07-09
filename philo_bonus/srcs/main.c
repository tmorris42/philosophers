#include "philo.h"

int	main(int argc, char **argv)
{
	t_data	*data;

	data = data_init();
	if (!data)
		return (-1);
	if (get_settings(argc, argv, data) < 0)
		return (usage(&data));
	if (create_forks(data) < 0)
		return (-1);
	if (create_philos(data) < 0)
		return (-1);
	if (create_threads(data) < 0)
		return (-1);
	while (data->playing)
		check_end_conditions(data);
	rejoin_threads(data);
	data_free(&data);
	return (0);
}