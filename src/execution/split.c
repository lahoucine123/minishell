/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laoubaid <laoubaid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 15:48:47 by laoubaid          #+#    #+#             */
/*   Updated: 2024/08/19 23:53:20 by laoubaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"

static int	count_words(char const *s, char c)
{
	int	count;
	int	i;
	int	len;

	i = 1;
	count = 0;
	len = ft_strlen(s);
	while (i <= len)
	{
		if ((s[i] == c || s[i] == '\0') && s[i - 1] != c)
			count++;
		i++;
	}
	return (count);
}

static char	*count_chars(char const *s, char c, int *nbc, int *ns)
{
	int		i;
	char	*ptr;

	i = 0;
	*ns = 0;
	*nbc = 0;
	ptr = (char *)s;
	while (ptr[i] == c)
		i++;
	while (!(ptr[i] == c || ptr[i] == '\0'))
	{
		(*nbc)++;
		i++;
	}
	*ns = *nbc;
	while (ptr[i] == c)
	{
		i++;
		(*ns)++;
	}
	return (&ptr[i]);
}

char	**ft_free(char **ptr)
{
	int	i;

	i = 0;
	if (!ptr)
		return (NULL);
	while (ptr[i])
	{
		free(ptr[i]);
		i++;
	}
	free(ptr);
	return (NULL);
}

static char	**ft_fill(char **ptr, const char *s, char c, int nw)
{
	int	i;
	int	nc;
	int	j;
	int	ns;

	nc = 0;
	ns = 0;
	i = 0;
	while (i < nw)
	{
		s = count_chars(s, c, &nc, &ns);
		ptr[i] = (char *)malloc(nc + 2);
		if (!ptr[i])
			return (ft_free(ptr));
		j = 0;
		while (*(s - ns) != c && *(s - ns))
		{
			ptr[i][j++] = *(s - ns);
			ns--;
		}
		ptr[i][j++] = 47;
		ptr[i++][j] = '\0';
	}
	return (ptr);
}

char	**split_all(char const *s, char c)
{
	char	**ptr;
	int		nword;

	if (!s)
		return (0);
	nword = count_words(s, c);
	ptr = (char **)malloc((nword + 1) * sizeof(char *));
	if (!ptr)
		return (NULL);
	ptr = ft_fill(ptr, s, c, nword);
	if (ptr)
		ptr[nword] = (NULL);
	return (ptr);
}
