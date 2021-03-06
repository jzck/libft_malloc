/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_internal.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhalford <jack@crans.org>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/17 23:00:24 by jhalford          #+#    #+#             */
/*   Updated: 2017/10/22 17:17:13 by jhalford         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_INTERNAL_H
# define MALLOC_INTERNAL_H

# define M_NTINY			((size_t)64)
# define M_NSMALL			((size_t)1024)
# define M_PAGEALIGN(x)		((x / getpagesize() + 1) * getpagesize()) * 10
# define M_TINYCHUNK		(M_PAGEALIGN(101 * M_NTINY))
# define M_SMALLCHUNK		(M_PAGEALIGN(101 * M_NSMALL))
# define M_CHUNKHEAD		(sizeof(t_chunk))
# define M_NODEHEAD			(sizeof(t_node))
# define M_ISTINY(x)		(x < (M_NTINY + 1))
# define M_ISSMALL(x)		(!M_ISTINY(x) && !M_ISLARGE(x))
# define M_ISLARGE(x)		(M_NSMALL < x)
# define NEXT(node)			(node->islast ? NULL : (void*)node + node->size)

# define FT_ABS(x)			(((x) < 0) ? -(x) : x)

# define DP_N(n)	ft_putnbr_fd(n, 2)
# define DP_H(n)	ft_putnbr_hex_fd(n, 2)
# define DP_C(n)	ft_putchar_fd(n, 2)
# define DP_S(n)	ft_putstr_fd(n, 2)
# define DGPID		DP_S("===");DP_N(getpid());DP_S("===");
# define DGW(d)		DGPID;d;DP_C('\n')
# define DGS(s)		do { DGW(DP_S(" "s)); } while(0)
# define DGSN(s, n)	do { DGW(DP_S(" "s"=");DP_N(n)); } while(0)
# define DGSH(s, n)	do { DGW(DP_S(" "s"=");DP_H(n)); } while(0)

# define FG_DEFAULT		"\e[0m"
# define ON_BOLD		"\e[1m"
# define ON_UNDERLINED	"\e[4m"
# define ON_INVERTED	"\e[7m"
# define FG_BLACK		"\e[30m"
# define FG_RED			"\e[31m"
# define FG_GREEN		"\e[32m"
# define FG_YELLOW		"\e[33m"
# define FG_BLUE		"\e[34m"
# define FG_MAGENTA		"\e[35m"
# define FG_CYAN		"\e[36m"
# define BG_BLACK		"\e[40m"
# define BG_RED			"\e[41m"
# define BG_GREEN		"\e[42m"
# define BG_YELLOW		"\e[43m"
# define BG_BLUE		"\e[44m"
# define BG_MAGENTA		"\e[45m"
# define BG_CYAN		"\e[46m"
# define BG_DEFAULT		"\e[49m"
# define FBG_DEFAULT	FG_DEFAULT BG_DEFAULT

# include <sys/mman.h>
# include <stdalign.h>
# include <unistd.h>
# include <pthread.h>
# include "malloc.h"

typedef struct		s_chunk {
	struct s_chunk	*next;
}					t_chunk;

typedef struct		s_node {
	size_t			size;
	unsigned int	isfree:1;
	unsigned int	islast:1;
}					t_node;

enum				e_zones {
	M_TINY,
	M_SMALL,
	M_LARGE,
	M_ZONES_MAX,
};

extern t_chunk			*g_zones[M_ZONES_MAX];
extern pthread_mutex_t	g_mutex;
extern int				g_malloc_debug;
/*
**	malloc_debug levels:
**		1: show malloc/free calls with arguments
**		2: show chunks before and after malloc/free, notify when coalescing.
**		3: show mmap values
*/

int					ft_free(void *ptr);
void				*ft_malloc(size_t size);
void				*ft_realloc(void *ptr, size_t size);
void				*ft_reallocf(void *ptr, size_t size);
void				*ft_calloc(size_t count, size_t size);
int					ret_free(void *ptr);

t_chunk				**get_zone(size_t size);
t_node				*find_node_firstfit(t_chunk *chunk, size_t size);
t_node				*find_prev_node(t_chunk *zone, t_node *node);
int					split_node(t_node *node, size_t size);

void				*hexdump(void *addr, unsigned int offset,
						unsigned int size);
void				show_chunk(char *name, t_chunk *chunk, int dump);

void				error_mmap(void);
void				error_free_notalloc(void *ptr);

int					ft_putchar(char c);
int					ft_putchar_fd(char c, int fd);
int					ft_putendl(char const *s);
int					ft_putendl_fd(char const *s, int fd);
int					ft_putstr(char const *s);
int					ft_putstr_fd(char const *s, int fd);

int					ft_putnbr(long n);
int					ft_putnbr_fd(long n, int fd);
int					ft_putnbr_hex(long n);
int					ft_putnbr_hex_fd(long n, int fd);
int					ft_putnbr_loop(long n, int base, int fd);

void				*ft_memcpy(void *dst, const void *src, size_t n);
void				*ft_memset(void *b, int c, size_t len);

#endif
