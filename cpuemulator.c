// This emulators reads the instructions from left to right,
// the LSB will be instruction[15] and the MSB will be instruction[0]
// when in the book instr[15] is mentioned, here you will see instr[0]
// Doesn't change anything in the way you compile assembly or anything else

#include "cpuemulator.h"

int			errorExit(int err)
{
	switch (err)
	{
		case ERR_NO_ARG:
			printf("Error: Please provide a .hack, .asm or .vm file (for this one a directory is also fine)\n");
			return (0);
		case ERR_OPEN_FAIL:
			printf("Error: Can't open the file\n");
			return (0);
		case ERR_FILE_EMPTY:
			printf("Error: The file is empty\n");
			return (0);
		case ERR_NO_INSTR:
			printf("Error: No instructions found in the file\n");
			return (0);
		case ERR_INSTR_SIZE:
			printf("Error: Wrong instruction length\n");
			return (0);
		case ERR_INSTR:
			printf("Error: Invalid instruction\n");
			return (0);
		case ERR_MAX_ROM:
			printf("Error: Number of instructions exceed the ROM size\n");
			return (0);
		case ERR_PC_OUT:
			printf("Error: Program counter out of range\n");
			return (0);
		case ERR_BAD_M:
			printf("Error: Try to access inlegal range for M\n");
			return (0);
		case ERR_INIT_TUI:
			printf("Error: Can't initialize the UI\n");
			return (0);
		default:
			printf("Error: Unknown error occurred\n");
			return (0);
	}
}

DEBUG_CODE(
void	ft_print_instructions(char **instructions)
{
	int i;

	printf("    ----> INSTRUCTIONS IN MEMORY\n");
	if (!instructions)
		return ;
	i = 0;
	while (instructions[i])
	{
		printf("%10d: %s\n", i, instructions[i]);
		i++;
	}
})

int			main(int argc, char **argv)
{
	FILE	*srcFile;
	char	**instructions;
	char	*file_name;

	if (argc != 2)
		return (errorExit(ERR_NO_ARG));

	file_name = create_hack_file(argv[1]);
	srcFile = fopen(file_name, "r");
	if (!srcFile)
		return (errorExit(ERR_OPEN_FAIL));
	instructions = set_instructions(srcFile);
	fclose(srcFile);
	if (!instructions)
		return (0);
	
	DEBUG_CODE(ft_print_instructions(instructions);)

	init_computer(instructions, file_name);

	ft_free_split(instructions);
	free(file_name);
	return (0);
}