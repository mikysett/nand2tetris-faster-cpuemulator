#include "cpuemulator.h"

static size_t	count_instr(char **instr)
{
	char	**instr_ptr;

	if (!instr)
		return (0);
	instr_ptr = instr;
	while (*instr)
		instr++;
	return (instr - instr_ptr);
}

static void		init_ram(int ram[RAM_SIZE])
{
	int i;

	i = 0;
	while (i < RAM_SIZE)
	{
		ram[i] = 0;
		i++;
	}
}

void			init_computer(char **instr, char *prog_name)
{
	size_t	instr_nb;
	int		ram[RAM_SIZE];

	instr_nb = count_instr(instr);
	if (instr_nb <= 0 || instr_nb > ROM_MAX_SIZE)
	{
		errorExit(ERR_MAX_ROM);
		return ;
	}
	init_ram(ram);
	run_computer(prog_name, instr_nb, instr, ram);
}