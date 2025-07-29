/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   creat_thread.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 10:17:14 by aelbouz           #+#    #+#             */
/*   Updated: 2025/07/25 14:58:35 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_is_someone_died(t_data	*data)
{
	int	result;

	pthread_mutex_lock(&data->death_mutex);
	result = data->someone_died;
	pthread_mutex_unlock(&data->death_mutex);
	return (result);
}

void	ft_usleep(size_t time, t_data *data)
{
	size_t	start;

	start = get_current_time();
	while ((get_current_time() - start) < time)
	{
		pthread_mutex_lock(&data->death_mutex);
		if (data->someone_died)
		{
			pthread_mutex_unlock(&data->death_mutex);
			return ;
		}
		pthread_mutex_unlock(&data->death_mutex);
		usleep(10);
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
	t_data	*data;

	data = arg;
	monitor_philosophers(data);
	return (0);
}

int	create_philo_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_create(&data->philos[i].thread, \
		NULL, philo_routine, &data->philos[i]) != 0)
		{
			printf("Error\n");
			return (1);
		}
		if (data->num_philos > 1)
			usleep(10);
		i++;
	}
	if (pthread_create(&data->is_dead_thread, NULL, thread_is_dead, data) != 0)
	{
		printf("Error\n");
		return (1);
	}
	return (0);
}
