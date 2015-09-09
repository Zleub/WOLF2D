/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adebray <adebray@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/09/07 17:22:00 by adebray           #+#    #+#             */
/*   Updated: 2015/09/09 00:08:37 by adebray          ###   ########.fr       */
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

struct		s_line
{
	int				id;
	struct s_vec4	vec4;
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
		if (g_space[i].vec4.x2 - g_space[i].vec4.x1 && g_space[i].vec4.y2 - g_space[i].vec4.y1)
			printf("%d -> %d, %d, %d, %d\n", g_space[i].id, g_space[i].vec4.x1, g_space[i].vec4.y1, g_space[i].vec4.x2, g_space[i].vec4.y2);
		i += 1;
	}
}

float		raycast(int x, int y)
{
	int i = 0;
	float bol = FLT_MAX;
	while (i < SPACE_LIMIT)
	{

		if (g_space[i].vec4.x2 - g_space[i].vec4.x1 || g_space[i].vec4.y2 - g_space[i].vec4.y1)
		{

			t_line *l = &(g_space[i]);
			t_vec4 _;

			_.x1 = l->vec4.x1 + g_player.position.x;
			_.y1 = l->vec4.y1 + g_player.position.y;
			_.x2 = l->vec4.x2 + g_player.position.x;
			_.y2 = l->vec4.y2 + g_player.position.y;

			_.x1 = _.x1 * cos(g_player.rotation) - _.y1 * sin(g_player.rotation);
			_.y1 = _.y1 * cos(g_player.rotation) + _.x1 * sin(g_player.rotation);
			_.x2 = _.x2 * cos(g_player.rotation) - _.y2 * sin(g_player.rotation);
			_.y2 = _.y2 * cos(g_player.rotation) + _.x2 * sin(g_player.rotation);

			// printf("%d, %d\n", MIN(_.x1, _.x2), MAX(_.x1, _.x2));
			if ((MIN(_.x1, _.x2)) <= x && x <= (MAX(_.x1, _.x2)))
			{
				float a = (float)(_.y2 - _.y1) / (float)(_.x2 - _.x1);
				float b = _.y1 - a * _.x1;

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

#define A 0
#define S 1
#define D 2
#define W 13
#define LEFT 123
#define RIGHT 124
#define DOWN 125
#define UP 126

//
#define X sin(g_player.rotation * M_PI / 2) * 2
#define Y cos(g_player.rotation * M_PI / 2) * 2
//
char	g_keys[255];

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
		g_player.position.x -= X;
		g_player.position.y -= Y;
	}
	if (g_keys[DOWN])
	{
		g_player.position.x += X;
		g_player.position.y += Y;
	}
	if (g_keys[LEFT])
		g_player.rotation -= 0.01f;
	if (g_keys[RIGHT])
		g_player.rotation += 0.01f;

	write(1, "test\n", 5);
	while (a < WIDTH)
	{
		float z_index = raycast(a - WIDTH / 2, 0);
		// printf("z_index: %d, %f\n", a - WIDTH / 2, z_index);
		if (z_index != FLT_MAX && z_index > 0)
		{
			int size = HEIGHT / 2 - z_index;
			// printf("size: %d\n", size);
			DRAW_COLN(data, a, 0xff0000, HEIGHT / 2 - size, HEIGHT / 2 + size);
		}
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
	g_space[0].vec4.x1 = -100;
	g_space[0].vec4.y1 = -100;
	g_space[0].vec4.x2 = 100;
	g_space[0].vec4.y2 = 100;
	g_space[1].id = 1;
	g_space[1].vec4.x1 = 100;
	g_space[1].vec4.y1 = 100;
	g_space[1].vec4.x2 = 200;
	g_space[1].vec4.y2 = 100;
	g_space[2].id = 1;
	g_space[2].vec4.x1 = -100;
	g_space[2].vec4.y1 = -100;
	g_space[2].vec4.x2 = 100;
	g_space[2].vec4.y2 = 100;

	g_player.position.x = 0;
	g_player.position.y = 0;
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
