/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbez--du <tbez--du@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 17:18:19 by tbez--du          #+#    #+#             */
/*   Updated: 2026/03/10 17:04:52 by tbez--du         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	total_pixel = 0;
pthread_mutex_t	pixel_lock;

int		create_scene(t_data *data)
{
	data->scene.s = calloc(7, sizeof(t_sphere));
	if (!data->scene.s)
		return (0);

	data->scene.s_n = 6;
	data->scene.s[0].center = (t_vec3){0.0, 0.0, -55.0};
	data->scene.s[0].radius = 20.0;
	data->scene.s[0].albedo = (t_vec3){1.0, 1.0, 1.0};
	data->scene.s[0].state = 0;
	
	data->scene.s[6].center = (t_vec3){0.0, 0.0, -55.0};
	data->scene.s[6].radius = 20.0;
	data->scene.s[6].albedo = (t_vec3){1.0, 1.0, 1.0};
	data->scene.s[6].state = 0;

	// 			FRONT
	data->scene.s[1].center = (t_vec3){0.0, 0.0, -2000.0 - 100};
	data->scene.s[1].radius = 2000.0;
	data->scene.s[1].albedo = (t_vec3){.0, 1.0, 1.0};
	data->scene.s[1].state = 0;

	// 			R1GHT
	data->scene.s[2].center = (t_vec3){2000 + 50.0, 0.0, 0.0};
	data->scene.s[2].radius = 2000.0;
	data->scene.s[2].albedo = (t_vec3){.0, .0, 1.0};
	data->scene.s[2].state = 0;

	// 			LEFT
	data->scene.s[3].center = (t_vec3){-2000-50.0, 0.0, 0.0};
	data->scene.s[3].radius = 2000.0;
	data->scene.s[3].albedo = (t_vec3){.0, 1.0, .0};
	data->scene.s[3].state = 0;

	// 			UP
	data->scene.s[4].center = (t_vec3){0.0, 2000.0 + 100, 0.0};
	data->scene.s[4].radius = 2000.0;
	data->scene.s[4].albedo = (t_vec3){1.0, 1.0, 1.0};
	data->scene.s[4].state = 0;

	// 			DOWN
	data->scene.s[5].center = (t_vec3){0.0, -2000.0 - 20, 0.0};
	data->scene.s[5].radius = 2000.0;
	data->scene.s[5].albedo = (t_vec3){1.0, 1.0, 1.0};
	data->scene.s[5].state = 0;

	data->scene.light.position = (t_vec3){15.0, 60.0, -40};
	data->scene.light.intensity = 100000000;

	data->scene.camera.fov = 90.0 * M_PI / 180.0;

	data->scene.camera.origin = (t_vec3){0.0, 0.0, 0.0};
	data->scene.camera.dir = (t_vec3){0.0, 0.0, -1.0};
	return (1);
}

void	put_pixel(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->mlx.addr + (y * data->mlx.line_len + x * (data->mlx.bpp / 8));
	*(unsigned int *)dst = color;
}

int	has_inter_s(t_sphere s, t_ray ray, t_poly *poly)
{
	poly->a = 1;
	poly->b = 2 * vec_dot(ray.dir, vec_sub(ray.origin, s.center));
	poly->c = vec_norm2(vec_sub(ray.origin, s.center)) - s.radius*s.radius;
	poly->delta = poly->b * poly->b - 4 * poly->a * poly->c;
	if (poly->delta < 0)
		return (0);
	poly->t1 = (-poly->b - sqrt(poly->delta)) / (2 * poly->a);
	poly->t2 = (-poly->b + sqrt(poly->delta)) / (2 * poly->a);
	if (poly->t2 > 0)
		return (1);
	return (0);
}

int	inter_s(t_sphere s, t_ray ray, t_vec3 *P, t_vec3 *N, double *t)
{
	t_poly poly;

	if (!has_inter_s(s, ray, &poly))
		return (0);
	if (poly.t1 > 0)
		*t = poly.t1;
	else
		*t = poly.t2;
	*P = vec_add(ray.origin, vec_prod(*t, ray.dir));
	*N = vec_normalize(vec_sub(*P, s.center));
	return (1);
}

int	inter_ss(t_scene scene, t_ray ray, t_vec3 *P, t_vec3 *N, int *s_id, double *min_t)
{
	t_vec3	lN, lP;
	double	lt;
	*min_t = 1E99;
	int		flag = 0;

	for (int i = 0; i < scene.s_n; i++)
	{
		if (inter_s(scene.s[i], ray, &lP, &lN, &lt))
		{
			flag = 1;
			if (lt < *min_t)
			{
				*min_t = lt;
				*N = lN;
				*P = lP;
				*s_id = i;
			}
		}
	}
	return (flag);
}

int	color_format(t_vec3 color)
{
//	color = (t_vec3){pow(color.x, 1.0/2.2), pow(color.y, 1.0/2.2), pow(color.z, 1.0/2.2)};
	//color = (t_vec3){fmin(255, fmax(0, color.x)), fmin(255, fmax(0, color.y)), fmin(255, fmax(0, color.z))};
	return ((unsigned char)color.x << 16 | (unsigned char)color.y << 8 | (unsigned char)color.z);
}

int	light_inter(t_scene s, t_vec3 P, t_vec3 N)
{
	t_ray	ray;
	double	t;
	int		id;
	t_vec3	lN, lP;

	ray.origin = vec_add(P, vec_prod(0.001, N));
	ray.dir = vec_normalize(vec_sub(s.light.position, P));
	if (inter_ss(s, ray, &lP, &lN, &id, &t))
	{
		if (t*t < vec_norm2(vec_sub(s.light.position, P)))
			return (1);
	}
	return (0);
}

/*t_vec3	get_color(t_data *data, t_ray ray, int max_booing)
{
	t_vec3	N, P;
	int		s_id;
	double	t;

	if (max_booing == 0)
		return ((t_vec3){0, 0, 0});
	//if (!inter_s(data->scene.s[0], ray, &P, &N, &t))
	if (!inter_ss(data->scene, ray, &P, &N, &s_id, &t))
		return ((t_vec3){0, 0, 0});

	if (data->scene.s[s_id].state == MIRROR)
	{
		t_ray m_ray;
		m_ray.origin = vec_add(P, vec_prod(0.001, N));
		m_ray.dir = vec_sub(ray.dir, vec_prod(2*vec_dot(N, ray.dir), N));
		return (get_color(data, m_ray, max_booing - 1));
	}
	else if (data->scene.s[s_id].state == TRANSPARENT)
	{
		t_ray r_ray;
		double	n1 = 1, n2 = 1.3;
		t_vec3 Nt = N;
		if (vec_dot(ray.dir, N) > 0)
		{n1 = 1.3; n2 = 1; Nt = vec_prod(-1, N);}
		r_ray.origin = vec_sub(P, vec_prod(0.001, Nt));
		double radical = 1- pow(n1/n2, 2)*(1-pow(vec_dot(Nt, ray.dir), 2));
		if (radical > 0)
		{
			r_ray.dir = vec_sub(vec_prod(n1/n2, vec_sub(ray.dir, vec_prod(vec_dot(ray.dir, Nt), Nt))), vec_prod(sqrt(radical), Nt));
			return (get_color(data, r_ray, max_booing - 1));
		}
	}

	// 		ECLAIRAGE DIRECT
	double p_I = data->scene.light.intensity * fmax(0, vec_dot(vec_normalize(vec_sub(data->scene.light.position, P)), N)) / vec_norm2(vec_sub(data->scene.light.position, P));
	if (light_inter(data->scene, P, N))
		p_I = 0;
	t_vec3 color = (t_vec3){fmin(255, fmax(0, pow(p_I, 1/2.2))), fmin(255, fmax(0, pow(p_I, 1/2.2))), fmin(255, fmax(0, pow(p_I, 1/2.2)))};
	//color = vec_prodv(data->scene.s[s_id].albedo, color);

	// 		ECLAIRAGE INDIRECT
	double r1 = random_double(), r2 = random_double();
	t_vec3 rand_dir = (t_vec3){cos(2 * M_PI * r1)*sqrt(1 - r2), sin(2 * M_PI * r1)*sqrt(1 - r2), sqrt(r2)};
	t_vec3 rand = (t_vec3){random_double(), random_double(), random_double()};
	t_vec3 t1 = vec_cross(N, rand); t1 = vec_normalize(t1);
	t_vec3 t2 = vec_cross(t1, N);

	t_ray rand_ray;
	rand_ray.dir = vec_add(vec_add(vec_prod(rand_dir.z, N), vec_prod(rand_dir.x, t1)), vec_prod(rand_dir.y, t2));
	rand_ray.origin = vec_add(P, vec_prod(0.001, N));
	color = vec_add(color, vec_prodv(get_color(data, rand_ray, max_booing - 1), data->scene.s[s_id].albedo));
	return (color);
}*/

