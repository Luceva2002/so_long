/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   players.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 12:43:59 by luevange          #+#    #+#             */
/*   Updated: 2025/03/12 00:12:12 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

// Funzione per renderizzare la mappa
void render_map(t_game *game)
{
    int i, j;
    
    for (i = 0; i < game->map.height; i++)
    {
        for (j = 0; j < game->map.width; j++)
        {
            // Renderizza il pavimento come sfondo per tutte le celle
            mlx_put_image_to_window(game->mlx, game->win, game->floor_img.img, 
                                   j * TILE_SIZE, i * TILE_SIZE);
            
            // Renderizza gli altri elementi sopra il pavimento
            if (game->map.grid[i][j] == '1')
                mlx_put_image_to_window(game->mlx, game->win, game->wall_img.img, 
                                       j * TILE_SIZE, i * TILE_SIZE);
            else if (game->map.grid[i][j] == 'C')
                mlx_put_image_to_window(game->mlx, game->win, game->collectible_img.img, 
                                       j * TILE_SIZE, i * TILE_SIZE);
            else if (game->map.grid[i][j] == 'E')
                mlx_put_image_to_window(game->mlx, game->win, game->exit_img.img, 
                                       j * TILE_SIZE, i * TILE_SIZE);
            else if (game->map.grid[i][j] == 'P')
                mlx_put_image_to_window(game->mlx, game->win, game->player_img.img, 
                                       j * TILE_SIZE, i * TILE_SIZE);
        }
    }
    
    // Visualizza il numero di mosse
    char moves_str[50];
    sprintf(moves_str, "Moves: %d", game->moves);
    mlx_string_put(game->mlx, game->win, 10, 20, 0xFFFFFF, moves_str);
}

// Gestione degli input da tastiera
int handle_key(int keycode, t_game *game)
{
    int new_x = game->player.x;
    int new_y = game->player.y;
    
    // Codici chiave: W/up: 13/126, A/left: 0/123, S/down: 1/125, D/right: 2/124, ESC: 53
    if (keycode == 13 || keycode == 126) // W o freccia su
        new_y--;
    else if (keycode == 0 || keycode == 123) // A o freccia sinistra
        new_x--;
    else if (keycode == 1 || keycode == 125) // S o freccia giù
        new_y++;
    else if (keycode == 2 || keycode == 124) // D o freccia destra
        new_x++;
    else if (keycode == 53) // ESC
        exit(0);
    else
        return (0);
    
    move_player(game, new_x, new_y);
    return (1);
}

// Movimento del giocatore
void move_player(t_game *game, int new_x, int new_y)
{
    // Controlla se la nuova posizione è un muro
    if (game->map.grid[new_y][new_x] == '1')
        return;
    
    // Controlla se la nuova posizione è l'uscita
    if (game->map.grid[new_y][new_x] == 'E')
    {
        // Verifica se tutti i collezionabili sono stati raccolti
        if (game->player.collectibles == game->map.collectibles)
        {
            printf("Congratulations! You completed the game in %d moves.\n", game->moves + 1);
            exit(0);
        }
        return;
    }
    
    // Controlla se la nuova posizione è un collezionabile
    if (game->map.grid[new_y][new_x] == 'C')
    {
        game->player.collectibles++;
        game->map.grid[new_y][new_x] = '0';
    }
    
    // Aggiorna la posizione del giocatore
    game->map.grid[game->player.y][game->player.x] = '0';
    game->map.grid[new_y][new_x] = 'P';
    game->player.x = new_x;
    game->player.y = new_y;
    game->moves++;
    
    // Aggiorna la mappa
    render_map(game);
    
    printf("Moves: %d, Collectibles: %d/%d\n", game->moves, 
           game->player.collectibles, game->map.collectibles);
}

// Funzione per liberare la memoria
void clean_game(t_game *game)
{
    int i;
    
    // Libera la memoria della griglia della mappa
    for (i = 0; i < game->map.height; i++)
        free(game->map.grid[i]);
    free(game->map.grid);
    
    // Distruggi le immagini
    mlx_destroy_image(game->mlx, game->player_img.img);
    mlx_destroy_image(game->mlx, game->wall_img.img);
    mlx_destroy_image(game->mlx, game->collectible_img.img);
    mlx_destroy_image(game->mlx, game->exit_img.img);
    mlx_destroy_image(game->mlx, game->floor_img.img);
    
    // Distruggi la finestra e il contesto MLX
    mlx_destroy_window(game->mlx, game->win);
    free(game->mlx);
}