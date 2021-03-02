//nodeの先頭をみた時、
//if (node->content->flag == FT_SEMICOLON_F)
	//nodeのl_nodeへ
	//nodeのr_nodeへ
//if (node->content->flag == FT_PIPE_F)
	//1.パイプラインの作成
		//int pipe_fd[2];
		//pipe(pipe_fd);
	//2.子プロセスの作成 & 3.子プロセスでコマンド実行
		//---l_node--------------------//
		//pid_t child_p1 = fork();
			//if (child_p1 == 0)
				//close(pipe_fd[0]);
				//dup2(pipe_fd[1], 1);
				//close(pipe_fd[1]);
				//exec_command();	//execve()実行時、forkしなくて良い
		//---r_node--------------------//
		//pid_t child_p2 = fork();
			//if (child_p2 == 0)
				//close(pipe_fd[1]);
				//dup2(pipe_fd[0], 0);
				//close(pipe_fd[0]);
				//exec_command();	//execve()実行時、forkしなくて良い
	//4.親側でpipeをclose
		//close(pipe_fd[0]);
		//close(pipe_fd[1]);
	//5.子プロセスの終了を待つ
		//waitpid(child_p1, &pid_status, 0);
		//waitpid(child_p2, &pid_status, 0);
//if (node->content->flag == FT_REDIRECT)
	//if (<)
		//---r_node(filename)---------------//
			//int file_fd = open("filename_path", OOO);

		//---l_node(FT_COMMAND_F || FT_REDIRECT)------//
			//if (l_node->content->flag == FT_REDIRECT)
				//
			//else (FT_COMMAND_F)
				//read(file_fd, buf, count); ????
				//dup2(file_fd, 0); ????
				//exec_command();	//execve()実行時、forkする
		//close(file_fd);

	//else if (>)
		//---r_node(filename)---------------//
			//file_fd = open("filename_path", OOO);

		//---l_node(FT_COMMAND_F || FT_REDIRECT)------//
			//if (l_node->content->flag == FT_REDIRECT)
				//
			//else (FT_COMMAND_F)
				//dup2(file_fd, 1);	//stdoutの書き換え
				//exec_command();	//execve()実行時、forkする
		//close(file_fd);
//if (node->content->flag ==  FT_COMMAND_F)
	//if (builtiin)
		//exec_command();
	//else
		//pid_t child_p  = fork();
		//if (child == 0)
			//exec_execve();
		//else
			//waitpid(child_p, &pid_status, 0);

	int		pipe_fd[2];
	pid_t	pid;
	int		pid_status;

	if (pipe(pipe_fd) == -1)
	{
		ft_putendl_fd(strerror(errno), 1);
	}
	pid = fork();
	if (pid == -1) /* forkのエラー(プロセス複製失敗) */
	{
		ft_putendl_fd(strerror(errno), 1);
		return(errno);
	}
	if (pid == 0) /* 以下のコードが子プロセスで実行される */
	{
		printf("in child pid=[%d]\n", getpid());//del
		close(pipe_fd[0]);
		dup2(pipe_fd[1], 1);
		close(pipe_fd[1]);
		*exit_status = execute_env(env->data);//exec_command();
	}
	else /* 以下のコードが親プロセスで実行される  */
	{
		printf("in parent pid=[%d]\n", getpid());//del
		if((waitpid(pid, &pid_status, 0) == -1))
		{
			// printf("in parent pid=[%d]\n", getpid());//del
			ft_putendl_fd(strerror(errno), 1);
			return (errno);
		}
		close(pipe_fd[1]);
		dup2(pipe_fd[0], 0);
		close(pipe_fd[0]);
		execute_execve(token, env);//exec_command();
	}
	printf("out parent pid=[%d]\n", getpid());//del
	return (WEXITSTATUS(pid_status));