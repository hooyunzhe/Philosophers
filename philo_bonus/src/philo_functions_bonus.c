/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_functions_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyun-zhe <hyun-zhe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 09:55:09 by hyun-zhe          #+#    #+#             */
/*   Updated: 2022/06/07 10:43:53 by hyun-zhe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

t_data	*get_data(int argc, char **argv)
{
	t_data	*data;

	if (!check_input(argc, argv))
		return (NULL);
	data = malloc(sizeof(t_data));
	data->num_of_philos = ft_atoi(argv[1]);
	data->philos = malloc(data->num_of_philos * sizeof(t_philo *));
	sem_unlink("/forks");
	sem_unlink("/microphone");
	sem_unlink("/sync");
	sem_unlink("/end_process");
	sem_unlink("/all_full");
	data->forks = sem_open("/forks", O_CREAT, 0644, data->num_of_philos);
	data->microphone = sem_open("/microphone", O_CREAT, 0644, 1);
	data->sync = sem_open("/sync", O_CREAT, 0644, 0);
	data->end_process = sem_open("/end_process", O_CREAT, 0644, 0);
	data->all_full = sem_open("/all_full", O_CREAT, 0644, 0);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->times_must_eat = 0;
	if (argc == 6)
		data->times_must_eat = ft_atoi(argv[5]);
	return (data);
}

void	init_philos(t_data *data)
{
	int		i;

	i = 0;
	while (i < data->num_of_philos)
	{
		data->philos[i] = malloc(sizeof(t_philo));
		data->philos[i]->id = i + 1;
		data->philos[i]->times_eaten = 0;
		data->philos[i]->last_eaten = get_current_time();
		data->philos[i]->data = data;
		i++;
	}
}

void	get_fork(t_philo *philo)
{
	sem_wait(philo->data->forks);
	print_log(philo, FORK, YELLOW);
}

void	*check_full(void *arg)
{
	int		i;
	t_data	*data;

	i = 0;
	data = (t_data *)arg;
	while (i < data->num_of_philos)
	{
		sem_wait(data->all_full);
		i++;
	}
	if (data->times_must_eat > 0)
		sem_post(data->end_process);
	return (NULL);
}

void	*check_death(void *arg)
{
	t_philo	*philo;
	long	current_time;

	philo = (t_philo *)arg;
	m_sleep(1000);
	while (1)
	{
		current_time = get_current_time();
		if ((current_time - philo->last_eaten) > philo->data->time_to_die)
		{
			print_log(philo, DIE, RED);
			sem_post(philo->data->end_process);
			return (NULL);
		}
		usleep(1);
	}
}
