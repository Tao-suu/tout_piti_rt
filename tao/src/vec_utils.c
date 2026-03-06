/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbez--du <tbez--du@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 13:49:58 by tbez--du          #+#    #+#             */
/*   Updated: 2026/03/06 17:22:25 by tbez--du         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

double	vec_dot(t_vec3 a, t_vec3 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

double	vec_norm2(t_vec3 a)
{
	return (a.x*a.x + a.y*a.y + a.z*a.z);
}

t_vec3	vec_prod(double factor, t_vec3 a)
{
	t_vec3	tmp;

	tmp = (t_vec3){a.x*factor, a.y*factor, a.z*factor};
	return (tmp);
}

t_vec3	vec_normalize(t_vec3 a)
{
	double	norme = sqrt(vec_norm2(a));
	t_vec3 tmp = (t_vec3){a.x / norme, a.y / norme, a.z / norme};
	return tmp;
}

t_vec3	vec_add(t_vec3 a, t_vec3 b)
{
	t_vec3	ret;
	
	ret = (t_vec3){a.x + b.x, a.y + b.y, a.z + b.z};
	return (ret);
}

t_vec3	vec_sub(t_vec3 a, t_vec3 b)
{
	t_vec3	ret;
	
	ret = (t_vec3){a.x - b.x, a.y - b.y, a.z - b.z};
	return (ret);
}

t_vec3	vec_prodv(t_vec3 a, t_vec3 b)
{
	t_vec3	ret;

	ret = (t_vec3){a.x * b.x, a.y * b.y, a.z * b.z};
	return (ret);
}

t_vec3	vec_subd(t_vec3 a, double i)
{
	return (t_vec3){a.x - i, a.y - i, a.z - i};
}
