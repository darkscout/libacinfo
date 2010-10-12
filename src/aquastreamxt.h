#ifndef AQUASTREAMXT_H
#define AQUASTREAMXT_H 1

#include <sys/types.h>
#include <stdio.h>

#include "libacinfo.h"
#include "basic_convert.h"
#include "hiddev.h"

#define AQUASTREAMXT_PUMP_ADVANCED_REPORT 1
#define AQUASTREAMXT_PUMP_EVENT_REPORT 2
#define AQUASTREAMXT_SENSOR_SETTINGS_DATA_REPORT 3
#define AQUASTREAMXT_PUMP_DATA_TRANSFER_REPORT 4
#define AQUASTREAMXT_KEYS_REPORT 5
#define AQUASTREAMXT_SETTINGS_REPORT 6

/*
const Double __gc* SCALE12V = 61;
const Double __gc* SCALEFANOUT = 63;
const Double __gc* SCALEPUMPCURRENT = 1.6;
m_impFlow = 0x100;
m_edgesFan = 8;
m_edgesFlow = 8;
*/

struct report_pump_advanced_setting /* report 1 */
{
        u_int8_t data[16];
} __attribute__((__packed__));


enum tAquastreamXtUsbEvent
{
	checkKeys = 1,
	loadDefaults = 3,
	loadDefaultSensor = 8,
	loadEeData = 4,
	resetController = 7,
	resetMaxFreq = 2,
	saveEeData = 5
};

/* Write only ?! */
struct report_pump_event /* report 2 */
{
        u_int16_t event;
} __attribute__((__packed__));


struct report_senosr_settings_data /* report 3 */
{
	/* AD values - 5 °C steps */
        u_int16_t sensorFanCurve[21];
        u_int16_t sensorExternCurve[21];
        u_int16_t sensorWaterCurve[21];
        u_int16_t temperatureMatchCurve[21];
} __attribute__((__packed__));


#define REPORT4_TEMP_PUMP 0
#define REPORT4_TEMP_EXTERN 1
#define REPORT4_TEMP_WATER 2

struct report_pump_data /* report 4 */
{
	u_int16_t rawSensorData[6]; /* littleEndian */
	/*
	 *	rawSensorData[0] = 
	 *	rawSensorData[1] = 
	 *	rawSensorData[2] = 
	 *	rawSensorData[3] = FAN Voltage
	 
	 *	get_pumpPower = ((this->GetPumpCurrent(this->m_rawSensorData[5]) / 1000) * this->GetVDD(this->m_rawSensorData[4]))
	 
	 *	rawSensorData[4] = (VDD)
	 *	rawSensorData[5] = (pumpCurrent) this->GetPumpCurrent(this->m_rawSensorData[5]);
	 */
	
	u_int16_t temperature_raw[3]; /* littleEndian double(value / 100)  */
	        
	u_int16_t frequency; /* littleEndian */        
	u_int16_t frequency_max; /* littleEndian */        

	u_int32_t flow; /* littleEndian */        
	u_int32_t fanRpm; /* littleEndian */        

	u_int8_t fanPower; /* percent (value / 2.55); */
	
	u_int8_t alarm_sensor0:1;
	u_int8_t alarm_sensor1:1;
	u_int8_t alarm_dummy1:1;
	u_int8_t alarm_fan:1;
	u_int8_t alarm_flow:1;
	u_int8_t alarm_dummy2:3;

	u_int8_t mode_advancedPumpSettings:1;
	u_int8_t mode_aquastreamModeAdvanced:1;
	u_int8_t mode_aquastreamModeUltra:1;
	u_int8_t mode_dummy:5;
  
	u_int32_t controllerOut; /* littleEndian   double(value / 167772) */
	int32_t controllerI; /* littleEndian 	double(value / 167772) */
	int32_t controllerP; /* littleEndian 	double(value / 167772)  */  
	int32_t controllerD; /* littleEndian 	double(value / 167772) */

	u_int16_t firmware; /* littleEndian */        
	u_int16_t bootloader; /* littleEndian */        
	u_int16_t hardware; /* littleEndian */        
		
	u_int8_t dummy[2];
		
	u_int16_t serial; /* littleEndian */        
		
	u_int8_t publicKey[6]; /* littleEndian */        
        
} __attribute__((__packed__));


struct report_keys /* report 5 */
{
	u_int8_t advancedPumpKey[6];
	u_int8_t fanAmpKey[6];
	u_int8_t fanControllerKey[6];
} __attribute__((__packed__));



enum tSensorBridge
{
	electronic = 0,
	external = 1,
	water = 2,
	none /* all other values */
};

struct report_settings /* report 6 */
{
	u_int8_t i2cAddress; 
	
	u_int8_t i2cSetting_aquabusEnable:1; 
	u_int8_t i2cSetting_dummy:7; 
        
	u_int8_t pumpMode_dearation:1;
	u_int8_t pumpMode_autoPumpMaxFreq:1;
	u_int8_t pumpMode_dearationModeSens:1;
	u_int8_t pumpMode_resetPumpMaxFreq:1;
	u_int8_t pumpMode_i2cControl:1;
	u_int8_t pumpMode_minFreqForce:1;
	u_int8_t pumpMode_dummy:2;
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
	u_int8_t fanMode_dummy:5; 
		
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
} __attribute__((__packed__));



void aquastreamxt_parse_report3(struct ac_device_list_element *device, struct aquastreamxt_settings *settings);
void aquastreamxt_parse_report4(struct ac_device_list_element *device, struct aquastreamxt_settings *settings, struct aquastreamxt_info *info);
void aquastreamxt_parse_report5(struct ac_device_list_element *device, struct aquastreamxt_settings *settings);
void aquastreamxt_parse_report6(struct ac_device_list_element *device, struct aquastreamxt_settings *settings);

#endif
