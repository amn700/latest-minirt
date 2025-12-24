/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/07 13:24:58 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "types.h"

typedef struct s_data	t_data;

void	errors_map(int code);
bool	check_extension(char *file);
bool	complete_parsing(char *file, t_data *data);
bool	extract_data(char *file, t_data *data);
void	apply_ambient_to_objects(t_data *data);
char	**ft_split_rt(char const *s, char *charset);
float	ft_atof(char *str);
bool	i_field_validation(char *str);
bool	f_field_validation(char *str);
bool	i_range_validator(int min, int max, char *line);
bool	f_range_validator(float min, float max, char *line);
void	check_parsed_data(t_data *data);
void	print_matrix(const char *label, t_matrix m);
void	debug_world_state(t_world world, t_camera cam);

#endif