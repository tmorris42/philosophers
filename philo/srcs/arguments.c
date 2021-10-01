/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arguments.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/02 13:25:13 by tmorris           #+#    #+#             */
/*   Updated: 2021/10/01 16:16:31 by tmorris          ###   ########.fr       */
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

int	ft_error(char *msg)
{
	int	len;

	len = 0;
	while (msg && msg[len])
		++len;
	if (len)
		write(2, msg, len);
	return (-1);
}

int	get_settings(int argc, char **argv, t_data *data)
{
	int			i;
	int			settings_array[5];

	if (argc < 5 || argc > 6)
		return (ft_error("Error: wrong number of arguments\n"));
	settings_array[4] = -1;
	i = 1;
	while (argc > 1 && i < argc)
	{
		settings_array[i - 1] = parse_argument(argv[i]);
		if (settings_array[i - 1] < 0)
			return (ft_error("Error: arguments must be positive integers\n"));
		++i;
	}
	data->num_of_philos = settings_array[0];
	if (data->num_of_philos > 200)
		return (ft_error("Error: number of philosophers must not exceed 200\n"));
	data->time_to_die = settings_array[1];
	data->time_to_eat = settings_array[2];
	data->time_to_sleep = settings_array[3];
	data->number_eats = settings_array[4];
	return (0);
}

int	usage(t_data *data)
{
	ft_error("Usage: ./philo <number_of_philosophers> <time_to_die> ");
	ft_error("<time_to_eat> <time_to_sleep> ");
	ft_error("[number_of_times_each_philosopher_must_eat]\n");
	data_free(data);
	return (-1);
}
