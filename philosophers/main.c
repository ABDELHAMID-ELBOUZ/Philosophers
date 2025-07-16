/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdelhamid <abdelhamid@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 08:57:32 by abdelhamid        #+#    #+#             */
/*   Updated: 2025/07/16 15:04:33 by abdelhamid       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	destroy_mutexes(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->death_mutex);
}

void	cleanup(t_data *data)
{
	destroy_mutexes(data);
	free(data->philos);
	free(data->forks);
}

void	handle_single_philo(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	print_action(philo, "has taken a fork");
	ft_usleep(data->time_to_die, data);
	print_action(philo, "died");
}

void	start_simulation(t_data *data)
{
	int	i;

	if (create_philo_threads(data))
	{
		cleanup(data);
		exit(1);
	}
	monitor_philosophers(data);
	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_join(data->philos[i].thread, NULL) != 0)
			printf("Error\n");
		i++;
	}
}

int	main(int ac, char **av)
{
	t_data	data;

	if (parse_args(ac, av, &data))
		return (1);
	if (data.num_philos == 1)
	{
		if (init_program(&data))
		{
			cleanup(&data);
			return (1);
		}
		handle_single_philo(&data.philos[0]);
		cleanup(&data);
		return (0);
	}
	if (init_program(&data) == 1)
	{
		cleanup(&data);
		return (1);
	}
	start_simulation(&data);
	cleanup(&data);
}
