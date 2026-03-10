/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbez--du <tbez--du@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 13:40:38 by tbez--du          #+#    #+#             */
/*   Updated: 2026/03/09 13:56:32 by tbez--du         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	create_thread(t_data *data)
{
	for (int i = 0; i < WIN_H; i++)
	{
		data->thread[i].y = i;
		data->thread[i].data = data;
		pthread_create(&data->thread[i].thread, NULL, routine, &data->thread[i]);
	}
	return (1);
}

void	join_thread(t_data *data)
{
	for (int i = 0; i < WIN_H; i++)
		pthread_join(data->thread[i].thread, NULL);
}
