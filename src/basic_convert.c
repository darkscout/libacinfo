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

#include "basic_convert.h"

double convert2temp(u_int16_t value)
{
	double ret = value;
	
	ret = ret / 100.0;
	
	return ret;
}

double convert2flow(u_int32_t value)
{
	if (value >= 0x927c0)
	{
		return 0;
	}

	double ret = value;
	
	ret = (double)(AQUASTREAMXT_EDGES_FLOW / 2) / (double)AQUASTREAMXT_IMP_FLOW;
	
	ret = (double)(46875 * 3600) / ((double)value / ret);
	
	return ret;
}

double convert2fanrpm(u_int32_t value)
{
	if (value >= 0x493e0)
	{
		return 0;
	}

	double ret = value;
	
	ret /= (AQUASTREAMXT_EDGES_FAN / 4);
	
	ret = (46875 * 60) / ret;

	if (ret >= 10000)
	{
		/* large values are reported for stopped fan */
		return 0;
	}
	else
	{
		return ret;
	}
}

double convert2pump_frequency(u_int32_t value)
{
	double ret = 1500000;
	
	ret /= (double)value;
	ret /= (double)2;
	
	return ret;
}

double convert2frequency_limiter(u_int32_t value)
{
	return (value * 0.01333155);
}

double convert2VDD(u_int32_t value)
{
	return (double)value / (double)61;
}

double convert2FanVoltage(u_int32_t value)
{
	return (double)value / (double)63;
}

double convert2scalepercent(u_int16_t value)
{
	double ret = (double)value;
	
	ret /= 2.55;
	
	return ret;
}

double convert2controllerOutScale(int32_t value)
{
	double ret = (double)value / (double)167772;
	return ret;
}

