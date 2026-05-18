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

typedef struct s_philot_philo;

/*
** STRUCT: s_rules
** PURPOSE: Store the simulation settings and shared synchronization tools.
** WHY IT EXISTS: Threads need a single place to read global rules safely.
*/
typedef struct s_rules
{
intphilo_count;
longtime_die;
longtime_eat;
longtime_sleep;
intmust_eat;
longstart_time;
intstop;
pthread_mutex_tstate_mutex;
pthread_mutex_twrite_mutex;
pthread_mutex_t*forks;
t_philo*philos;
}t_rules;

/*
** STRUCT: s_philo
** PURPOSE: Store per-philosopher data and references to shared resources.
** WHY IT EXISTS: Each thread needs its own state to act independently.
*/
typedef struct s_philo
{
intid;
pthread_tthread;
pthread_mutex_tmeal_mutex;
longlast_meal;
intmeals_eaten;
pthread_mutex_t*left_fork;
pthread_mutex_t*right_fork;
t_rules*rules;
}t_philo;

intinit_rules(t_rules *rules, int argc, char **argv);
void*philo_routine(void *arg);
intmonitor_simulation(t_rules *rules);
longget_time_ms(void);
intprecise_sleep(t_rules *rules, long duration);
intparse_positive(const char *str, long *out);
intsimulation_stopped(t_rules *rules);
intprint_state(t_philo *philo, const char *msg, int force);

#endif
