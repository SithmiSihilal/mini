/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: siellage <siellage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 15:40:50 by siellage          #+#    #+#             */
/*   Updated: 2026/01/11 14:30:13 by siellage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//void	singleexportarg(t_cmdlist *cmdnode);
//void	doubleexportarg(char *envcmd);

int	runexport(t_core *core, t_cmdlist *cmdnode)
{
	int		arraylen;
	char	**temppath;

	core->exec_output = 0;
	arraylen = getarraylen(cmdnode->path);
	if (arraylen > 1)
	{
		temppath = cmdnode->path;
		while (*(++temppath))
			doubleexportarg(core, *temppath);
		sync_my_env(core);
	}
	else
		singleexportarg(core, cmdnode);
	return (core->exec_output);
}

void	singleexportarg(t_core *core, t_cmdlist *cmdnode)
{
	t_env	*env;

	env = core->env_table;
	while (env)
	{
		write(cmdnode->outfile, "declare -x ", 11);
		write(cmdnode->outfile, env->env_name, ft_strlen(env->env_name));
		if (!env->content)
		{
			write(cmdnode->outfile, "\n", 1);
			env = env->next;
			continue ;
		}
		write(cmdnode->outfile, "=\"", 2);
		write(cmdnode->outfile, env->content, ft_strlen(env->content));
		write(cmdnode->outfile, "\"\n", 2);
		env = env->next;
	}
}

void	doubleexportarg(t_core *core, char *envcmd)
{
	char	*arg;
	int		isequal;
	char	*tempenvname;

	if (!envargcontrol(core, envcmd, 1))
		return ;
	tempenvname = getenvname(envcmd);
	arg = envcmd + ft_strlen(tempenvname);
	isequal = 0;
	if (*arg == '=')
		isequal |= 1;
	if (changeenv(core, tempenvname, ++arg, isequal))
	{
		free(tempenvname);
		return ;
	}
	addnewenv(&core->env_table, envcmd);
	if (!isequal)
		updateenv(core, envcmd, NULL);
	free(tempenvname);
}

int	changeenv(t_core *core, char *envname, char *arg, int isequal)
{
	t_env	*env;

	env = core->env_table;
	while (env)
	{
		if (str_compare(envname, env->env_name))
		{
			if (isequal)
				updateenv(core, envname, arg);
			return (1);
		}
		env = env->next;
	}
	return (0);
}
