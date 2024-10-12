#include "systemcalls.h"
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
 */
bool do_system(const char *cmd)
{

    /*
     * TODO  add your code here
     *  Call the system() function with the command set in the cmd
     *   and return a boolean true if the system() call completed with success
     *   or false() if it returned a failure
     */
    int ret = system(cmd);
    if (ret == -1 || ret == 127)
        return false;
    if (ret == 0 && cmd == NULL)
        return false;
    return true;
}

/**
 * @param count -The numbers of variables passed to the function. The variables are command to execute.
 *   followed by arguments to pass to the command
 *   Since exec() does not perform path expansion, the command to execute needs
 *   to be an absolute path.
 * @param ... - A list of 1 or more arguments after the @param count argument.
 *   The first is always the full path to the command to execute with execv()
 *   The remaining arguments are a list of arguments to pass to the command in execv()
 * @return true if the command @param ... with arguments @param arguments were executed successfully
 *   using the execv() call, false if an error occurred, either in invocation of the
 *   fork, waitpid, or execv() command, or if a non-zero return value was returned
 *   by the command issued in @param arguments with the specified arguments.
 */

bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);
    char *command[count + 1];
    int i;
    for (i = 0; i < count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    command[count] = command[count];

    /*
     * TODO:
     *   Execute a system command by calling fork, execv(),
     *   and wait instead of system (see LSP page 161).
     *   Use the command[0] as the full path to the command to execute
     *   (first argument to execv), and use the remaining arguments
     *   as second argument to the execv() command.
     *
     */

	pid_t pid;
    int exec_ret;
    int child_status;
    int ret_status;
	pid = fork();
	
	// if no child created, pid return value is negative
	if(pid == -1)
		exec_ret=0;
	else if (pid == 0) // current process is child process
	{
		ret_status=execv(command[0],command);
		if(ret_status==-1)
			exit(EXIT_FAILURE);
	}
	
	if (waitpid (-1, &child_status, 0) == -1)
			exec_ret=0;
	else if (WIFEXITED (child_status))
	{
		if(WEXITSTATUS(child_status)==0)
			exec_ret=1;
		else 
			exec_ret=0;		
	}
	else
		exec_ret=0;

    va_end(args);

    return exec_ret;
}

/**
 * @param outputfile - The full path to the file to write with command output.
 *   This file will be closed at completion of the function call.
 * All other parameters, see do_exec above
 */
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    va_start(args, count);
    char *command[count + 1];
    int i;
    for (i = 0; i < count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    command[count] = command[count];

    /*
     * TODO
     *   Call execv, but first using https://stackoverflow.com/a/13784315/1446624 as a refernce,
     *   redirect standard out to a file specified by outputfile.
     *   The rest of the behaviour is same as do_exec()
     *
     */

    //  redirect standard out to a file specified by outputfile
	int fd = open(outputfile, O_WRONLY|O_TRUNC|O_CREAT, 0644);
	dup2(fd, 1);
	
	//Fork to create a child process while the parent process is intact    
	pid_t pid;
    int exec_ret;
    int child_status;
    int ret_status;
	pid = fork();
	
	//If could not fork, no child created and pid return value is negative
	if(pid == -1)
		exec_ret=0;
	else if (pid == 0) //If current process is child process, execute command
	{
		ret_status=execv(command[0],command);
		if(ret_status==-1)
			exit(EXIT_FAILURE);
	}
	
	//Wait for child to terminate, returns -1 on error of any sort
	//else returns the exit code after replacing the child process and running the command provided in the shell
	//of the child. Based on the exit code, if command successfully executed, returns true else false

	if (waitpid (-1, &child_status, 0) == -1)
			exec_ret=0;
	else if (WIFEXITED (child_status))
	{
		if(WEXITSTATUS(child_status)==0)
			exec_ret=1;
		else 
			exec_ret=0;		
	}
	else
		exec_ret=0;

    va_end(args);

    return exec_ret;
}
