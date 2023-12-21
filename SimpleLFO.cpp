/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2007 Adobe Systems Incorporated                       */
/* All Rights Reserved.                                            */
/*                                                                 */
/* NOTICE:  All information contained herein is, and remains the   */
/* property of Adobe Systems Incorporated and its suppliers, if    */
/* any.  The intellectual and technical concepts contained         */
/* herein are proprietary to Adobe Systems Incorporated and its    */
/* suppliers and may be covered by U.S. and Foreign Patents,       */
/* patents in process, and are protected by trade secret or        */
/* copyright law.  Dissemination of this information or            */
/* reproduction of this material is strictly forbidden unless      */
/* prior written permission is obtained from Adobe Systems         */
/* Incorporated.                                                   */
/*                                                                 */
/*******************************************************************/

/*	SimpleLFO.cpp	

	This is a compiling husk of a project. Fill it in with interesting
	pixel processing code.
	
	Revision history: 

	1.0 (seemed like a good idea at the time)	bbb		6/1/2002

	1.0 Okay, I'm leaving the version at 1.0,	bbb		2/15/2006
		for obvious reasons; you're going to 
		copy these files directly! This is the
		first XCode version, though.

	1.0	Let's simplify this barebones sample	zal		11/11/2010

*/

#include "SimpleLFO.h"

static PF_Err 
About (	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	AEGP_SuiteHandler suites(in_data->pica_basicP);
	
	suites.ANSICallbacksSuite1()->sprintf(	out_data->return_msg,
											"%s v%d.%d\r%s",
											STR(StrID_Name), 
											MAJOR_VERSION, 
											MINOR_VERSION, 
											STR(StrID_Description));
	return PF_Err_NONE;
}

static PF_Err 
GlobalSetup (	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	out_data->my_version = PF_VERSION(	MAJOR_VERSION, 
										MINOR_VERSION,
										BUG_VERSION, 
										STAGE_VERSION, 
										BUILD_VERSION);

	out_data->out_flags =  PF_OutFlag_DEEP_COLOR_AWARE;	// just 16bpc, not 32bpc
	
	return PF_Err_NONE;
}

static PF_Err 
ParamsSetup (	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	PF_Err		err		= PF_Err_NONE;
	PF_ParamDef	def;	

	AEFX_CLR_STRUCT(def);

	PF_ADD_CHECKBOXX(STR(StrID_lfoOn_Param_name),
		lfoOnDefault,
				0,
		lfoOn_DISK_ID);

	AEFX_CLR_STRUCT(def);

	PF_ADD_FLOAT_SLIDERX(STR(StrID_LFO_Param_Name),
		lfoRateMin,
		lfoRateMax,
		lfoRateMin,
		lfoRateMax,
		lfoRateDefault,
		PF_Precision_HUNDREDTHS,
		0,
		PF_ParamFlag_SUPERVISE,
		lfo_DISK_ID);


	AEFX_CLR_STRUCT(def);

	PF_ADD_FLOAT_SLIDERX(STR(StrID_Amp_Param_Name),
		lfoAmpMin,
		lfoAmpMax,
		lfoAmpMin,
		lfoAmpMax,
		lfoAmpDefault,
		PF_Precision_HUNDREDTHS,
		0,
		0,
		amp_DISK_ID);

	AEFX_CLR_STRUCT(def);


	PF_ADD_FLOAT_SLIDERX(STR(StrID_Output_Param_Name),
		outputMin,
		outputMax,
		outputMin,
		outputMax,
		outputDefault,
		PF_Precision_HUNDREDTHS,
		0,
		0,
		output_DISK_ID);

		
	
	out_data->num_params = SimpleLFO_NUM_PARAMS;

	return err;
}

void SimpleLFO(int x) 
{
//	int dx;


	//params[SimpleLFO_OUTPUT]->u.td.x_value = dx;


}
PF_Pixel *sampleIntegral32(PF_EffectWorld &def, int x, int y){
	return (PF_Pixel*)((char*)def.data +
	(y * def.rowbytes) +
	(x * sizeof(PF_Pixel)));
}
void drawline(int x0, int y0, int x1, int y1, PF_EffectWorld *output)
{
	int dx, dy, p, x, y;

	dx = x1 - x0;
	dy = y1 - y0;

	x = x0;
	y = y0;

	p = 2 * dy - dx;

	while (x < x1)
	{
		if (p >= 0)
		{
			PF_Pixel *myPixel = sampleIntegral32(*output, x, y);
			myPixel->red = myPixel->alpha = 255;
			y = y + 1;
			p = p + 2 * dy - 2 * dx;
		}
		else
		{
			PF_Pixel *myPixel = sampleIntegral32(*output, x, y);
			myPixel->red = myPixel->alpha = 255;
			p = p + 2 * dy;
		}
		x = x + 1;
	}
}

static PF_Err 
Render(
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output)
{
	PF_Err				err = PF_Err_NONE;
	AEGP_SuiteHandler	suites(in_data->pica_basicP);



	/*

	A_long	x = params[SimpleLFO_LFO]->u.fs_d.value,
			y = FIX_2_FLOAT(params[SimpleLFO_OUTPUT]->u.td.y_value),
			x1 = output->width, 
			y1 =output->height,
			z = params[SimpleLFO_LFO]->u.fs_d.value;

	drawline(x,	y, x1, y1, output);

	//SimpleLFO(z);

	params[SimpleLFO_OUTPUT]->u.td.x_value = params[SimpleLFO_LFO]->u.fs_d.value;


	params[SimpleLFO_OUTPUT]->u.td.x_value = params[SimpleLFO_LFO]->u.fs_d.value;
	params[SimpleLFO_OUTPUT]->u.td.x_value = params[SimpleLFO_AMP]->u.fs_d.value;

	
	A_long x = FIX2INT(params[SimpleLFO_LFO]->u.fs_d.value);
	params[SimpleLFO_OUTPUT]->u.fs_d.value = x;	/*/


	if (params[SimpleLFO_LFOON]->u.fs_d.value == true)
	{

		


	}
	else {
		return err;
	}
	return err;

}


DllExport	
PF_Err 
EntryPointFunc (
	PF_Cmd			cmd,
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	void			*extra)
{
	PF_Err		err = PF_Err_NONE;

	try {
		switch (cmd) {
			case PF_Cmd_ABOUT:

				err = About(in_data,
							out_data,
							params,
							output);
				break;
				
			case PF_Cmd_GLOBAL_SETUP:

				err = GlobalSetup(	in_data,
									out_data,
									params,
									output);
				break;
				
			case PF_Cmd_PARAMS_SETUP:

				err = ParamsSetup(	in_data,
									out_data,
									params,
									output);
				break;
				
			case PF_Cmd_RENDER:

				err = Render(	in_data,
								out_data,
								params,
								output);
				break;
		}
	}
	catch(PF_Err &thrown_err){
		err = thrown_err;
	}
	return err;
}

