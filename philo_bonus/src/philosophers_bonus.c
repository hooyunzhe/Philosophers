/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyun-zhe <hyun-zhe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 10:43:42 by hyun-zhe          #+#    #+#             */
/*   Updated: 2022/03/21 10:09:04 by hyun-zhe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

void	philo_process(t_philo *philo)
{
	sem_wait(philo->data->sync);
	print_log(philo, THINK, BLUE);
	pthread_create(&philo->death_checker, NULL, check_death, philo);
	pthread_detach(philo->death_checker);
	if (philo->id % 2 == 0)
		m_sleep(60);
	while (1)
	{
		get_fork(philo);
		while (philo->data->num_of_philos == 1)
			;
		get_fork(philo);
		print_log(philo, EAT, GREEN);
		philo->last_eaten = get_current_time();
		m_sleep(philo->data->time_to_eat);
		print_log(philo, SLEEP, PURPLE);
		philo->times_eaten++;
		if (philo->times_eaten == philo->data->times_must_eat)
			sem_post(philo->data->all_full);
		sem_post(philo->data->forks);
		sem_post(philo->data->forks);
		m_sleep(philo->data->time_to_sleep);
		print_log(philo, THINK, BLUE);
	}
	exit(0);
}

void	terminate_all_philos(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_of_philos)
	{
		kill(data->philos[i]->pid, SIGTERM);
		sem_post(data->all_full);
		i++;
	}
}

void	terminate_philos(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_of_philos)
	{
		free(data->philos[i]);
		i++;
	}
	sem_close(data->forks);
	sem_close(data->microphone);
	sem_close(data->sync);
	sem_close(data->end_process);
	sem_close(data->all_full);
	sem_unlink("/forks");
	sem_unlink("/microphone");
	sem_unlink("/sync");
	sem_unlink("/end_process");
	sem_unlink("/all_full");
	free(data->philos);
	free(data);
}

int	philosophers(t_data *data)
{
	int	i;

	i = 0;
	data->start_time = get_current_time();
	while (i < data->num_of_philos)
	{
		data->philos[i]->pid = fork();
		if (data->philos[i]->pid < 0)
			return (1);
		else if (data->philos[i]->pid == 0)
			philo_process(data->philos[i]);
		i++;
	}
	i = 0;
	while (i < data->num_of_philos)
	{
		sem_post(data->sync);
		i++;
	}
	pthread_create(&data->full_checker, NULL, check_full, data);
	pthread_detach(data->full_checker);
	sem_wait(data->end_process);
	terminate_all_philos(data);
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
		print_error("philo: failed to create subprocess...\n", 0);
	terminate_philos(data);
	return (0);
}
