/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: siellage <siellage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 18:17:15 by glugo-mu          #+#    #+#             */
/*   Updated: 2026/01/11 14:31:32 by siellage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_core		core;
t_cmdlist	*cmd_to_cmdlist(t_cmd *cmd);

static t_token	*process_input(char *input, char ***parts_out, char **orig_out)
{
	char	**parts;
	char	*error;
	t_token	*tokens;

	*orig_out = ft_strdup(input);
	if (!*orig_out || !validate_input(input))
		return (NULL);
	parts = split_input(input);
	*parts_out = parts;
	if (!parts)
		return (NULL);
	error = NULL;
	tokens = tokenize(parts, &error);
	
	if (!tokens && error)
	{
		printf("minishell: syntax error near unexpected token `%s'\n", error);
		free(error);
	}
	return (tokens);
}

static int	execute_builtin(t_core *core, t_cmd *cmd)
{
	return (execute_builtin_simple(core, cmd));
}

static int	execute_cmd(t_core *core, t_cmd *first, char **my_env)
{
	int	status;

	status = 0;
	if (first->next)
		status = execute_pipeline(core, first, my_env);
	else if (first->argv && first->argv[0])
	{
		if (isbuiltin(first->argv[0]))
			status = execute_builtin(core, first);
		else
			status = execute_external(first, my_env);
	}
	return (status);
}

static int	process_loop(t_core *core, int *exit_status)
{
	char	*input;
	char	**parts;
	char	*original;
	t_token	*tokens;
	t_cmd	*cmds;

	input = readline("minishell> ");
	if (!input || ft_strcmp(input, "exit") == 0)
		return (free_resources(input, NULL, NULL), 0);
	if (*input)
		add_history(input);
	tokens = process_input(input, &parts, &original);
	if (tokens)
	{
		expand_tokens(tokens, core->my_env, *exit_status);
		cmds = commands_from_tokens(tokens, NULL);
		core->cmds = cmds;
		token_clear(&tokens);
		free_resources(input, parts, original);
		if (cmds)
		{
			*exit_status = execute_cmd(core, cmds, core->my_env);
			cmd_clear(&cmds);
		}
	}
	return (1);
}

int	main( int argc, char **argv, char **envp)
{
	char	**my_env;
	int		exit_status;
	

	t_core core;
	core = (t_core){0};
	(void)argc;
	(void)argv;
	exit_status = 0;
	if (!envp || !envp[0])
		my_env = init_env();
	else
		my_env = copy_env(envp);
	if (!my_env)
		return (1);
	// core = &core;
	init_global_env(&core, my_env);
	core.my_env = my_env;
	setup_signals();
	while (process_loop(&core, &exit_status))
		;
	free_env(core.my_env);
	free_core(&core);
	return (0);
}
