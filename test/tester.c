#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <malloc.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/resource.h>

#include "libasm.h"

#define GREEN "\033[32m"
#define RED   "\033[31m"
#define YEL   "\033[33m"
#define BOLD  "\033[1m"
#define RST   "\033[0m"

/// Utils
static char*	make_str(size_t len, int c)
{
    char *s = malloc(len + 1);
	if (!s)
	{
		printf(RED "malloc tester failed\n" RST);
		exit(1);
	}
    memset(s, c, len);
    s[len] = 0;
    return s;
}

static void	print_status(int ok)
{
	if (ok)
		printf(GREEN " [OK] " RST);
	else
		printf(RED " [KO] " RST);
}

/////// FT_STRLEN ///////
static void	test_strlen_basic(void)
{
    const char *t[] = { "", "a", "ab", "hello", "Hello, World!", "42", "   ",
        "\t\n\r", "\x80\xff\x01\x7f", "0123456789012345678901234567890123456789", 
		"abc\0def", &"abc\0def"[4], "\0abc", NULL
	};
    for (int i = 0; t[i]; i++)
	{
		print_status(ft_strlen(t[i]) == strlen(t[i]));
		printf("result : %zu | expected :%zu\n", ft_strlen(t[i]), strlen(t[i]));
	}
}

static void	test_strlen_long(void)
{
    size_t n = 10 * 1024 * 1024;
    char *s = make_str(n, 'a');
	print_status(ft_strlen(s) == strlen(s));
	printf("result : %zu | expected :%zu\n", ft_strlen(s), strlen(s));
    free(s);
}

static void	test_strlen(void)
{
	printf(BOLD "ft_strlen\n" RST);
	test_strlen_basic();
	test_strlen_long();
	printf("\n");
}

/////// FT_STRCPY ///////
static void	test_strcpy_basic(void)
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

static void	test_strcpy_embedded_nul(void)
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

