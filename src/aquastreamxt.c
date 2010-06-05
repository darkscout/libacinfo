
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

void aquastreamxt_parse_report3(struct ac_device_list_element *device)
{	
	unsigned char *buffer = (unsigned char*)malloc(REPORT_INPUT_LEN);
	struct report_senosr_settings_data *r = (struct report_senosr_settings_data*) buffer;
	int bytes, i;
        	
	bytes = hiddev_get_feature_report(device->handle, 
		AQUASTREAMXT_SENSOR_SETTINGS_DATA_REPORT, 
		buffer, REPORT_INPUT_LEN);
        
        if (bytes <= 0)
        {
        	return;
        }

       	
	printf("Fan Curve:\n");
	for (i=0; i<21; i++)
	{
		printf("%04X ", r->sensorFanCurve[i]);
	}
	printf("\n\n");
	
	printf("Extern Sensor Curve:\n");
	for (i=0; i<21; i++)
	{ 
		printf("%04X ", r->sensorExternCurve[i]);
	}
	printf("\n\n");

	printf("Water Sensor Curve:\n");
	for (i=0; i<21; i++)
	{
		printf("%04X ", r->sensorWaterCurve[i]);
	}
	printf("\n\n");

	printf("Temperature Match Curve:\n");
	for (i=0; i<21; i++)
	{
		printf("%04X ", r->temperatureMatchCurve[i]);
	}
	printf("\n\n");

	free(buffer);
}

void aquastreamxt_parse_report4(struct ac_device_list_element *device)
{	
	unsigned char *buffer = (unsigned char*)malloc(REPORT_INPUT_LEN);
	struct report_pump_data *r = (struct report_pump_data*) buffer;
	int bytes, i;
	
	bytes = hiddev_get_feature_report(device->handle, 
		AQUASTREAMXT_PUMP_DATA_TRANSFER_REPORT, 
		buffer, REPORT_INPUT_LEN);
        
        if (bytes <= 0)
        {
        	return;
        }

       	
	printf("Pump Temp : %2.1f\n",
		convert2temp(r->temperature_raw[REPORT4_TEMP_PUMP]));
	printf("Extern Sensor Temp : %2.1f\n", 
		convert2temp(r->temperature_raw[REPORT4_TEMP_EXTERN]));
	printf("Water Temp : %2.1f\n", 
		convert2temp(r->temperature_raw[REPORT4_TEMP_WATER]));
	
	
	printf("\nFan RPM: %2.0f\n", convert2fanrpm(r->fanRpm));	 
	printf("Flow: %2.0f\n", convert2flow(r->flow));	 
	
	double fan_voltage = convert2VDD(r->rawSensorData[4]) * (convert2scalepercent(r->fanPower) / 100);
	printf("Fan Voltage: %2.1f\n", fan_voltage);	 
	printf("Fan Voltage(measured): %2.1f\n", convert2FanVoltage(r->rawSensorData[3]));	 
		
	printf("\nPump Frequency: %2.1f\n", convert2pump_frequency(r->frequency));	 
	printf("Pump Frequency (MAX): %2.1f\n", convert2fanrpm(r->frequency_max));	 

	printf("\nVDD: %2.2f\n", convert2VDD(r->rawSensorData[4]));	 


	printf("\nController Out: %3.3f\n", 
		aquastreamxt2controllerOutScale(r->controllerOut));	 
	printf("I Controller: %3.3f\n", 
		aquastreamxt2controllerOutScale(r->controllerI));	 
	printf("P Controller: %3.3f\n", 
		aquastreamxt2controllerOutScale(r->controllerP));	 
	printf("D Controller: %3.3f\n", 	
		aquastreamxt2controllerOutScale(r->controllerD));	 


	printf("\nAlarm sensor 0: %d\n", r->alarm_sensor0);
	printf("Alarm sensor 1: %d\n", r->alarm_sensor1);
	/*printf("Alarm 1-bit dummy: %d\n", r->alarm_dummy1);*/
	printf("Alarm fan 0: %d\n", r->alarm_fan);
	printf("Alarm flow 0: %d\n", r->alarm_flow);
	/*printf("Alarm 3-bit dummy 0: 0x%02X\n", r->alarm_dummy2);*/
	
	printf("\nAdvanced Pump Settings: %d\n", r->mode_advancedPumpSettings);
	printf("Advanced Mode: %d\n", r->mode_aquastreamModeAdvanced);
	printf("Ultra Mode: %d\n", r->mode_aquastreamModeUltra);
	/*printf("Mode 5-bit Dummy: 0x%02X\n", r->mode_dummy);*/

	printf("\nFirmware: %d\n", r->firmware);
	printf("Bootloader: %d\n", r->bootloader);
	printf("Hardware: %d\n", r->hardware);

	printf("\nSerial: %d\n", r->serial);
	

	printf("Public key: ");
	for (i=0; i<6; i++)
	{
		printf("%02X ", r->publicKey[i]);
	}
	printf("\n");
	
	free(buffer);
}        	

void aquastreamxt_parse_report5(struct ac_device_list_element *device)
{	
	unsigned char *buffer = (unsigned char*)malloc(REPORT_INPUT_LEN);
	struct report_keys *r = (struct report_keys*) buffer;
	int bytes, i;
	
	bytes = hiddev_get_feature_report(device->handle, 
		AQUASTREAMXT_KEYS_REPORT, 
		buffer, REPORT_INPUT_LEN);
        
        if (bytes <= 0)
        {
        	return;
        }

       	
	printf("Advanced Pump Settings Key: ");
	for (i=0; i<6; i++)
	{
		printf("%02X ", r->advancedPumpKey[i]);
	}
	printf("\n");
	
	printf("Fan Amp Key (Adv. Version): ");
	for (i=0; i<6; i++)
	{ 
		printf("%02X ", r->fanAmpKey[i]);
	}
	printf("\n");

	printf("Fan Controller Key (Ultra Version): ");
	for (i=0; i<6; i++)
	{
		printf("%02X ", r->fanControllerKey[i]);
	}
	printf("\n");

	free(buffer);
}

