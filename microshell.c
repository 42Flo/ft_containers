#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define END 0
#define PIPE 1
#define SEMI 2

typedef struct  s_list
{
    char    **args;
    int size;
    int type;
    int  fd[2];

    struct s_list   *prev;
    struct s_list   *next;
}               t_list;

int ft_strlen(char *s)
{
    int i = 0;

    while (s[i] != 0)
        i++;
    return (i);
}

char    *ft_strdup(char *s)
{
    int size = ft_strlen(s);
    char    *new;

    if (!s)
        return (NULL);
    if (!(new = malloc(sizeof(char) * (size + 1))))
        return (NULL);
    new[size] = 0;
    while (--size >= 0)
        new[size] = s[size];
    return (new);
}

void    ft_putstr(int fd, char *s)
{
    write(fd, s, ft_strlen(s));
}

void    exitFatal(void)
{
    ft_putstr(STDERR, "error, fatal\n");
    exit(EXIT_FAILURE);
}

void    exitExecve(char *s)
{
    ft_putstr(STDERR, "error: cannot execute ");
    ft_putstr(STDERR, s);
    ft_putstr(STDERR, "\n");
    exit(EXIT_FAILURE);
}

int returnCdBadArg()
{
    ft_putstr(STDERR, "error: cd: bad arguments\n");
    return (EXIT_FAILURE);
}

int returnCdBadDir(char *str)
{
    ft_putstr(STDERR, "error: cd: cannot change directory to ");
    ft_putstr(STDERR, str);
    ft_putstr(STDERR, "\n");
    return (EXIT_FAILURE);
}

int getArgSize(char **argv)
{
    int i = 0;

    while (argv[i] && strcmp(argv[i], "|") != 0 && strcmp(argv[i], ";") != 0)
        i++;
    return (i);
}

void    lstAddBack(t_list **ptr, t_list *new)
{
    t_list  *tmp;

    if (*ptr == NULL)
        *ptr = new;
    else
    {
        tmp = *ptr;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new;
        new->prev = tmp;
    }
}

int getType(char *arg)
{
    if (!arg)
        return (END);
    if (strcmp(arg, "|") == 0)
        return (PIPE);
    if (strcmp(arg, ";") == 0)
        return (SEMI);
    return (-1);
}

int parseArgs(t_list **head, char **av)
{
    int size = getArgSize(av);
    t_list  *new;

    if (!(new = malloc(sizeof(t_list))))
        exitFatal();
    if (!(new->args = malloc(sizeof(char *) * (size + 1))))
        exitFatal();
    new->size = size;
    new->prev = NULL;
    new->args[size] = NULL;
    while (--size >= 0)
        new->args[size] = ft_strdup(av[size]);
    new->type = getType(av[new->size]);
    lstAddBack(head, new);
    return (new->size);
}

void    execute(t_list *tmp, char **env)
{
    pid_t   pid;
    int pipeOpen = 0;
    int status;

    if (tmp->type == PIPE || (tmp->prev && tmp->prev->type == PIPE))
    {
        pipeOpen = 1;
        if (pipe(tmp->fd))
            exitFatal();
    }
    pid = fork();
    if (pid < 0)
        exitFatal();
    else if (pid == 0)
    {
        if (tmp->type == PIPE && dup2(tmp->fd[STDOUT], STDOUT) < 0)
            exitFatal();
        if (tmp->prev && tmp->prev->type == PIPE && dup2(tmp->prev->fd[STDIN], STDIN) < 0)
            exitFatal();
        if ((execve(tmp->args[0], tmp->args, env)) < 0)
            exitExecve(tmp->args[0]);
        exit(EXIT_SUCCESS);
    }
    else
    {
        waitpid(pid, &status, 0);
        if (pipeOpen)
        {
            close(tmp->fd[STDOUT]);
            if (!tmp->next || tmp->type == SEMI)
                close(tmp->fd[STDIN]);
        }
        if (tmp->prev && tmp->prev->type == PIPE)
            close(tmp->prev->fd[STDIN]);
    }
}

void    exeCmd(t_list *ptr, char **env)
{
    t_list  *tmp = ptr;

    while (tmp)
    {
        if (strcmp("cd", tmp->args[0]) == 0)
        {
            if (tmp->size < 2)
                returnCdBadArg();
            else if (chdir(tmp->args[1]))
                returnCdBadDir(tmp->args[1]);
        }
        else
            execute(tmp, env);
        tmp = tmp->next;
    }
}

void    freeList(t_list *head)
{
    t_list  *tmp;
    int i;

    while (head)
    {
        tmp = head->next;
        i = 0;
        while (i < head->size)
            free(head->args[i++]);
        free(head->args);
        free(head);
        head = tmp;
    }
    head = NULL;
}

int main(int ac, char **av, char **env)
{
    t_list  *head = NULL;

    if (ac > 1)
    {
        int i = 1;

        while (av[i])
        {
            if (strcmp(av[i], ";") == 0)
            {
                i++;
                continue ;
            }
            i += parseArgs(&head, &av[i]);
            if (!av[i])
                break ;
            i++;
        }
        if (head)
            exeCmd(head, env);
        freeList(head);
    }
    return (0);
}
