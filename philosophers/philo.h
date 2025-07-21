/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdelhamid <abdelhamid@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 15:07:52 by abdelhamid        #+#    #+#             */
/*   Updated: 2025/07/20 13:15:26 by abdelhamid       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long			last_meal_time;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	struct s_data	*data;
}	t_philo;

typedef struct s_data
{
	int				num_philos;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				meals_needed;
	long			start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	death_mutex;
	int				someone_died;
	t_philo			*philos;
	pthread_t		is_dead_thread;
}	t_data;

int		is_digit(const char *s);
long	ft_atol(const char *str);
long	get_current_time(void);
void	print_action(t_philo *philo, char *action);
int		parse_args(int ac, char **av, t_data *data);
int		init_all(t_data *data);
void	ft_usleep(size_t time, t_data *data);
void	monitor_philosophers(t_data *data);
int		create_philo_threads(t_data *data);
void	*philo_routine(void *arg);
int		did_anyone_die(t_data *data, int i);
int		check_everyone_full(t_data *data);
#endif