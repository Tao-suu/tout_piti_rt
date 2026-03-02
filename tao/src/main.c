/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbez--du <tbez--du@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 17:18:19 by tbez--du          #+#    #+#             */
/*   Updated: 2026/03/02 18:52:04 by tbez--du         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

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
}	t_sphere;

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

double	dot(t_vec3 a, t_vec3 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

double	get_norm2(t_vec3 a)
{
	return (a.x*a.x + a.y*a.y + a.z*a.z);
}

t_vec3	vec3_prod(double factor, t_vec3 a)
{
	t_vec3	tmp;

	tmp = (t_vec3){a.x*factor, a.y*factor, a.z*factor};
	return (tmp);
}

int		inter_s(t_sphere s, t_vec3 ray)
{
	//	P = C + t*u
	//	S: || P - O ||**2 = R **2
	double	a = 1;
	double	b = 2 * dot(ray, vec3_prod(-1, s.center));
	double	c = get_norm2(s.center) - s.radius * s.radius;

	double delta = b * b - 4 * a * c;
	if (delta < 0)
		return (0);
//	double t1 = (-b - sqrt(delta)) / (2.0 * a);
	double t2 = (-b + sqrt(delta)) / (2.0 * a);
	if (t2 > 0)
		return (1);
	return (0);
}

t_vec3	normalize(t_vec3 a)
{
	double	norme = sqrt(get_norm2(a));
	t_vec3 tmp = (t_vec3){a.x / norme, a.y / norme, a.z / norme};
	return tmp;

}

void	compute(t_data *data)
{
	t_sphere	s;

	s.center = (t_vec3){0.0, 0.0, -55.0};
	s.radius = 20.0;
	s.color = (t_vec3){255.0, 0.0, 0.0};

	double	fov = 90.0 * M_PI / 180.0;

//	t_vec3	r_origin = (t_vec3){0.0, 0.0, 0.0};

	for (int x = 0; x < 800; x++)
	{
		for (int y = 0; y < 800; y++)
		{
			t_vec3	r_dir = (t_vec3){x - 800 / 2.0, y - 800 / 2.0, -800 / (2 * tan(fov / 2.0))};
			r_dir = normalize(r_dir);
			//printf("%d %d: r_dir = {%f;%f;%f}\n", x, y, r_dir.x, r_dir.y, r_dir.z);
			if (inter_s(s, r_dir))
				mlx_pixel_put(data->mlx.mlx, data->mlx.win, x, y, 0x00ff0000);
			else
				mlx_pixel_put(data->mlx.mlx, data->mlx.win, x, y, 0x00000000);
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
	mlx_loop(data.mlx.mlx);

	printf("?\n");
	destroy_mlx(&data);
}
