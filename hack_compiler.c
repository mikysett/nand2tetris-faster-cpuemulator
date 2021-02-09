#include "cpuemulator.h"

static int	ft_isextension(char *f, char *ext)
{
	int	f_len;
	int	ext_len;

	f_len = strlen(f);
	ext_len = strlen(ext);

	while (--f_len > 0 && --ext_len > 0)
		if (f[f_len] != ext[ext_len])
			return (0);
	return (1);
}

static char	*set_ext_name(char *src, char *ext_in, char *ext_out)
{
	char	*dst_name;
	char	*ext_pos;
	size_t	ext_in_len;
	size_t	ext_out_len;
	size_t	ext_max_len;

	ext_in_len = strlen(ext_in);
	ext_out_len = strlen(ext_out);
	ext_max_len = ext_in_len < ext_out_len ? ext_out_len : ext_in_len;
	dst_name = malloc(sizeof(char) * (strlen(src) + ext_max_len + 1));
	strcpy(dst_name, src);
	ext_pos = strstr(dst_name, ext_in);
	if (!ext_pos)
		return (0);
	strcpy(ext_pos, ext_out);
	return (dst_name);
}

char		*create_hack_file(char *full_path)
{
	char	*final_file;
	char	*buff_file;
	char	*path;
	char	*path_copy;
	char	*file;
	pid_t	pid;

	if (ft_isextension(full_path, ".hack"))
		return (strdup(full_path));
	if (ft_isextension(full_path, ".asm"))
	{
		printf("Creating the .hack dile\n");
		pid = fork();
		if (pid == 0)
			execlp("my_assembler/assembler", "my_assembler/assembler", full_path, NULL);
		wait(NULL);
		return (set_ext_name(full_path, ".asm", ".hack"));
	}
	else // full_path is a directory or a .vm file
	{
		printf("Creating the .asm dile\n");
		pid = fork();
		if (pid == 0)
			execlp("my_vm/VMtranslator", "my_vm/VMtranslator", full_path, NULL);
		wait(NULL);
		if (ft_isextension(full_path, ".vm"))
			buff_file = set_ext_name(full_path, ".vm", ".asm");
		else // The input is a directory
		{
			path_copy = strdup(full_path);
			path = strdup(full_path);
			file = basename(path_copy);
			buff_file = malloc(sizeof(char) * (strlen(path) + strlen(file) + 5));
			strcpy(buff_file, path);
			strcat(buff_file, file);
			strcat(buff_file, ".asm");
			free(path_copy);
			free(path);
		}
		printf("Creating the .hack dile\n");
		pid = fork();
		if (pid == 0)
			execlp("my_assembler/assembler", "my_assembler/assembler", buff_file, NULL);
		wait(NULL);
		final_file = set_ext_name(buff_file, ".asm", ".hack");
		free (buff_file);
		return (final_file);
	}
}