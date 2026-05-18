/*
** FILE: monitor.c
** PURPOSE: Watch philosophers for death or completion conditions.
** KEY CONCEPTS: Monitoring loop, race condition prevention, timing checks.
*/

#include "philo.h"

/*
** CONCEPT: Race Condition
** ------------------------------------------
** A race condition happens when two threads read and write shared data at the
** same time without coordination. We avoid it by locking a mutex before
** accessing shared meal times and the global stop flag.
*/

/*
** FUNCTION: check_death
** PURPOSE: Detect if a philosopher has exceeded time_to_die.
** PARAMETERS:
**   - rules: shared simulation settings.
**   - philo: philosopher to check.
** RETURNS: 1 if a death was detected, 0 otherwise.
** WHY IT EXISTS: Isolates the death check logic for clarity.
** CONCURRENCY NOTE: Uses meal_mutex and state_mutex to avoid races.
*/
static int	check_death(t_rules *rules, t_philo *philo)
{
	long	now; /* current time in milliseconds */
	long	last_meal; /* snapshot of last meal time */

	now = get_time_ms(); /* read current time */
	pthread_mutex_lock(&philo->meal_mutex); /* protect meal time */
	last_meal = philo->last_meal; /* copy shared value */
	pthread_mutex_unlock(&philo->meal_mutex); /* release meal lock */
	if (now - last_meal >= rules->time_die) /* starvation detected */
	{
		pthread_mutex_lock(&rules->state_mutex); /* protect stop flag */
		if (!rules->stop) /* only announce the first death */
			rules->stop = 1; /* stop the simulation */
		pthread_mutex_unlock(&rules->state_mutex); /* release stop lock */
		print_state(philo, "died", 1); /* forced death message */
		return (1); /* signal monitor to stop */
	}
	return (0); /* philosopher still alive */
}

/*
** FUNCTION: check_meals
** PURPOSE: Determine if all philosophers ate enough times.
** PARAMETERS:
**   - rules: shared simulation settings.
** RETURNS: 1 if everyone is full, 0 otherwise.
** WHY IT EXISTS: Keeps completion logic separate from the main loop.
** CONCURRENCY NOTE: Reads meal counts under meal_mutex protection.
*/
static int	check_meals(t_rules *rules)
{
	int	i; /* loop index for philosophers */
	int	meals; /* snapshot of meals eaten */

	if (rules->must_eat <= 0) /* no meal limit specified */
		return (0); /* do not stop by meals */
	i = 0; /* start from first philosopher */
	while (i < rules->philo_count) /* check each philosopher */
	{
		pthread_mutex_lock(&rules->philos[i].meal_mutex); /* protect count */
		meals = rules->philos[i].meals_eaten; /* read count safely */
		pthread_mutex_unlock(&rules->philos[i].meal_mutex); /* release lock */
		if (meals < rules->must_eat) /* this one is not done yet */
			return (0); /* overall condition not met */
		i++; /* move to the next philosopher */
	}
	return (1); /* everyone has eaten enough */
}

/*
** FUNCTION: monitor_simulation
** PURPOSE: Continuously check for death or completion.
** PARAMETERS:
**   - rules: shared simulation settings.
** RETURNS: 0 when monitoring ends.
** WHY IT EXISTS: Main thread needs a single place to supervise the simulation.
** CONCURRENCY NOTE: Uses mutexes to read shared data safely.
*/
int	monitor_simulation(t_rules *rules)
{
	int	i; /* loop index for philosophers */

	while (!simulation_stopped(rules)) /* keep monitoring until stop */
	{
		i = 0; /* start at the first philosopher */
		while (i < rules->philo_count) /* check every philosopher */
		{
			if (check_death(rules, &rules->philos[i])) /* stop on death */
				return (0); /* exit monitoring immediately */
			i++; /* move to the next philosopher */
		}
		if (check_meals(rules)) /* stop if all ate enough */
		{
			pthread_mutex_lock(&rules->state_mutex); /* protect stop flag */
			rules->stop = 1; /* signal clean completion */
			pthread_mutex_unlock(&rules->state_mutex); /* release stop */
			return (0); /* done monitoring */
		}
		usleep(1000); /* sleep 1 ms to reduce CPU usage */
	}
	return (0); /* stop flag was set elsewhere */
}
