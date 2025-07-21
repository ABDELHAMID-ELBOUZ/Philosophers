/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   creat_thread_helper.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdelhamid <abdelhamid@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:45:32 by abdelhamid        #+#    #+#             */
/*   Updated: 2025/07/20 13:27:59 by abdelhamid       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		print_action(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		print_action(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		print_action(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_action(philo, "has taken a fork");
	}
}

void	philo_state(t_philo *philo)
{
	print_action(philo, "is eating");
	pthread_mutex_lock(&philo->data->print_mutex);
	philo->last_meal_time = get_current_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->data->print_mutex);
	ft_usleep(philo->data->time_to_eat, philo->data);
	if (philo->id % 2 == 0)
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
	}
	else
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
	}
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	philo->last_meal_time = get_current_time();
	if (philo->id % 2)
		usleep(1);
	while (!data->someone_died)
	{
		take_forks(philo);
		philo_state(philo);
		if (data->someone_died)
			break ;
		if (data->meals_needed > 0 && philo->meals_eaten >= data->meals_needed)
			break ;
		print_action(philo, "is sleeping");
		ft_usleep(data->time_to_sleep, data);
		if (data->someone_died)
			break ;
		print_action(philo, "is thinking");
	}
	return (NULL);
}

int	did_anyone_die(t_data *data, int i)
{
	long	current_time;

	pthread_mutex_lock(&data->print_mutex);
	current_time = get_current_time();
	if ((current_time - data->philos[i].last_meal_time) >= data->time_to_die)
	{
		pthread_mutex_lock(&data->death_mutex);
		data->someone_died = 1;
		printf("%ld %d died\n", \
current_time - data->start_time, data->philos[i].id);
		pthread_mutex_unlock(&data->death_mutex);
		pthread_mutex_unlock(&data->print_mutex);
		return (1);
	}
	pthread_mutex_unlock(&data->print_mutex);
	return (0);
}

int	check_everyone_full(t_data *data)
{
	int	i;
	int	everyone_ate;

	everyone_ate = 1;
	pthread_mutex_lock(&data->print_mutex);
	i = 0;
	while (i < data->num_philos)
	{
		if (data->philos[i].meals_eaten < data->meals_needed)
		{
			everyone_ate = 0;
			break ;
		}
		i++;
	}
	if (everyone_ate)
	{
		pthread_mutex_lock(&data->death_mutex);
		data->someone_died = 1;
		pthread_mutex_unlock(&data->death_mutex);
		pthread_mutex_unlock(&data->print_mutex);
		return (1);
	}
	pthread_mutex_unlock(&data->print_mutex);
	return (0);
}
