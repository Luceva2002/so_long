/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 12:43:56 by luevange          #+#    #+#             */
/*   Updated: 2025/03/11 23:50:29 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

// Funzione per contare le righe del file della mappa
static int count_lines(char *file_path)
{
    int fd;
    int count;
    char buffer[1];
    
    count = 0;
    fd = open(file_path, O_RDONLY);
    if (fd < 0)
        return (0);
    
    while (read(fd, buffer, 1) > 0)
    {
        if (buffer[0] == '\n')
            count++;
    }
    
    close(fd);
    return (count + 1);
}

// Funzione per ottenere la lunghezza della prima riga
static int get_line_length(char *file_path)
{
    int fd;
    int len;
    char buffer[1];
    
    len = 0;
    fd = open(file_path, O_RDONLY);
    if (fd < 0)
        return (0);
    
    while (read(fd, buffer, 1) > 0 && buffer[0] != '\n')
        len++;
    
    close(fd);
    return (len);
}

// Funzione per leggere la mappa dal file
int parse_map(t_game *game, char *map_path)
{
    int fd;
    int i;
    int j;
    char buffer[1];
    
    game->map.height = count_lines(map_path);
    game->map.width = get_line_length(map_path);
    
    if (game->map.height == 0 || game->map.width == 0)
        return (0);
    
    // Allocazione della memoria per la griglia della mappa
    game->map.grid = (char **)malloc(sizeof(char *) * game->map.height);
    if (!game->map.grid)
        return (0);
    
    for (i = 0; i < game->map.height; i++)
    {
        game->map.grid[i] = (char *)malloc(sizeof(char) * (game->map.width + 1));
        if (!game->map.grid[i])
            return (0);
    }
    
    // Lettura del file e popolamento della griglia
    fd = open(map_path, O_RDONLY);
    if (fd < 0)
        return (0);
    
    i = 0;
    j = 0;
    game->map.collectibles = 0;
    
    while (read(fd, buffer, 1) > 0)
    {
        if (buffer[0] == '\n')
        {
            game->map.grid[i][j] = '\0';
            i++;
            j = 0;
        }
        else
        {
            game->map.grid[i][j] = buffer[0];
            
            // Tieni traccia del giocatore e degli elementi della mappa
            if (buffer[0] == 'P')
            {
                game->player.x = j;
                game->player.y = i;
            }
            else if (buffer[0] == 'C')
                game->map.collectibles++;
            else if (buffer[0] == 'E')
            {
                game->map.exit_x = j;
                game->map.exit_y = i;
            }
            
            j++;
        }
    }
    
    close(fd);
    return (1);
}

// Verifica della validità della mappa
int check_map(t_game *game)
{
    int i, j;
    int player_count = 0;
    int exit_count = 0;
    
    // Verifica che la mappa sia rettangolare
    for (i = 0; i < game->map.height; i++)
    {
        if ((int)strlen(game->map.grid[i]) != game->map.width)
            return (0);
    }
    
    // Verifica che la mappa sia circondata da muri
    for (i = 0; i < game->map.height; i++)
    {
        for (j = 0; j < game->map.width; j++)
        {
            if (i == 0 || i == game->map.height - 1 || j == 0 || j == game->map.width - 1)
            {
                if (game->map.grid[i][j] != '1')
                    return (0);
            }
            
            // Conta giocatori ed uscite
            if (game->map.grid[i][j] == 'P')
                player_count++;
            else if (game->map.grid[i][j] == 'E')
                exit_count++;
            
            // Verifica caratteri validi
            if (game->map.grid[i][j] != '0' && game->map.grid[i][j] != '1' &&
                game->map.grid[i][j] != 'C' && game->map.grid[i][j] != 'E' &&
                game->map.grid[i][j] != 'P')
                return (0);
        }
    }
    
    // Verifica la presenza di un solo giocatore, almeno un collezionabile e un'uscita
    if (player_count != 1 || exit_count != 1 || game->map.collectibles < 1)
        return (0);
    
    return (1);
}