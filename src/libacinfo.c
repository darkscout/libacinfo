#include "libacinfo.h"
#include "debug.h"
#include "hiddev.h"

#include "aquastreamxt.h"


int detect_usb_hid_devices(struct libacinfo_handle *handle) 
{
	struct ac_device_list_element *dev, *prev = NULL;
	struct hiddev_string_descriptor hStr;
	struct hiddev_devinfo dinfo;
	char evdev[20];
	int fd, i;
	
	// reset values
	handle->device_list_head = NULL;
	handle->device_count = 0;


	for (i = 0; i<DETETECT_MAX; i++)
	{
		// build filename
		sprintf(evdev, "/dev/usb/hiddev%d", i);
		
		if ((fd = open(evdev, O_RDONLY)) >= 0)
		{
#ifdef DEBUG
			u_int32_t version;
		        ioctl(fd, HIDIOCGVERSION, &version);
		        printf("hiddev driver version is %d.%d.%d\n",
		               version >> 16, (version >> 8) & 0xff, version & 0xff);
#endif

		 	ioctl(fd, HIDIOCGDEVINFO, &dinfo);
		

#ifdef DEBUG
		        printf("HID: vendor 0x%x product 0x%x(0x%x) version 0x%x\n",
		               dinfo.vendor, dinfo.product & 0xffff, dinfo.product, dinfo.version);
#endif

	        	if ((dinfo.vendor == VENDOR_ID) && ((dinfo.product & 0xffff) == AQUASTREAM_XT))
	        	{
				hStr.index = 2; /* Vendor = 1, Product = 2 */
				hStr.value[0] = 0;
				ioctl(fd, HIDIOCGSTRING, &hStr);

		        	dev = (struct ac_device_list_element *) malloc(sizeof(struct ac_device_list_element));

		        	dev->next = NULL;
		        	dev->deviceType = AQUASTREAM_XT;
		        	dev->handle = fd; /* save file handle */
		        	strcpy(dev->name, hStr.value); /* save product name */

#ifdef DEBUG
		        	printf("Found '%s'\n", dev->name);
#endif		       
		        	handle->device_count++;

				// add to linked list 
		        	if (handle->device_list_head == NULL)
		        	{
		        		/* make first element in linked list */
		        		handle->device_list_head = dev;
		        		prev = dev;
		        	}
		        	else
		        	{
		        		/* append to list */
		        		prev->next = dev;
		        		prev = dev;
		        	}
			}
			else
			{
				close(fd);
			}
		}
	}

	return handle->device_count;
}

struct libacinfo_handle acinfo_init()
{
	struct libacinfo_handle handle;
	
	detect_usb_hid_devices(&handle);
	
	return handle;
}

int acinfo_deinit(struct libacinfo_handle *handle)
{
	return 0;
}



struct aquastreamxt_info* acinfo_read_info_aquastreamXT(struct ac_device_list_element *device)
{
        aquastreamxt_parse_report3(device);
        aquastreamxt_parse_report4(device);
        aquastreamxt_parse_report5(device);
        aquastreamxt_parse_report6(device);

	return NULL;
}

struct aquastreamxt_settings* acinfo_read_settings_aquastreamXT(struct ac_device_list_element *device)
{
	return NULL;
}

int acinfo_write_settings_aquastreamXT(struct ac_device_list_element *device, struct aquastreamxt_settings *values)
{
	return 0;
}


