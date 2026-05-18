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
intphilo_count;
longtime_die;
longtime_eat;
longtime_sleep;
intmust_eat;
longstart_time;
sem_t*forks;
sem_t*write;
pid_t*pids;
}t_rules;

typedef struct s_philo
{
intid;
longlast_meal;
intmeals_eaten;
pthread_mutex_tmeal_mutex;
pthread_tmonitor;
t_rules*rules;
}t_philo;

intinit_rules(t_rules *rules, int argc, char **argv);
voidrun_philo(t_rules *rules, int id);
void*death_monitor(void *arg);
longget_time_ms(void);
voidprecise_sleep(long duration);
intparse_positive(const char *str, long *out);
voidprint_state(t_philo *philo, const char *msg);
voidclose_sems(t_rules *rules);

# define SEM_FORKS "/philo_forks"
# define SEM_WRITE "/philo_write"

#endif
