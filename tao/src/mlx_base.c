/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_base.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbez--du <tbez--du@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 13:45:43 by tbez--du          #+#    #+#             */
/*   Updated: 2026/03/03 13:46:24 by tbez--du         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	init_mlx(t_data *data)
{
	data->mlx.mlx = mlx_init();
	if (!data->mlx.mlx)
		return (0);
	data->mlx.win = mlx_new_window(data->mlx.mlx, 800, 800, "poc_tao");
	if (!data->mlx.win)
	{
		mlx_destroy_display(data->mlx.mlx);
		return (0);
	}
	return (1);
}

int	get_mlx_image(t_data *data)
{
	data->mlx.img = mlx_new_image(data->mlx.mlx, 800, 800);
	if (!data->mlx.img)
		return (0);
	data->mlx.addr = mlx_get_data_addr(data->mlx.img, &data->mlx.bpp,
			&data->mlx.line_len, &data->mlx.end);
	if (!data->mlx.addr)
		return (0);
	return (1);
}

void	destroy_mlx(t_data *data)
{
	if (data->mlx.img)
		mlx_destroy_image(data->mlx.mlx, data->mlx.img);
	if (data->mlx.win)
		mlx_destroy_window(data->mlx.mlx, data->mlx.win);
	mlx_destroy_display(data->mlx.mlx);
	free(data->mlx.mlx);
}
