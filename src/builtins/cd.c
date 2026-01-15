/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: siellage <siellage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 15:32:04 by siellage          #+#    #+#             */
/*   Updated: 2026/01/11 14:29:46 by siellage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	runcd(t_core *core, t_cmdlist *cmdnode)
{
	int		arraylen;

	arraylen = getarraylen(cmdnode->path);
	if (arraylen > 2)
	{
		core->exec_output = 1;
		print_error("-bash: cd: too many arguments\n", NULL, NULL);
	}
	else if (arraylen == 2)
		cddoublearg(core, cmdnode);
	else
		cdsinglearg(core);
}

void	cdsinglearg(t_core *core)
{
	t_env	*tempenv;
	char	*content;

	tempenv = core->env_table;
	while (tempenv)
	{
		if (str_compare("HOME", tempenv->env_name))
		{
			content = tempenv->content;
			if (!content)
				return ;
			changedir(core, content);
			return ;
		}
		tempenv = tempenv->next;
	}
	print_error("-bash: cd: HOME not set\n", NULL, NULL);
	core->exec_output |= 1;
}

void	cddoublearg(t_core *core, t_cmdlist *cmdnode)
{
	if (strcmp(cmdnode->path[1], "~") == 0)
	{
        cdsinglearg(core);
        return;
    }
	if (!changedir(core, cmdnode->path[1]))
	{
		print_error("--bash: cd: ", cmdnode->path[1],
			": No such file or directory\n");
		core->exec_output = 1;
		return ;
	}
}

int	changedir(t_core *core, char *path)
{
	char	pwd[256];
	int		ispwdaccess;
	char	*oldpwd;
	int		error;

	oldpwd = ft_strdup(getcwd(pwd, 256));
	error = chdir(path);
	if (error == -1)
	{
		if (oldpwd)
			free(oldpwd);
		return (0);
	}
	ispwdaccess = updatepwdfromexport(core, "PWD", getcwd(pwd, 256));
	if (ispwdaccess)
		updatepwdfromexport(core, "OLDPWD", oldpwd);
	else
		deleteenv(core, "OLDPWD");
	if (oldpwd)
		free(oldpwd);
	sync_my_env(core);
	changetitle();
	return (1);
}

int	updatepwdfromexport(t_core *core, char *pwdname, char *pwdcontent)
{
	char	*temppwd;

	if (!updateenv(core, pwdname, pwdcontent))
	{
		temppwd = NULL;
		ownstrjoin(&temppwd, pwdname);
		straddchar(&temppwd, '=');
		ownstrjoin(&temppwd, pwdcontent);
		addnewenv(&core->env_table, temppwd);
		free(temppwd);
		return (0);
	}
	return (1);
}
