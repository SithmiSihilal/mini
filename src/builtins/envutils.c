/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envutils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: siellage <siellage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 11:23:23 by siellage          #+#    #+#             */
/*   Updated: 2026/01/11 14:15:35 by siellage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	getenvlen(t_core *core)
{
	t_env	*envlist;
	int		count;

	envlist = core->env_table;
	count = 0;
	while (envlist)
	{
		if (envlist->content)
			count++;
		envlist = envlist->next;
	}
	return (count);
}

char	**getenvcpy(t_core *core)
{
	int		envlen;
	char	**envlist;
	char	*tempenv;
	t_env	*tempenvlist;

	envlen = getenvlen(core);
	envlist = (char **)malloc(sizeof(char *) * (envlen + 1));
	envlist[envlen] = NULL;
	tempenvlist = core->env_table;
	while (tempenvlist)
	{
		if (tempenvlist->content)
		{
			tempenv = NULL;
			ownstrjoin(&tempenv, tempenvlist->env_name);
			straddchar(&tempenv, '=');
			ownstrjoin(&tempenv, tempenvlist->content);
			envlist[--envlen] = tempenv;
		}
		tempenvlist = tempenvlist->next;
	}
	return (envlist);
}

void	freeenvcpy(char **envlist)
{
	char	**tempenvlist;

	tempenvlist = envlist;
	while (*tempenvlist)
		free(*(tempenvlist++));
	free(envlist);
}

void	fillenvs(t_core *core, char **env)
{
	while (env && *env)
		addnewenv(&core->env_table, *(env++));
}

t_env	*addnewenv(t_env **envtable, char *env)
{
	t_env	*lastnode;
	char	*content;

	if (!*envtable)
	{
		*envtable = (t_env *)malloc(sizeof(t_env));
		lastnode = *envtable;
	}
	else
	{
		lastnode = *envtable;
		while (lastnode->next)
			lastnode = lastnode->next;
		lastnode->next = (t_env *)malloc(sizeof(t_env));
		lastnode = lastnode->next;
	}
	lastnode->env_name = getenvname(env);
	content = env + ft_strlen(lastnode->env_name);
	if (*content == '=' && *(content + 1))
		lastnode->content = ft_strdup(content + 1);
	else
		lastnode->content = NULL;
	lastnode->next = NULL;
	return (lastnode);
}