static void	test_strcpy_long(void)
{
    size_t n = 10 * 1024 * 1024;
    char *src = make_str(n, 'b');
	char *dst = malloc(n + 1);
	if (!dst)
	{
		free(src);
		printf(RED "malloc tester failed\n" RST);
		return ;
	}
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

static void	test_strcpy(void)
{
	printf(BOLD "ft_strcpy\n" RST);
	test_strcpy_basic();
	test_strcpy_embedded_nul();
	test_strcpy_long();
	printf("\n");
}

/////// FT_STRCMP ///////
static void	test_strcmp_basic(void)
{
    const char *p[][2] = {
        {"", ""}, {"", "a"}, {"a", ""}, {"a", "a"}, {"a", "b"}, {"b", "a"},
        {"abc", "abc"}, {"abc", "abd"}, {"abd", "abc"}, {"abc", "abcd"},
        {"abcd", "abc"}, {"Hello", "hello"}, {"hello", "Hello"},
        {"abc\0x", "abc\0y"}, {"  ", " "}, {"\t", " "}, {"0", "1"}, 
		{"zzzzzzzzzzzzzzzzzzzz", "zzzzzzzzzzzzzzzzzzzy"},
        {"azzzzzzzzzzzzzzzzzzzz", "bzzzzzzzzzzzzzzzzzzzz"},
		{"\x80", "\x7f"}, {"\x7f", "\x80"}, {"\xff", "a"}, {"a", "\xff"},
		{"\xff", "\xfe"}, {"abc\xff", "abc\x01"}, {"\xe9", ""}, 
        {NULL, NULL}
    };
    for (int i = 0; p[i][0]; i++) 
	{
		print_status(ft_strcmp(p[i][0], p[i][1]) == strcmp(p[i][0], p[i][1]));
        printf("result : %d | expected : %d\n", ft_strcmp(p[i][0], p[i][1]), strcmp(p[i][0], p[i][1]));
    }
}

static void	test_strcmp_long(void)
{
    size_t n = 5 * 1024 * 1024;
    char *a = make_str(n, 'a');
	char *b = make_str(n, 'a');
	print_status(ft_strcmp(a, b) == strcmp(a, b));
	printf("identical | result : %d | expected : %d\n", ft_strcmp(a, b), strcmp(a, b));
	b[n - 1] = 'b';
	print_status(ft_strcmp(a, b) == strcmp(a, b));
	printf("last char differs | result : %d | expected : %d\n", ft_strcmp(a, b), strcmp(a, b));
    free(a); 
	free(b);
}

static void	test_strcmp(void)
{
	printf(BOLD "ft_strcmp\n" RST);
	test_strcmp_basic();
	test_strcmp_long();
	printf("\n");
}

/////// FT_WRITE ///////
static void	check_write_err(const char *name, int fd, const void *buf, size_t count)
{
	errno = 0;
	ssize_t e = write(fd, buf, count);
	int e_errno = errno;
	errno = 0;
	ssize_t r = ft_write(fd, buf, count);
	int r_errno = errno;

	print_status(r == e && r_errno == e_errno);
	printf("%-22s | ret : %zd (exp %zd) | errno : %d (exp %d)\n", name, r, e, r_errno, e_errno);
}

static void	test_write_pipe(void)
{
	int		fd[2];
	char	buf[64];
	ssize_t	r;

	if (pipe(fd) == -1)
	{
		printf(RED "pipe failed\n" RST);
		return ;
	}

	// basic
	r = ft_write(fd[1], "hello", 5);
	memset(buf, 0, sizeof buf);
	ssize_t rd = read(fd[0], buf, sizeof buf);
	print_status(r == 5 && rd == 5 && memcmp(buf, "hello", 5) == 0);
	printf("%-22s | ret : %zd (exp 5)\n", "pipe \"hello\"", r);

	// count = 0
	r = ft_write(fd[1], "hello", 0);
	print_status(r == 0);
	printf("%-22s | ret : %zd (exp 0)\n", "count = 0", r);

	// with '\0' in the middle of the string
	r = ft_write(fd[1], "abc\0def", 7);
	memset(buf, 0, sizeof buf);
	rd = read(fd[0], buf, sizeof buf);
	print_status(r == 7 && rd == 7 && memcmp(buf, "abc\0def", 7) == 0);
	printf("%-22s | ret : %zd (exp 7)\n", "embedded \\0", r);

	// writting only a part of a string 
	r = ft_write(fd[1], "hello", 3);
	memset(buf, 0, sizeof buf);
	rd = read(fd[0], buf, sizeof buf);
	print_status(r == 3 && rd == 3 && memcmp(buf, "hel", 3) == 0);
	printf("%-22s | ret : %zd (exp 3)\n", "partial (3 of 5)", r);

	// writting non-ASCII
	r = ft_write(fd[1], "\x80\xff\x01", 3);
	memset(buf, 0, sizeof buf);
	rd = read(fd[0], buf, sizeof buf);
	print_status(r == 3 && rd == 3 && memcmp(buf, "\x80\xff\x01", 3) == 0);
	printf("%-22s | ret : %zd (exp 3)\n", "non-ascii bytes", r);

	close(fd[0]);
	close(fd[1]);
}

static void	test_write_file(void)
{
	const char	*path = "/tmp/libasm_tester_write.tmp";
	size_t		n = 1024 * 1024;
	char		*src = make_str(n, 'x');
	char		*back = malloc(n);
	int			fd = open(path, O_RDWR | O_CREAT | O_TRUNC, 0600);

	if (!back || fd < 0)
	{
		printf(RED "write test setup failed\n" RST);
		free(src);
		free(back);
		if (fd >= 0)
			close(fd);
		return ;
	}
	ssize_t r = ft_write(fd, src, n);
	lseek(fd, 0, SEEK_SET);
	ssize_t rd = read(fd, back, n);
	print_status(r == (ssize_t)n && rd == (ssize_t)n && memcmp(src, back, n) == 0);
	printf("%-22s | ret : %zd (exp %zu)\n", "1MB to file", r, n);
	close(fd);
	unlink(path);
	free(src);
	free(back);
}

static void	test_write_stdout(void)
{
	fflush(stdout);
	ssize_t r = ft_write(1, " [stdout test]\n", 15);
	print_status(r == 15);
	printf("%-22s | ret : %zd (exp 15)\n", "write to stdout", r);
}

static void	test_write_errors(void)
{
	int	ro = open("/dev/null", O_RDONLY);
	int	closed = open("/dev/null", O_WRONLY);

	if (closed >= 0)
		close(closed);

	check_write_err("fd = -1", -1, "hello", 5);
	check_write_err("closed fd", closed, "hello", 5);
	check_write_err("fd = 9999", 9999, "hello", 5);
	check_write_err("read-only fd", ro, "hello", 5);
	check_write_err("NULL buffer", 1, NULL, 5);
	check_write_err("NULL buffer, bad fd", -1, NULL, 5);

	errno = 0;
	ssize_t r = ft_write(-1, "x", 1);
	print_status(r == -1 && errno == EBADF);
	printf("%-22s | ret : %zd (exp -1) | errno : %d (EBADF = %d)\n", "returns -1 + EBADF", r, errno, EBADF);

	int fd[2];
	if (pipe(fd) == 0)
	{
		void (*old)(int) = signal(13, SIG_IGN);
		close(fd[0]);
		check_write_err("broken pipe (EPIPE)", fd[1], "hello", 5);
		signal(13, old);
		close(fd[1]);
	}
	if (ro >= 0)
		close(ro);
}

static void	test_write(void)
{
	printf(BOLD "ft_write\n" RST);
	test_write_pipe();
	test_write_file();
	test_write_stdout();
	test_write_errors();
	printf("\n");
}

/////// FT_READ ///////
static void	check_read_err(const char *name, int fd, void *buf, size_t count)
{
	errno = 0;
	ssize_t e = read(fd, buf, count);
	int e_errno = errno;
	errno = 0;
	ssize_t r = ft_read(fd, buf, count);
	int r_errno = errno;

	print_status(r == e && r_errno == e_errno);
	printf("%-22s | ret : %zd (exp %zd) | errno : %d (exp %d)\n",
		name, r, e, r_errno, e_errno);
}

static void	test_read_pipe(void)
{
	int		fd[2];
	char	buf[64];
	ssize_t	r;

	if (pipe(fd) == -1)
	{
		printf(RED "pipe failed\n" RST);
		return ;
	}

	// basic
	write(fd[1], "hello", 5);
	memset(buf, 0xAA, sizeof buf);
	r = ft_read(fd[0], buf, sizeof buf);
	print_status(r == 5 && memcmp(buf, "hello", 5) == 0
		&& (unsigned char)buf[5] == 0xAA);
	printf("%-22s | ret : %zd (exp 5)\n", "pipe \"hello\"", r);

	// reading only a part of the string
	write(fd[1], "hello", 5);
	memset(buf, 0xAA, sizeof buf);
	r = ft_read(fd[0], buf, 3);
	print_status(r == 3 && memcmp(buf, "hel", 3) == 0
		&& (unsigned char)buf[3] == 0xAA);
	printf("%-22s | ret : %zd (exp 3)\n", "partial (3 of 5)", r);
	r = ft_read(fd[0], buf, sizeof buf);
	print_status(r == 2 && memcmp(buf, "lo", 2) == 0);
	printf("%-22s | ret : %zd (exp 2)\n", "remaining (2)", r);

	// count = 0
	write(fd[1], "x", 1);
	r = ft_read(fd[0], buf, 0);
	print_status(r == 0);
	printf("%-22s | ret : %zd (exp 0)\n", "count = 0", r);
	ft_read(fd[0], buf, 1);

	// non-ASCII + '\0' 
	write(fd[1], "abc\0def\xff", 8);
	memset(buf, 0xAA, sizeof buf);
	r = ft_read(fd[0], buf, sizeof buf);
	print_status(r == 8 && memcmp(buf, "abc\0def\xff", 8) == 0);
	printf("%-22s | ret : %zd (exp 8)\n", "embedded \\0 + 0xff", r);

	// End Of File
	close(fd[1]);
	r = ft_read(fd[0], buf, sizeof buf);
	print_status(r == 0);
	printf("%-22s | ret : %zd (exp 0)\n", "EOF on pipe", r);
	close(fd[0]);
}

static void	test_read_file(void)
{
	const char	*path = "/tmp/libasm_tester_read.tmp";
	size_t		n = 1024 * 1024;
	char		*src = make_str(n, 'y');
	char		*back = malloc(n);
	int			fd = open(path, O_RDWR | O_CREAT | O_TRUNC, 0600);

	if (!back || fd < 0)
	{
		printf(RED "read test setup failed\n" RST);
		free(src);
		free(back);
		if (fd >= 0)
			close(fd);
		return ;
	}
	write(fd, src, n);
	lseek(fd, 0, SEEK_SET);
	ssize_t r = ft_read(fd, back, n);
	print_status(r == (ssize_t)n && memcmp(src, back, n) == 0);
	printf("%-22s | ret : %zd (exp %zu)\n", "1MB from file", r, n);

	r = ft_read(fd, back, 10);
	print_status(r == 0);
	printf("%-22s | ret : %zd (exp 0)\n", "EOF on file", r);
	close(fd);
	unlink(path);
	free(src);
	free(back);
}

static void	test_read_devices(void)
{
	char	buf[16];
	int		fd = open("/dev/null", O_RDONLY);
	ssize_t	r = ft_read(fd, buf, sizeof buf);

	print_status(r == 0);
	printf("%-22s | ret : %zd (exp 0)\n", "/dev/null", r);
	if (fd >= 0)
		close(fd);

	fd = open("/dev/zero", O_RDONLY);
	memset(buf, 0xAA, sizeof buf);
	r = ft_read(fd, buf, sizeof buf);
	char zeros[16] = {0};
	print_status(r == 16 && memcmp(buf, zeros, 16) == 0);
	printf("%-22s | ret : %zd (exp 16)\n", "/dev/zero", r);
	if (fd >= 0)
		close(fd);
}

static void	test_read_errors(void)
{
	char	buf[16];
	int		wo = open("/dev/null", O_WRONLY);
	int		dir = open(".", O_RDONLY);
	int		closed = open("/dev/null", O_RDONLY);

	if (closed >= 0)
		close(closed);

	check_read_err("fd = -1", -1, buf, sizeof buf);
	check_read_err("closed fd", closed, buf, sizeof buf);
	check_read_err("fd = 9999", 9999, buf, sizeof buf);
	check_read_err("write-only fd", wo, buf, sizeof buf);
	check_read_err("directory (EISDIR)", dir, buf, sizeof buf);

	int fd = open("/dev/zero", O_RDONLY);
	check_read_err("NULL buffer (EFAULT)", fd, NULL, 5);
	if (fd >= 0)
		close(fd);

	errno = 0;
	ssize_t r = ft_read(-1, buf, sizeof buf);
	print_status(r == -1 && errno == EBADF);
	printf("%-22s | ret : %zd (exp -1) | errno : %d (EBADF = %d)\n", "returns -1 + EBADF", r, errno, EBADF);

	if (wo >= 0)
		close(wo);
	if (dir >= 0)
		close(dir);
}

static void	test_read(void)
{
	printf(BOLD "ft_read\n" RST);
	test_read_pipe();
	test_read_file();
	test_read_devices();
	test_read_errors();
	printf("\n");
}

/////// FT_STRDUP ///////
static void test_strdup_basic(void)
{
    const char *t[] = { "", "a", "hello", "Hello, World!", "\x80\xff\x01", "abc\0def", NULL };
    for (int i = 0; t[i]; i++) 
	{
        char *res = ft_strdup(t[i]);
        size_t len = strlen(t[i]);
		if (!res)
		{
			printf(RED " [KO] " RST "return NULL ");
			continue;
		}
		if (res == t[i])
			printf(RED " [KO] " RST "return the same address ");
        else if (memcmp(res, t[i], len + 1) != 0)
			printf(RED " [KO] " RST "wrong content ");
		else if (malloc_usable_size(res) < len + 1)
			printf(RED " [KO] " RST "allocated block too small ");
        else
			printf(GREEN " [OK] " RST);
		printf("case : %s | result : %s\n", t[i], res);
        free(res);
    }
}

static void	test_strdup_long(void)
{
	size_t n = 10 * 1024 * 1024;
	char *s = make_str(n, 'z');
	char *res = ft_strdup(s);

	if (!res)
		printf(RED " [KO] " RST "return NULL\n");
	else
	{
		print_status(res != s && memcmp(res, s, n + 1) == 0);
		printf("result length: %zu | expected length: %zu\n", strlen(res), n);
		free(res);
	}
	free(s);
}

static void	test_strdup_independent(void)
{
	char src[] = "original";
	char *res = ft_strdup(src);

	if (!res)
	{
		printf(RED " [KO] " RST "return NULL\n");
		return ;
	}
	src[0] = 'X';
	print_status(strcmp(res, "original") == 0);
	printf("modify src | result : %s | expected : original\n", res);
	free(res);
}

static size_t	get_vm_size(void)
{
	long	pages = 0;
	FILE	*f = fopen("/proc/self/statm", "r");

	if (!f)
		return (0);
	if (fscanf(f, "%ld", &pages) != 1)
		pages = 0;
	fclose(f);
	return ((size_t)pages * (size_t)sysconf(_SC_PAGESIZE));
}

static void	test_strdup_malloc_fail(void)
{
	struct rlimit	old, lim;
	size_t			n = 64 * 1024 * 1024;
	size_t			vm;
	char			*s = make_str(n, 'm');
	char			*res;

	vm = get_vm_size();
	if (!vm || getrlimit(RLIMIT_AS, &old) != 0)
	{
		printf(YEL " [SKIP] " RST "malloc failure : cannot read process limits\n");
		free(s);
		return ;
	}
	lim = old;
	lim.rlim_cur = vm + 16 * 1024 * 1024;
	if (old.rlim_max != RLIM_INFINITY && lim.rlim_cur > old.rlim_max)
		lim.rlim_cur = old.rlim_max;
	if (setrlimit(RLIMIT_AS, &lim) != 0)
	{
		printf(YEL " [SKIP] " RST "malloc failure : setrlimit failed\n");
		free(s);
		return ;
	}
	errno = 0;
	res = ft_strdup(s);
	int saved_errno = errno;
	setrlimit(RLIMIT_AS, &old);

	if (res)
	{
		printf(RED " [KO] " RST "expected NULL, got a pointer\n");
		free(res);
	}
	else
	{
		print_status(saved_errno == ENOMEM);
		printf("malloc fails | result : NULL | errno : %d (ENOMEM = %d)\n",
			saved_errno, ENOMEM);
	}
	free(s);
}

static void	test_strdup(void)
{
	printf(BOLD "ft_strdup\n" RST);
	test_strdup_basic();
	test_strdup_long();
	test_strdup_independent();
	test_strdup_malloc_fail();
	printf("\n");
}

/********************************* BONUS PART *********************************/

# define LIST_MAX 1000000
# define PRINT_MAX 20
/////// BONUS UTILS ///////

static void	put_escaped(const char *s)
{
	for (; *s; s++)
	{
		switch (*s)
		{
			case '\t': printf("\\t"); break ;
			case '\n': printf("\\n"); break ;
			case '\v': printf("\\v"); break ;
			case '\f': printf("\\f"); break ;
			case '\r': printf("\\r"); break ;
			default: putchar(*s);
		}
	}
}

static t_list	*lst_new(void *data)
{
	t_list *new_node = (t_list *) malloc(sizeof(t_list));
	if (!new_node)
		return NULL;
	new_node->data = data;
	new_node->next = NULL;
	return (new_node);
}

static t_list	*build_list(void **data, size_t n)
{
	t_list	*head = NULL;
	t_list	**cur = &head;
 
	for (size_t i = 0; i < n; i++)
	{
		*cur = lst_new(data[i]);
		cur = &(*cur)->next;
	}
	return (head);
}

static void	print_int_list(t_list **lst)
{
	int		i = 0;
	t_list	*tmp = *lst;

	if (!tmp)
	{
		printf("(empty)");
		return ;
	}
	while (tmp && i < PRINT_MAX)
	{
		if (i != 0)
			printf(" -> ");
		printf("%d", *((int *)tmp->data));
		i++;
		tmp = tmp->next;
	}
	if (tmp)
		printf(" -> ...");
}

static void	print_string_list(t_list **lst)
{
	int		i = 0;
	t_list	*tmp = *lst;

	if (!tmp)
	{
		printf("(empty)");
		return ;
	}
	while (tmp && i < PRINT_MAX)
	{
		if (i != 0)
			printf(" -> ");
		printf("%s", ((char *)tmp->data));
		i++;
		tmp = tmp->next;
	}
	if (tmp)
		printf(" -> ...");
}

static void	print_int_array(const int *v, size_t n)
{
	if (n == 0)
	{
		printf("(empty)");
		return ;
	}
	for (size_t i = 0; i < n && i < PRINT_MAX; i++)
		printf(i ? " -> %d" : "%d", v[i]);
	if (n > PRINT_MAX)
		printf(" -> ...");
}

static void	print_str_array(const char **v, size_t n)
{
	if (n == 0)
	{
		printf("(empty)");
		return ;
	}
	for (size_t i = 0; i < n && i < PRINT_MAX; i++)
		printf(i ? " -> \"%s\"" : "\"%s\"", v[i]);
	if (n > PRINT_MAX)
		printf(" -> ...");
}

static void	free_list(t_list *l)
{
	while (l)
	{
		t_list *next = l->next;
		free(l);
		l = next;
	}
}

static size_t	real_len(t_list *l)
{
	size_t n = 0;
 
	while (l && n < LIST_MAX)
	{
		n++;
		l = l->next;
	}
	return (n);
}

static int	cmp_int(void *a, void *b)
{
	int x = *(int *)a;
	int y = *(int *)b;

	return ((x > y) - (x < y));
}

/////// FT_ATOI_BASE ///////

typedef struct s_atoi_case
{
	const char	*str;
	const char	*base;
	int			expected;
	const char	*desc;
}				t_atoi_case;

static void	test_atoi_base_valid_base(void)
{
	static const t_atoi_case t[] = {
		{"42", "0123456789", 42, "decimal"},
		{"0", "0123456789", 0, "zero"},
		{"0000042", "0123456789", 42, "leading zeros"},
		{"-42", "0123456789", -42, "negative"},
		{"+42", "0123456789", 42, "plus sign"},
		{"--42", "0123456789", 0, "two minus"},
		{"-+-+42", "0123456789", 0, "mixed signs"},
		{"  \t\n\v\f\r42", "0123456789", 42, "leading whitespace"},
		{"  -42", "0123456789", -42, "whitespace then sign"},
		{"42abc", "0123456789", 42, "stops at invalid char"},
		{"4 2", "0123456789", 4, "stops at space"},
		{"abc", "0123456789", 0, "no digit"},
		{"", "0123456789", 0, "empty string"},
		{"-", "0123456789", 0, "sign only"},
		{"- 42", "0123456789", 0, "space after sign"},
		{"2147483647", "0123456789", 2147483647, "INT_MAX"},
		{"-2147483648", "0123456789", -2147483648, "INT_MIN"},
		{"ff", "0123456789abcdef", 255, "hex lower"},
		{"FF", "0123456789abcdef", 0, "hex: case sensitive"},
		{"7FFFFFFF", "0123456789ABCDEF", 2147483647, "hex INT_MAX"},
		{"-80000000", "0123456789ABCDEF", -2147483648, "hex INT_MIN"},
		{"101010", "01", 42, "binary"},
		{"-101010", "01", -42, "negative binary"},
		{"12", "01", 1, "binary stops at '2'"},
		{"52", "01234567", 42, "octal"},
		{"poney", "poneyvif", 668, "word base (8): poney"},
		{"vif", "poneyvif", 375, "word base (8): vif"},
		{"zz", "abcdefghijklmnopqrstuvwxyz", 25 * 26 + 25, "base 26"},
		{"ba", "ab", 2, "base 2 with letters"},
		{NULL, NULL, 0, NULL}
	};

	for (int i = 0; t[i].str; i++)
	{
		int r = ft_atoi_base((char *)t[i].str, (char *)t[i].base);
 
		print_status(r == t[i].expected);
		printf("%-30s | \"", t[i].desc);
		put_escaped(t[i].str);
		printf("\" in \"");
		put_escaped(t[i].base);
		printf("\" | result : %d | expected : %d\n", r, t[i].expected);
	}
}

static void	test_atoi_base_invalid_base(void)
{
	static const t_atoi_case t[] = {
		{"42", "", 0, "invalid base: empty"},
		{"42", "0", 0, "invalid base: size 1"},
		{"42", "00", 0, "invalid base: duplicate (00)"},
		{"42", "0123456789 ", 0, "invalid base: space"},
		{"42", "012\t345", 0, "invalid base: tab"},
		{"42", "012\n345", 0, "invalid base: newline"},
		{"42", "01+", 0, "invalid base: '+'"},
		{"42", "01-", 0, "invalid base: '-'"},
		{"42", "01234567890", 0, "invalid base: dup at the end"},
		{"1", "0112", 0, "invalid base: dup (0112)"},
		{"-1", "aba", 0, "invalid base: dup (aba)"},
		{NULL, NULL, 0, NULL}
	};

	for (int i = 0; t[i].str; i++)
	{
		int r = ft_atoi_base((char *)t[i].str, (char *)t[i].base);
 
		print_status(r == t[i].expected);
		printf("%-30s | \"", t[i].desc);
		put_escaped(t[i].str);
		printf("\" in \"");
		put_escaped(t[i].base);
		printf("\" | result : %d | expected : %d\n", r, t[i].expected);
	}
}

static void	test_atoi_base(void)
{
	printf(BOLD "ft_atoi_base\n" RST);
	test_atoi_base_valid_base();
	test_atoi_base_invalid_base();
	printf("\n");
}

/////// FT_LIST_PUSH_FRONT ///////
static void	test_list_push_front_basic(void)
{
	int		a = 1, b = 2, c = 3;
	t_list	*head = NULL;

	// empty list
	ft_list_push_front(&head, &a);
	print_status(head && head->data == &a && head->next == NULL
		&& real_len(head) == 1);
	printf("%-22s | head->data = &a, next = NULL\n", "push on empty list");

	// add a node 
	t_list *old = head;
	ft_list_push_front(&head, &b);
	print_status(head && head != old && head->data == &b && head->next == old
		&& old->data == &a && real_len(head) == 2);
	printf("%-22s | head->data = &b, next = old head\n", "push 2nd element");

	ft_list_push_front(&head, &c);
	print_status(head && head->data == &c && head->next->data == &b
		&& head->next->next == old && head->next->next->data == &a
		&& head->next->next->next == NULL && real_len(head) == 3);
	printf("%-22s | order : c, b, a\n", "push 3rd element");

	// add node with data NULL
	ft_list_push_front(&head, NULL);
	print_status(head && head->data == NULL && head->next->data == &c
		&& real_len(head) == 4);
	printf("%-22s | head->data = NULL\n", "push NULL data");
	free_list(head);
}

static void test_list_push_front_multiple_pushes(void)
{
	t_list	*head = NULL;
	size_t	n = 1000;
	int		*vals = malloc(n * sizeof(int));
	int		ok = 1;

	if (!vals)
	{
		printf(RED "list_push_front malloc failed\n" RST);
		exit(1);
	}
	head = NULL;
	for (size_t i = 0; i < n; i++)
	{
		vals[i] = (int)i;
		ft_list_push_front(&head, &vals[i]);
	}
	t_list *cur = head;
	for (size_t i = 0; i < n && ok; i++)
	{
		ok = cur && cur->data == &vals[n - 1 - i];
		cur = cur ? cur->next : NULL;
	}
	print_status(ok && cur == NULL && real_len(head) == n);
	printf("%-22s | %zu nodes in reverse order\n", "1000 pushes", n);
	free_list(head);
	free(vals);
}

static void	test_list_push_front(void)
{
	printf(BOLD "ft_list_push_front\n" RST);
	test_list_push_front_basic();
	test_list_push_front_multiple_pushes();
	printf("\n");
}

/////// FT_LIST_SIZE ///////
static void	test_list_size_null(void)
{
	print_status(ft_list_size(NULL) == 0);
	printf("%-22s | result : %d | expected : 0\n", "NULL list", ft_list_size(NULL));
}

static void	test_list_size_basic(void)
{
	size_t	sizes[] = {1, 2, 3, 5, 10, 1000, 100000};
	void	**data;
	int		r;
 
	for (size_t i = 0; i < sizeof(sizes) / sizeof(sizes[0]); i++)
	{
		size_t n = sizes[i];
		data = calloc(n, sizeof(void *));
		if (!data)
		{
			printf(RED "list size malloc failed\n" RST);
			exit(1);
		}
		t_list *l = build_list(data, n);
		r = ft_list_size(l);
		print_status(r == (int)n);
		printf("%-22s | result : %d | expected : %zu\n", "list of N nodes", r, n);
		free_list(l);
		free(data);
	}
 
	int		a = 1, b = 2, c = 3;
	void	*d[] = {&a, &b, &c};
	t_list	*l = build_list(d, 3);
	t_list	*second = l->next;
	ft_list_size(l);
	print_status(real_len(l) == 3 && l->data == &a && l->next == second
		&& second->data == &b && second->next->data == &c);
	printf("%-22s | list unchanged after call\n", "no side effect");
	free_list(l);
}

static void	test_list_size(void)
{
	printf(BOLD "ft_list_size\n" RST);
	test_list_size_null();
	test_list_size_basic();
	printf("\n");
}

/////// FT_LIST_SORT ///////
static void test_list_sort_empty(void)
{
	t_list *head = NULL;
	ft_list_sort(&head, cmp_int);
	print_status(head == NULL);
	printf("%-26s | list stays NULL\n", "empty list");
}

static void	check_sort_int(const char *name, int *in, int *exp, size_t n)
{
	void	*ptrs[PRINT_MAX];
 
	for (size_t i = 0; i < n; i++)
		ptrs[i] = &in[i];
 
	t_list	*head = build_list(ptrs, n);
	ft_list_sort(&head, cmp_int);
 
	int		ok = (real_len(head) == n);
	t_list	*cur = head;
	for (size_t i = 0; i < n && ok; i++, cur = cur->next)
		ok = (*(int *)cur->data == exp[i]);
 
	print_status(ok);
	printf("%s\n", name);
	printf("        input    : ");
	print_int_array(in, n);
	printf("\n%s        result   : " RST, ok ? "" : RED);
	print_int_list(&head);
	printf("\n");
	if (!ok)
	{
		printf("        expected : ");
		print_int_array(exp, n);
		printf("\n");
	}
	free_list(head);
}
 
static void	test_list_sort_int(void)
{
	check_sort_int("single element", (int[]){42}, (int[]){42}, 1);
	check_sort_int("already sorted (2)", (int[]){1, 2}, (int[]){1, 2}, 2);
	check_sort_int("reversed (2)", (int[]){2, 1}, (int[]){1, 2}, 2);
	check_sort_int("unsorted (3)", (int[]){3, 1, 2}, (int[]){1, 2, 3}, 3);
	check_sort_int("already sorted (5)", (int[]){1, 2, 3, 4, 5},
		(int[]){1, 2, 3, 4, 5}, 5);
	check_sort_int("reversed (5)", (int[]){5, 4, 3, 2, 1},
		(int[]){1, 2, 3, 4, 5}, 5);
	check_sort_int("all equal", (int[]){7, 7, 7, 7}, (int[]){7, 7, 7, 7}, 4);
	check_sort_int("duplicates", (int[]){3, 1, 3, 1, 2, 2},
		(int[]){1, 1, 2, 2, 3, 3}, 6);
	check_sort_int("negatives", (int[]){-5, 10, 0, -10, 5, -1, 1},
		(int[]){-10, -5, -1, 0, 1, 5, 10}, 7);
	check_sort_int("INT_MIN / INT_MAX",
		(int[]){2147483647, -2147483647 - 1, 0, 2147483647, -2147483647 - 1},
		(int[]){-2147483647 - 1, -2147483647 - 1, 0, 2147483647, 2147483647}, 5);
	check_sort_int("random values (10)",
		(int[]){42, -7, 13, 0, 99, -50, 13, 8, -7, 21},
		(int[]){-50, -7, -7, 0, 8, 13, 13, 21, 42, 99}, 10);
}

static void	test_list_sort_str(void)
{
	const char	*words[] = {"pear", "apple", "fig", "banana", "apple", "cherry", "Zed", ""};
	const char	*sorted[] = {"", "Zed", "apple", "apple", "banana", "cherry", "fig", "pear"};
	size_t		nw = sizeof(words) / sizeof(words[0]);
	void		*ptrs[8];
	t_list		*head;

	for (size_t i = 0; i < nw; i++)
		ptrs[i] = (void *)words[i];
	head = build_list(ptrs, nw);
	ft_list_sort(&head, ft_strcmp);
	int ok = (real_len(head) == nw);
	t_list *cur = head;
	for (size_t i = 0; i < nw && ok; i++, cur = cur->next)
		ok = (strcmp((char *)cur->data, sorted[i]) == 0);
	print_status(ok);
	printf("strings\n");
	printf("        input    : ");
	print_str_array(words, 8);
	printf("\n%s        result   : " RST, ok ? "" : RED);
	print_string_list(&head);
	printf("\n");
	if (!ok)
	{
		printf("        expected : ");
		print_str_array(sorted, 8);
		printf("\n");
	}
	free_list(head);
}

static void	test_list_sort(void)
{
	printf(BOLD "ft_list_sort\n" RST);
	test_list_sort_empty();
	test_list_sort_int();
	test_list_sort_str();
	printf("\n");
}

/////// FT_LIST_REMOVE_IF ///////
#define FREED_MAX 64
static void		*g_freed[FREED_MAX];
static size_t	g_nfreed;

// Record the number of time the free function is called
static void	record_free(void *p)
{
	if (g_nfreed < FREED_MAX)
		g_freed[g_nfreed] = p;
	g_nfreed++;
}

static void	run_remove_if(const char *name, const char **in, size_t n,const char *ref, const char **exp, size_t nexp)
{
	char	pool[32][8];
	void	*ptrs[32];
	char	ref_buf[8];

	for (size_t i = 0; i < n; i++)
	{
		strncpy(pool[i], in[i], 7);
		pool[i][7] = 0;
		ptrs[i] = pool[i];
	}
	strncpy(ref_buf, ref, 7);
	ref_buf[7] = 0;

	t_list *head = build_list(ptrs, n);
	g_nfreed = 0;
	ft_list_remove_if(&head, ref_buf, ft_strcmp, record_free);

	int		list_ok = (real_len(head) == nexp);
	t_list	*cur = head;
	for (size_t i = 0; i < nexp && list_ok; i++, cur = cur->next)
		list_ok = (strcmp((char *)cur->data, exp[i]) == 0);

	int		free_ok = (g_nfreed == n - nexp);
	for (size_t i = 0; i < g_nfreed && i < FREED_MAX && free_ok; i++)
		free_ok = (g_freed[i] && strcmp((char *)g_freed[i], ref_buf) == 0);

	print_status(list_ok && free_ok);
	printf("%s\n", name);
	printf("        input    : ");
	print_str_array(in, n);
	printf("\n        remove   : \"%s\"\n", ref_buf);
	printf("%s        result   : " RST, list_ok ? "" : RED);
	print_string_list(&head);
	printf("\n");
	if (!list_ok)
	{
		printf("        expected : ");
		print_str_array(exp, nexp);
		printf("\n");
	}
	printf("%s        free_fct : called %zu time(s), expected %zu" RST "\n",
		free_ok ? "" : RED, g_nfreed, n - nexp);
	free_list(head);
}

static void	test_list_remove_if_basic(void)
{
	const char *none[] = {NULL};
	const char *l1[] = {"apple"};
	const char *l2[] = {"pear"};
	const char *l3[] = {"pear", "fig", "plum"};
	const char *l4[] = {"apple", "pear", "fig"};
	const char *l5[] = {"pear", "apple", "fig"};
	const char *l6[] = {"pear", "fig", "apple"};
	const char *l7[] = {"apple", "apple", "apple", "apple"};
	const char *l8[] = {"apple", "apple", "pear", "fig"};
	const char *l9[] = {"pear", "apple", "apple", "apple", "fig"};
	const char *l10[] = {"pear", "fig", "apple", "apple"};
	const char *l11[] = {"apple", "pear", "apple", "fig", "apple"};
	const char *l12[] = {"apple", "pear", "apple", "apple", "fig", "apple", "plum", "apple", "apple"};
	const char *l13[] = {"app", "apple", "applee"};

	const char *e_pear[] = {"pear"};
	const char *e_pear_fig[] = {"pear", "fig"};
	const char *e_pear_fig_plum[] = {"pear", "fig", "plum"};
	const char *e_prefix[] = {"app", "applee"};

	run_remove_if("empty list", none, 0, "apple", none, 0);
	run_remove_if("single element, match", l1, 1, "apple", none, 0);
	run_remove_if("single element, no match", l2, 1, "apple", e_pear, 1);
	run_remove_if("no match", l3, 3, "apple", e_pear_fig_plum, 3);
	run_remove_if("remove head", l4, 3, "apple", e_pear_fig, 2);
	run_remove_if("remove middle", l5, 3, "apple", e_pear_fig, 2);
	run_remove_if("remove tail", l6, 3, "apple", e_pear_fig, 2);
	run_remove_if("remove all", l7, 4, "apple", none, 0);
	run_remove_if("consecutive at head", l8, 4, "apple", e_pear_fig, 2);
	run_remove_if("consecutive in middle", l9, 5, "apple", e_pear_fig, 2);
	run_remove_if("consecutive at tail", l10, 4, "apple", e_pear_fig, 2);
	run_remove_if("alternating", l11, 5, "apple", e_pear_fig, 2);
	run_remove_if("mixed runs", l12, 9, "apple", e_pear_fig_plum, 3);
	run_remove_if("similar strings kept", l13, 3, "apple", e_prefix, 2);
}

static void	test_list_remove_if(void)
{
	printf(BOLD "ft_list_remove_if\n" RST);
	test_list_remove_if_basic();
	printf("\n");
}

/*******************************************************************************/
 
static const t_test	g_tests[] = {
	{"strlen", test_strlen, 0},
	{"strcpy", test_strcpy, 0},
	{"strcmp", test_strcmp, 0},
	{"write", test_write, 0},
	{"read", test_read, 0},
	{"strdup", test_strdup, 0},
	{"atoi_base", test_atoi_base, 1},
	{"list_push_front", test_list_push_front, 1},
	{"list_size", test_list_size, 1},
	{"list_sort", test_list_sort, 1},
	{"list_remove_if", test_list_remove_if, 1},
	{NULL, NULL, 0}
};
 
static void	print_usage(FILE *out, const char *prog)
{
	fprintf(out, "Usage: %s [all | mandatory | bonus | <function> ...]\n\n", prog);
	fprintf(out, "  (no argument)   mandatory tests only\n");
	fprintf(out, "  mandatory       mandatory tests only\n");
	fprintf(out, "  bonus           bonus tests only\n");
	fprintf(out, "  all             mandatory + bonus tests\n");
	fprintf(out, "  <function>      one or more functions, with or without the ft_ prefix\n");
	fprintf(out, "  -h, --help      show this help\n\n");
	fprintf(out, "Functions:\n");
	for (int bonus = 0; bonus <= 1; bonus++)
	{
		fprintf(out, "  %-10s", bonus ? "bonus:" : "mandatory:");
		for (int i = 0; g_tests[i].name; i++)
			if (g_tests[i].bonus == bonus)
				fprintf(out, " %s", g_tests[i].name);
		fprintf(out, "\n");
	}
	fprintf(out, "\nExamples:\n");
	fprintf(out, "  %s                    # mandatory part\n", prog);
	fprintf(out, "  %s all                # everything\n", prog);
	fprintf(out, "  %s ft_strlen strdup   # only these two\n", prog);
}
 
static const t_test	*find_test(const char *arg)
{
	if (strncmp(arg, "ft_", 3) == 0)
		arg += 3;
	for (int i = 0; g_tests[i].name; i++)
		if (strcmp(arg, g_tests[i].name) == 0)
			return (&g_tests[i]);
	return (NULL);
}
 
static void	select_group(int *selected, int mandatory, int bonus)
{
	for (int i = 0; g_tests[i].name; i++)
		if (g_tests[i].bonus ? bonus : mandatory)
			selected[i] = 1;
}
 
int	main(int argc, char **argv)
{
	int	selected[sizeof(g_tests) / sizeof(g_tests[0])] = {0};
	int	bonus_header = 0;
 
	if (argc == 1)
		select_group(selected, 1, 0);
	for (int a = 1; a < argc; a++)
	{
		if (strcmp(argv[a], "-h") == 0 || strcmp(argv[a], "--help") == 0)
		{
			print_usage(stdout, argv[0]);
			return (0);
		}
		else if (strcmp(argv[a], "all") == 0)
			select_group(selected, 1, 1);
		else if (strcmp(argv[a], "mandatory") == 0)
			select_group(selected, 1, 0);
		else if (strcmp(argv[a], "bonus") == 0)
			select_group(selected, 0, 1);
		else
		{
			const t_test *t = find_test(argv[a]);
			if (!t)
			{
				fprintf(stderr, RED "Unknown argument: %s" RST "\n\n", argv[a]);
				print_usage(stderr, argv[0]);
				return (1);
			}
			selected[t - g_tests] = 1;
		}
	}
 
	printf(BOLD "=== LIBASM TESTER ===\n\n" RST);
	for (int i = 0; g_tests[i].name; i++)
	{
		if (!selected[i])
			continue ;
		if (g_tests[i].bonus && !bonus_header)
		{
			printf(BOLD "--- BONUS TESTER ---\n" RST);
			bonus_header = 1;
		}
		g_tests[i].fn();
	}
	return (0);
}
