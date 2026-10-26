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
	setrlimit(RLIMIT_AS, &old);	/* on restaure la limite tout de suite */

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

int	main()
{
	printf(BOLD "=== LIBASM TESTER ===\n\n" RST);
	test_strlen();
	test_strcpy();
	test_strcmp();
	test_write();
	test_read();
	test_strdup();
}
