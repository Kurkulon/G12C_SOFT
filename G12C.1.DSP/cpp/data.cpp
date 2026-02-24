#include "common.h"
#include "data.h"
#include "options.h"

/*
Дата и Опции разделены, т.к. настройки могут меняться в процессе регистрации и данные будут с предыдущими настройками
*/

namespace Software
{
	Data::data_type Data::_data;

	void Data::Init()
	{
	}

	void Data::Idle()
	{
		static unsigned char state = 0;
		switch (state)
		{
		case 0: // wait
			if (Device::Options::Ready())
				state++;
			break;
		case 1: // init
			SetCounter(0);
			SetWaveAmp(Device::Options::GetWaveAmp());
			SetWaveStep(Device::Options::GetWaveStep());
			SetWaveLength(Device::Options::GetWaveLength());
			SetWaveFilter(Device::Options::GetWaveFilter());
			SetSpectrumChannels(Device::Options::GetSpectrumChannels());
			SetSpectrumPartition(1);
			SetSpectrumOverlapp(Device::Options::GetSpectrumOverlapp());
			for (unsigned j = 0; j < GetWaveLength(); j++)
				GetWave()[j] = 0;
			for (unsigned j = 0; j < _data.spectrum.options.channels; j++)
				((unsigned short *)(((unsigned int)_data.spectrum.spectrum) << 1))[j] = 0;
			SetMeasureAmplitudeMax(0);
			SetMeasureAmplitudeMid(0);
			state++;
			break;
		default:
			break;
		}
	}

}
