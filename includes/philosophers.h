#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <sys/time.h>

typedef struct s_philo	t_philo;

typedef struct s_rules
{
	int		num_philo;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		must_eat_count;
	int		dead;
	long long	start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	t_philo		*philos;
}	t_rules;

typedef struct s_philo
{
	int		id;
	int		left_fork;
	int		right_fork;
	long long	last_meal;
	int		meals_eaten;
	pthread_t	thread;
	t_rules		*rules;
}	t_philo;

int			ft_atoi(char *str);
int			parse_args(int argc, char **argv, t_rules *rules);
int			init_simulation(t_rules *rules);
void		*philo_routine(void *arg);
long long	get_time_ms(void);
void		precise_usleep(long long time);
void		print_action(t_philo *philo, char *msg);
void		monitor_philos(t_rules *rules);
void		clean_all(t_rules *rules);
void		ft_putchar_fd(char c, int fd);
void		ft_putstr_fd(char *str, int fd);
void		ft_putnbr_fd(int nbr, int fd);

#endif
