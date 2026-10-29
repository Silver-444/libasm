# libasm

A small C-callable library written in x86-64 assembly (NASM, Intel syntax). It re-implements a handful of libc functions, plus a few linked-list and number-parsing helpers for the bonus part.

## Description

The goal of the project is to get comfortable with assembly: the System V AMD64 calling convention, registers, the stack, system calls, error handling through `errno`, and calling C functions (`malloc`, `free`, `__errno_location`) from assembly.

### Functions

**Mandatory**

| Function    | Prototype                                                 | Behaves like |
|-------------|-----------------------------------------------------------|--------------|
| `ft_strlen` | `size_t ft_strlen(const char *s)`                         | `strlen(3)`  |
| `ft_strcpy` | `char *ft_strcpy(char *dst, const char *src)`             | `strcpy(3)`  |
| `ft_strcmp` | `int ft_strcmp(const char *s1, const char *s2)`           | `strcmp(3)`  |
| `ft_write`  | `ssize_t ft_write(int fd, const void *buf, size_t count)` | `write(2)`   |
| `ft_read`   | `ssize_t ft_read(int fd, void *buf, size_t count)`        | `read(2)`    |
| `ft_strdup` | `char *ft_strdup(const char *s)`                          | `strdup(3)`  |

**Bonus**

| Function             | Prototype                                                                                             | Description                                                                                                   |
|----------------------|-------------------------------------------------------------------------------------------------------|---------------------------------------------------------------------------------------------------------------|
| `ft_atoi_base`       | `int ft_atoi_base(char *str, char *base)`                                                             | Converts `str` to an `int`, using `base` as the digit alphabet                                                |
| `ft_list_push_front` | `void ft_list_push_front(t_list **begin_list, void *data)`                                            | Adds a new node at the beginning of the list                                                                  |
| `ft_list_size`       | `int ft_list_size(t_list *begin_list)`                                                                | Returns the number of nodes                                                                                   |
| `ft_list_sort`       | `void ft_list_sort(t_list **begin_list, int (*cmp)())`                                                | Sorts the list in ascending order according to `cmp`                                                          |
| `ft_list_remove_if`  | `void ft_list_remove_if(t_list **begin_list, void *data_ref, int (*cmp)(), void (*free_fct)(void *))` | Removes every node whose data matches `data_ref` (`cmp(data, data_ref) == 0`), calling `free_fct` on its data |

The list type is:

```c
typedef struct s_list
{
	void			*data;
	struct s_list	*next;
}	t_list;
```

### Behavior details

- `ft_write` and `ft_read` return `-1` on error and set `errno` to the value reported by the kernel (for example `EBADF` on a bad file descriptor).
- `ft_strdup` returns `NULL` and sets `errno` to `ENOMEM` if the allocation fails. The returned pointer must be released with `free`.
- `ft_atoi_base` skips leading whitespace, accepts at most one sign (`+` or `-`), and stops at the first character that is not in the base. A string with several signs (`--42`) gives `0`. It returns `0` if the base is invalid: fewer than 2 characters, a repeated character, or a `+`, `-` or whitespace character in it.
- `ft_list_sort` and `ft_list_remove_if` take the comparison as a function pointer, like `strcmp`: it returns `0` when both values are equal.

## Instructions

### Requirements

- Linux, x86-64
- `nasm`
- `ar`
- a C compiler (`cc`, `gcc` or `clang`)

### Build

```sh
make          # builds libasm.a (mandatory part)
make bonus    # builds libasm.a with the bonus functions
make clean    # removes object files
make fclean   # removes object files and libasm.a
make re       # fclean + all
```

### Use the library

Include the header and link against `libasm.a`:

```c
#include "libasm.h"

int main(void)
{
	char *s = ft_strdup("hello");
	ft_write(1, s, ft_strlen(s));
	free(s);
	return (0);
}
```

```sh
cc main.c -L. -lasm -o main
```

### Tester

A tester (`tester.c`) compares each function against its libc counterpart. It checks return values and `errno`, edge cases (empty strings, embedded `\0`, non-ASCII bytes, very large inputs, invalid file descriptors, `NULL` pointers, allocation failure), and prints the lists it sorts or filters so a failure is easy to read.

The tester links every function, bonus included, so build the library with `make bonus` first:

```sh
make bonus
cc -Wall -Wextra tester.c -L. -lasm -o tester
```

```sh
./tester                          # mandatory part (default)
./tester all                      # mandatory + bonus
./tester bonus                    # bonus part only
./tester ft_strlen strdup         # one or more functions (ft_ prefix optional)
./tester --help                   # list of available tests
```

Each test line is prefixed with `[OK]` or `[KO]`. To also check memory handling of the list functions, run the tester under valgrind:

```sh
valgrind --leak-check=full ./tester all
```

## Resources

- [System V AMD64 ABI](https://gitlab.com/x86-psABIs/x86-64-ABI): calling convention
- [NASM documentation](https://www.nasm.us/docs.php)
- [Intel 64 and IA-32 Architectures Software Developer's Manual](https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html)

**Use of AI:** AI was used to help write the tester (test cases and output formatting) and this README. The assembly code was written by the author.
