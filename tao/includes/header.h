/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbez--du <tbez--du@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 13:40:30 by tbez--du          #+#    #+#             */
/*   Updated: 2026/03/07 13:56:20 by tbez--du         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_H
# define HEADER_H

# include <fcntl.h>
# include <unistd.h>
# include <stdint.h>
# include <limits.h>
# include <mlx.h>
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include "struct.h"

	//	MLX_BASE
int		init_mlx(t_data *data);
int		get_mlx_image(t_data *data);
void	destroy_mlx(t_data *data);

	//	MLX_HOOK
int		quit_hook(t_data *data);
int		loop_hook(t_data *data);
int		key_hook(int key, t_data *data);
void	set_hook(t_data *data);

	//	VEC_UTILS
double	vec_dot(t_vec3 a, t_vec3 b);
double	vec_norm2(t_vec3 a);
t_vec3	vec_add(t_vec3 a, t_vec3 b);
t_vec3	vec_sub(t_vec3 a, t_vec3 b);
t_vec3	vec_prod(double factor, t_vec3 a);
t_vec3	vec_prodv(t_vec3 a, t_vec3 b);
t_vec3	vec_normalize(t_vec3 a);
t_vec3	vec_subd(t_vec3 a, double i);
t_vec3	vec_cross(t_vec3 a, t_vec3 b);

#endif
