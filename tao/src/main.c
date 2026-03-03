/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbez--du <tbez--du@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 17:18:19 by tbez--du          #+#    #+#             */
/*   Updated: 2026/03/03 19:01:54 by tbez--du         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int		inter_s(t_sphere s, t_ray ray, t_vec3 *P, t_vec3 *N, double *t)
{
	double	a = 1;
	double	b = 2 * vec_dot(ray.dir, vec_sub(ray.origin, s.center));
	double	c = vec_norm2(vec_sub(ray.origin, s.center)) - s.radius * s.radius;

	double delta = b * b - 4 * a * c;
	if (delta < 0)
		return (0);
	double t1 = (-b - sqrt(delta)) / (2.0 * a);
	double t2 = (-b + sqrt(delta)) / (2.0 * a);
	if (t2 < 0)
		return (0);
	if (t1 > 0)
		*t = t1;
	else
		*t = t2;
	*P = vec_add(ray.origin, vec_prod(*t, ray.dir));
	*N = vec_normalize(vec_sub(*P, s.center));
	return (1);
}

int		inter_ss(t_sphere *s, int n, t_ray ray, t_vec3 *P, t_vec3 *N, int *s_id)
{
	int	has_inter = 0;
	double min_t = 1E99;

	for (int i = 0; i < n; i++)
	{
		t_vec3	lP, lN;
		double	t;
		int lhas_inter = inter_s(s[i], ray, &lP, &lN, &t);
		if (lhas_inter)
		{
			has_inter = 1;
			if (t < min_t) 
			{
				*s_id = i;
				min_t = t;
				*P = lP;
				*N = lN;
			}
		}
	}
	return (has_inter);
}



void	compute(t_data *data)
{
	t_sphere	s[6];

	s[0].center = (t_vec3){0.0, 0.0, -55.0};
	s[0].radius = 20.0;
	s[0].albedo = (t_vec3){1.0, 0.0, 0.0};

	s[1].center = (t_vec3){0.0, -2000.0 - 20.0, 0.0};
	s[1].radius = 2000.0;
	s[1].albedo = (t_vec3){1.0, 1.0, 1.0};

	s[2].center = (t_vec3){0.0, 2000.0 + 100.0, 0.0};
	s[2].radius = 2000.0;
	s[2].albedo = (t_vec3){1.0, 1.0, 1.0};

	s[3].center = (t_vec3){-2000-50, 0.0, 0.0};
	s[3].radius = 2000.0;
	s[3].albedo = (t_vec3){0.0, 1.0, 0.0};

	s[4].center = (t_vec3){2000+50, 0.0 , 0.0};
	s[4].radius = 2000.0;
	s[4].albedo = (t_vec3){0.0, 0.0, 1.0};

	s[5].center = (t_vec3){0.0, 0.0 , -2000-100};
	s[5].radius = 2000.0;
	s[5].albedo = (t_vec3){0.0, 1.0, 1.0};

	t_vec3	l_pos = (t_vec3){15.0, 70.0, 15};
	double	l_I = 1000000;

	double	fov = 60.0 * M_PI / 180.0;

	t_vec3	c_origin = (t_vec3){0.0, 0.0, 0.0};
//	t_vec3	c_dir = (t_vec3){0.0, 0.0, -1.0};

	for (int x = 0; x < 800; x++)
	{
		for (int y = 0; y < 800; y++)
		{
			t_ray	ray;
			ray.dir = (t_vec3){x - 800 / 2.0, y - 800 / 2.0, -800 / (2 * tan(fov / 2.0))};
			ray.dir = vec_normalize(ray.dir);
			ray.origin = c_origin;

			t_vec3	N, P;
			double	p_I = 0;
			int		s_id;

			int		has_inter = inter_ss(s, 6, ray, &P, &N, &s_id);
			if (has_inter)
			{
				p_I = l_I * fmax(0, vec_dot(vec_normalize(vec_sub(l_pos, P)), N)) / vec_norm2(vec_sub(l_pos, P));		

				t_vec3	color = (t_vec3){fmin(255, fmax(0, p_I)), fmin(255, fmax(0, p_I)), fmin(255, fmax(0, p_I))};
				color = vec_prodv(color, s[s_id].albedo);

				int	c = 00 << 24 | (unsigned char)color.x << 16 | (unsigned char)color.y << 8 | (unsigned char)color.z;
				mlx_pixel_put(data->mlx.mlx, data->mlx.win, x, 800 - y, c);
			}
			else
				mlx_pixel_put(data->mlx.mlx, data->mlx.win, x, 800 - y, 0x00000000);
		}
	}

	(void)data;
}

int	main(void)
{
	t_data data;

	if (!init_mlx(&data))
	{
		destroy_mlx(&data);
		return (0);
	}
	if (!get_mlx_image(&data))
	{
		destroy_mlx(&data);
		return (0);
	}
	set_hook(&data);

	compute(&data);
	printf("?\n");
	mlx_loop(data.mlx.mlx);

	destroy_mlx(&data);
}
