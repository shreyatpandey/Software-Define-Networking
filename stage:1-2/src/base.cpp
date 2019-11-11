#include "base.h"


void Base::readinputfile(string s)
{
        ifstream config_file(s);
		string line_read;
		while(!config_file.eof())
		{
			getline(config_file,line_read);
			if(!(line_read[0] == '#'))
			{
				if(regex_search(line_read,regex("stage")))
				{
					for(size_t i=0;i<line_read.length();i++)
					{
						if(isdigit(line_read[i]))
						{
							stage = stage*10 + (line_read[i] - '0');
						}
					}
				}
				if(regex_search(line_read,regex("num_routers")))
				{
					for(size_t i=0;i<line_read.length();i++)
				{
					if(isdigit(line_read[i]))
					{
						number_of_routers = number_of_routers*10 + (line_read[i] - '0');	
					}
				}

			}
		}
    
    }
}
void Base::cancellationpoint(int value,string message)
{
	if (value < 0)
	{
		cout<<message<<endl;
		exit(0);
	}

}
unsigned int Base::checksum(char *addr, short count) /**Checksum code taken from class-provided code*/
{
	/* Compute Internet Checksum for "count" bytes
        *         beginning at location "addr".
        */
       register long sum = 0;

        while( count > 1 )  
        {

           /*  This is the inner loop */
               sum += *(unsigned short *) addr;
	       addr += 2;
               count -= 2;
        }

           /*  Add left-over byte, if any */
       if( count > 0 )
               sum += * (unsigned char *) addr;

           /*  Fold 32-bit sum to 16 bits */
       while (sum>>16)
           sum = (sum & 0xffff) + (sum >> 16);

       return (unsigned short) ~sum;
}
