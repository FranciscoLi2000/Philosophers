#include "philosophers.h"

void	print_action(t_philo *philo, char *msg)
{
	long long	timestamp;

	pthread_mutex_lock(&philo->rules->print_mutex);
	timestamp = get_time_ms() - philo->rules->start_time;
	if (!philo->rules->dead)
	{
		ft_putchar_fd('[', 1);
		ft_putnbr_fd(timestamp, 1);
		ft_putchar_fd(']', 1);
		ft_putchar_fd(' ', 1);
		ft_putnbr_fd(philo->id, 1);
		ft_putchar_fd(' ', 1);
		ft_putstr_fd(msg, 1);
		ft_putchar_fd('\n', 1);
	}
	pthread_mutex_unlock(&philo->rules->print_mutex);
}

static void	eat(t_philo *philo)
{
	t_rules	*rules;

	rules = philo->rules;
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&rules->forks[philo->right_fork]);
		print_action(philo, "has taken the right fork");
		pthread_mutex_lock(&rules->forks[philo->left_fork]);
		print_action(philo, "has taken the left fork");
	}
	else
	{
		pthread_mutex_lock(&rules->forks[philo->left_fork]);
		print_action(philo, "has taken the left fork");
		pthread_mutex_lock(&rules->forks[philo->right_fork]);
		print_action(philo, "has taken the right fork");
	}
	print_action(philo, "is eating");
	philo->last_meal = get_time_ms();
	philo->meals_eaten++;
	precise_usleep(rules->time_to_eat);
	pthread_mutex_unlock(&rules->forks[philo->left_fork]);
	pthread_mutex_unlock(&rules->forks[philo->right_fork]);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_rules	*rules;

	philo = (t_philo *)arg;
	rules = philo->rules;
	if (rules->num_philo == 1)
	{
		print_action(philo, "has taken a fork");
		precise_usleep(rules->time_to_die);
		print_action(philo, "died");
		rules->dead = 1;
		return (NULL);
	}
	if (philo->id % 2 == 0)
		usleep(100);
	while (!rules->dead && (rules->must_eat_count == -1 || philo->meals_eaten < rules->must_eat_count))
	{
		eat(philo);
		print_action(philo, "is sleeping");
		precise_usleep(rules->time_to_sleep);
		print_action(philo, "is thinking");
	}
	return (NULL);
}
