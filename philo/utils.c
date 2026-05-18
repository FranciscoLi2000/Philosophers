/*
** FILE: utils.c
** PURPOSE: Provide timing, parsing, and printing helpers.
** KEY CONCEPTS: Millisecond timing, input validation, synchronized logging.
*/

#include "philo.h"

/*
** CONCEPT: Millisecond Timing
** ------------------------------------------
** We measure time in milliseconds (1 ms = 1/1000 second) so we can compare
** small delays like eating and sleeping times precisely.
*/

/*
** FUNCTION: get_time_ms
** PURPOSE: Get the current time in milliseconds.
** PARAMETERS: None
** RETURNS: Current time in ms since the Unix epoch.
** WHY IT EXISTS: Timing in ms matches the project requirements.
*/
longget_time_ms(void)
{
struct timevaltv; /* time structure from gettimeofday */
longms; /* computed milliseconds */

gettimeofday(&tv, NULL); /* read current wall-clock time */
ms = tv.tv_sec * 1000L + tv.tv_usec / 1000L; /* sec + usec to ms */
return (ms); /* return millisecond timestamp */
}

/*
** FUNCTION: simulation_stopped
** PURPOSE: Check if the simulation stop flag is set.
** PARAMETERS:
**   - rules: shared simulation settings.
** RETURNS: 1 if stop is set, 0 otherwise.
** WHY IT EXISTS: Centralizes stop checks under mutex protection.
** CONCURRENCY NOTE: Uses state_mutex to protect the stop flag.
*/
intsimulation_stopped(t_rules *rules)
{
intstopped; /* local copy of stop flag */

pthread_mutex_lock(&rules->state_mutex); /* protect stop flag */
stopped = rules->stop; /* read stop flag safely */
pthread_mutex_unlock(&rules->state_mutex); /* release lock */
return (stopped); /* return the snapshot */
}

/*
** FUNCTION: precise_sleep
** PURPOSE: Sleep for a duration while allowing early stop.
** PARAMETERS:
**   - rules: shared simulation settings.
**   - duration: time to sleep in milliseconds.
** RETURNS: 0 after sleeping or stopping.
** WHY IT EXISTS: usleep can oversleep, so we loop in small steps.
** CONCURRENCY NOTE: Checks stop flag to exit early if needed.
*/
intprecise_sleep(t_rules *rules, long duration)
{
longstart; /* sleep start timestamp */
longnow; /* current timestamp */

start = get_time_ms(); /* mark the beginning of sleep */
while (!simulation_stopped(rules)) /* keep sleeping until stop */
{
now = get_time_ms(); /* check how much time passed */
if (now - start >= duration) /* required time elapsed */
break; /* exit the sleep loop */
usleep(500); /* sleep 500 microseconds to reduce CPU */
}
return (0); /* return after sleeping */
}

/*
** FUNCTION: parse_positive
** PURPOSE: Convert a string to a positive long with validation.
** PARAMETERS:
**   - str: input string to parse.
**   - out: destination for the parsed number.
** RETURNS: 0 on success, 1 on invalid input.
** WHY IT EXISTS: The project forbids using atoi and similar helpers.
*/
intparse_positive(const char *str, long *out)
{
longvalue; /* accumulated numeric value */
inti; /* index into the string */

if (!str || !str[0]) /* reject NULL or empty strings */
return (1); /* invalid input */
value = 0; /* start accumulation at zero */
i = 0; /* start from first character */
if (str[i] == '+') /* allow an optional plus sign */
i++; /* skip the plus sign */
if (!str[i]) /* string was only a plus sign */
return (1); /* invalid input */
while (str[i]) /* process each character */
{
if (str[i] < '0' || str[i] > '9') /* reject non-digits */
return (1); /* invalid input */
value = value * 10 + (str[i] - '0'); /* base-10 accumulation */
if (value > 2147483647L) /* clamp to int range */
return (1); /* overflow not allowed */
i++; /* move to next character */
}
if (value <= 0) /* must be strictly positive */
return (1); /* invalid input */
*out = value; /* store valid result */
return (0); /* success */
}

/*
** FUNCTION: print_state
** PURPOSE: Print a philosopher state message safely.
** PARAMETERS:
**   - philo: philosopher whose state is printed.
**   - msg: message text (e.g., "is eating").
**   - force: 1 to print even after stop (used for death).
** RETURNS: 0 when printed, 1 when skipped.
** WHY IT EXISTS: Keeps log formatting and mutex logic in one place.
** CONCURRENCY NOTE: state_mutex prevents printing after stop; write_mutex
** is used to avoid interleaved output from multiple threads.
*/
intprint_state(t_philo *philo, const char *msg, int force)
{
longtimestamp; /* time since simulation start */
intstopped; /* snapshot of stop flag */

pthread_mutex_lock(&philo->rules->state_mutex); /* protect stop flag */
stopped = philo->rules->stop; /* read stop status */
if (stopped && !force) /* skip normal logs after stop */
{
pthread_mutex_unlock(&philo->rules->state_mutex); /* release lock */
return (1); /* caller can skip further work */
}
pthread_mutex_lock(&philo->rules->write_mutex); /* lock output */
timestamp = get_time_ms() - philo->rules->start_time; /* ms since start */
printf("%ld %d %s\n", timestamp, philo->id, msg); /* formatted log */
pthread_mutex_unlock(&philo->rules->write_mutex); /* release output */
pthread_mutex_unlock(&philo->rules->state_mutex); /* release stop lock */
return (0); /* message printed */
}
