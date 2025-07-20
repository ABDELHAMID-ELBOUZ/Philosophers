/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   creat_thread.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdelhamid <abdelhamid@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:47:12 by abdelhamid        #+#    #+#             */
/*   Updated: 2025/07/18 10:53:42 by abdelhamid       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_usleep(size_t time, t_data *data)
{
	size_t	start;

	start = get_current_time();
	while ((get_current_time() - start) < time)
	{
		if (data->someone_died)
			return ;
		usleep(1);
	}
}

void	monitor_philosophers(t_data *data)
{
	int	i;

	while (1)
	{
		i = 0;
		while (i < data->num_philos)
		{
			if (did_anyone_die(data, i))
				return ;
			i++;
		}
		if (data->meals_needed > 0 && check_everyone_full(data))
			return ;
		usleep(500);
	}
	return ;
}

void	*thread_is_dead(void *arg)
{
	t_philo	*philo;

	philo = arg;
	monitor_philosophers(philo->data);
	return (0);
}

int	create_philo_threads(t_data *data)
{
	int	i;
	pthread_t is_dead_thread;

	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_create(&data->philos[i].thread, \
NULL, philo_routine, &data->philos[i]) != 0)
		{
			printf("Error\n");
			return (1);
		}
		if (pthread_create(&is_dead_thread, NULL, thread_is_dead, &data->philos) != 0)
		{
			printf("Error\n");
			return (1);
		}
		if (data->num_philos > 1)
			usleep(3);
		i++;
	}
	return (0);
}
