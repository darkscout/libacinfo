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

#include "aquastreamxt.h"

double aquastreamxt2staticTachoRpm(u_int32_t value)
{
	double ret = (46875 * 60) / ((double)value * 9.9);
	return ret;
}

double aquastreamxt2controllerOutScale(int32_t value)
{
	double ret = (double)value / (double)167772;
	return ret;
}

void aquastreamxt_parse_report3(struct ac_device_list_element *device, struct aquastreamxt_settings *settings)
{	
	unsigned char *buffer = (unsigned char*)malloc(REPORT_INPUT_LEN);
	struct report_senosr_settings_data *r = (struct report_senosr_settings_data*) buffer;
	int bytes;
        	
	bytes = hiddev_get_feature_report(device->handle, 
		AQUASTREAMXT_SENSOR_SETTINGS_DATA_REPORT, 
		buffer, REPORT_INPUT_LEN);
        
	if (bytes <= 0)
	{
		free(buffer);
		return;
	}

	/* copy the received values to presentation structure */       	
	memcpy(settings->sensorFanCurve, r->sensorFanCurve, 21*2 /* 21 u_int16_t */);
	memcpy(settings->sensorExternCurve, r->sensorExternCurve, 21*2 /* 21 u_int16_t */);
	memcpy(settings->sensorWaterCurve, r->sensorWaterCurve, 21*2 /* 21 u_int16_t */);
	memcpy(settings->temperatureMatchCurve, r->temperatureMatchCurve, 21*2 /* 21 u_int16_t */);
       	
	free(buffer);
}

void aquastreamxt_parse_report4(struct ac_device_list_element *device, struct aquastreamxt_settings *settings, struct aquastreamxt_info *info)
{	
	unsigned char *buffer = (unsigned char*)malloc(REPORT_INPUT_LEN);
	struct report_pump_data *r = (struct report_pump_data*) buffer;
	int bytes;
	
	bytes = hiddev_get_feature_report(device->handle, 
		AQUASTREAMXT_PUMP_DATA_TRANSFER_REPORT, 
		buffer, REPORT_INPUT_LEN);
        
	if (bytes <= 0)
	{
		free(buffer);
		return;
	}

	info->temp_pump = convert2temp(r->temperature_raw[REPORT4_TEMP_PUMP]);
	info->temp_extern = convert2temp(r->temperature_raw[REPORT4_TEMP_EXTERN]);
	info->temp_water = convert2temp(r->temperature_raw[REPORT4_TEMP_WATER]);
	
	info->fan_rpm = convert2fanrpm(r->fanRpm, settings->measureFanEdges);
	info->fan_voltage = convert2VDD(r->rawSensorData[4]) * (convert2scalepercent(r->fanPower) / 100);
	info->fan_voltage_measured = convert2FanVoltage(r->rawSensorData[3]);
       	
	info->controller_I = aquastreamxt2controllerOutScale(r->controllerI);
	info->controller_P = aquastreamxt2controllerOutScale(r->controllerP);
	info->controller_D = aquastreamxt2controllerOutScale(r->controllerD);
	info->controller_Output = aquastreamxt2controllerOutScale(r->controllerOut);


	info->flow = convert2flow(r->flow, settings->measureFlowEdges/*report 6*/, AQUASTREAMXT_IMP_FLOW);
	info->pumpFreqency = convert2pump_frequency(r->frequency);
	info->pumpFreqencyMax = convert2fanrpm(r->frequency_max, settings->measureFanEdges);
	
	info->vdd = convert2VDD(r->rawSensorData[4]);


	info->alarm_sensor0 = r->alarm_sensor0;
	info->alarm_sensor1 = r->alarm_sensor1;
	info->alarm_fan = r->alarm_fan;
	info->alarm_flow = r->alarm_flow;


	/*printf("Alarm 1-bit dummy: %d\n", r->alarm_dummy1);*/
	/*printf("Alarm 3-bit dummy 0: 0x%02X\n", r->alarm_dummy2);*/
	/*printf("Mode 5-bit Dummy: 0x%02X\n", r->mode_dummy);*/
	
	info->mode_advancedPumpSettings = r->mode_advancedPumpSettings;
	info->mode_aquastreamModeAdvanced = r->mode_aquastreamModeAdvanced;
	info->mode_aquastreamModeUltra = r->mode_aquastreamModeUltra;


	info->firmware = r->firmware;
	info->bootloader = r->bootloader;
	info->hardware = r->hardware;

	info->serial = r->serial;
	
	memcpy(info->publicKey, r->publicKey, 6 /*bytes*/);
	
	free(buffer);
}        	

void aquastreamxt_parse_report5(struct ac_device_list_element *device, struct aquastreamxt_settings *settings)
{	
	unsigned char *buffer = (unsigned char*)malloc(REPORT_INPUT_LEN);
	struct report_keys *r = (struct report_keys*) buffer;
	int bytes;
	
	bytes = hiddev_get_feature_report(device->handle, 
		AQUASTREAMXT_KEYS_REPORT, 
		buffer, REPORT_INPUT_LEN);
        
	if (bytes <= 0)
	{
		free(buffer);
		return;
	}

	/* copy the received values to presentation structure */       	
	memcpy(settings->advancedPumpKey, r->advancedPumpKey, 6 /*bytes*/);
	memcpy(settings->fanAmpKey, r->fanAmpKey, 6 /*bytes*/);
	memcpy(settings->fanControllerKey, r->fanControllerKey, 6 /*bytes*/);
       	
	free(buffer);
}

