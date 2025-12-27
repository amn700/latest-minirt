/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_png.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/27 00:00:00 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PNG_H
# define FT_PNG_H

# include "types.h"
# include <stdbool.h>

// PNG loading API
bool	ft_load_png_to_tex(const char *path, t_tex *out);
void	ft_free_tex(t_tex *tex);

#endif
