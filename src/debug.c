#include "debug.h"

void debug_buffer(char *buffer, int len) 
{
	int i;
	char tmp[2];
	tmp[0] = ' ';
	tmp[1] = '\0';
	
	printf( "buffer:\n>>>\n" );
	for(i = 0; i < len; ++i) 
	{
		char c = buffer[i];
		
		if ((c >= ' ' && c <= 'z') || (c == 0x0d)  || (c == 0x0a))
		{
			tmp[0] = c;
		}
		else
		{
			tmp[0] = '*';
		}
		
		printf("%s", (char*)tmp);
	}
	printf("\n<<<\n");
}

void debug_buffer_hex(unsigned char *buffer, int len) 
{
	int i;
	
	printf( "hex-dump of buffer:\n" );
	printf( "---------------------------\n" );
	
	for(i=0; i < len; i++) 
	{
		unsigned char c = (unsigned char)buffer[i];
		printf( "%02X ", (int)c );
		
		if((i+1) % 16 == 0 ) printf( "\n" );
	}
	printf( "\n" "---------------------------" "\n" );
}
