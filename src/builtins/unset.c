/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: siellage <siellage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 15:34:23 by siellage          #+#    #+#             */
/*   Updated: 2026/01/11 14:29:12 by siellage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	rununset(t_core *core, t_cmdlist *cmdnode)
{
	char	**tempname;
	int		arraylen;

	arraylen = getarraylen(cmdnode->path);
	if (arraylen > 1)
	{
		tempname = cmdnode->path;
		while (*(++tempname))
		{
			if (!envargcontrol(core, *tempname, 0))
			{
				// print_error("-bash: unset: `", *tempname,
				// 	"': not a valid identifier\n");
				// core->exec_output = 1;
				continue ;
			}
			else
				deleteenv(core, *tempname);
		}
		sync_my_env(core);
	}
}

void	deleteenv(t_core *core, char *name)
{
	t_env	*env;
	t_env	*tempenv;

	env = core->env_table;
	while (env)
	{
		if (str_compare(env->env_name, name))
		{
			if (env->content)
				free(env->content);
			free(env->env_name);
			if (core->env_table == env)
				core->env_table = core->env_table->next;
			else
				tempenv->next = env->next;
			free(env);
			break ;
		}
		tempenv = env;
		env = env->next;
	}
}
