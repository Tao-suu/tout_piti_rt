/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbez--du <tbez--du@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 17:18:19 by tbez--du          #+#    #+#             */
/*   Updated: 2026/03/07 17:58:10 by tbez--du         ###   ########.fr       */
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

int		inter_ss(t_sphere *s, int n, t_ray ray, t_vec3 *P, t_vec3 *N, int *s_id, double *min_t)
{
	int	has_inter = 0;
	*min_t = 1E99;

	for (int i = 0; i < n; i++)
	{
		t_vec3	lP, lN;
		double	t;
		int lhas_inter = inter_s(s[i], ray, &lP, &lN, &t);
		if (lhas_inter)
		{
			has_inter = 1;
			if (t < *min_t) 
			{
				*s_id = i;
				*min_t = t;
				*P = lP;
				*N = lN;
			}
		}
	}
	return (has_inter);
}

double	rand_double(void)
{
	int fd = open("/dev/urandom", O_RDONLY);
	uint64_t r;
	read(fd, &r, sizeof(r));
	close(fd);
	return ((double)r / (double)UINT64_MAX);
}

int		has_inter_s(t_sphere s, t_ray ray, double *t)
{
	t_poly poly;

	poly.a = 1;
	poly.b = 2 * vec_dot(ray.dir, vec_sub(ray.origin, s.center));
	poly.c = vec_norm2(vec_sub(ray.origin, s.center)) - s.radius * s.radius;
	poly.delta = poly.b*poly.b - 4 * poly.a * poly.c;
	if (poly.delta < 0)
		return (0);
	*t = (-poly.b + sqrt(poly.delta)) / (2.0 * poly.a);
	if (*t < 0)
		return (0);
	return (1);
}

int		has_inter_ss(t_scene *s, t_ray ray, double *min_t)
{
	double	t;
	int		has_inter;
	int		i;

	*min_t = 1E99;
	has_inter = 0;
	i = 0;
	while (i < s->s_n)
	{
		if (has_inter_s(s->s[i], ray, &t))
		{
			has_inter = 1;
			if (t < *min_t)
				*min_t = t;
		}
		i++;
	}
	return (has_inter);
}

int		color_format(t_vec3 color)
{
	return (00 << 24 | (unsigned char)pow(color.x * 255, 1) << 16
			| (unsigned char)pow(color.y * 255, 1) << 8 |(unsigned char)pow(color.z * 255, 1));
}

t_vec3		get_color(t_scene scene, t_ray ray, int max_boing)
{
	t_vec3	N, P;
	int		s_id;
	double	t;
	int		has_inter;
	t_vec3	p_In;
	
	if (max_boing == 0)
		return ((t_vec3){0, 0, 0});
	has_inter = inter_ss(scene.s, scene.s_n, ray, &P, &N, &s_id, &t);
	if (has_inter)
	{
		if (scene.s[s_id].state == MIRROR)
		{
			t_ray	mir_ray;
			t_vec3 mir_dir = vec_sub(ray.dir, vec_prod(2 * vec_dot(N, ray.dir), N));
			mir_ray.origin = vec_add(P, vec_prod(0.001, N));
			mir_ray.dir = mir_dir;
			return (get_color(scene, mir_ray, --max_boing));
		}
		else if (scene.s[s_id].state == TRANSPARENT)
		{
			double	n1 = 1;
			double	n2 = 1.3;
			t_vec3	N_p = N;
			if (vec_dot(ray.dir, N) > 0)
			{ // on sort de la bouboule
				n1 = n2;
				n2 = 1;
				N_p = vec_prod(-1, N);
			}
			double	radical = 1 - pow(n1 / n2, 2)*(1 - pow(vec_dot(N_p, ray.dir), 2));
			if (radical > 0)
			{
				t_vec3	ref_dir = vec_sub(vec_prod(n1/n2, vec_sub(ray.dir, vec_prod(vec_dot(ray.dir, N_p), N_p))), vec_prod(sqrt(radical), N_p));
				t_ray	ref_ray = (t_ray){vec_sub(P, vec_prod(0.001, N_p)), ref_dir};
				return (get_color(scene, ref_ray, --max_boing));
			}
		}
		else
		{
		//		ECLAIRAGE DIRECT
			t_ray	light_ray = (t_ray){vec_add(P, vec_prod(0.01, N)), vec_normalize(vec_sub(scene.light.position, P))};
			double	lt;
			double	has_inter_l = has_inter_ss(&scene, light_ray, &lt);
			double	d_light2 = vec_norm2(vec_sub(scene.light.position, P));
			if (has_inter_l && lt*lt < d_light2)
				p_In = (t_vec3){0, 0, 0};
			else
			{
				t_vec3	s1 = vec_prod(scene.light.intensity, vec_prod(1 / M_PI, scene.s[s_id].albedo));
				double	s2 = fmax(0.0, vec_dot(vec_normalize(vec_sub(scene.light.position, P)), N));
				t_vec3	s3 = vec_prod(s2, s1);
				p_In = (t_vec3){s3.x / d_light2, s3.y / d_light2, s3.z / d_light2};
				p_In.x = fmin(1.0, fmax(0.0, p_In.x));
				p_In.y = fmin(1.0, fmax(0.0, p_In.y));
				p_In.z = fmin(1.0, fmax(0.0, p_In.z));
			}

		//		ECLAIRAGE INDIRECT

			return (p_In);
			//t_vec3	rand_dir = vec_sub(ray.dir, vec_prod(2 * vec_dot(N, ray.dir), N));
			//t_ray	rand_ray = (t_ray){vec_add(P, vec_prod(0.01, N)), mirror_dir};
//			return (0 << 24 | (unsigned char)(p_In.x * 255) << 16 | (unsigned char)(p_In.y * 255) << 8 | (unsigned char)(p_In.z * 255));
			//return (color_format(scene.s[s_id].albedo, p_I));
		}
	}
	return ((t_vec3){0, 0, 0});
}

