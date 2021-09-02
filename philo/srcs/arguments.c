/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arguments.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/02 13:25:13 by tmorris           #+#    #+#             */
/*   Updated: 2021/09/02 13:25:15 by tmorris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	parse_argument(char *arg)
{
	int			i;
	long int	value;

	if (!arg || !arg[0])
		return (-1);
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

int	get_settings(int argc, char **argv, t_data *data)
{
	int			i;
	int			settings_array[5];

	if (argc < 5 || argc > 6)
		return (-1);
	settings_array[4] = -1;
	i = 1;
	while (argc > 1 && i < argc)
	{
		settings_array[i - 1] = parse_argument(argv[i]);
		if (settings_array[i - 1] < 0)
			return (-1);
		++i;
	}
	data->num_of_philos = settings_array[0];
	data->time_to_die = settings_array[1];
	data->time_to_eat = settings_array[2];
	data->time_to_sleep = settings_array[3];
	data->number_eats = settings_array[4];
	return (0);
}

int	usage(t_data **data)
{
	int	i;

	i = 50;
	write(2, "Usage: ./philo number_of_philosophers time_to_die ", i);
	i = 26;
	write(2, "time_to_eat time_to_sleep ", i);
	i = 44;
	write(2, "[number_of_times_each_philosopher_must_eat]\n", i);
	data_free(data);
	return (-1);
}
