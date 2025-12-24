/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_bonus.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/09 07:31:59 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_BONUS_H
# define PARSING_BONUS_H

# include "types_bonus.h"

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
bool	parse_material_params(char **fields, int start_idx, t_material *mat);
int		count_fields(char **fields);

#endif




// <R,G,B>


//objects sphere/plane/cylinder
//sp <x,y,z> <diameter> <R,G,B> 

// <coordinates (x,y,z)>

//  <diameter>  must be positive
// ratio float (0.0 <=> 1.0)
// float

//pl <x,y,z> <orientation x,y,z> <R,G,B>
//Orientation vector (x,y,z) → normalized direction vector (must be in [-1,1] and not zero).
// <FOV [0, 180]>
// unsigned int
// A <lighting ratio float (0.0 <=> 1.0)>		<R,G,B>
// C <coordinates (x,y,z)>						<orientation vector (x,y,z)> 				<FOV>

// L <coordinates (x,y,z)>						<brightness ratio float (0.0 <=> 1.0)>		<R,G,B>

//cy <x,y,z> <orientation x,y,z> <diameter> <height> <R,G,B>
// A union of t_sphere, t_plane, t_cylinder inside a generic t_object, plus an enum telling which type it is.