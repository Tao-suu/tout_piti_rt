/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbez--du <tbez--du@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 19:18:55 by tbez--du          #+#    #+#             */
/*   Updated: 2026/03/10 16:37:29 by tbez--du         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

double	random_double(void)
{
	static int	fd = -1;
	uint64_t	buff;

	if (fd < 0)
		fd = open("/dev/urandom", O_RDONLY);
	read(fd, &buff, sizeof(buff));
	return ((double)buff / (double)UINT64_MAX);
}
