/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by amn700            #+#    #+#             */
/*   Updated: 2025/12/02 21:05:44 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT_bonus.h"
/*
	⚠️ Things to keep in mind for parsing:

	Identifiers (A, C, L, sp, pl, cy) must be followed by the right number of arguments.

	Values (coords, colors, FOV, ratios) should be valid and within range.
*/


/*
Your program must take as its first argument a scene description file with the .rt
extension.
◦ Each type of element can be separated by one or more line breaks.
◦ Each type of information from an element can be separated by one or more
spaces.
◦ Each type of element can be set in any order in the file.
◦ Elements defined by a capital letter can only be declared once in the scene.
The first piece of information for each element is the type identifier (composed
of one or two characters), followed by all specific information for each object
in a strict order
*/
void	errors_map(int code)
{
	if (code == 1)
		printf("Error:\twrong file extension. needed (*.rt)\n");
	if (code == 2)
		printf("Error:\tfile not found or could not be opened\n");
	if (code == 3)
		printf("Error:\tinvalid or incomplete scene description\n");
}

bool	check_extension(char *file)
{
	int	i;

	i = 0;
	while (file[i])
		i++;
	if (i < 3 || ft_strncmp(&file[i - 3], ".rt", 3) != 0)
		return (false);
	return (true);
}

bool	complete_parsing(char *file, t_data *data)
{
	if (!check_extension(file))
		return (errors_map(1), false);
	if (!extract_data(file, data))
		return (false);
	apply_ambient_to_objects(data);
	// check_parsed_data(data);
	return (true);
}

// int main(int argc, char **argv)
// {
// 	t_data	data;

// 	if (argc != 2)
// 	{
// 		printf("Error:\nwrong number of arguments. usage: ./miniRT <scene.rt>\n");
// 		return (1);
// 	}
// 	if (!complete_parsing(argv[1], &data))
// 		return (1);
// 	// Further processing with parsed data...
// 	return (0);
// }
