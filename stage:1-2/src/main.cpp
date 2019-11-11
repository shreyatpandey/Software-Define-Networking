#include "base.h"
#include "primaryrouter.h"
#include "secondaryrouter.h"


int main(int argc,char* argv[])
{
    if(argc<2)
	{	
		perror("less number of arguments");
		exit(1);
	}
    Base base;
    base.readinputfile(argv[1]);
   Primary_Router primaryrouter(argv[1]);
   int primaryportnumber = primaryrouter.udpsocketcreation();
   for(int i=0;i<base.number_of_routers;i++)
   {
	   pid_t pid_fork = fork();
	   if(pid_fork == 0)
    {
         Secondary secondary(i+1,argv[1],primaryportnumber);
    	 secondary.secondaryudpsetup();	
    }
	
   }
   primaryrouter.tunnelreader();
   

    exit(0); 

}
