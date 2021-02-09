#include "cpuemulator.h"

static size_t	get_size(FILE *fd)
{
	size_t file_size;

	fseek(fd, 0, SEEK_END);
	file_size = ftell(fd);
	rewind(fd);
	return (file_size);
}

static void		copy_file(FILE *fd, char *file_content)
{
	int		comment_line;
	int		prev_exist;

	comment_line = 0;
	prev_exist = 0;
	while (fread(file_content, 1, 1, fd))
	{
		if (*file_content == '\n')
		{
			file_content++;
			comment_line = 0;
			prev_exist = 1;
		}
		else if (comment_line || isspace(*file_content))
			continue ;
		else
		{
			if (prev_exist
				&& *file_content == '/'
				&& *(file_content - 1) == '/')
			{
				file_content--;
				comment_line = 1;
				prev_exist = 0;
			}
			else
			{
				file_content++;
				prev_exist = 1;
			}
		}
	}
	*file_content = '\0';
}

static int		correct_instrusctions(char **instructions)
{
	int	i;
	int	j;

	i = 0;
	if (!instructions || !instructions[0])
		return (errorExit(ERR_NO_INSTR));
	while (instructions[i])
	{
		if (strlen(instructions[i]) != INSTR_SIZE)
		{
			printf("Line %d: ", i);
			return (errorExit(ERR_INSTR_SIZE));
		}
		j = 0;
		while (j < INSTR_SIZE)
		{
			if (instructions[i][j] != '0' && instructions[i][j] != '1')
			{
				printf("Line %d: ", i);
				return (errorExit(ERR_INSTR));
			}
			j++;
		}
		i++;
	}
	return (1);
}

char			**set_instructions(FILE *fd)
{
	size_t	file_size;
	char	*file_content;
	char	**instructions;

	file_size = get_size(fd);
	if (file_size <= 0)
	{
		errorExit(ERR_FILE_EMPTY);
		return (0);
	}
	file_content = malloc(sizeof(char) * (file_size + 1));
	if (!file_content)
	{
		errorExit(-1);
		return (0);
	}
	copy_file(fd, file_content);
	instructions = ft_split(file_content, '\n');
	free(file_content);
	if(!correct_instrusctions(instructions))
	{
		ft_free_split(instructions);
		return (0);
	}
	return (instructions);
}