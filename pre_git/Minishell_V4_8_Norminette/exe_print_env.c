/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_print_env.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <vloddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/06/30 18:41:18 by vloddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sort_declare(t_env **array, int count)
{
	t_env	*temp;
	int		i_sort;
	int		j;

	i_sort = 0;
	while (i_sort < count - 1)
	{
		j = 0;
		while (j < count - i_sort - 1)
		{
			if (ft_strcmp(array[j]->key, array[j + 1]->key) > 0)
			{
				temp = array[j];
				array[j] = array[j + 1];
				array[j + 1] = temp;
			}
			j++;
		}
		i_sort++;
	}
}

static void	print_declare(t_env **array, int count)
{
	int		k;

	k = 0;
	while (k < count)
	{
		if (array[k]->value)
			printf("declare -x %s=\"%s\"\n", array[k]->key, array[k]->value);
		else
			printf("declare -x %s\n", array[k]->key);
		k++;
	}
}

static void	loop_cur_array(t_env *cur, t_env **array)
{
	int	i;

	i = 0;
	while (cur)
	{
		if (cur->exported)
			array[i++] = cur;
		cur = cur->next;
	}
}

int	print_exported_env(t_env *env)
{
	int		count;
	t_env	*cur;
	t_env	**array;

	count = 0;
	cur = env;
	while (cur)
	{
		if (cur->exported)
			count++;
		cur = cur->next;
	}
	array = malloc(count * sizeof(t_env *));
	if (!array)
		return (1);
	cur = env;
	loop_cur_array(cur, array);
	sort_declare(array, count);
	print_declare(array, count);
	free(array);
	return (0);
}
