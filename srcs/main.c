#include "philosophers.h"

static int	is_positive_number(char *str)
{
	int	i;

	i = 0;
	if (!str || str[0] == '\0')
		return (0);
	while (str[i])
	{
		if (str[i] < '0' && str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	parse_args(int argc, char **argv, t_rules *rules)
{
	int	i;

	if (argc != 5 && argc != 6)
	{
		ft_putstr_fd("Usage: ", 2);
		ft_putstr_fd(argv[0], 2);
		ft_putstr_fd(" nb_philo time_die time_eat time_sleep [must_eat]\n", 2);
		return (1);
	}
	i = 1;
	while (i < argc)
	{
		if (!is_positive_number(argv[i]))
		{
			ft_putstr_fd("Error: invalid argument: ", 2);
			ft_putstr_fd(argv[i], 2);
			return (1);
		}
		i++;
	}
	rules->num_philo = ft_atoi(argv[1]);
	rules->time_to_die = ft_atoi(argv[2]);
	rules->time_to_eat = ft_atoi(argv[3]);
	rules->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		rules->must_eat_count = ft_atoi(argv[5]);
	else
		rules->must_eat_count = -1;
	rules->dead = 0;
	return (0);
}

int	main(int argc, char **argv)
{
	t_rules	rules;

	if (parse_args(argc, argv, &rules) != 0)
		return (1);
	if (init_simulation(&rules) != 0)
		return (1);
	clean_all(&rules);
	return (0);
}