int		create_scene(t_data *data)
{
	data->scene.s = calloc(7, sizeof(t_sphere));
	if (!data->scene.s)
		return (0);

	data->scene.s_n = 7;
	data->scene.s[0].center = (t_vec3){-10.0, 10.0, -55.0};
	data->scene.s[0].radius = 10.0;
	data->scene.s[0].albedo = (t_vec3){1.0, 0.0, 0.0};
	data->scene.s[0].state = 1;

	data->scene.s[6].center = (t_vec3){15.0, 0.0, -55.0};
	data->scene.s[6].radius = 10.0;
	data->scene.s[6].albedo = (t_vec3){1.0, 1.0, 0.0};
	data->scene.s[6].state = 2;

	data->scene.s[1].center = (t_vec3){0.0, -2000.0 - 20.0, 0.0};
	data->scene.s[1].radius = 2000.0;
	data->scene.s[1].albedo = (t_vec3){1.0, 1.0, 1.0};
	data->scene.s[1].state = 0;

	data->scene.s[2].center = (t_vec3){0.0, 2000.0 + 100.0, 0.0};
	data->scene.s[2].radius = 2000.0;
	data->scene.s[2].albedo = (t_vec3){1.0, 1.0, 1.0};
	data->scene.s[2].state = 0;

	data->scene.s[3].center = (t_vec3){-2000-50, 0.0, 0.0};
	data->scene.s[3].radius = 2000.0;
	data->scene.s[3].albedo = (t_vec3){1.0, 0.0, 0.0};
	data->scene.s[3].state = 0;

	data->scene.s[4].center = (t_vec3){2000+50, 0.0 , 0.0};
	data->scene.s[4].radius = 2000.0;
	data->scene.s[4].albedo = (t_vec3){0.0, 1.0, 0.0};
	data->scene.s[4].state = 0;

	data->scene.s[5].center = (t_vec3){0.0, 0.0 , -2000-100};
	data->scene.s[5].radius = 2000.0;
	data->scene.s[5].albedo = (t_vec3){0.0, 1.0, 1.0};
	data->scene.s[5].state = 0;

	data->scene.light.position = (t_vec3){15.0, 70.0, -30};
	data->scene.light.intensity = 10000;

	data->scene.camera.fov = 60.0 * M_PI / 180.0;

	data->scene.camera.origin = (t_vec3){0.0, 0.0, 0.0};
	data->scene.camera.dir = (t_vec3){0.0, 0.0, -1.0};
	return (1);
}

void	compute(t_data *data)
{
	if (!create_scene(data))
	{
		perror("rt");
		return ;
	}
	for (int x = 0; x < 800; x++)
	{
		for (int y = 0; y < 800; y++)
		{
			t_ray	ray;
			ray.dir = (t_vec3){x - 800 / 2.0, y - 800 / 2.0, -800 / (2 * tan(data->scene.camera.fov / 2.0))};
			ray.dir = vec_normalize(ray.dir);
			ray.origin = data->scene.camera.origin;

			t_vec3	color = get_color(data->scene, ray, 5);
			printf("(%f | %f | %f)\n", 255 * color.x, 255 * color.y, 255 * color.z);
			mlx_pixel_put(data->mlx.mlx, data->mlx.win, x, 800 - y, color_format(color));
		}
	}
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
	mlx_loop(data.mlx.mlx);

	destroy_mlx(&data);
}
