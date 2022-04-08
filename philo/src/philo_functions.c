/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyun-zhe <hyun-zhe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 09:43:12 by hyun-zhe          #+#    #+#             */
/*   Updated: 2022/03/21 09:51:40 by hyun-zhe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_data	*get_data(int argc, char **argv)
{
	t_data	*data;

	if (!check_input(argc, argv))
		return (NULL);
	data = malloc(sizeof(t_data));
	data->num_of_philos = ft_atoi(argv[1]);
	data->philos = malloc(data->num_of_philos * sizeof(t_philo *));
	data->forks = malloc(data->num_of_philos * sizeof(pthread_mutex_t));
	data->all_initiated = 0;
	data->all_alive = 1;
	data->all_not_full = 1;
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->times_must_eat = 0;
	if (argc == 6)
		data->times_must_eat = ft_atoi(argv[5]);
	pthread_mutex_init(&data->microphone, NULL);
	return (data);
}

void	init_philos(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_of_philos)
	{
		data->philos[i] = malloc(sizeof(t_philo));
		data->philos[i]->id = i + 1;
		data->philos[i]->times_eaten = 0;
		data->philos[i]->last_eaten = get_current_time();
		pthread_mutex_init(&(data->forks[i]), NULL);
		if (i > 0)
		{
			data->philos[i - 1]->left_fork = &data->forks[i - 1];
			data->philos[i - 1]->right_fork = &data->forks[i];
		}
		data->philos[i]->data = data;
		i++;
	}
	data->philos[i - 1]->left_fork = &data->forks[i - 1];
	data->philos[i - 1]->right_fork = &data->forks[0];
}

void	take_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_log(philo, FORK, YELLOW);
	if (philo->data->num_of_philos > 1)
		pthread_mutex_lock(philo->right_fork);
	else
	{
		while (philo->data->all_alive)
			;
	}
	print_log(philo, FORK, YELLOW);
}

void	put_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	print_log(t_philo *philo, char *message, char *color)
{
	if (philo->data->all_alive && philo->data->all_not_full)
	{
		pthread_mutex_lock(&philo->data->microphone);
		printf("%s", NO_COLOR);
		printf("%s", ITALICS);
		printf("%ld ", get_current_time() - philo->data->start_time);
		printf("%s%d %s\n", color, philo->id, message);
		if (*message != 'd')
			pthread_mutex_unlock(&philo->data->microphone);
	}
}
