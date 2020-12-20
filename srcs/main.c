#include <stdio.h>

int main(int argc, char **argv, char **envv)
{
	int i;
	i = 0;
	while (*(envv + i))
	{
		printf("%s\n", *(envv + i++));
	}
}
