/*
** FILE: monitor.c
** PURPOSE: Detect philosopher death inside each process.
** KEY CONCEPTS: Monitoring threads, timing checks, protected state.
*/

#include "philo_bonus.h"

/*
** FUNCTION: announce_death
** PURPOSE: Print the death message and block further output.
** PARAMETERS:
**   - philo: philosopher who died.
**   - now: current time in milliseconds.
** RETURNS: Nothing (void)
** WHY IT EXISTS: Centralizes the special death logging behavior.
** CONCURRENCY NOTE: We lock the write semaphore and never unlock it to
** prevent any other process from printing after a death.
*/
static voidannounce_death(t_philo *philo, long now)
{
longtimestamp; /* time since simulation start */

sem_wait(philo->rules->write); /* lock output and keep it locked */
timestamp = now - philo->rules->start_time; /* compute ms since start */
printf("%ld %d died\n", timestamp, philo->id); /* death log */
}

/*
** FUNCTION: death_monitor
** PURPOSE: Continuously check if the philosopher starved.
** PARAMETERS:
**   - arg: pointer to the philosopher struct.
** RETURNS: Nothing (void)
** WHY IT EXISTS: Each process must watch its own philosopher.
** CONCURRENCY NOTE: Uses meal_mutex to read last_meal safely.
*/
void*death_monitor(void *arg)
{
t_philo*philo; /* typed philosopher pointer */
longnow; /* current time in milliseconds */
longlast_meal; /* snapshot of last meal time */

philo = (t_philo *)arg; /* convert void pointer */
while (1) /* loop until death */
{
now = get_time_ms(); /* read current time */
pthread_mutex_lock(&philo->meal_mutex); /* protect meal time */
last_meal = philo->last_meal; /* copy shared value */
pthread_mutex_unlock(&philo->meal_mutex); /* release meal lock */
if (now - last_meal >= philo->rules->time_die) /* starvation */
{
announce_death(philo, now); /* print death once */
exit(1); /* exit process with death status */
}
usleep(1000); /* sleep 1 ms to reduce CPU usage */
}
return (NULL); /* never reached */
}
