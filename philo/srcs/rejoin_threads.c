/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rejoin_threads.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/01 13:04:01 by tmorris           #+#    #+#             */
/*   Updated: 2021/10/01 13:04:19 by tmorris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	rejoin_threads(t_philo *philos[], int i)
{
	while (i >= 0)
	{
		pthread_join(philos[i]->tid, NULL);
		--i;
	}
}
