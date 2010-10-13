#include <stdio.h>
#include <stdlib.h>


#include <libacinfo.h>

void print_info(struct aquastreamxt_info *info)
{
	int i;
	
	printf("Pump Temp : %2.1f Celsius\n", info->temp_pump);
	printf("Extern Sensor Temp : %2.1f Celsius\n", info->temp_extern);
	printf("Water Temp : %2.1f Celsius\n", info->temp_water);
		
	printf("\nFan RPM: %2.0f\n", info->fan_rpm);	 	
	printf("Fan Voltage: %2.1fV\n", info->fan_voltage);	 
	printf("Fan Voltage(measured): %2.1fV\n", info->fan_voltage_measured);	 

	printf("\nController Out: %3.3f%%\n", info->controller_Output);	
	printf("I Controller: %3.3f%%\n", info->controller_I);	 
	printf("P Controller: %3.3f%%\n", info->controller_P);	 
	printf("D Controller: %3.3f%%\n", info->controller_D);	 

	printf("\nPump Frequency: %2.1f\n", info->pumpFreqency);	 
	printf("Pump Frequency (MAX): %2.1f\n", info->pumpFreqencyMax);	 

	printf("\nVDD: %2.2fV\n", info->vdd);	 
	printf("Flow: %2.0f\n", info->flow);

	printf("\nFirmware: %d\n", info->firmware);
	printf("Bootloader: %d\n", info->bootloader);
	printf("Hardware: %d\n", info->hardware);

	printf("\nSerial: %d\n", info->serial);	

	printf("Public key: ");
	for (i=0; i<6; i++)
	{
		printf("%02X ", info->publicKey[i]);
	}
	printf("\n");

	printf("\nAdvanced Pump Settings: %d\n", info->mode_advancedPumpSettings);
	printf("Advanced Mode: %d\n", info->mode_aquastreamModeAdvanced);
	printf("Ultra Mode: %d\n", info->mode_aquastreamModeUltra);

	printf("\nAlarm sensor 0: %d\n", info->alarm_sensor0);
	printf("Alarm sensor 1: %d\n", info->alarm_sensor1);
	printf("Alarm fan 0: %d\n", info->alarm_fan);
	printf("Alarm flow 0: %d\n", info->alarm_flow);
}


