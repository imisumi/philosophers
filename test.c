# include <stdio.h>
# define P_EAT "\x1b[1;33mis eating\x1b[0m"
# define P_SLEEP "\x1b[1;34mis sleeping\x1b[0m"
# define P_THINK "\x1b[1;35mis thinking\x1b[0m"
# define P_FORK "\x1b[1;32mhas taken a fork\x1b[0m"
# define P_DIED "\x1b[1;31mdied\x1b[0m"

int	main()
{
	printf("%s\n", P_EAT);
	return (0);
}