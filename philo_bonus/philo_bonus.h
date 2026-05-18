/*
** FILE: philo_bonus.h
** PURPOSE: Declare shared structures and function prototypes for the bonus.
** KEY CONCEPTS: Processes, semaphores, shared configuration structs.
*/

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <fcntl.h>
# include <pthread.h>
# include <semaphore.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_rules
{
	int		philo_count;
	long	time_die;
	long	time_eat;
	long	time_sleep;
	int		must_eat;
	long	start_time;
	sem_t	*forks;
	sem_t	*write;
	pid_t	*pids;
}t_rules;

typedef struct s_philo
{
	int			id;
	long		last_meal;
	int			meals_eaten;
	pthread_mutex_t	meal_mutex;
	pthread_t		monitor;
	t_rules			*rules;
}t_philo;

int		init_rules(t_rules *rules, int argc, char **argv);
void	run_philo(t_rules *rules, int id);
void	*death_monitor(void *arg);
long	get_time_ms(void);
void	precise_sleep(long duration);
int		parse_positive(const char *str, long *out);
void	print_state(t_philo *philo, const char *msg);
void	close_sems(t_rules *rules);

# define SEM_FORKS "/philo_forks"
# define SEM_WRITE "/philo_write"

#endif
