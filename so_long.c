/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 12:44:01 by luevange          #+#    #+#             */
/*   Updated: 2025/03/12 00:16:20 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// so_long.h
#ifndef SO_LONG_H
# define SO_LONG_H

# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>

# define TILE_SIZE 32

typedef struct s_player
{
    int x;
    int y;
    int collectibles;
}   t_player;

typedef struct s_map
{
    char **grid;
    int width;
    int height;
    int collectibles;
    int exit_x;
    int exit_y;
}   t_map;

typedef struct s_img
{
    void *img;
    int width;
    int height;
}   t_img;

typedef struct s_game
{
    void *mlx;
    void *win;
    t_map map;
    t_player player;
    t_img player_img;
    t_img wall_img;
    t_img collectible_img;
    t_img exit_img;
    t_img floor_img;
    int moves;
}   t_game;

// Funzioni principali
int     init_game(t_game *game, char *map_path);
int     parse_map(t_game *game, char *map_path);
int     check_map(t_game *game);
void    render_map(t_game *game);
int     handle_key(int keycode, t_game *game);
void    move_player(t_game *game, int new_x, int new_y);
void    clean_game(t_game *game);

#endif