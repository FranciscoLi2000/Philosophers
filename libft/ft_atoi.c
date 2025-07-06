#include "libft.h"

int	ft_atoi(const char *str)
{
	long	num;
	int		i;
	int		sign;

	i = 0;
	num = 0;
	sign = 1;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (num > (LONG_MAX - (str[i] - '0')) / 10)
		{
			if (sign == 1)
				return (-1);
			else
				return (0);
		}
		num = num * 10 + (str[i] - '0');
		i++;
	}
	return ((int)(num * sign));
}
