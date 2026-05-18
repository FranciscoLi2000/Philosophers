/*
** FILE: routine.c
** PURPOSE: Define the philosopher process routine and actions.
** KEY CONCEPTS: Process loops, semaphore forks, per-process state.
*/

#include "philo_bonus.h"

/*
** CONCEPT: Mutex (Mutual Exclusion Lock)
** ------------------------------------------
** Even inside a process, multiple threads can touch the same data.
** We use a mutex to protect each philosopher's last_meal timestamp from
** being read and written at the same time by the routine and the monitor.
*/

/*
** FUNCTION: take_forks
** PURPOSE: Reserve two forks from the shared semaphore.
** PARAMETERS:
**   - philo: philosopher performing the action.
** RETURNS: Nothing (void)
** WHY IT EXISTS: Encapsulates semaphore usage for taking forks.
** CONCURRENCY NOTE: The forks semaphore represents the fork count.
*/
static void	take_forks(t_philo *philo)
{
	sem_wait(philo->rules->forks); /* decrement fork counter */
	print_state(philo, "has taken a fork"); /* log first fork */
	sem_wait(philo->rules->forks); /* decrement fork counter again */
	print_state(philo, "has taken a fork"); /* log second fork */
}

/*
** FUNCTION: eat
** PURPOSE: Update meal data, print, and hold forks while eating.
** PARAMETERS:
**   - philo: philosopher performing the action.
** RETURNS: Nothing (void)
** WHY IT EXISTS: Encapsulates the eating sequence and shared state updates.
** CONCURRENCY NOTE: Meal data is protected by meal_mutex.
*/
static void	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->meal_mutex); /* protect meal data */
	philo->last_meal = get_time_ms(); /* record meal start time */
	philo->meals_eaten++; /* count this meal */
	pthread_mutex_unlock(&philo->meal_mutex); /* release meal lock */
	print_state(philo, "is eating"); /* log eating state */
	precise_sleep(philo->rules->time_eat); /* eat duration */
	sem_post(philo->rules->forks); /* return first fork */
	sem_post(philo->rules->forks); /* return second fork */
}

/*
** FUNCTION: sleep_think
** PURPOSE: Handle the sleeping and thinking phases.
** PARAMETERS:
**   - philo: philosopher performing the action.
** RETURNS: Nothing (void)
** WHY IT EXISTS: Keeps the routine loop readable by grouping actions.
*/
static void	sleep_think(t_philo *philo)
{
	print_state(philo, "is sleeping"); /* log sleeping state */
	precise_sleep(philo->rules->time_sleep); /* sleep time */
	print_state(philo, "is thinking"); /* log thinking state */
}

/*
** FUNCTION: run_philo
** PURPOSE: Run the philosopher loop inside a child process.
** PARAMETERS:
**   - rules: shared simulation settings.
**   - id: philosopher id (1..N).
** RETURNS: Nothing (void)
** WHY IT EXISTS: Each child process runs this function after fork.
** CONCURRENCY NOTE: Starts a monitor thread to detect death.
*/
void	run_philo(t_rules *rules, int id)
{
	t_philo	philo; /* local philosopher state in this process */

	philo.id = id; /* set philosopher id */
	philo.rules = rules; /* store shared rules pointer */
	philo.meals_eaten = 0; /* no meals yet */
	philo.last_meal = get_time_ms(); /* initial meal time */
	pthread_mutex_init(&philo.meal_mutex, NULL); /* protect meal data */
	if (pthread_create(&philo.monitor, NULL, death_monitor, &philo) != 0)
		exit(1); /* stop if we cannot start the monitor thread */
	pthread_detach(philo.monitor); /* monitor exits with the process */
	if (rules->philo_count == 1) /* single philosopher edge case */
	{
		sem_wait(rules->forks); /* take the only fork */
		print_state(&philo, "has taken a fork"); /* log fork */
		while (1) /* wait until monitor exits the process */
			usleep(1000); /* 1000 microseconds = 1 ms */
	}
	while (1) /* infinite loop until exit on death or completion */
	{
		take_forks(&philo); /* reserve two forks */
		eat(&philo); /* eat and release forks */
		if (rules->must_eat > 0
			&& philo.meals_eaten >= rules->must_eat) /* done eating */
			exit(0); /* normal completion */
		sleep_think(&philo); /* sleep then think */
	}
}
