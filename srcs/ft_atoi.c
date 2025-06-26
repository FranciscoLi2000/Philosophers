#include "libft.h"

int	ft_atoi(const char *str)
{
	long	num;
	int		i;
	int		sign;

	i = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	sign = -1;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	num = 0;
	while (str[i] >= 48 && str[i] <= 57)
	{
		num = num * 10 + (str[i] - '0');
		if (sign == 1 && num > INT_MAX)
			return (-1);
		if (sign == -1 && num < INT_MIN)
			return (0);
		i++;
	}
	return ((int)(num * sign));
}
