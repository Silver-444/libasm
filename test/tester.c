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

static char *make_str(size_t len, int c)
{
    char *s = malloc(len + 1);
	// protection malloc
    memset(s, c, len);
    s[len] = 0;
    return s;
}

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


/////// FT_STRCPY ///////
static void test_strcpy_basic(void)
{
	const char *t[] = { "", "a", "hello", "Hello, World!", "\x80\xff\x01", NULL };
    for (int i = 0; t[i]; i++) {
        char dst[64];
        memset(dst, 0xAA, sizeof dst);
        char *r = ft_strcpy(dst, t[i]);
        size_t l = strlen(t[i]);
		if (r != dst)
			printf(RED " [KO] " RST "wrong return value ");
		else if (memcmp(dst, t[i], l + 1) != 0)
			printf(RED " [KO] " RST "dst content incorrect ");
		else if ((unsigned char)dst[l + 1] != 0xAA)
			printf(RED " [KO] " RST "writting after \\0 ");
		else
			printf(GREEN " [OK] " RST);
        printf("result : %s | expected : %s\n", dst, t[i]);
    }
}

static void test_strcpy_embedded_nul(void)
{
	const char src[] = "abc\0def";
	size_t l = strlen(src);
	char dst[16];
	memset(dst, 0xAA, sizeof dst);
    ft_strcpy(dst, src);
	if (memcmp(dst, src, l + 1) != 0)
		printf(RED " [KO] " RST "dst content incorrect ");
	else if ((unsigned char)dst[l + 1] != 0xAA)
		printf(RED " [KO] " RST "writting after \\0 ");
	else
		printf(GREEN " [OK] " RST);
    printf("result : %s | expected : %s\n", dst, src);
}

static void test_strcpy_long(void)
{
    size_t n = 10 * 1024 * 1024;
    char *src = make_str(n, 'b');
	char *dst = malloc(n + 1);
	//protection malloc
    char *r = ft_strcpy(dst, src);
	if (r != dst)
		printf(RED " [KO] " RST "return value incorrect ");
	else if (memcmp(dst, src, n + 1) != 0)
		printf(RED " [KO] " RST "dst content incorrect ");
    else
		printf(GREEN " [OK] " RST);
	printf("result lenght: %ld | expected length: %ld\n", strlen(r), strlen(src));
    free(src); 
	free(dst);
}

static void test_strcpy(void)
{
	printf(BOLD "ft_strcpy\n" RST);
	test_strcpy_basic();
	test_strcpy_embedded_nul();
	test_strcpy_long();
	printf("\n");
}

int	main()
{
	printf(BOLD "=== LIBASM TESTER ===\n\n" RST);
	test_strlen();
	test_strcpy();
}
