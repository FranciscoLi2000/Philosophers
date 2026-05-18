/*
** FILE: main.c
** PURPOSE: Launch philosopher processes and wait for completion.
** KEY CONCEPTS: Process creation, parent supervision, cleanup.
*/

#include "philo_bonus.h"

/*
** CONCEPT: Process
** ------------------------------------------
** A process is like a separate program instance with its own memory.
** In the bonus part, each philosopher is a full process, not just a thread.
** The parent process creates them using fork and then supervises them.
*/

/*
** FUNCTION: start_processes
** PURPOSE: Fork one process per philosopher.
** PARAMETERS:
**   - rules: shared simulation settings.
** RETURNS: 0 on success, 1 on failure.
** WHY IT EXISTS: Keeps fork logic centralized and easy to manage.
** CONCURRENCY NOTE: Each child process runs independently after fork.
*/
static int	start_processes(t_rules *rules)
{
	int		i; /* loop index for philosophers */
	pid_t	pid; /* process id from fork */

	rules->start_time = get_time_ms(); /* shared reference time */
	i = 0; /* start from first philosopher */
	while (i < rules->philo_count) /* create each philosopher process */
	{
		pid = fork(); /* fork creates a new process */
		if (pid < 0) /* fork failed */
			return (1); /* report error to parent */
		if (pid == 0) /* child process */
			run_philo(rules, i + 1); /* run philosopher routine */
		rules->pids[i] = pid; /* store child pid in parent */
		i++; /* move to next philosopher */
	}
	return (0); /* all processes created */
}

/*
** FUNCTION: wait_processes
** PURPOSE: Wait for philosophers to finish or die.
** PARAMETERS:
**   - rules: shared simulation settings.
** RETURNS: Nothing (void)
** WHY IT EXISTS: Parent must reap child processes to avoid zombies.
** CONCURRENCY NOTE: waitpid blocks until a child changes state.
*/
static void	wait_processes(t_rules *rules)
{
	int		status; /* status returned by waitpid */
	int		finished; /* count of normal exits */
	int		i; /* loop index for kill */
	pid_t	pid; /* pid from waitpid */

	finished = 0; /* no child has finished yet */
	while (finished < rules->philo_count) /* keep waiting for children */
	{
		pid = waitpid(-1, &status, 0); /* wait for any child */
		if (pid < 0) /* waitpid failed */
			break; /* exit loop on error */
		if (WIFEXITED(status) && WEXITSTATUS(status) == 1) /* death */
		{
			i = 0; /* start killing from first pid */
			while (i < rules->philo_count) /* kill all remaining children */
			{
				kill(rules->pids[i], SIGTERM); /* terminate child */
				i++; /* next pid */
			}
			break; /* stop waiting once death occurs */
		}
		finished++; /* one child exited normally */
	}
	while (waitpid(-1, NULL, 0) > 0) /* reap any remaining children */
		; /* empty loop body by design */
}

/*
** FUNCTION: cleanup
** PURPOSE: Close semaphores and free allocations.
** PARAMETERS:
**   - rules: shared simulation settings.
** RETURNS: Nothing (void)
** WHY IT EXISTS: Ensures no leaked resources after the run.
*/
static void	cleanup(t_rules *rules)
{
	close_sems(rules); /* close and unlink named semaphores */
	free(rules->pids); /* free pid array memory */
}

/*
** FUNCTION: main
** PURPOSE: Parse arguments, initialize resources, and start processes.
** PARAMETERS:
**   - argc: number of command-line arguments.
**   - argv: array of argument strings.
** RETURNS: 0 on success, 1 on error.
** WHY IT EXISTS: This is the entry point for the bonus program.
*/
int	main(int argc, char **argv)
{
	t_rules	rules; /* shared configuration for the parent */

	memset(&rules, 0, sizeof(t_rules)); /* clear struct for safety */
	if (init_rules(&rules, argc, argv)) /* parse and init resources */
	{
		write(2, "Error\n", 6); /* simple error output to stderr */
		return (1); /* report initialization failure */
	}
	if (start_processes(&rules)) /* fork all philosopher processes */
	{
		cleanup(&rules); /* release resources on failure */
		return (1); /* report fork failure */
	}
	wait_processes(&rules); /* wait for end condition */
	cleanup(&rules); /* close semaphores and free memory */
	return (0); /* normal exit */
}