void aquastreamxt_parse_report6(struct ac_device_list_element *device)
{	
	unsigned char *buffer = (unsigned char*)malloc(REPORT_INPUT_LEN);
	struct report_settings *r = (struct report_settings*) buffer;
	int bytes;
	
	bytes = hiddev_get_feature_report(device->handle, 
		AQUASTREAMXT_SETTINGS_REPORT, 
		buffer, REPORT_INPUT_LEN);
        
        if (bytes <= 0)
        {
        	return;
        }

       	
	printf("I2C Address: %d\n", r->i2cAddress);
	printf("I2C Aquabus Enable: %d\n", r->i2cSetting_aquabusEnable);
	/*printf("I2C Dummy: 0x%02X\n", r->i2cSetting_dummy);*/

	printf("\nPump Deaeration: %d\n", r->pumpMode_dearation);
	printf("Pump Auto Max Frequency: %d\n", r->pumpMode_autoPumpMaxFreq);
	printf("Pump Deaeration Mode Sens: %d\n", r->pumpMode_dearationModeSens);
	printf("Pump Reset Max Freq: %d\n", r->pumpMode_resetPumpMaxFreq);
	printf("Pump I2C Control: %d\n", r->pumpMode_i2cControl);
	printf("Pump Force Min Frequency: %d\n", r->pumpMode_minFreqForce);
	/*printf("Pump Dummy: 0x%02X\n", r->pumpMode_dummy);*/
	printf("Pump ModeB: 0x%02X\n", r->pumpModeB);

	/* u_int8_t sensorBridge; // enum tSensorBridge */

	printf("\nMeasure Fan Edges: %d\n", r->measureFanEdges);
	printf("Measure Flow Edges: %d\n", r->measureFlowEdges);


	printf("\nPump Frequency: %3.1f\n", convert2pump_frequency(r->pumpFrequency));
	printf("Frequency Reset Cycle: %d\n", r->frequencyResetCycle / 0x113);

	printf("\nAlarm Sensor 0: %d\n", r->alarm_sensor0);
	printf("Alarm Sensor 1: %d\n", r->alarm_sensor1);
	printf("Alarm Pump: %d\n", r->alarm_pump);
	printf("Alarm Fan: %d\n", r->alarm_fan);
	printf("Alarm Flow: %d\n", r->alarm_flow);
	printf("Alarm Fan Short: %d\n", r->alarm_fanShort);
	printf("Alarm Over 90°C: %d\n", r->alarm_fanOverTemp90);
	printf("Alarm Over 70°C: %d\n", r->alarm_fanOverTemp70);

	printf("\nTacho-Link Fan: %d\n", r->tachoMode_linkFan);
	printf("Tacho-Link Flow: %d\n", r->tachoMode_linkFlow);
	printf("Tacho-Link Pump: %d\n", r->tachoMode_linkPump);
	printf("Tacho-Link Static: %d\n", r->tachoMode_linkStatic);
	printf("Tacho-Link Alarm Interrupt: %d\n", r->tachoMode_linkAlarmInterrupt);
	/*printf("Tacho-Link Dummy: 0x%02X\n", r->tachoMode_dummy);*/

	printf("\nTacho Frequency: %4.0f\n", aquastreamxt2staticTachoRpm(r->tachoFrequency));

/*
	u_int32_t flowAlarmValue; 
	u_int16_t sensorAlarmTemperature[2]; 
*/	

	printf("\nFan Mode Manual: %d\n", r->fanMode_manual);
	printf("Fan Mode Auto: %d\n", r->fanMode_auto);
	printf("Fan Mode Hold Min Power: %d\n", r->fanMode_holdMinPower);
	/*printf("Fan Mode Dummy: 0x%02X\n", r->fanMode_dummy);*/

	printf("Fan Manual Power Value: %3.1f\n", convert2scalepercent(r->fanManualPower));

	printf("\nController Hysterese: %4.1f\n", convert2temp(r->controllerHysterese));
	printf("Controller Sensor: %d\n", r->controllerSensor);
	printf("Controller Set Temp: %3.1f\n", convert2temp(r->controllerSetTemp));
	printf("Controller P: %d\n", r->controllerP);
	printf("Controller I: %d\n", r->controllerI);
	printf("Controller D: %d\n", r->controllerD);

	printf("\nSensor Min Temp: %3.1f\n", convert2temp(r->sensorMinTemperature));
	printf("Sensor Max Temp: %3.1f\n", convert2temp(r->sensorMaxTemperature));

	printf("\nFan Min Power: %3.1f\n", convert2scalepercent(r->fanMinimumPower));
	printf("Fan Max Power: %3.1f\n", convert2scalepercent(r->fanMaximumPower));

/*			
	u_int8_t ledSettings; 
	u_int8_t aquabusTimeout; 

	*/

	printf("\nPump Min Frequency: %3.1f\n", convert2pump_frequency(r->minPumpFreqency));
	printf("Pump Max Freqeuncy: %3.1f\n", convert2pump_frequency(r->maxPumpFreqency));

	free(buffer);
}

