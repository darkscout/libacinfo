/*
 *
 * You can find the latest source code at:
 *
 *   http://github.com/darkscout/libacinfo
 *
 * Copyright (c) 2009, 2010 Swen Weiland  <Swen.Weiland@gmx.net>
 *
 * See LICENSE.txt in root directory.
 *
 *
 */

#include "hiddev.h"

/* some info
 * http://www.mail-archive.com/linux-usb-devel@lists.sourceforge.net/msg17143.html
 * https://svn.symbianos.org/people/alfredh/hid/hid.c
 */


int hiddev_get_feature_report(int fd, int report_id, unsigned char *buffer, int length)
{
	struct hiddev_report_info rinfo;
	struct hiddev_field_info finfo;
  	struct hiddev_usage_ref_multi ref_multi_i;
	int ret, report_length, i;
	
	finfo.report_type = HID_REPORT_TYPE_FEATURE;
	finfo.report_id = report_id;
	finfo.field_index = 0;

	/* request info about the feature report (get byte length) */
        ret = ioctl(fd, HIDIOCGFIELDINFO, &finfo);
	
	report_length = finfo.maxusage;
		
	if (report_length > length)
	{
		fprintf(stderr, "report length %d - buffen length %d\n", report_length, length);
		/* buffer too small */
		return 0;
	}
	
	
	rinfo.report_type = HID_REPORT_TYPE_FEATURE;
	rinfo.report_id = report_id;
	rinfo.num_fields = 1;
	
	ref_multi_i.uref.report_type = HID_REPORT_TYPE_FEATURE;
	ref_multi_i.uref.report_id = report_id;
	ref_multi_i.uref.field_index = 0;
	ref_multi_i.uref.usage_index = 0; /* byte index??? */
	ref_multi_i.num_values = report_length;
	
	/* request feature report */
        ret = ioctl(fd, HIDIOCGREPORT, &rinfo);
        
	if (ret != 0)
	{
		fprintf(stderr, "HIDIOCGREPORT (%s)\n", strerror(errno));
		return 0;
	}                

        /* multibyte transfer to local buffer */
	ret = ioctl(fd, HIDIOCGUSAGES, &ref_multi_i);
	
	if (ret != 0)
	{
		fprintf(stderr, "HIDIOCGUSAGES (%s)\n", strerror(errno));
		return 0;
	}                
	
	/* copy bytes to output buffer (missing the first byte - report id)
	 * can't use memcpy, because values are signed int32 
	 */
	for(i=0; i<report_length-1; i++)
		buffer[i] = ref_multi_i.values[i];

	
	return report_length;
}

int hiddev_set_feature_report(int fd, int report_id, unsigned char *buffer, int length)
{
	struct hiddev_report_info rinfo;
	struct hiddev_field_info finfo;
  	struct hiddev_usage_ref_multi ref_multi_i;
	int ret, report_length, i;
	
	finfo.report_type = HID_REPORT_TYPE_FEATURE;
	finfo.report_id = report_id;
	finfo.field_index = 0;

	/* request info about the feature report (get byte length) */
        ret = ioctl(fd, HIDIOCGFIELDINFO, &finfo);
	
	report_length = finfo.maxusage;
		
	if (report_length != length)
	{
		fprintf(stderr, "report length %d - buffen length %d\n", report_length, length);
		/* buffer has wrong size - we need full report */
		return 0;
	}
	
	
	rinfo.report_type = HID_REPORT_TYPE_FEATURE;
	rinfo.report_id = report_id;
	rinfo.num_fields = 1;
	
	ref_multi_i.uref.report_type = HID_REPORT_TYPE_FEATURE;
	ref_multi_i.uref.report_id = report_id;
	ref_multi_i.uref.field_index = 0;
	ref_multi_i.uref.usage_index = 0; /* byte index??? */
	ref_multi_i.num_values = report_length;

	/* copy bytes to output buffer (missing the first byte - report id)
	 * can't use memcpy, because values are signed int32 
	 */
	for(i=0; i<report_length-1; i++)
		ref_multi_i.values[i] = buffer[i];

        /* multibyte transfer to device */
	ret = ioctl(fd, HIDIOCSUSAGES, &ref_multi_i);
	
	if (ret != 0)
	{
		fprintf(stderr, "HIDIOCSUSAGE (%s)\n", strerror(errno));
		return 0;
	}
	
		
	/* write report to device */
        ret = ioctl(fd, HIDIOCSREPORT, &rinfo);
        
	if (ret != 0)
	{
		fprintf(stderr, "HIDIOCSREPORT (%s)\n", strerror(errno));
		return 0;
	}                               	
	
	return report_length;
}

/*
set

	ioctl(fd,HIDIOCGUSAGES, &ref_multi_u); //Write ref_multi_u.values
	ioctl(fd,HIDIOCSREPORT, &rep_info_u);
	
*/
