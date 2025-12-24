/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   progress_bar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/02 22:48:42 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT_bonus.h"

void	print_progress_bar(int current, int total, int bar_width)
{
	float	progress;
	int		pos;
	int		i;

	progress = (float)current / total;
	pos = bar_width * progress;
	printf("\r[");
	i = 0;
	while (i < bar_width)
	{
		if (i < pos)
			printf("█");
		else if (i == pos)
			printf(">");
		else
			printf(" ");
		i++;
	}
	printf("] %d%%", (int)(progress * 100));
	fflush(stdout);
	if (current == total)
		printf("\n");
}
