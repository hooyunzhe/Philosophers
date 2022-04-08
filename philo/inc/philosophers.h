#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include "libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

# define NO_COLOR "\033[0m"
# define BOLD "\033[1m"
# define ITALICS "\033[3m"
# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define YELLOW "\033[0;33m"
# define BLUE "\033[0;34m"
# define PURPLE "\033[0;35m"

# define FORK "has taken a fork"
# define EAT "is eating"
# define SLEEP "is sleeping"
# define THINK "is thinking"
# define DIE "died"

# define USAGE_1 "[num_of_philos > 0] [time_to_die >= 60] [time_to_eat >= 60]"
# define USAGE_2 " [time_to_sleep >= 60] [times_must_eat > 0]"

typedef struct s_philo
{
	int				id;
	pthread_t		tid;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	int				times_eaten;
	long			last_eaten;
	struct s_data	*data;
}					t_philo;

typedef struct s_data
{
	t_philo			**philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	microphone;
	long			start_time;
	int				all_initiated;
	int				all_alive;
	int				all_not_full;
	int				num_of_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				times_must_eat;
}		t_data;

long	get_current_time(void);
void	m_sleep(int ms);
int		check_input(int argc, char **argv);
int		print_error(char *str, int usage);
t_data	*get_data(int argc, char **argv);
void	init_philos(t_data *data);
void	take_forks(t_philo *philo);
void	put_forks(t_philo *philo);
void	print_log(t_philo *philo, char *message, char *color);

#endif