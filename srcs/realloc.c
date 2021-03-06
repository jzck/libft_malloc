/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhalford <jack@crans.org>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/18 13:23:20 by jhalford          #+#    #+#             */
/*   Updated: 2017/10/22 17:28:05 by jhalford         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_internal.h"

void	*ft_realloc(void *ptr, size_t size)
{
	void	*new;
	size_t	old_size;

	if (!ptr)
		return (ft_malloc(size));
	if (ft_free(ptr))
		return (NULL);
	old_size = ((t_node*)(ptr - M_NODEHEAD))->size;
	if (!(new = ft_malloc(size)))
		return (NULL);
	ft_memcpy(new, ptr, old_size);
	return (new);
}
