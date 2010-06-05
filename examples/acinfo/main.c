#include <stdio.h>
#include <stdlib.h>


#include <libacinfo.h>

int main(int argc, char **argv)
{
	struct libacinfo_handle libhandle;
	
	struct ac_device_list_element *device;	

	                 
////    	start_listening(); 


	libhandle = acinfo_init();

	if (libhandle.device_count <= 0)
	{
		fprintf(stderr, "Couldn't find Aqua Computer USB device.\n");
		exit(1);
	}

	device = libhandle.device_list_head;
		
    	while (device != 0)
        {
		struct aquastreamxt_info *info;
		info = acinfo_read_info_aquastreamXT(device);

		device = device->next;
	}
	                              
	return 0;
}



