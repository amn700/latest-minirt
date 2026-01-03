/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/03 12:01:41 by amn               #+#    #+#             */
/*   Updated: 2026/01/03 12:01:50 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

static char	*ft_strchr_local(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == (char)c)
			return ((char *)&s[i]);
		i++;
	}
	return (NULL);
}

static void	free_split_matrix(char **matrix, int i)
{
	while (i >= 0)
		free(matrix[i--]);
	free(matrix);
}

int	count_words(char *str, char *charset)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		while (str[i] && ft_strchr_local(charset, str[i]))
			i++;
		if (str[i] != '\0')
			count++;
		while (str[i] && !ft_strchr_local(charset, str[i]))
			i++;
	}
	return (count);
}

static char	*ft_addstr(char *str, char *charset)
{
	int		i;
	int		size;
	char	*new;

	size = 0;
	while (!ft_strchr_local(charset, str[size]) && str[size])
		size++;
	new = malloc(size + 1);
	if (new == NULL)
		return (NULL);
	i = 0;
	while (i < size)
	{
		new[i] = str[i];
		i++;
	}
	new[size] = '\0';
	return (new);
}

int	populate(char **matrix, char *str, char *charset)
{
	int	i;

	i = 0;
	while (*str)
	{
		while (ft_strchr_local(charset, *str))
			str++;
		if (*str)
		{
			matrix[i] = ft_addstr(str, charset);
			if (!matrix[i])
			{
				free_split_matrix(matrix, i - 1);
				return (0);
			}
			i++;
		}
		while (!ft_strchr_local(charset, *str) && *str)
			str++;
	}
	matrix[i] = NULL;
	return (1);
}
