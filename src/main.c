/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adebray <adebray@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/09/07 17:22:00 by adebray           #+#    #+#             */
/*   Updated: 2015/09/09 19:24:37 by adebray          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#include <time.h>
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

#define A 0
#define S 1
#define D 2
#define W 13
#define LEFT 123
#define RIGHT 124
#define DOWN 125
#define UP 126

char	g_keys[255];

void	bzero(void *b, unsigned long size);

struct		s_vec2
{
	double		x;
	double		y;
};

struct		s_line
{
	int				id;
	struct s_vec2	p1;
	struct s_vec2	p2;
};

struct		s_player
{
	struct s_vec2	position;
	float			rotation;
};

typedef struct s_line t_line;
typedef struct s_vec2 t_vec2;
typedef struct s_vec4 t_vec4;
typedef struct s_player t_player;

void	*g_mlx;
void	*g_window;
void	*g_images[2];
int		*g_datas[2];
int		g_infos[3];

#define	SPACE_LIMIT	2048

t_line		g_space[SPACE_LIMIT];
t_player	g_player;

void	debug_space(void)
{
	int	i;

	i = 0;
	while (i < SPACE_LIMIT)
	{
		if (g_space[i].p2.x - g_space[i].p1.x && g_space[i].p2.y - g_space[i].p1.y)
			printf("%d -> %f, %f, %f, %f\n", g_space[i].id, g_space[i].p1.x, g_space[i].p1.y, g_space[i].p2.x, g_space[i].p2.y);
		i += 1;
	}
}

float		raycast(int x, int y)
{
	int i = 0;
	float bol = FLT_MAX;
	while (i < SPACE_LIMIT)
	{

		if (g_space[i].p2.x - g_space[i].p1.x || g_space[i].p2.y - g_space[i].p1.y)
		{

			t_line *l = &(g_space[i]);
			t_line _;

			_.p1.x = l->p1.x - g_player.position.x;
			_.p1.y = l->p1.y - g_player.position.y;
			_.p2.x = l->p2.x - g_player.position.x;
			_.p2.y = l->p2.y - g_player.position.y;

			_.p1.x = _.p1.x * cos(g_player.rotation) - _.p1.y * sin(g_player.rotation);
			_.p1.y = _.p1.y * cos(g_player.rotation) + _.p1.x * sin(g_player.rotation);
			_.p2.x = _.p2.x * cos(g_player.rotation) - _.p2.y * sin(g_player.rotation);
			_.p2.y = _.p2.y * cos(g_player.rotation) + _.p2.x * sin(g_player.rotation);

			// printf("%d, %d\n", MIN(_.p1.x, _.p2.x), MAX(_.p1.x, _.p2.x));

			if ((MIN(_.p1.x, _.p2.x)) <= x && x <= (MAX(_.p1.x, _.p2.x)))
			{
				float a = (float)(_.p2.y - _.p1.y) / (float)(_.p2.x - _.p1.x);
				float b = _.p1.y - a * _.p1.x;

				(void)y;
				// printf("%f\n", a * x + b);
				if (a * x + b < bol)
					bol = a * x + b;
			}
		}
		i += 1;
	}
	return (bol);
}

#define X cos(g_player.rotation)
#define Y sin(g_player.rotation)

int		update_function(void)
{
	static int cmp;
	void *image;
	int  *data;
	int  *destroy;
	int  a;

	a = 0;
	data = g_datas[cmp % 2];
	image = g_images[cmp % 2];
	destroy = g_datas[(cmp + 1) % 2];
	cmp += 1;

	if (g_keys[UP])
	{
		g_player.position.x += X * 2;
		g_player.position.y += Y * 2;
	}
	if (g_keys[DOWN])
	{
		g_player.position.x -= X * 2;
		g_player.position.y -= Y * 2;
	}
	if (g_keys[LEFT])
		g_player.rotation -= 0.1f;
	if (g_keys[RIGHT])
		g_player.rotation += 0.1f;

	printf("%f\n", g_player.rotation);
	while (a < WIDTH)
	{
		int b = 0;
		while (b < HEIGHT)
		{
			int c = 0;
			while (c < 2)
			{
				t_line *l = &(g_space[c]);
				t_line _;

				_.p1.x = l->p1.x ; // - g_player.position.x;
				_.p1.y = l->p1.y ; // - g_player.position.y;
				_.p2.x = l->p2.x ; // - g_player.position.x;
				_.p2.y = l->p2.y ; // - g_player.position.y;
				//
				// _.p1.x = _.p1.x * cos(g_player.rotation) - _.p1.y * sin(g_player.rotation);
				// _.p1.y = _.p1.y * cos(g_player.rotation) + _.p1.x * sin(g_player.rotation);
				// _.p2.x = _.p2.x * cos(g_player.rotation) - _.p2.y * sin(g_player.rotation);
				// _.p2.y = _.p2.y * cos(g_player.rotation) + _.p2.x * sin(g_player.rotation);

				if ((MIN(_.p1.x, _.p2.x)) <= a && a <= (MAX(_.p1.x, _.p2.x)))
				{
					float m = (float)(_.p2.y - _.p1.y) / (float)(_.p2.x - _.p1.x);
					float n = _.p1.y - m * _.p1.x;

					if (m * a + n == b)
					{
						DRAW_PIX(data, a, b, 0xff0000);
					}
						// bol = a * x + b;
				}
				c += 1;
			}
			b += 1;
		}
		DRAW_PIX(data, (int)g_player.position.x, (int)g_player.position.y, 0x00ff00);
		//---------------
		// float z_index = raycast(a - WIDTH / 2, 0);
		// if (z_index != FLT_MAX && z_index > 0)
		// {
		// 	int size = HEIGHT / 2 - z_index;
		// 	printf("%d, %f\n", a, z_index);
		// 	DRAW_COLN(data, a, 0xff0000, HEIGHT / 2 - size, HEIGHT / 2 + size);
		// }
		DRAW_COL(destroy, a, 0x000000);
		a += 1;
	}

	mlx_put_image_to_window(g_mlx, g_window, image, 0, 0);
	return (0);
}

int		keydown(int keycode, void *param)
{
	(void)param;
	printf("KEYCODE[%d]\n", keycode);
	g_keys[keycode] = 1;
	return (0);
}

int		keyup(int keycode, void *param)
{
	(void)param;
	g_keys[keycode] = 0;
	return (0);
}

int		main(void)
{
	printf("Hello World !\n");

	bzero(g_space, sizeof(g_space));
	g_space[0].id = 0;
	g_space[0].p1.x = -100;
	g_space[0].p1.y = -100;
	g_space[0].p2.x = 100;
	g_space[0].p2.y = 100;
	g_space[1].id = 1;
	g_space[1].p1.x = 100;
	g_space[1].p1.y = 100;
	g_space[1].p2.x = 200;
	g_space[1].p2.y = 100;
	g_space[2].id = 1;
	g_space[2].p1.x = -100;
	g_space[2].p1.y = -100;
	g_space[2].p2.x = 100;
	g_space[2].p2.y = 100;

	g_player.position.x = 50;
	g_player.position.y = 70;
	g_player.rotation = 0;

	g_mlx = mlx_init();
	g_window = mlx_new_window(g_mlx, WIDTH, HEIGHT, "WOLF2D");
	g_images[0] = mlx_new_image(g_mlx, WIDTH, HEIGHT);
	g_images[1] = mlx_new_image(g_mlx, WIDTH, HEIGHT);
	g_datas[0] = (int *)mlx_get_data_addr(g_images[0], &(g_infos[0]), &(g_infos[1]), &(g_infos[2]));
	g_datas[1] = (int *)mlx_get_data_addr(g_images[1], &(g_infos[0]), &(g_infos[1]), &(g_infos[2]));

	mlx_loop_hook(g_mlx, &update_function, NULL);
	mlx_hook(g_window, 2, 0, &keydown, NULL);
	mlx_hook(g_window, 3, 0, &keyup, NULL);
	mlx_loop(g_mlx);
	return (0);
}
