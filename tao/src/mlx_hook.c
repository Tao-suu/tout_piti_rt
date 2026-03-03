/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_hook.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbez--du <tbez--du@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 13:46:54 by tbez--du          #+#    #+#             */
/*   Updated: 2026/03/03 13:47:16 by tbez--du         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	quit_hook(t_data *data)
{
	mlx_loop_end(data->mlx.mlx);
	return (1);
}

int	loop_hook(t_data *data)
{
	(void)data;
	return (1);
}
 
int	key_hook(int key, t_data *data)
{
	if (key == 0xff1b)
		quit_hook(data);
	return (0);
}

void	set_hook(t_data *data)
{
	mlx_hook(data->mlx.win, 17, 0, quit_hook, data);
	mlx_loop_hook(data->mlx.mlx, loop_hook, data);
	mlx_key_hook(data->mlx.win, key_hook, data);
}
