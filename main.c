#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define MAXWORD 10
#define MAXCHAR  100


int inredirectionflag=0,outredirectionflag=0,pipingflag=0;
char* input=NULL;
char* output=NULL;


void read_line(char line[])
{
    fgets(line,MAXCHAR,stdin);
    int i;
    for(i=0; i<MAXCHAR; i++)
    {
        if(line[i]=='\n')
        {
            line[i]='\0';
            break;
        }
    }
    if(!strcmp(line,"exit"))
    {
        exit(0);
    }

}

int check (char* temp[])
{
    int i=0;
    while(temp[i]!=NULL)
    {
        if(strcmp(temp[i],">")==0)
        {
            outredirectionflag=1;
            output=temp[i+1];
            return i;
        }
        if(strcmp(temp[i],"<")==0)
        {
            inredirectionflag=1;
            input=temp[i+1];
            return i;
        }
        if(strcmp(temp[i],"|")==0)
        {
            pipingflag=1;
            return i;
        }
        i++;
    }
    return i;
}


int parse(char* args[],char line[])
{
    int i=0;
    args[i]=strtok(line," ");

    if(args[i]==NULL)
    {
        printf("NO COMMAND\n");
        return 1;
    }

    while(args[i]!=NULL)
    {
        i++;
        args[i]=strtok(NULL," ");
    }


    return 1;
}
int read_parse_line(char* args[],char line[])
{
    char* temp[MAXWORD];
    int i=0,pos;
    read_line(line);

    parse(temp,line);
    pos=check(temp);
    while(i<pos)
    {
        args[i]=temp[i];
        i++;
    }
    return 1;
}


int main()
{
    char * args[MAXWORD];
    char line[MAXCHAR];
    printf("Simple Shell : ");

    while(read_parse_line(args,line))
    {
        pid_t childpid=fork();
        if(childpid==0)
        {
            if(inredirectionflag==1&&input!=NULL)
                dup2(open(input,O_RDWR|O_CREAT,0777),0);

            if(outredirectionflag==1&&output!=NULL)
                dup2(open(output,O_RDWR|O_CREAT,0777),1);

            execvp(args[0],args);

        }
        else
        {
            waitpid(childpid,0);
            inredirectionflag=0;
            outredirectionflag=0;
            pipingflag=0;
            input=NULL;
            output=NULL;

        }
        printf("Simple Shell : ");

    }
    return 0;
}
