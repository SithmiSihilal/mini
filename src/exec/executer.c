/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: siellage <siellage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 13:51:12 by siellage          #+#    #+#             */
/*   Updated: 2025/12/19 11:18:11 by siellage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell2.h"

void	runmultiplecommand(t_cmdlist *cmdlist)
{
	int			*fd;

	fd = createpipe();
	while (cmdlist)
	{
		switchpipe(&fd);
		pipe(&fd[2]);
		cmdlist->pid = fork();
		if (cmdlist->pid <= 0)
			runprocess(cmdlist, fd, 2);
		cmdlist = cmdlist->next;
		if (fd[4] && fd[5])
		{
			close(fd[4]);
			close(fd[5]);
			fd[4] = 0;
			fd[5] = 0;
		}
	}
	clearpipe(fd);
	waitall();
}

void	runsinglecommand(t_core *core, t_cmdlist *cmdlist, int *fd)
{
	if (cmdlist->infile != SSTDERR && cmdlist->outfile != SSTDERR)
		execcommand(cmdlist, fd, -1);
}

void	executer(t_core *core)
{
	if (!core->cmd_table)
		return ;
	else if (!core->cmd_table->next)
		runsinglecommand(core->cmd_table, NULL);
	else
		runmultiplecommand(core->cmd_table);
}
