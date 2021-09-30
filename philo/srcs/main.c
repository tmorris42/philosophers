/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/02 13:25:33 by tmorris           #+#    #+#             */
/*   Updated: 2021/09/30 10:50:55 by tmorris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	if (!(data->num_of_philos % 2) && create_threads_even(data) < 0)
		return (-1);
	if (data->num_of_philos % 2 && create_threads_odd(data) < 0)
		return (-1);
	while (get_playing(data))
		check_end_conditions(data);
	rejoin_threads(data);
	data_free(&data);
	return (0);
}
