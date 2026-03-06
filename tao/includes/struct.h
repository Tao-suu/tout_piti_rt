/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbez--du <tbez--du@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 13:43:12 by tbez--du          #+#    #+#             */
/*   Updated: 2026/03/06 15:59:10 by tbez--du         ###   ########.fr       */
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

typedef struct {
	t_vec3	origin;
	t_vec3	dir;
	double	fov;
}	t_camera;

typedef struct {
	t_vec3	position;
	double	intensity;
}	t_light;

typedef struct {
	t_sphere	*s;
	int			s_n;
	t_camera	camera;
	t_light		light;
}	t_scene;

typedef struct {
	t_mlx	mlx;
	t_scene	scene;
}	t_data;

typedef struct {
	double	a;
	double	b;
	double	c;
	double	delta;
	double	t1;
	double	t2;
}	t_poly;

#endif