t_vec3	get_color(t_data *data, t_ray ray, int max_booing)
{
	if (max_booing == 0) return (t_vec3){0, 0, 0};

	t_vec3	P, N;
	int		s_id;
	double	t;
	int		has_inter = inter_ss(data->scene, ray, &P, &N, &s_id, &t);
	t_vec3	color = (t_vec3){0, 0, 0};

	if (has_inter)
	{
		if (data->scene.s[s_id].state == MIRROR)
		{
			t_ray	m_ray;
			m_ray.dir = vec_sub(ray.dir, vec_prod(2*vec_dot(N, ray.dir), N));
			m_ray.origin = vec_add(P, vec_prod(0.001, N));
			color = get_color(data, m_ray, max_booing - 1);
		}
		else if (data->scene.s[s_id].state == TRANSPARENT)
		{
			double n1 = 1, n2 = 1.3;
			t_vec3 Nt = N;
			if (vec_dot(ray.dir, N) > 0)
			{
				n1 = 1.3; n2 = 1;
				Nt = vec_prod(-1, N);
			}

			double radical = 1- pow(n1/n2, 2)*(1-pow(vec_dot(Nt, ray.dir), 2));
			if (radical > 0)
			{
				t_ray r_ray;
				r_ray.dir = vec_sub(vec_prod(n1/n2, vec_sub(ray.dir, vec_prod(vec_dot(ray.dir, Nt), Nt))), vec_prod(sqrt(radical), Nt));
				r_ray.origin = vec_sub(P, vec_prod(0.001, Nt));
				color = get_color(data, r_ray, max_booing - 1);
			}
		}
		else
		{
			t_ray l_ray;
			l_ray.dir = vec_normalize(vec_sub(data->scene.light.position, P));
			l_ray.origin = vec_add(P, vec_prod(0.001, N));
			t_vec3	Pl, Nl;
			int		sl_id;
			double	tl;
			int		has_inter_light = inter_ss(data->scene, l_ray, &Pl, &Nl, &sl_id, &tl);
			double	d_light2 = vec_norm2(vec_sub(data->scene.light.position, P));
			if (has_inter_light && tl*tl < d_light2)
				color = (t_vec3){0, 0, 0};
			else
			{
				double	prod = data->scene.light.intensity * fmax(0.0, vec_dot(vec_normalize(vec_sub(data->scene.light.position, P)), N)) / d_light2;
				prod = fmin(255, fmax(0, pow(prod, 1.0/2.2)));
				color = vec_prod(prod, data->scene.s[s_id].albedo);
			}
			
			double r1 = random_double(), r2 = random_double();
			t_vec3 rand_dir = (t_vec3){cos(2 * M_PI * r1)*sqrt(1 - r2), sin(2 * M_PI * r1)*sqrt(1 - r2), sqrt(r2)};
			t_vec3 rand = (t_vec3){random_double() - 0.5, random_double() - 0.5, random_double() - 0.5};
			t_vec3 t1 = vec_cross(N, rand); t1 = vec_normalize(t1);
			t_vec3 t2 = vec_cross(t1, N);
			
			t_ray	rand_ray;
			rand_ray.dir = vec_add(vec_add(vec_prod(rand_dir.z, N), vec_prod(rand_dir.x, t1)), vec_prod(rand_dir.y, t2));
			rand_ray.origin = vec_add(P, vec_prod(0.001, N));
			t_vec3	tmp = vec_prodv(get_color(data, rand_ray, max_booing - 1), data->scene.s[s_id].albedo);
			color = vec_add(color, vec_prod(0.5, tmp));
		}
	}
	return color;
}

