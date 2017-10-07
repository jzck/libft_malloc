/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhalford <jack@crans.org>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/17 12:28:02 by jhalford          #+#    #+#             */
/*   Updated: 2017/10/07 17:10:38 by jhalford         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_internal.h"

t_chunk		*g_zones[M_ZONES_MAX] =
{
	NULL,
	NULL,
	NULL,
};

volatile int			g_malloc_debug = 0;

t_chunk		**get_zone(size_t size)
{
	if (M_ISLARGE(size))
		return (&g_zones[M_LARGE]);
	else if (M_ISSMALL(size))
		return (&g_zones[M_SMALL]);
	else
		return (&g_zones[M_TINY]);
}

void		add_chunk(t_chunk **chunk, size_t size)
{
	int		chunk_size;
	t_chunk	*new;
	t_node	*node;

	if (M_ISLARGE(size))
		chunk_size = M_PAGEALIGN(size);
	else
		chunk_size = M_ISTINY(size) ? M_TINYCHUNK : M_SMALLCHUNK;
	chunk_size += M_CHUNKHEAD;
	if (!(new = mmap(NULL, chunk_size, PROT_READ | PROT_WRITE,
					MAP_ANON | MAP_PRIVATE, -1, 0)))
		error_mmap();
	if (g_malloc_debug >= 3)
		DGSN("malloc_debug", g_malloc_debug);
	new->next = *chunk;
	*chunk = new;
	node = (t_node*)(*chunk + 1);
	node->size = chunk_size - M_CHUNKHEAD;
	node->isfree = 1;
	node->islast = 1;
}

void		*ft_malloc(size_t size)
{
	t_chunk	**zone;
	t_node	*node;
	void	*ret;

	g_malloc_debug = 1;
	if (g_malloc_debug >= 1)
		DGSN("malloc called with size", size);
	if (g_malloc_debug >= 2)
	{
		DGSN("malloc", size);
		show_alloc_mem();
	}
	size += M_NODEHEAD;
	zone = get_zone(size);
	while (!(node = find_node_firstfit(*zone, size)))
		add_chunk(zone, size);
	split_node(node, size);
	ret = (void*)(node + 1);
	if (g_malloc_debug >= 1)
	{
		DGSH("user got ptr", (long)ret);
		show_alloc_mem();
	}
	return (ret);
}