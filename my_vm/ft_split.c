#include "vmtranslator.h"

void			ft_free_split(char **split_result)
{
	int i;

	if (!split_result)
		return ;
	i = 0;
	while (split_result[i])
	{
		free(split_result[i]);
		i++;
	}
	free(split_result);
}

static int		ft_nb_strs(char const *s, char c)
{
	int nb_s;

	if (!*s)
		return (0);
	s++;
	nb_s = 0;
	while (*s)
	{
		if (*s == c && *(s - 1) != c)
		{
			nb_s++;
			s++;
			if (!*s)
				break;
		}
		s++;
	}
	if (*(s - 1) != c)
		return (nb_s + 1);
	return (nb_s);
}

static void		ft_set_strs_size(int *strs_size, char const *str, char c)
{
	int		s_size;

	s_size = 0;
	while (*str)
	{
		if (*str != c)
			s_size++;
		else if (s_size != 0)
		{
			*strs_size = s_size + 1;
			strs_size++;
			s_size = 0;
		}
		str++;
	}
	if (s_size != 0)
		*strs_size = s_size + 1;
}

static void		ft_cpy_strs(char **strs, char const *str, char c)
{
	int		j;

	j = 0;
	while (*str)
	{
		if (*str != c)
			(*(strs))[j++] = *str;
		else if (j != 0)
		{
			(*(strs++))[j] = '\0';
			j = 0;
		}
		str++;
	}
	if (j != 0)
		(*(strs++))[j] = '\0';
	*(strs) = 0;
}

static char		**ft_free_exit(char **res, int *strs_size)
{
	ft_free_split(res);
	if (strs_size)
		free(strs_size);
	return (0);
}

char			**ft_split(char const *s, char c)
{
	int		nb_strs;
	int		*strs_size;
	char	**res;

	if (!s)
		return (0);
	while (*s == c)
		s++;
	nb_strs = ft_nb_strs(s, c);
	if (!(res = malloc(sizeof(char *) * (nb_strs + 1))))
		return (0);
	if (nb_strs)
	{
		if (!(strs_size = malloc(sizeof(int) * (nb_strs))))
			return (ft_free_exit(res, 0));
		ft_set_strs_size(strs_size, s, c);
		while (--nb_strs >= 0)
			if (!(res[nb_strs] = malloc(sizeof(char) * strs_size[nb_strs])))
				return (ft_free_exit(res, strs_size));
		free(strs_size);
	}
	ft_cpy_strs(res, s, c);
	return (res);
}
