#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

typedef struct	s_zone
{
	int		w;
	int		h;
	char	c;
}				t_zone;

typedef struct	s_form
{
	char	type;
	float	x;
	float	y;
	float	r;
	char	c;
}				t_form;

int	o_putstr_error(const char *str, int rv)
{
	int	i;

	i = 0;
	while (str[i])
		write(1, &str[i++], 1);
	return (rv);
}

void	o_set_tab(char tab[300][300], t_zone *zone)
{
	int	x;
	int	y;

	y = -1;
	while (++y < zone->h)
	{
		x = -1;
		while (++x < zone->w)
			tab[y][x] = zone->c;
	}
}

void	o_print(char tab[300][300], t_zone *zone)
{
	int	x;
	int	y;

	y = -1;
	while (++y < zone->h)
	{
		x = -1;
		while (++x < zone->w)
			write(1, &tab[y][x], 1);
		write(1, "\n", 1);
	}
}

int	o_draw_form(char tab[300][300], t_zone *zone, t_form *form)
{
	int		x;
	int		y;
	float	dist;

	y = -1;
	while (++y < zone->h)
	{
		x = -1;
		while (++x < zone->w)
		{
			dist = sqrt((x - form->x) * (x - form->x) + (y - form->y) * (y - form->y));
			if (form->type == 'C')
			{
				if (dist <= form->r)
					tab[y][x] = form->c;
			}
			else if (form->type == 'c')
			{
				if (dist <= form->r && dist > form->r - 1)
					tab[y][x] = form->c;
			}
			else
				return (1);
		}
	}
	return (0);
}

int	o_form(char tab[300][300], t_zone *zone, FILE *file)
{
	int		rv;
	t_form	form;

	while ((rv = fscanf(file, "%c %f %f %f %c\n", &form.type, &form.x, &form.y, &form.r, &form.c)) == 5)
	{
		if (form.r <= 0)
			return (1);
		if (o_draw_form(tab, zone, &form))
			return (1);
	}
	if (rv != -1)
		return (1);
	return (0);
}

int	main(int argc, char *argv[])
{
	t_zone	zone;
	FILE	*file;
	char	tab[300][300];

	if (argc != 2)
		return (o_putstr_error("Error: argument\n", 1));
	file = fopen(argv[1], "r");
	if (!file)
		return (o_putstr_error("Error: Operation file corrupted\n", 1));
	if (fscanf(file, "%d %d %c\n", &zone.w, &zone.h, &zone.c) != 3 || zone.w <= 0 || zone.h <= 0 || zone.w > 300 || zone.h > 300)
		return (o_putstr_error("Error: Operation file corrupted\n", 1));
	o_set_tab(tab, &zone);
	if (o_form(tab, &zone, file))
		return (o_putstr_error("Error: Operation file corrupted\n", 1));
	fclose(file);
	o_print(tab, &zone);
	return (0);
}
