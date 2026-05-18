/*
** FILE: utils.c
** PURPOSE: Provide timing, parsing, and printing helpers for the bonus.
** KEY CONCEPTS: Millisecond timing, input validation, synchronized logging.
*/

#include "philo_bonus.h"

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
** FUNCTION: precise_sleep
** PURPOSE: Sleep for a duration with small increments.
** PARAMETERS:
**   - duration: time to sleep in milliseconds.
** RETURNS: Nothing (void)
** WHY IT EXISTS: usleep can oversleep, so we loop in small steps.
*/
voidprecise_sleep(long duration)
{
longstart; /* sleep start timestamp */
longnow; /* current timestamp */

start = get_time_ms(); /* mark the beginning of sleep */
while (1) /* loop until enough time has passed */
{
now = get_time_ms(); /* check how much time passed */
if (now - start >= duration) /* required time elapsed */
break; /* exit the sleep loop */
usleep(500); /* sleep 500 microseconds to reduce CPU */
}
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
** RETURNS: Nothing (void)
** WHY IT EXISTS: Keeps log formatting and semaphore logic in one place.
** CONCURRENCY NOTE: write semaphore avoids interleaved output.
*/
voidprint_state(t_philo *philo, const char *msg)
{
longtimestamp; /* time since simulation start */

sem_wait(philo->rules->write); /* lock output */
timestamp = get_time_ms() - philo->rules->start_time; /* ms since start */
printf("%ld %d %s\n", timestamp, philo->id, msg); /* formatted log */
sem_post(philo->rules->write); /* release output */
}
