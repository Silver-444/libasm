#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <malloc.h>

#include "libasm.h"

#define GREEN "\033[32m"
#define RED   "\033[31m"
#define YEL   "\033[33m"
#define BOLD  "\033[1m"
#define RST   "\033[0m"

/////// FT_STRLEN ///////
static void test_strlen(void)
{
	printf(BOLD "ft_strlen\n" RST);
    const char *t[] = { "", "a", "ab", "hello", "Hello, World!", "42", "   ",
        "\t\n\r", "\x80\xff\x01\x7f", "0123456789012345678901234567890123456789", 
		"abc\0def", "\0abc", NULL
	};
    for (int i = 0; t[i]; i++)
	{
		if (ft_strlen(t[i]) != strlen(t[i]))
			printf(RED " [KO] " RST);
		else
			printf(GREEN " [OK] " RST);
		printf("result : %zu | expected :%zu\n", ft_strlen(t[i]), strlen(t[i]));
	}
	printf("\n");
}

int	main()
{
	printf(BOLD "=== LIBASM TESTER ===\n\n" RST);
	test_strlen();
}
