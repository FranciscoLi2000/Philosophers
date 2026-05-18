/*
** FILE: init.c
** PURPOSE: Parse arguments and initialize semaphores for the bonus.
** KEY CONCEPTS: Named semaphores, input validation, resource setup.
*/

#include "philo_bonus.h"

/*
** CONCEPT: Semaphore
** ------------------------------------------
** A semaphore is like a counter shared by many people. If the counter is
** above zero, a philosopher can take a fork (decrement the counter). When
** they finish, they return the fork (increment the counter). This prevents
** more forks from being taken than exist on the table.
*/

/*
** FUNCTION: parse_args
** PURPOSE: Validate and store command-line arguments.
** PARAMETERS:
**   - rules: shared simulation settings to fill in.
**   - argc: number of command-line arguments.
**   - argv: array of argument strings.
** RETURNS: 0 on success, 1 on invalid input.
** WHY IT EXISTS: Keeps input validation separate and easy to read.
*/
static int	parse_args(t_rules *rules, int argc, char **argv)
{
	long	value; /* temporary storage for parsed numbers */

	if (argc != 5 && argc != 6) /* must have 4 or 5 parameters */
		return (1); /* reject wrong argument count */
	if (parse_positive(argv[1], &value)) /* number_of_philosophers */
		return (1); /* invalid number */
	rules->philo_count = (int)value; /* safe cast after validation */
	if (parse_positive(argv[2], &value)) /* time_to_die */
		return (1); /* invalid number */
	rules->time_die = value; /* milliseconds to die */
	if (parse_positive(argv[3], &value)) /* time_to_eat */
		return (1); /* invalid number */
	rules->time_eat = value; /* milliseconds to eat */
	if (parse_positive(argv[4], &value)) /* time_to_sleep */
		return (1); /* invalid number */
	rules->time_sleep = value; /* milliseconds to sleep */
	rules->must_eat = -1; /* default: no meal limit */
	if (argc == 6) /* optional number_of_times_each_philosopher_must_eat */
	{
		if (parse_positive(argv[5], &value)) /* parse optional limit */
			return (1); /* invalid number */
		rules->must_eat = (int)value; /* save required meal count */
	}
	return (0); /* arguments look valid */
}

/*
** FUNCTION: open_sems
** PURPOSE: Create or open the named semaphores.
** PARAMETERS:
**   - rules: shared simulation settings.
** RETURNS: 0 on success, 1 on failure.
** WHY IT EXISTS: Centralizes semaphore setup for cleanup and reuse.
** CONCURRENCY NOTE: Semaphores are shared across all processes.
*/
static int	open_sems(t_rules *rules)
{
	sem_unlink(SEM_FORKS); /* remove any previous semaphore */
	sem_unlink(SEM_WRITE); /* remove any previous semaphore */
	rules->forks = sem_open(SEM_FORKS, O_CREAT, 0644,
		rules->philo_count); /* forks counter */
	if (rules->forks == SEM_FAILED) /* open failed */
		return (1); /* report error */
	rules->write = sem_open(SEM_WRITE, O_CREAT, 0644, 1); /* output lock */
	if (rules->write == SEM_FAILED) /* open failed */
		return (1); /* report error */
	return (0); /* semaphores are ready */
}

/*
** FUNCTION: allocate_pids
** PURPOSE: Allocate the pid array for child processes.
** PARAMETERS:
**   - rules: shared simulation settings.
** RETURNS: 0 on success, 1 on failure.
** WHY IT EXISTS: The parent needs to track each child process id.
*/
static int	allocate_pids(t_rules *rules)
{
	rules->pids = malloc(sizeof(pid_t) * rules->philo_count); /* pid array */
	if (!rules->pids) /* allocation failed */
		return (1); /* report error */
	return (0); /* allocation succeeded */
}

/*
** FUNCTION: close_sems
** PURPOSE: Close and unlink named semaphores.
** PARAMETERS:
**   - rules: shared simulation settings.
** RETURNS: Nothing (void)
** WHY IT EXISTS: Cleans up OS resources after the program ends.
*/
void	close_sems(t_rules *rules)
{
	if (rules->forks) /* close fork semaphore if opened */
		sem_close(rules->forks); /* release handle */
	if (rules->write) /* close write semaphore if opened */
		sem_close(rules->write); /* release handle */
	sem_unlink(SEM_FORKS); /* unlink fork semaphore name */
	sem_unlink(SEM_WRITE); /* unlink write semaphore name */
}

/*
** FUNCTION: init_rules
** PURPOSE: Initialize all simulation data and resources.
** PARAMETERS:
**   - rules: shared simulation settings to populate.
**   - argc: number of command-line arguments.
**   - argv: array of argument strings.
** RETURNS: 0 on success, 1 on failure.
** WHY IT EXISTS: Provides a single initialization entry point for main.
*/
int	init_rules(t_rules *rules, int argc, char **argv)
{
	if (parse_args(rules, argc, argv)) /* validate and store inputs */
		return (1); /* invalid parameters */
	if (allocate_pids(rules)) /* allocate pid array */
		return (1); /* allocation failed */
	if (open_sems(rules)) /* create semaphores */
	{
		free(rules->pids); /* release pid array */
		return (1); /* report initialization failure */
	}
	return (0); /* ready to fork processes */
}
