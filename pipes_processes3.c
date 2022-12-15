#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

/**
 * Pipes Processes 3
 * Executes the command "cat scores | grep Lakers".  In this quick-and-dirty
 * implementation the parent doesn't wait for the child to finish and
 * so the command prompt may reappear before the child terminates.
 *
 */

int main(int argc, char **argv)
{
  int pipefd[2];
  int pipe2fd[2];
  int pid;

  char *cat_args[] = {"cat", "scores", NULL};
  char *sort_args[] = {"sort", NULL};
  //char *test_args[] = {"grep", "28", NULL};
  char *grep_2_args[3];

  grep_2_args[0] = "grep";
  int i = 1;
  //printf("%d\n", argc);
  while (i<2){
    grep_2_args[i]= argv[i];
    i++;
  }
  grep_2_args[i] = NULL;
  /*
  i = 0;
  while ((i < argc) && (grep_2_args[i]!=NULL)){
    printf("%s\n", grep_2_args[i]);
    i++;
  }
  */
  // make a pipe (fds go in pipefd[0] and pipefd[1])

  pipe(pipefd);

  pid = fork();

  if (pid == 0)//child 1
    {
      pipe(pipe2fd);
      pid_t pid2 = fork();

      if(pid2 == 0){ // child 2
        dup2(pipe2fd[0], 0);
        close(pipe2fd[1]);
        //close(pipefd[0]);
        close(pipefd[1]);
        execvp("sort", sort_args);
        //printf("This is a conundrum\n");
      }else{ // child 1

        // child gets here and handles "grep Villanova"

        // replace standard input with input part of pipe

        dup2(pipefd[0], 0);
        dup2(pipe2fd[1], 1);

        // close unused half of pipe

        close(pipefd[1]);
        close(pipe2fd[0]);
        // replace standard output with output part of pipe
        //dup2(pipefd[1], 1);
        // execute grep

        execvp("grep", grep_2_args);
      }      
    }
  else //parent
    {
      // parent gets here and handles "cat scores"

      // replace standard output with output part of pipe

      dup2(pipefd[1], 1);

      // close unused unput half of pipe

      close(pipefd[0]);

      // execute cat

      execvp("cat", cat_args);
      //printf("This Lakers is fun\n");
    }
}