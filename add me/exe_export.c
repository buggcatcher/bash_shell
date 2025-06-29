/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/06/24 19:29:23 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_export_without_value(char *arg, t_env **env)
{
    t_env *existing = find_env_node(*env, arg);
    
    if (existing)
        existing->exported = 1; // esportato
	else
	{
        t_env *new_node = create_env_node(arg, NULL, 1);
        if (new_node)
		{
            new_node->next = *env;
            *env = new_node;
        }
    }
}

void	handle_key_value_export(char *arg, char *equals, t_env **env)
{
	char	*key;
	char	*value;
	t_env	*existing;
	t_env	*new_node;

	*equals = '\0';
	key = arg;
	value = equals + 1;
	existing = find_env_node(*env, key);
	if (existing)
	{
		free(existing->value);
		existing->value = ft_strdup(value);
		existing->exported = 1;
	}
	else
	{
		new_node = create_env_node(key, value, 1);
		if (new_node)
		{
			new_node->next = *env;
			*env = new_node;
		}
	}
	*equals = '=';
}
int handle_single_export(char *arg, t_env **env)
{
    char *equals;
    int export_failed;

    export_failed = 0;
    equals = strchr(arg, '=');
    if (equals)
    {
        char temp = *equals;
        *equals = '\0';
        if (!is_valid_variable_name(arg))
        {
            print_export_error(arg);
            export_failed = 1;
        }
        else
            handle_key_value_export(arg, equals, env);
        *equals = temp;
    }
    else
    {
        if (!is_valid_variable_name(arg))
        {
            print_export_error(arg);
            export_failed = 1;
        }
        else
            handle_export_without_value(arg, env);
    }
    return (export_failed);
}

int print_exported_env(t_env *env)
{
    int count;
	int k;
	int i;
	int j;
	int i_sort;

	count = 0;
    t_env *cur = env;
    while (cur) {
        if (cur->exported)
            count++;
        cur = cur->next;
    }
    t_env **array = malloc(count * sizeof(t_env *));
    if (!array)
        return 1;
    i = 0;
    cur = env;
    while (cur)
	{
        if (cur->exported)
            array[i++] = cur;
        cur = cur->next;
    }
    i_sort = 0;
    while (i_sort < count - 1)
	{
        j = 0;
        while (j < count - i_sort - 1)
		{
            if (ft_strcmp(array[j]->key, array[j + 1]->key) > 0)
			{
                t_env *temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
            j++;
        }
        i_sort++;
    }
    k = 0;
    while (k < count)
	{
        if (array[k]->value)
            printf("declare -x %s=\"%s\"\n", array[k]->key, array[k]->value);
        else
            printf("declare -x %s\n", array[k]->key);
        k++;
    }
    free(array);
    return 0;
}


int exe_export(char **args, t_env **env)
{
    int i;
    int export_failed;
    int status;

    if (!args[1])
        return (print_exported_env(*env));
    i = 1;
    export_failed = 0;
    while (args[i])
    {
        status = handle_single_export(args[i], env);
        if (status != 0)
            export_failed = 1;
        i++;
    }
    return (export_failed);
}