void aquastreamxt_parse_report6(struct ac_device_list_element *device, struct aquastreamxt_settings *settings)
{	
	unsigned char *buffer = (unsigned char*)malloc(REPORT_INPUT_LEN);
	struct report_settings *r = (struct report_settings*) buffer;
	int bytes;
	
	bytes = hiddev_get_feature_report(device->handle, 
		AQUASTREAMXT_SETTINGS_REPORT, 
		buffer, REPORT_INPUT_LEN);
        
	if (bytes <= 0)
	{
		free(buffer);
		return;
	}

	settings->i2cAddress = r->i2cAddress;
	settings->i2cSetting_aquabusEnable = r->i2cSetting_aquabusEnable;
        
	settings->pumpMode_dearation = r->pumpMode_dearation;
	settings->pumpMode_autoPumpMaxFreq = r->pumpMode_autoPumpMaxFreq;
	settings->pumpMode_dearationModeSens = r->pumpMode_dearationModeSens;
	settings->pumpMode_resetPumpMaxFreq = r->pumpMode_resetPumpMaxFreq;
	settings->pumpMode_i2cControl = r->pumpMode_i2cControl;
	settings->pumpMode_minFreqForce = r->pumpMode_minFreqForce;
	settings->pumpModeB = r->pumpModeB;

	settings->sensorBridge = r->sensorBridge;
	
	settings->measureFanEdges = r->measureFanEdges;
	settings->measureFlowEdges = r->measureFlowEdges;
	
	settings->pumpFrequency = convert2pump_frequency(r->pumpFrequency);
	settings->frequencyResetCycle = r->frequencyResetCycle / 0x113;
	
	settings->alarm_sensor0 = r->alarm_sensor0;
	settings->alarm_sensor1 = r->alarm_sensor1;
	settings->alarm_pump = r->alarm_pump;
	settings->alarm_fan = r->alarm_fan;
	settings->alarm_flow = r->alarm_flow;
	settings->alarm_fanShort = r->alarm_fanShort;
	settings->alarm_fanOverTemp90 = r->alarm_fanOverTemp90;
	settings->alarm_fanOverTemp70 = r->alarm_fanOverTemp70;
	
	settings->tachoMode_linkFan = r->tachoMode_linkFan;
	settings->tachoMode_linkFlow = r->tachoMode_linkFlow;
	settings->tachoMode_linkPump = r->tachoMode_linkPump;
	settings->tachoMode_linkStatic = r->tachoMode_linkStatic;
	settings->tachoMode_linkAlarmInterrupt = r->tachoMode_linkAlarmInterrupt;
	
	settings->tachoFrequency = aquastreamxt2staticTachoRpm(r->tachoFrequency);
	
/*
	u_int32_t flowAlarmValue; 
	u_int16_t sensorAlarmTemperature[2]; 
*/	
	//settings->flowAlarmValue = ;
	//settings->sensorAlarmTemperature[2]; 
	
	settings->fanMode_manual = r->fanMode_manual;
	settings->fanMode_auto = r->fanMode_auto;
	settings->fanMode_holdMinPower = r->fanMode_holdMinPower;
		
	settings->fanManualPower = convert2scalepercent(r->fanManualPower);
	
	settings->controllerHysterese = convert2temp(r->controllerHysterese);
	settings->controllerSensor = r->controllerSensor;
	settings->controllerSetTemp = convert2temp(r->controllerSetTemp);
	settings->controllerP = r->controllerP;
	settings->controllerI = r->controllerI;
	settings->controllerD = r->controllerD;
	
	settings->sensorMinTemperature = convert2temp(r->sensorMinTemperature);
	settings->sensorMaxTemperature = convert2temp(r->sensorMaxTemperature);
	
	settings->fanMinimumPower = convert2scalepercent(r->fanMinimumPower);
	settings->fanMaximumPower = convert2scalepercent(r->fanMaximumPower);
	
	settings->ledSettings = r->ledSettings;
	settings->aquabusTimeout = r->aquabusTimeout;
	
	settings->minPumpFreqency = convert2pump_frequency(r->minPumpFreqency);
	settings->maxPumpFreqency = convert2scalepercent(r->fanMaximumPower);
       

	/*printf("I2C Dummy: 0x%02X\n", r->i2cSetting_dummy);*/
	/*printf("Pump Dummy: 0x%02X\n", r->pumpMode_dummy);*/

	/* u_int8_t sensorBridge; // enum tSensorBridge */

	/*printf("Tacho-Link Dummy: 0x%02X\n", r->tachoMode_dummy);*/

	/*printf("Fan Mode Dummy: 0x%02X\n", r->fanMode_dummy);*/

	/*			
	u_int8_t ledSettings; 
	u_int8_t aquabusTimeout; 
	*/

	free(buffer);
}

