/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyun-zhe <hyun-zhe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 10:41:24 by hyun-zhe          #+#    #+#             */
/*   Updated: 2022/03/21 10:34:17 by hyun-zhe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long	get_current_time(void)
{
	struct timeval	c_time;
	long			m_time;

	gettimeofday(&c_time, NULL);
	m_time = c_time.tv_sec * 1000 + c_time.tv_usec / 1000;
	return (m_time);
}

void	m_sleep(int ms)
{
	long	start_time;

	start_time = get_current_time();
	while (get_current_time() - start_time < ms)
		usleep(100);
}

int	check_input(int argc, char **argv)
{
	if (!ft_isnum(argv[1]) || !ft_isnum(argv[2]) || !ft_isnum(argv[3])
		|| !ft_isnum(argv[4]) || (argc == 6 && !ft_isnum(argv[5])))
		return (0);
	if (ft_atoi(argv[1]) < 1 || ft_atoi(argv[2]) < 60 || ft_atoi(argv[3]) < 60
		 || ft_atoi(argv[4]) < 60 || (argc == 6 && ft_atoi(argv[5]) < 1))
		return (0);
	return (1);
}

int	print_error(char *str, int usage)
{
	if (str)
		printf("%s%s", RED, str);
	else
		perror("philo:");
	if (usage)
		printf("%susage: %s%s%s\n", GREEN, ITALICS, USAGE_1, USAGE_2);
	return (-1);
}
