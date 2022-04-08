/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyun-zhe <hyun-zhe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 10:43:42 by hyun-zhe          #+#    #+#             */
/*   Updated: 2022/03/21 10:33:28 by hyun-zhe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (!philo->data->all_initiated)
		;
	print_log(philo, THINK, BLUE);
	if (philo->id % 2 == 0)
		m_sleep(60);
	while (philo->data->all_alive && philo->data->all_not_full)
	{
		take_forks(philo);
		print_log(philo, EAT, GREEN);
		philo->last_eaten = get_current_time();
		m_sleep(philo->data->time_to_eat);
		philo->times_eaten++;
		put_forks(philo);
		print_log(philo, SLEEP, PURPLE);
		m_sleep(philo->data->time_to_sleep);
		print_log(philo, THINK, BLUE);
	}
	return (NULL);
}

void	terminate_philos(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_of_philos)
	{
		free(data->philos[i]);
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&data->microphone);
	free(data->philos);
	free(data->forks);
	free(data);
}

void	check_alive(t_data *data)
{
	int		i;
	int		all_full;
	long	current_time;

	while (data->all_alive && data->all_not_full)
	{
		i = 0;
		all_full = (data->times_must_eat > 0);
		current_time = get_current_time();
		while (data->all_alive && i < data->num_of_philos)
		{
			if (current_time - data->philos[i]->last_eaten > data->time_to_die)
			{
				print_log(data->philos[i], DIE, RED);
				data->all_alive = 0;
			}
			if (data->philos[i]->times_eaten < data->times_must_eat)
				all_full = 0;
			usleep(1);
			i++;
		}
		if (all_full)
			data->all_not_full = 0;
	}
}

int	philosophers(t_data *data)
{
	int		i;

	i = 0;
	while (i < data->num_of_philos)
	{
		if (pthread_create(&(data->philos[i]->tid), NULL,
				philo_routine, data->philos[i]) != 0)
			return (1);
		i++;
	}
	data->all_initiated = 1;
	data->start_time = get_current_time();
	check_alive(data);
	i = 0;
	while (i < data->num_of_philos)
	{
		pthread_join(data->philos[i]->tid, NULL);
		i++;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	*data;

	if (argc < 5 || argc > 6)
		return (print_error("philo: invalid number of arguments...\n", 1));
	data = get_data(argc, argv);
	if (data == NULL)
		return (print_error("philo: invalid input...\n", 1));
	init_philos(data);
	if (philosophers(data) == 1)
		print_error("philo: failed to create thread...\n", 0);
	terminate_philos(data);
	return (0);
}
