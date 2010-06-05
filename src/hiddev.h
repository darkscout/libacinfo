#ifndef HIDDEV_H
#define HIDDEV_H 1

#include <stdio.h>
#include <string.h>

#include <sys/ioctl.h>
#include <fcntl.h> 
#include <errno.h>

#include <linux/hiddev.h>


/*
 * The Aqua Computer Aquastream XT shows itself as a HID (Human Interface Device) usb device 
 * and communicates via Feature Reports.
 * 
 * With the following GET and SET functions these can be used.
 *
 */

int hiddev_get_feature_report(int fd, int report_id, unsigned char *buffer, int length);
int hiddev_set_feature_report(int fd, int report_id, unsigned char *buffer, int length);

#endif
