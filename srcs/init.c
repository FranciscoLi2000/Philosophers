#include "philosophers.h"

static int	init_mutexes(t_rules *rules)
{
	int	i;

	rules->forks = malloc(sizeof(pthread_mutex_t) * rules->num_philo);
	if (!rules->forks)
		return (1);
	i = 0;
	while (i < rules->num_philo)
	{
		if (pthread_mutex_init(&rules->forks[i], NULL) != 0)
			return (1);
		i++;
	}
	if (pthread_mutex_init(&rules->print_mutex, NULL) != 0)
		return (1);
	return (0);
}

static void	init_philos(t_rules *rules)
{
	int	i;

	rules->philos = malloc(sizeof(t_philo) * rules->num_philo);
	i = 0;
	while (i < rules->num_philo)
	{
		rules->philos[i].id = i + 1;
		rules->philos[i].left_fork = i;
		rules->philos[i].right_fork = (i + 1) % rules->num_philo;
		rules->philos[i].last_meal = 0;
		rules->philos[i].meals_eaten = 0;
		rules->philos[i].rules = rules;
		i++;
	}
}

long long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000));
}

void	precise_usleep(long long time_ms)
{
	long long	start;

	start = get_time_ms();
	while ((get_time_ms() - start) < time_ms)
		usleep(500);
}

int	init_simulation(t_rules *rules)
{
	int	i;

	if (init_mutexes(rules) != 0)
		return (1);
	init_philos(rules);
	rules->start_time = get_time_ms();
	i = 0;
	while (i < rules->num_philo)
	{
		if (pthread_create(&rules->philos[i].thread, NULL, philo_routine, &rules->philos[i]) != 0)
			return (1);
		i++;
	}
	monitor_philos(rules);
	i = 0;
	while (i < rules->num_philo)
	{
		pthread_join(rules->philos[i].thread, NULL);
		i++;
	}
	return (0);
}
