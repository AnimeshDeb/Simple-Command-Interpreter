#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    int childprocess1;
    int childprocess2;
    while(1)
    {
        //The line directly below makes it so that an array of char is created, the size being 500. The type is of char because this array will store the commands.
        char cmd[500];  
        //Line below will indicate to user to input a command.           
        printf("command: ");
        //The cmd array will store the inputted command.
        scanf(" %[^\n]s", cmd); 
        //The code section below is included so that we can pipe successfully.
        char *str = NULL;
        char *holder = strtok_r(cmd, " ", &str);
        int d = 0;
        int e = 0;
        int f = 0;
        int g = 0;
        char *arr_arg[100];
        char *arr_arg_p[100];        
        char *arr_arg_q[100];        
        bool qpipe;
        //The while loop below makes it so that we keep on getting values into holder until the value of NULL is received.
        while(holder != NULL)
        {
            arr_arg[d] = holder;
            if(!strcmp(arr_arg[d], "|"))
            {
                qpipe= true;
                g = d;
            }
            d++;
            holder = strtok_r(NULL, " ", &str);   
        }
        //The final value in the arr_arg array is set to the value of NULL.
        arr_arg[d] = NULL;                  
        //The if statment below makes it so that both the first and the second command can be taken in from the user.
        if(qpipe=false)
        {   for(f=0; f < d-g; f++)
            {
                arr_arg_q[f] = arr_arg[g+1];     
                g++;
            }
            arr_arg_q[f] = NULL;                         
            for(e=0; e<g; e++)
            {
                arr_arg_p[e] = arr_arg[e];  
            }
            arr_arg_p[e] = NULL;
        }
        int pdesc[2];
        //Checking below if piping is successful. Error message will be printed if it's not successful.
        if(pipe(pdesc) == -1)
        {
            printf("The command, pipe(), was not successful in being executed.\n");
            perror("pipe");
            return 1;
        }
        //fork() is used to create first child process.
        childprocess1 = fork();
        //If forking is not successful, then error message will be approprietly outputed.
        if(childprocess1== -1)
        {
            printf("\n The command, fork(), was not successful in being executed with respect to the first child process.\n");
        }
        //If forking is successful and a child process is created accordingly, the following code will run. In the first part of the child process, the arr_arg[] will
        //be executed.
        if(childprocess1== 0)
        {      
            if(!qpipe)
            {
                execvp(arr_arg[0], arr_arg);        
                printf("\nEXECVP FAILED.\n");
                return 0;
            }
            //Using the else, we now refer to the parent process. The code below accordingly all refer to the parent process.
            else
            {
                //fork() is used to create a second child process.
                childprocess2= fork();
                //If forking is unsuccessful then an error message will be printed like before.
                if(childprocess2== -1)
                {
                    printf("\n The command, fork(), was not executed successfully with respect to the second child process.\n");
                }
                //If forking is successful, then the code below will run. The dup2 will make it so that the 0 is going to be the same as the read-from end of the pipe. The dup2 is also used to create a copy
                //of the file descriptor. Afterwards, the execvp command will be run with respect to arr_arg_q[]. Then the close call is used to close the pdesc.
                if(childprocess2== 0)
                {
                    dup2(pdesc[0],0); 
                    execvp(arr_arg_q[0], arr_arg_q);        
                    printf("\nEXECVP FAILED.\n");
                    close(pdesc[1]);
                }
                else
                {
                    //This is the parent process and the first part of this code makes it so that 1 is the same as the write-to end of pipe. The dup2 is used like before, to create a copy
                    // of the file descriptor used here. The execvp command is used with respect to arr_arg_p[], and then the pdesc is closed using the close call.
                    dup2(pdesc[1],1);      
                    execvp(arr_arg_p[0], arr_arg_p);        
                    printf("\nEXECVP FAILED.\n");
                    close(pdesc[0]);
                }
            }
        }
        //The code below refers to the main parent process as is indicated with the else. In this parent process, wait is used with respect to NULL.
        else
        {             
            wait(NULL);
        }
}}