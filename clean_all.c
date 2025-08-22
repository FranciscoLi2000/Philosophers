#include "philosophers.h"

void	clean_all(t_rules *rules)
{
	int	i;

	i = 0;
	while (i < rules->num_philo)
	{
		pthread_mutex_destroy(&rules->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&rules->print_mutex);
	free(rules->forks);
	free(rules->philos);
}
