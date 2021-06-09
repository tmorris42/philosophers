#include <stdio.h>
#include "philo.h"

int	parse_argument(char *arg)
{
	int	i;
	int	value;

	value = 0;
	i = 0;
	while (arg && arg[i])
	{
		if (arg[i] < '0' || arg[i] > '9')
			return (-1);
		value = (value * 10) + (arg[i] - '0');
		++i;
	}
	return (value);
}

int	run(int argc, char **argv)
{
	int	i;
	int	value;
	int	settings[5];

	if (argc < 5 || argc > 6)
		return (-1);
	i = 0;
	while (i < 5)
	{
		settings[i] = -1;
		++i;
	}

	i = 1;
	while (argc > 1 && i < argc)
	{
		printf("Arg #%d = \"%s\"\n", i, argv[i]);
		value = parse_argument(argv[i]);
		if (value < 0)
			printf("\tInvalid argument\n");
		else
			settings[i - 1] = value;
		++i;
	}

	i = 0;
	while (i < 5)
	{
		printf("Setting %d = %d\n", i + 1, settings[i]);
		++i;
	}
	return (1);
}

int	main(int argc, char **argv)
{
	return (run(argc, argv));
}
