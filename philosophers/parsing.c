/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 10:18:09 by aelbouz           #+#    #+#             */
/*   Updated: 2025/07/22 10:18:12 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_valid_arg(char **av)
{
	int	i;

	i = 1;
	while (av[i])
	{
		if (is_digit(av[i]) == 1)
			return (1);
		i++;
	}
	return (0);
}

int	parse_args(int ac, char **av, t_data *data)
{
	if (ac < 5 || ac > 6)
		return (printf("Error\n"), 1);
	if (is_valid_arg(av) == 1)
		return (printf("Error\n"), 1);
	data->num_philos = ft_atol(av[1]);
	data->time_to_die = ft_atol(av[2]);
	data->time_to_eat = ft_atol(av[3]);
	data->time_to_sleep = ft_atol(av[4]);
	if (ac == 6)
		data->meals_needed = ft_atol(av[5]);
	else
		data->meals_needed = -1;
	if (data->num_philos <= 0 || data->time_to_die <= 0 || \
	data->time_to_eat <= 0 || data->time_to_sleep <= 0 || \
	(ac == 6 && data->meals_needed <= 0))
	{
		printf("Error: Invalid argument values\n");
		return (1);
	}
	return (0);
}

int	init_data(t_data *data)
{
	int	i;

	data->philos = malloc(sizeof(t_philo) * data->num_philos);
	if (!data->philos)
		return (printf("Error\n"), 1);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
	if (!data->forks)
	{
		free(data->philos);
		return (printf("Error\n"), 1);
	}
	data->someone_died = 0;
	i = 0;
	while (i < data->num_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].meals_eaten = 0;
		data->philos[i].last_meal_time = 0;
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->num_philos];
		data->philos[i].data = data;
		i++;
	}
	data->start_time = get_current_time();
	return (0);
}

int	init_mutexes(t_data *data)
{
	int	i;

	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
		return (printf("Error\n"), 1);
	if (pthread_mutex_init(&data->death_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&data->print_mutex);
		return (printf("Error\n"), 1);
	}
	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&data->forks[i]);
			pthread_mutex_destroy(&data->print_mutex);
			pthread_mutex_destroy(&data->death_mutex);
			return (printf("Error\n"), 1);
		}
		i++;
	}
	return (0);
}

int	init_all(t_data *data)
{
	if (init_data(data) == 1)
		return (1);
	if (init_mutexes(data) == 1)
		return (1);
	return (0);
}
