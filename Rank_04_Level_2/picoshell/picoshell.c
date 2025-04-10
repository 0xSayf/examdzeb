int picoshell(char **cmds[])
{
    int fd[2], prev_fd = -1;
    int res = 0;
    int i = 0;
    pid_t pid;

    while (cmds[i])
    {
        if (cmds[i + 1])
            pipe(fd);

        pid = fork();
        
        if (pid == 0)
        {
            if (prev_fd != -1)
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }
        
            if (cmds[i + 1])
            {
                close(fd[0]);               
                dup2(fd[1], STDOUT_FILENO); 
                close(fd[1]);                
            }
        
            execvp(cmds[i][0], cmds[i]);
            exit(1); 
        }
        
        if (prev_fd != -1)
            close(prev_fd);
        
        if (cmds[i + 1])
        {
            close(fd[1]);
            prev_fd = fd[0];
        }
        i++;
    }
    
    int status;
    
    while (wait(&status) != -1)
    {
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
            res = 1;
    }
    return res;
}