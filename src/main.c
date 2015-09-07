/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adebray <adebray@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/09/07 17:22:00 by adebray           #+#    #+#             */
/*   Updated: 2015/09/07 21:17:39 by adebray          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <mlx.h>

#define WIDTH 640
#define HEIGHT 480
#define DRAW_PIX(data, x, y, color) data[x + y * g_infos[1] / 4] = color
#define DRAW_LINE(d, y, c) for(int i=0; i < WIDTH; i++) DRAW_PIX(d, i, y, c)
#define DRAW_COL(d, x, c) for(int i=0; i < HEIGHT; i++) DRAW_PIX(d, x, i, c)
#define DRAW_COLN(d, x, c, s, e) for(int i=s; i < e; i++) DRAW_PIX(d, x, i, c)

#define MIN(x, y) (x < y) ? x : y
#define MAX(x, y) (x > y) ? x : y

#define MAP_WIDTH 200
#define MAP_HEIGHT 200

void	bzero(void *b, unsigned long size);

struct		s_vec2
{
	int		x;
	int		y;
};

struct		s_vec4
{
	int		x1;
	int		y1;
	int		x2;
	int		y2;
};

typedef struct s_vec4 t_line;
typedef struct s_vec2 t_point;
typedef struct s_vec2 t_vector;
typedef struct s_vec2 t_player;

void	*g_mlx;
void	*g_window;
void	*g_images[2];
int		*g_datas[2];
int		g_infos[3];

#define	SPACE_LIMIT	2048

t_line		g_space[SPACE_LIMIT];
t_player	g_player;

void	translate(const t_vector *v)
{
	int i;

	i = 0;
	printf("%d, %d || ", v->x, v->y);
	while(i < SPACE_LIMIT)
	{
		if (g_space[i].x2 - g_space[i].x1 && g_space[i].y2 - g_space[i].y1)
		{
			g_space[i].x1 += v->x;
			g_space[i].y1 += v->y;
			g_space[i].x2 += v->x;
			g_space[i].y2 += v->y;
		}
		i += 1;
	}
}

void	debug_space(void)
{
	int	i;

	i = 0;
	while (i < SPACE_LIMIT)
	{
		if (g_space[i].x2 - g_space[i].x1 && g_space[i].y2 - g_space[i].y1)
			printf("%d -> %d, %d, %d, %d\n", i, g_space[i].x1, g_space[i].y1, g_space[i].x2, g_space[i].y2);
		i += 1;
	}
}

int		function(void)
{
	static int time;
	void *image;
	int  *data;
	int a;

	a = 0;
	data = g_datas[time % 2];
	image = g_images[(time + 1) % 2];
	time += 1;

	// translate(&((t_vector){55, 70}));
	debug_space();
	// while (a < WIDTH)
	// {
		DRAW_COLN(data, a, 0xfefefe, 200, 300);
		a += 1;
	// }
	mlx_put_image_to_window(g_mlx, g_window, image, 0, 0);
	return (0);
}

int		main(void)
{
	printf("Hello World !\n");

	bzero(g_space, sizeof(g_space));
	g_space[0].x1 = 10;
	g_space[0].y1 = 10;
	g_space[0].x2 = 100;
	g_space[0].y2 = 100;

	g_player.x = 55;
	g_player.y = 70;

	g_mlx = mlx_init();
	g_window = mlx_new_window(g_mlx, WIDTH, HEIGHT, "WOLF2D");
	g_images[0] = mlx_new_image(g_mlx, WIDTH, HEIGHT);
	g_images[1] = mlx_new_image(g_mlx, WIDTH, HEIGHT);
	g_datas[0] = (int *)mlx_get_data_addr(g_images[0], &(g_infos[0]), &(g_infos[1]), &(g_infos[2]));
	g_datas[1] = (int *)mlx_get_data_addr(g_images[1], &(g_infos[0]), &(g_infos[1]), &(g_infos[2]));
	mlx_loop_hook(g_mlx, &function, NULL);
	mlx_loop(g_mlx);
	return (0);
}
