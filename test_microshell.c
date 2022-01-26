#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/wait.h>

int ft_strlen(char *s)
{
    int i = 0;

    while (s[i])
        ++i;
    return (i);
}

void    putstr_error(char *s)
{
    write(STDERR_FILENO, s, ft_strlen(s));
}

void    exit_fatal()
{
    putstr_error("error: fatal\n");
}

bool    ispipe(char *s)
{
    return (s && strcmp(s, "|") == 0);
}

bool    issemicolon(char *s)
{
    return (s && strcmp(s, ";") == 0);
}

int find_next_break(char **av)
{
    int i = 0;

    while (av[i])
    {
        if (ispipe(av[i]) || issemicolon(av[i]))
            break ;
        ++i;
    }
    return (i);
}

bool    check_pipe(char **av)
{
    return (ispipe(av[find_next_break(av)]));
}

int exec_cd(char **av)
{
    int i = find_next_break(av);

    if (i != 2)
        putstr_error("error: cd bad arguments\n");
    else if (chdir(av[1]) == -1)
    {
        putstr_error("error: cd: cannot change directory to ");
        putstr_error(av[1]);
        putstr_error("\n");
    }
    return (i);
}

void    exec_cmds(char **av, char **env, int *prevpipe)
{
    bool    pipeneed = check_pipe(av);
    int pipefd[2];
    pid_t   pid;

    if (pipeneed)
        if (pipe(pipefd) == -1)
            exit_fatal();
    if ((pid = fork()) == -1)
        exit_fatal();
    else if (pid == 0)
    {
        int i = find_next_break(av);

        av[i] = NULL;
        if (*prevpipe > 1)
        {
            if (dup2(*prevpipe, STDIN_FILENO) == -1)
                exit_fatal();
        }
        if (pipeneed)
        {
            if (dup2(pipefd[STDOUT_FILENO], STDOUT_FILENO) == -1)
                exit_fatal();
        }
        if (execve(av[0], av, env) == -1)
        {
            putstr_error("error: canoot execute ");
            putstr_error(av[0]);
            putstr_error("\n");
        }
        exit(EXIT_FAILURE);
    }
    else
    {
        if (*prevpipe > 1)
        {
            if (close(*prevpipe) == -1)
                exit_fatal();
            *prevpipe = 0;
        }
        if (pipeneed)
        {
            if (close(pipefd[STDOUT_FILENO]) == -1)
                exit_fatal();
            *prevpipe = pipefd[STDIN_FILENO];
        }
        waitpid(0, NULL, 0);
    }
}

int main(int ac, char **av, char **env)
{
    (void)ac;
    int i = 1;
    int prevpipe = 0;

    while (av[i])
    {
        while (issemicolon(av[i]))
            ++i;
        if (!av[i])
            break;
        if (strcmp(av[i], "cd") == 0)
            i += exec_cd(&av[i]);
        else
        {
            exec_cmds(&av[i], env, &prevpipe);
            i += find_next_break(&av[i]);
        }
        if (av[i])
            ++i;
    }

    return (0);
}
