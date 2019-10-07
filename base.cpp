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
