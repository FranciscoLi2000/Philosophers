/*
** FILE: philo.h
** PURPOSE: Declare shared structures and function prototypes for the program.
** KEY CONCEPTS: Header organization, shared state structs, compilation units.
*/

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_philo		t_philo;

/*
** STRUCT: s_rules
** PURPOSE: Store the simulation settings and shared synchronization tools.
** WHY IT EXISTS: Threads need a single place to read global rules safely.
*/
typedef struct s_rules
{
	int				philo_count;
	long			time_die;
	long			time_eat;
	long			time_sleep;
	int				must_eat;
	long			start_time;
	int				stop;
	pthread_mutex_t	state_mutex;
	pthread_mutex_t	write_mutex;
	pthread_mutex_t	*forks;
	t_philo			*philos;
} t_rules;

/*
** STRUCT: s_philo
** PURPOSE: Store per-philosopher data and references to shared resources.
** WHY IT EXISTS: Each thread needs its own state to act independently.
*/
typedef struct s_philo
{
	int				id;
	pthread_t		thread;
	pthread_mutex_t	meal_mutex;
	long			last_meal;
	int				meals_eaten;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_rules			*rules;
} t_philo;

int		init_rules(t_rules *rules, int argc, char **argv);
void	*philo_routine(void *arg);
int		monitor_simulation(t_rules *rules);
long	get_time_ms(void);
int		precise_sleep(t_rules *rules, long duration);
int		parse_positive(const char *str, long *out);
int		simulation_stopped(t_rules *rules);
int		print_state(t_philo *philo, const char *msg, int force);

#endif