void	*routine(void *arg)
{
	t_data *data;
	t_thread *thread;

	thread = (t_thread *)arg;
	data = thread->data;
	for (int x = 0; x < WIN_W; x++)
	{
		t_ray	ray;
		ray.origin = data->scene.camera.origin;
		ray.dir	= vec_normalize((t_vec3){x - WIN_W / 2, thread->y - WIN_H / 2, -WIN_W / (2 * tan(data->scene.camera.fov / 2))});
		t_vec3	res = (t_vec3){0, 0, 0};
		for (int z = 0; z < MAX_RAYS; z++)
			res = vec_prod(1.0/(double)MAX_RAYS, vec_add(res, get_color(data, ray, MAX_REBOND)));
		int	color = color_format(res);
		put_pixel(data, x, WIN_H - thread->y, color);
		pthread_mutex_lock(&pixel_lock);
		total_pixel++;
		pthread_mutex_unlock(&pixel_lock);
	}
	return (NULL);
}

void	compute(t_data *data)
{
	if (!create_scene(data) || !create_thread(data))
	{
		perror("rt");
		return ;
	}
	int	last_pixel = 0;
	while (1)
	{
		pthread_mutex_lock(&pixel_lock);
		if (total_pixel != last_pixel)
		{
			printf("%d / %d\n", total_pixel, WIN_H * WIN_W);
			last_pixel = total_pixel;
		}
		pthread_mutex_unlock(&pixel_lock);
		if (total_pixel <= last_pixel)
			break ;
	}
	
	/*
	for (int y = 0; y < WIN_H; y++)
	{
		for (int x = 0; x < WIN_W; x++)
		{
			t_ray	ray;
			ray.origin = data->scene.camera.origin;
			ray.dir	= vec_normalize((t_vec3){x - WIN_W / 2, y - WIN_H / 2, -WIN_W / (2 * tan(data->scene.camera.fov / 2))});
			int	color = color_format(get_color(data, ray));
			put_pixel(data, x, WIN_H - y, color);
		}
	}
	*/
	join_thread(data);
}

int	main(void)
{
	t_data data;

	if (!init_mlx(&data) || !get_mlx_image(&data))
	{
		destroy_mlx(&data);
		return (0);
	}
	pthread_mutex_init(&pixel_lock, NULL);
	set_hook(&data);
	compute(&data);
	//printf("%f\n", random_double());
	printf("complete\n");
	mlx_put_image_to_window(data.mlx.mlx, data.mlx.win, data.mlx.img, 0, 0);
	mlx_loop(data.mlx.mlx);
	destroy_mlx(&data);
}
