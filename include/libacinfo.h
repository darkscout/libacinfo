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

#ifndef LIBACINFO_H
#define LIBACINFO_H 1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>

#include <sys/ioctl.h>
#include <fcntl.h> 
#include <linux/hiddev.h>


#ifndef NULL
	#define NULL 0
#endif



/* 'MCT Elektronikladen' = Aqua Computer*/
#define VENDOR_ID 		0x0c70 

/**
 * USB Product Codes
 **/

#define AQUADUCT240ECO		0xf0bc
#define AQUADUCT240PRO		0xf0b7
#define AQUADUCT360 		0xf0b8
#define AQUADUCT360ECO		0xf0bb
#define AQUAERO_REV4		0xf0b0
#define AQUAERO_REV4_HUB 	0xf0b3
#define AQUASTREAM_USB 		0xf0b6
#define AQUASTREAM_XT 		AQUASTREAM_USB
#define CNC_EXPLORERDONGLE 	0xf0ba
#define FLOWSENSOR_USB 		0xf0b9
#define MULTISWITCH_REV141 	0xf0b2
#define POWERADJUST 		0xf0b5
#define TUBEMETER_REV1 		0xf0b1
#define TURBOPLEX_REV1 		0xf0b4

#define DETETECT_MAX 6
#define REPORT_INPUT_LEN 512


struct ac_device_list_element 
{
	char name[256];
	int handle;
	
	u_int16_t deviceType; // see product code defines above!
	
	struct ac_device_list_element *next;
};

struct libacinfo_handle 
{
	int device_count;
	struct ac_device_list_element *device_list_head;
};



struct aquastreamxt_settings
{
	/* report 6 */
        u_int8_t i2cAddress; 

        u_int8_t i2cSetting_aquabusEnable:1; 
        u_int8_t i2cSetting_dummy:7; 
        
	u_int8_t pumpMode_dearation:1;
	u_int8_t pumpMode_autoPumpMaxFreq:1;
	u_int8_t pumpMode_dearationModeSens:1;
	u_int8_t pumpMode_resetPumpMaxFreq:1;
	u_int8_t pumpMode_i2cControl:1;
	u_int8_t pumpMode_minFreqForce:1;
	u_int8_t pumpModeB; 

	u_int8_t sensorBridge; /* enum tSensorBridge */
	
	u_int8_t measureFanEdges; 
	u_int8_t measureFlowEdges; 
	
	u_int16_t pumpFrequency; 
	u_int32_t frequencyResetCycle; /* int32((frequencyResetCycle / 0x113)) */
	
	u_int8_t alarm_sensor0:1; 
	u_int8_t alarm_sensor1:1; 
	u_int8_t alarm_pump:1; 
	u_int8_t alarm_fan:1; 
	u_int8_t alarm_flow:1; 
	u_int8_t alarm_fanShort:1; 
	u_int8_t alarm_fanOverTemp90:1; 
	u_int8_t alarm_fanOverTemp70:1; 
	
	u_int8_t tachoMode_linkFan:1; 
	u_int8_t tachoMode_linkFlow:1; 
	u_int8_t tachoMode_linkPump:1; 
	u_int8_t tachoMode_linkStatic:1; 
	u_int8_t tachoMode_linkAlarmInterrupt:1; 
	u_int8_t tachoMode_dummy:3;
	
	u_int16_t tachoFrequency; 
	
	u_int32_t flowAlarmValue; 
	u_int16_t sensorAlarmTemperature[2]; 
	
	u_int8_t fanMode_manual:1; 
	u_int8_t fanMode_auto:1; 
	u_int8_t fanMode_holdMinPower:1; 
		
	u_int8_t fanManualPower; 
	
	u_int16_t controllerHysterese; /* use convert2temp */
	u_int8_t controllerSensor; 
	u_int16_t controllerSetTemp;  /* use convert2temp */
	u_int16_t controllerP; 
	u_int16_t controllerI; 
	u_int16_t controllerD; 
	
	u_int16_t sensorMinTemperature; 
	u_int16_t sensorMaxTemperature; 
	
	u_int8_t fanMinimumPower; 
	u_int8_t fanMaximumPower; 
	
	u_int8_t ledSettings; 
	u_int8_t aquabusTimeout; 
	
	u_int16_t minPumpFreqency; 
	u_int16_t maxPumpFreqency; 

	/* report 5 */
        u_int8_t advancedPumpKey[6];
        u_int8_t fanAmpKey[6];
        u_int8_t fanControllerKey[6];

	/* AD values - 5 °C steps report 3 */
        u_int16_t sensorFanCurve[21];
        u_int16_t sensorExternCurve[21];
        u_int16_t sensorWaterCurve[21];
        u_int16_t temperatureMatchCurve[21];
};

struct aquastreamxt_info
{
	double temp_pump;
	double temp_extern;
	double temp_water;
	
	double fan_rpm; 
	double fan_voltage; 
	double fan_voltage_measured;
	
	double fan_power; /* percent */
	double pump_power; /* percent TODO */

	double flow;
	
	double controller_I;
	double controller_P;
	double controller_D;
	double controller_Output;
	
	double frequency;
	double frequency_max;
	
        /*
         *	rawSensorData[0] = 
         *	rawSensorData[1] = 
         *	rawSensorData[2] = 
         *	rawSensorData[3] = FAN Voltage
         
         *	get_pumpPower = ((this->GetPumpCurrent(this->m_rawSensorData[5]) / 1000) * this->GetVDD(this->m_rawSensorData[4]))
         
         *	rawSensorData[4] = (VDD)
         *	rawSensorData[5] = (pumpCurrent) this->GetPumpCurrent(this->m_rawSensorData[5]);
         */
        
	u_int8_t alarm_sensor0:1;
	u_int8_t alarm_sensor1:1;
	u_int8_t alarm_dummy1:1;
	u_int8_t alarm_fan:1;
	u_int8_t alarm_flow:1;

	u_int8_t mode_advancedPumpSettings:1;
	u_int8_t mode_aquastreamModeAdvanced:1;
	u_int8_t mode_aquastreamModeUltra:1;
  
        u_int16_t firmware; /* littleEndian */        
        u_int16_t bootloader; /* littleEndian */        
        u_int16_t hardware; /* littleEndian */        
        	
        u_int16_t serial; /* littleEndian */        
        	
        u_int8_t publicKey[6]; /* littleEndian */        
};




struct libacinfo_handle acinfo_init();
int acinfo_deinit(struct libacinfo_handle *handle);



struct aquastreamxt_info* acinfo_read_info_aquastreamXT(struct ac_device_list_element *device);
struct aquastreamxt_settings* acinfo_read_settings_aquastreamXT(struct ac_device_list_element *device);

int acinfo_write_settings_aquastreamXT(struct ac_device_list_element *device, struct aquastreamxt_settings *values);

#endif