void print_settings(struct aquastreamxt_settings *settings)
{
	int i;
	
	printf("\nAdvanced Pump Settings Key: ");
	for (i=0; i<6; i++)
	{
		printf("%02X ", settings->advancedPumpKey[i]);
	}
	printf("\n");
	
	printf("Fan Amp Key (Adv. Version): ");
	for (i=0; i<6; i++)
	{ 
		printf("%02X ", settings->fanAmpKey[i]);
	}
	printf("\n");

	printf("Fan Controller Key (Ultra Version): ");
	for (i=0; i<6; i++)
	{
		printf("%02X ", settings->fanControllerKey[i]);
	}
	printf("\n");

	printf("Fan Curve:\n");
	for (i=0; i<21; i++)
	{
		printf("%04X ", settings->sensorFanCurve[i]);
	}
	printf("\n\n");
	
	printf("Extern Sensor Curve:\n");
	for (i=0; i<21; i++)
	{ 
		printf("%04X ", settings->sensorExternCurve[i]);
	}
	printf("\n\n");

	printf("Water Sensor Curve:\n");
	for (i=0; i<21; i++)
	{
		printf("%04X ", settings->sensorWaterCurve[i]);
	}
	printf("\n\n");

	printf("Temperature Match Curve:\n");
	for (i=0; i<21; i++)
	{
		printf("%04X ", settings->temperatureMatchCurve[i]);
	}
	printf("\n\n");


	printf("I2C Address: %d\n", settings->i2cAddress);
	printf("I2C Aquabus Enable: %d\n", settings->i2cSetting_aquabusEnable);

	printf("\nPump Deaeration: %d\n", settings->pumpMode_dearation);
	printf("Pump Auto Max Frequency: %d\n", settings->pumpMode_autoPumpMaxFreq);
	printf("Pump Deaeration Mode Sens: %d\n", settings->pumpMode_dearationModeSens);
	printf("Pump Reset Max Freq: %d\n", settings->pumpMode_resetPumpMaxFreq);
	printf("Pump I2C Control: %d\n", settings->pumpMode_i2cControl);
	printf("Pump Force Min Frequency: %d\n", settings->pumpMode_minFreqForce);
	printf("Pump ModeB: 0x%02X\n", settings->pumpModeB);


	printf("\nMeasure Fan Edges: %d\n", settings->measureFanEdges);
	printf("Measure Flow Edges: %d\n", settings->measureFlowEdges);


	printf("\nPump Frequency: %3.1f\n", settings->pumpFrequency);
	printf("Frequency Reset Cycle: %d\n", settings->frequencyResetCycle);

	printf("\nAlarm Sensor 0: %d\n", settings->alarm_sensor0);
	printf("Alarm Sensor 1: %d\n", settings->alarm_sensor1);
	printf("Alarm Pump: %d\n", settings->alarm_pump);
	printf("Alarm Fan: %d\n", settings->alarm_fan);
	printf("Alarm Flow: %d\n", settings->alarm_flow);
	printf("Alarm Fan Short: %d\n", settings->alarm_fanShort);
	printf("Alarm Over 90 Celsius: %d\n", settings->alarm_fanOverTemp90);
	printf("Alarm Over 70 Celsius: %d\n", settings->alarm_fanOverTemp70);

	printf("\nTacho-Link Fan: %d\n", settings->tachoMode_linkFan);
	printf("Tacho-Link Flow: %d\n", settings->tachoMode_linkFlow);
	printf("Tacho-Link Pump: %d\n", settings->tachoMode_linkPump);
	printf("Tacho-Link Static: %d\n", settings->tachoMode_linkStatic);
	printf("Tacho-Link Alarm Interrupt: %d\n", settings->tachoMode_linkAlarmInterrupt);

	printf("\nTacho Frequency: %4.0f\n", settings->tachoFrequency);

	printf("\nFan Mode Manual: %d\n", settings->fanMode_manual);
	printf("Fan Mode Auto: %d\n", settings->fanMode_auto);
	printf("Fan Mode Hold Min Power: %d%%\n", settings->fanMode_holdMinPower);

	printf("Fan Manual Power Value: %3.1f%%\n", settings->fanManualPower);

	printf("\nController Hysterese: %4.1f\n", settings->controllerHysterese);
	printf("Controller Sensor: %d\n", settings->controllerSensor);
	printf("Controller Set Temp: %3.1f Celsius\n", settings->controllerSetTemp);
	printf("Controller P: %d\n", settings->controllerP);
	printf("Controller I: %d\n", settings->controllerI);
	printf("Controller D: %d\n", settings->controllerD);

	printf("\nSensor Min Temp: %3.1f Celsius\n", settings->sensorMinTemperature);
	printf("Sensor Max Temp: %3.1f Celsius\n", settings->sensorMaxTemperature);

	printf("\nFan Min Power: %3.1f%%\n", settings->fanMinimumPower);
	printf("Fan Max Power: %3.1f%%\n", settings->fanMaximumPower);

	printf("\nPump Min Frequency: %3.1f\n", settings->minPumpFreqency);
	printf("Pump Max Freqeuncy: %3.1f\n", settings->maxPumpFreqency);
}


int main(int argc, char **argv)
{
	struct libacinfo_handle libhandle;
	struct ac_device_list_element *device;	
	struct aquastreamxt_info info;
	struct aquastreamxt_settings settings;
		

////	TODO reverse TCP protocol	                 
////    	start_listening(); 


	libhandle = acinfo_init();

	if (libhandle.device_count <= 0)
	{
		fprintf(stderr, "Couldn't find Aqua Computer USB device.\n");
		exit(1);
	}

	device = libhandle.device_list_head;
		
	while (device != NULL)
	{		
		if (acinfo_read_settings_aquastreamXT(device, &settings) == NULL)
		{
			fprintf(stderr, "Error reading settings\n");
			device = device->next;
			continue;
		}
		
		if (acinfo_read_info_aquastreamXT(device, &settings, &info) == NULL)
		{
			fprintf(stderr, "Error reading info\n");
			device = device->next;
			continue;
		}
		
		
		print_settings(&settings);		
		print_info(&info);
		
		
		device = device->next;
	}
	
	
	acinfo_deinit(&libhandle);
	                              
	return 0;
}



