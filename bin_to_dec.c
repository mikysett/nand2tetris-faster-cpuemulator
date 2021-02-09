#include "cpuemulator.h"

int	bin_to_dec(char *str, size_t nb_digits, int *powers_table)
{
	int i;
	int res;

	i = 0;
	res = 0;
	while (nb_digits > 0)
	{
		nb_digits--;
		res += (str[i] - '0') * powers_table[nb_digits];
		i++;
	}
	return (res);
}
