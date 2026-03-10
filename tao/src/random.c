/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbez--du <tbez--du@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 19:18:55 by tbez--du          #+#    #+#             */
/*   Updated: 2026/03/09 19:33:52 by tbez--du         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

double	random_double(void)
{
	int			fd;
	uint64_t	buff;

	fd = open("/dev/urandom", O_RDONLY);
	read(fd, &buff, sizeof(buff));
	return ((double)buff / (double)UINT64_MAX);
}
