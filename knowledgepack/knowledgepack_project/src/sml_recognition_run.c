//SensiML Includes
#include "kb.h"
#include "sml_output.h"
#include "sml_recognition_run.h"
#include "sensor_ssss.h"
//FILL_USE_TEST_DATA

#ifdef SML_USE_TEST_DATA
#include "testdata.h"
int td_index = 0;
#endif //SML_USE_TEST_DATA

#ifndef SENSOR_AUDIO_ID
#define SENSOR_AUDIO_ID              (0x4155444F)
#endif //ifndef SENSOR_AUDIO_ID

int sml_recognition_run_batch(signed short *data_batch, int batch_sz, uint8_t num_sensors, uint32_t sensor_id)
{
	int ret;

	int batch_index = 0;
	signed short* data;
	for(batch_index=0; batch_index < batch_sz; batch_index++)
	{
	#ifdef SML_USE_TEST_DATA
	ret = kb_run_model((SENSOR_DATA_T*)&testdata[td_index++], TD_NUMCOLS, 0);
	if(td_index >= TD_NUMROWS)
	{
		td_index = 0;
	}
	if(ret >= 0)
	{
		sml_output_results(0, ret);
		kb_reset_model(0);
	}
	#else
	data = &data_batch[batch_index*num_sensors];
	switch(sensor_id)
	{
		case SENSOR_SSSS_ID:
			//FILL_RUN_MODEL_MOTION
			//FILL_RUN_MODEL_CUSTOM
			break;

		case SENSOR_AUDIO_ID:
					ret = kb_run_model((SENSOR_DATA_T *)data, num_sensors, KB_MODEL_Smart_Beehive_rank_0_INDEX);
		if (ret >= 0){
			sml_output_results(KB_MODEL_Smart_Beehive_rank_0_INDEX, ret);
			kb_reset_model(0);
		};
			break;

		default:
			break;
	}
	#endif //SML_USE_TEST_DATA
	}
	return ret;
}

int sml_recognition_run_single(signed short *data, uint32_t sensor_id)
{
	int ret;
	uint8_t num_sensors = 0;
	#ifdef SML_USE_TEST_DATA
	ret = kb_run_model((SENSOR_DATA_T*)&testdata[td_index++], TD_NUMCOLS, 0);
	if(td_index >= TD_NUMROWS)
	{
		td_index = 0;
	}
	if(ret >= 0)
	{
		sml_output_results(0, ret);
		kb_reset_model(0);
	}
	#else
	switch(sensor_id)
	{
		case SENSOR_SSSS_ID:
			//FILL_RUN_MODEL_MOTION
			//FILL_RUN_MODEL_CUSTOM
			break;

		case SENSOR_AUDIO_ID:
					ret = kb_run_model((SENSOR_DATA_T *)data, num_sensors, KB_MODEL_Smart_Beehive_rank_0_INDEX);
		if (ret >= 0){
			sml_output_results(KB_MODEL_Smart_Beehive_rank_0_INDEX, ret);
			kb_reset_model(0);
		};
			break;

		default:
			break;
	}
	#endif //SML_USE_TEST_DATA
	return ret;
}
