/*
** FILE: routine.c
** PURPOSE: Define the philosopher thread routine and actions.
** KEY CONCEPTS: Fork ordering, critical sections, cooperative looping.
*/

#include "philo.h"

/*
** CONCEPT: Deadlock
** ------------------------------------------
** Deadlock happens when everyone waits forever. If each philosopher grabs one
** fork and waits for the other, nobody can move. We avoid this by making
** philosophers pick up forks in a consistent order based on their id.
*/

/*
** FUNCTION: take_forks
** PURPOSE: Lock the two fork mutexes in a safe order.
** PARAMETERS:
**   - philo: philosopher performing the action.
** RETURNS: Nothing (void)
** WHY IT EXISTS: Centralizes fork locking logic to avoid deadlocks.
** CONCURRENCY NOTE: Fork mutexes protect shared fork resources.
*/
static voidtake_forks(t_philo *philo)
{
pthread_mutex_t*first; /* first fork to lock */
pthread_mutex_t*second; /* second fork to lock */

if (philo->id % 2 == 0) /* even ids pick left first */
{
first = philo->left_fork; /* left fork pointer */
second = philo->right_fork; /* right fork pointer */
}
else /* odd ids pick right first */
{
first = philo->right_fork; /* right fork pointer */
second = philo->left_fork; /* left fork pointer */
}
pthread_mutex_lock(first); /* lock first fork */
print_state(philo, "has taken a fork", 0); /* log first fork */
pthread_mutex_lock(second); /* lock second fork */
print_state(philo, "has taken a fork", 0); /* log second fork */
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
static voideat(t_philo *philo)
{
pthread_mutex_lock(&philo->meal_mutex); /* protect meal data */
philo->last_meal = get_time_ms(); /* record meal start time */
philo->meals_eaten++; /* count this meal */
pthread_mutex_unlock(&philo->meal_mutex); /* release meal lock */
print_state(philo, "is eating", 0); /* log eating state */
precise_sleep(philo->rules, philo->rules->time_eat); /* eat duration */
pthread_mutex_unlock(philo->left_fork); /* put down left fork */
pthread_mutex_unlock(philo->right_fork); /* put down right fork */
}

/*
** FUNCTION: sleep_think
** PURPOSE: Handle the sleeping and thinking phases.
** PARAMETERS:
**   - philo: philosopher performing the action.
** RETURNS: Nothing (void)
** WHY IT EXISTS: Keeps the routine loop readable by grouping actions.
*/
static voidsleep_think(t_philo *philo)
{
print_state(philo, "is sleeping", 0); /* log sleeping state */
precise_sleep(philo->rules, philo->rules->time_sleep); /* sleep time */
print_state(philo, "is thinking", 0); /* log thinking state */
}

/*
** FUNCTION: philo_routine
** PURPOSE: Main loop for each philosopher thread.
** PARAMETERS:
**   - arg: pointer to the philosopher struct.
** RETURNS: Nothing (void)
** WHY IT EXISTS: Each philosopher must run independently as a thread.
** CONCURRENCY NOTE: Uses mutexes for forks and meal data.
*/
void*philo_routine(void *arg)
{
t_philo*philo; /* typed pointer to this philosopher */

philo = (t_philo *)arg; /* convert void pointer */
if (philo->rules->philo_count == 1) /* special case: one fork only */
{
pthread_mutex_lock(philo->left_fork); /* take the single fork */
print_state(philo, "has taken a fork", 0); /* log fork */
precise_sleep(philo->rules, philo->rules->time_die); /* wait death */
pthread_mutex_unlock(philo->left_fork); /* release fork */
return (NULL); /* thread ends after inevitable death */
}
if (philo->id % 2 == 0) /* small delay for even philosophers */
usleep(1000); /* 1000 microseconds = 1 ms */
while (!simulation_stopped(philo->rules)) /* loop until stop flag */
{
take_forks(philo); /* lock two forks */
eat(philo); /* eat while holding forks */
sleep_think(philo); /* sleep then think */
}
return (NULL); /* thread exits cleanly */
}
