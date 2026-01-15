/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: siellage <siellage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 10:35:14 by siellage          #+#    #+#             */
/*   Updated: 2025/12/19 11:27:09 by siellage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell2.h"

void	runprocess(t_core *core, t_cmdlist *cmdlist, int *fd, int fdindex)
{
	if (cmdlist->infile != SSTDERR && cmdlist->outfile != SSTDERR)
		exec_command(cmdlist, fd, fdindex);
	else
		clearpipe(fd);
	freeforloop();
	freecore();
	exit(core->exec_output);
}

void	waitall(t_core *core)
{
	t_cmdlist	*cmdlist;

	cmdlist = core->cmd_table;
	while (cmdlist)
	{
		if (!cmdlist->next)
			waitpid(cmdlist->pid, &core->exec_output, 0);
		else
			waitpid(cmdlist->pid, 0, 0);
		cmdlist = cmdlist->next;
	}
	core->exec_output = WEXITSTATUS(core->exec_output);
}
