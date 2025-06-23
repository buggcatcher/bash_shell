/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/05/28 12:50:15 by vloddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_env_var(t_env **env, char *key, char *value)
{
	t_env	*cur;
	t_env	*new;

	cur = *env;
	while (cur)
	{
		if (ft_strcmp(cur->key, key) == 0)
		{
			free(cur->value);
			cur->value = ft_strdup_m(value);
			return ;
		}
		cur = cur->next;
	}
	new = malloc(sizeof(t_env));
	if (!new)
		return ;
	new->key = ft_strdup_m(key);
	new->value = ft_strdup_m(value);
	new->next = *env;
	*env = new;
}

int	exe_cd(char **args, t_env **env)
{
	char	oldpwd[PATH_MAX];
	char	newpwd[PATH_MAX];
	char	*target;

	if (!getcwd(oldpwd, sizeof(oldpwd)))
		return (perror("cd getcwd (OLDPWD)"), 1);
	target = args[1];
	if (!target)
	{
		target = get_env_value("HOME", *env);
		if (!target)
			return (write(2, "cd: HOME not set\n", 18), 1);
	}
	if (chdir(target) != 0)
	{
		perror("cd");
		return (1);
	}
	if (!getcwd(newpwd, sizeof(newpwd)))
		return (perror("cd getcwd (PWD)"), 1);
	set_env_var(env, "OLDPWD", oldpwd);
	set_env_var(env, "PWD", newpwd);
	return (0);
}

int	exe_pwd(char **args, t_env **env)
{
	char	cwd[PATH_MAX + 1];

	(void)args;
	(void)env;
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd);
		return (0);
	}
	perror("pwd");
	return (1);
}

int	is_parent_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strcmp(cmd, "cd") || \
			!ft_strcmp(cmd, "export") || \
			!ft_strcmp(cmd, "unset") || \
			!ft_strcmp(cmd, "exit"));
}

int	is_child_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strcmp(cmd, "echo") || \
			!ft_strcmp(cmd, "pwd") || \
			!ft_strcmp(cmd, "env"));
}

int	should_execute_in_parent(t_node *node)
{
	return (is_parent_builtin(node->argv[0]) && !node->next);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (is_parent_builtin(cmd) || is_child_builtin(cmd));
}

t_env	*find_env_node(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

t_env	*create_env_node( char *key, char *value, int exported)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = ft_strdup_m(key);
	new_node->value = ft_strdup_m(value);
	if (!new_node->key || !new_node->value)
	{
		free(new_node->key);
		free(new_node->value);
		free(new_node);
		return (NULL);
	}
	new_node->exported = exported;
	new_node->next = NULL;
	return (new_node);
}

int	remove_env_node(t_env **env, char *key)
{
	t_env	*current;
	t_env	*prev;

	current = *env;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*env = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return (0);
		}
		prev = current;
		current = current->next;
	}
	return (1);
}

int	exe_export(char **args, t_env **env)
{
	int		i;
	t_env	*current;
	char	*equals;
	char	*key;
	char	*value;
	t_env	*existing;
	t_env	*new_node;

	i = 1;
	if (!args[1])
	{
		current = *env;
		while (current)
		{
			if (current->exported)
				printf("declare -x %s=\"%s\"\n", current->key, current->value);
			current = current->next;
		}
		return (0);
	}
	while (args[i])
	{
		equals = strchr(args[i], '=');
		if (equals)
		{
			*equals = '\0';
			key = args[i];
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
		else
		{
			existing = find_env_node(*env, args[i]);
			if (existing)
				existing->exported = 1;
			else
			{
				new_node = create_env_node(args[i], "", 1);
				if (new_node)
				{
					new_node->next = *env;
					*env = new_node;
				}
			}
		}
		i++;
	}
	return (0);
}

int	exe_unset(char **args, t_env **env)
{
	int	i;
	int	exit_status;

	i = 1;
	exit_status = 0;
	if (!args[1])
		return (write(2, "unset: not enough arguments\n", 28), 1);
	while (args[i])
	{
		if (remove_env_node(env, args[i]) != 0)
			printf("nodo non rimosso\n");
		i++;
	}
	return (exit_status);
}

int	exe_env(t_env *env)
{
	while (env)
	{
		if (env->exported)
			printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
	return (0);
}

int	exe_echo(char **args)
{
	int	i;
	int	no_newline;

	i = 1;
	no_newline = 0;
	if (args[1] && ft_strcmp(args[1], "-n") == 0)
	{
		no_newline = 1;
		i = 2;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (!no_newline)
		printf("\n");
	return (0);
}

int	exit_numeric_error(char *arg, t_shell_state *state)
{
	ft_putstr_stderr("exit: ");
	ft_putstr_stderr(arg);
	ft_putstr_stderr(": numeric argument required\n");
	printf("exit\n");
	state->should_exit = 1;
	state->exit_code = 2;
	return (2);
}

int	exit_too_many_args(t_shell_state *state)
{
	ft_putstr_stderr("exit: too many arguments\n");
	state->last_status = 1;
	return (1);
}

int	exe_exit(char **args, t_shell_state *state)
{
	int		exit_code;
	long	val;
	char	*endptr;

	exit_code = state->last_status;
	if (args[1])
	{
		errno = 0;
		val = ft_strtol(args[1], &endptr);
		if (*endptr != '\0' || errno == ERANGE)
			return (exit_numeric_error(args[1], state));
		else if (args[2])
			return (exit_too_many_args(state));
		else
			exit_code = (int)(val & 0xFF);
	}
	printf("exit\n");
	state->should_exit = 1;
	state->exit_code = exit_code;
	return (0);
}
