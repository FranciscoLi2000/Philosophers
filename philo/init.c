/*
** FILE: init.c
** PURPOSE: Parse arguments and initialize shared simulation resources.
** KEY CONCEPTS: Input validation, mutex setup, shared state initialization.
*/

#include "philo.h"

/*
** CONCEPT: Mutex (Mutual Exclusion Lock)
** ------------------------------------------
** A mutex is like a bathroom key in an office. Only one person (thread)
** can hold the key at a time. When a philosopher wants to pick up a fork
** (shared resource), they must "lock" the mutex first. If another thread
** already holds the lock, they wait. This prevents two threads from
** modifying the same data at the same time — which would cause a "data race".
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
** FUNCTION: init_mutexes
** PURPOSE: Initialize all mutexes used by the simulation.
** PARAMETERS:
**   - rules: shared simulation settings and arrays.
** RETURNS: 0 on success, 1 on failure.
** WHY IT EXISTS: Centralizes mutex creation for easier cleanup.
** CONCURRENCY NOTE: Mutexes protect forks, meal data, and shared flags.
*/
static int	init_mutexes(t_rules *rules)
{
	int	i; /* loop index for mutex arrays */

	if (pthread_mutex_init(&rules->state_mutex, NULL) != 0) /* stop flag */
		return (1); /* cannot continue without this mutex */
	if (pthread_mutex_init(&rules->write_mutex, NULL) != 0) /* output lock */
		return (1); /* cannot continue without this mutex */
	i = 0; /* start from the first fork */
	while (i < rules->philo_count) /* init one mutex per fork */
	{
		if (pthread_mutex_init(&rules->forks[i], NULL) != 0) /* fork lock */
			return (1); /* stop on failure */
		i++; /* move to the next fork */
	}
	i = 0; /* start from the first philosopher */
	while (i < rules->philo_count) /* init meal mutexes */
	{
		if (pthread_mutex_init(&rules->philos[i].meal_mutex, NULL) != 0)
			return (1); /* stop on failure */
		i++; /* move to the next philosopher */
	}
	return (0); /* all mutexes initialized */
}

/*
** FUNCTION: init_philos
** PURPOSE: Fill in per-philosopher data fields.
** PARAMETERS:
**   - rules: shared simulation settings and arrays.
** RETURNS: Nothing (void)
** WHY IT EXISTS: Keeps philosopher setup separate from argument parsing.
*/
static void	init_philos(t_rules *rules)
{
	int	i; /* loop index for philosophers */

	i = 0; /* start from philosopher 1 */
	while (i < rules->philo_count) /* initialize each philosopher */
	{
		rules->philos[i].id = i + 1; /* ids start at 1 */
		rules->philos[i].meals_eaten = 0; /* no meals yet */
		rules->philos[i].last_meal = 0; /* filled when threads start */
		rules->philos[i].rules = rules; /* back-reference to shared state */
		rules->philos[i].left_fork = &rules->forks[i]; /* fork on left */
		rules->philos[i].right_fork = &rules->forks[(i + 1)
			% rules->philo_count]; /* fork on right */
		i++; /* move to the next philosopher */
	}
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
	rules->forks = malloc(sizeof(pthread_mutex_t) * rules->philo_count);
	if (!rules->forks) /* allocation failure */
		return (1); /* cannot proceed */
	rules->philos = malloc(sizeof(t_philo) * rules->philo_count);
	if (!rules->philos) /* allocation failure */
	{
		free(rules->forks); /* free earlier allocation */
		return (1); /* cannot proceed */
	}
	memset(rules->forks, 0, sizeof(pthread_mutex_t) * rules->philo_count);
	memset(rules->philos, 0, sizeof(t_philo) * rules->philo_count);
	if (init_mutexes(rules)) /* create mutexes */
	{
		free(rules->forks); /* release forks array */
		free(rules->philos); /* release philosophers array */
		return (1); /* report initialization failure */
	}
	rules->stop = 0; /* simulation starts in running state */
	init_philos(rules); /* fill per-philosopher fields */
	return (0); /* ready to start threads */
}
