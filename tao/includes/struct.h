/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbez--du <tbez--du@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 13:43:12 by tbez--du          #+#    #+#             */
/*   Updated: 2026/03/03 17:42:05 by tbez--du         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

typedef struct {
	void	*mlx;
	void	*img;
	void	*win;
	int		bpp;
	int		end;
	int		line_len;
	char	*addr;
}	t_mlx;

typedef struct {
	t_mlx	mlx;
}	t_data;

typedef struct {
	float	x;
	float	y;
	float	z;
}	t_vec3;

typedef struct {
	float	radius;
	t_vec3	center;
	t_vec3	color;
	t_vec3	albedo;
}	t_sphere;

typedef struct {
	t_vec3	origin;
	t_vec3	dir;
}	t_ray;

#endif
