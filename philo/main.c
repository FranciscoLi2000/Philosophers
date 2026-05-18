/*
** FILE: main.c
** PURPOSE: Program entry point and high-level simulation control.
** KEY CONCEPTS: Thread creation, lifecycle management, resource cleanup.
*/

#include "philo.h"

/*
** CONCEPT: Thread
** ------------------------------------------
** A thread is like a mini-worker inside one process.
** Each philosopher is a thread that runs in parallel with the others.
** We create threads so every philosopher can eat, sleep, and think at the same time.
*/

/*
** FUNCTION: start_threads
** PURPOSE: Create one thread per philosopher and set the shared start time.
** PARAMETERS:
**   - rules: shared simulation settings and arrays.
** RETURNS: 0 on success, 1 on failure.
** WHY IT EXISTS: Thread creation is isolated so errors are handled in one place.
** CONCURRENCY NOTE: Threads start executing the routine immediately after creation.
*/
static int	start_threads(t_rules *rules)
{
	int	i; /* loop index for philosophers */

	rules->start_time = get_time_ms(); /* shared zero time for all logs */
	i = 0; /* start from the first philosopher */
	while (i < rules->philo_count) /* create a thread for each philosopher */
	{
		rules->philos[i].last_meal = rules->start_time; /* initial meal time */
		if (pthread_create(&rules->philos[i].thread, NULL,
				philo_routine, &rules->philos[i]) != 0) /* spawn thread */
		{
			pthread_mutex_lock(&rules->state_mutex); /* protect shared stop */
			rules->stop = 1; /* signal threads to stop on error */
			pthread_mutex_unlock(&rules->state_mutex); /* release stop lock */
			while (i > 0) /* join only threads that were created */
			{
				i--; /* move to the last created thread */
				pthread_join(rules->philos[i].thread, NULL); /* wait it out */
			}
			return (1); /* propagate failure to main */
		}
		i++; /* move to the next philosopher */
	}
	return (0); /* all threads started successfully */
}

/*
** FUNCTION: join_threads
** PURPOSE: Wait for all philosopher threads to finish.
** PARAMETERS:
**   - rules: shared simulation settings and arrays.
** RETURNS: Nothing (void)
** WHY IT EXISTS: Joining threads prevents zombie threads and leaks.
** CONCURRENCY NOTE: Join waits for each thread to exit cleanly.
*/
static void	join_threads(t_rules *rules)
{
	int	i; /* loop index for philosophers */

	i = 0; /* start from the first philosopher */
	while (i < rules->philo_count) /* wait for each thread */
	{
		pthread_join(rules->philos[i].thread, NULL); /* block until done */
		i++; /* move to the next philosopher */
	}
}

/*
** FUNCTION: cleanup
** PURPOSE: Destroy mutexes and free allocated memory.
** PARAMETERS:
**   - rules: shared simulation settings and arrays.
** RETURNS: Nothing (void)
** WHY IT EXISTS: Frees resources to avoid leaks after the simulation.
** CONCURRENCY NOTE: Mutexes are destroyed after threads are joined.
*/
static void	cleanup(t_rules *rules)
{
	int	i; /* loop index for arrays */

	if (!rules->forks || !rules->philos) /* nothing to clean if NULL */
		return; /* exit early when initialization failed */
	pthread_mutex_destroy(&rules->state_mutex); /* stop flag protection */
	pthread_mutex_destroy(&rules->write_mutex); /* ordered printing */
	i = 0; /* start from the first fork */
	while (i < rules->philo_count) /* destroy each fork and meal mutex */
	{
		pthread_mutex_destroy(&rules->forks[i]); /* fork resource */
		pthread_mutex_destroy(&rules->philos[i].meal_mutex); /* meal data */
		i++; /* move to the next slot */
	}
	free(rules->forks); /* release fork array memory */
	free(rules->philos); /* release philosopher array memory */
}

/*
** FUNCTION: main
** PURPOSE: Parse arguments, launch threads, and monitor the simulation.
** PARAMETERS:
**   - argc: number of command-line arguments.
**   - argv: array of argument strings.
** RETURNS: 0 on success, 1 on error.
** WHY IT EXISTS: This is the program's entry point.
*/
int	main(int argc, char **argv)
{
	t_rules	rules; /* main shared state container */

	memset(&rules, 0, sizeof(t_rules)); /* clear struct for safety */
	if (init_rules(&rules, argc, argv)) /* parse and initialize */
	{
		write(2, "Error\n", 6); /* simple error output to stderr */
		return (1); /* report initialization failure */
	}
	if (start_threads(&rules)) /* create philosopher threads */
	{
		cleanup(&rules); /* free resources before exiting */
		return (1); /* report thread creation failure */
	}
	monitor_simulation(&rules); /* main thread monitors death/finish */
	join_threads(&rules); /* wait for all philosophers to stop */
	cleanup(&rules); /* free mutexes and memory */
	return (0); /* normal exit */
}
