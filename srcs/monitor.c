#include "philosophers.h"

void	monitor_philos(t_rules *rules)
{
	long long	now;
	int			i;

	while (!rules->dead)
	{
		i = 0;
		while (i < rules->num_philo)
		{
			now = get_time_ms();
			if ((now - rules->philos[i].last_meal) > rules->time_to_die)
			{
				pthread_mutex_lock(&rules->print_mutex);
				ft_putchar_fd('[', 1);
				ft_putnbr_fd(now - rules->start_time, 1);
				ft_putstr_fd("] ", 1);
				ft_putnbr_fd(rules->philos[i].id, 1);
				ft_putstr_fd(" died\n", 1);
				pthread_mutex_unlock(&rules->print_mutex);
				rules->dead = 1;
				return ;
			}
			i++;
		}
		usleep(1000);
	}
}
