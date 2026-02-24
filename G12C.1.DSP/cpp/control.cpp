#include "common.h"
#include "control.h"
#include "options.h"
#include "debug.h"
#include "main.h"
#include "adc.h"
#include "data.h"

namespace Device
{
	Control::status_type Control::_status;
	unsigned char Control::_math;

	void Control::Start(unsigned char math)
	{
		if (_status == STATUS_WAIT)
			_status = STATUS_START;
		_math = math;
	}

	void Control::Init()
	{
		_status = STATUS_WAIT;
	}

	void Control::Idle()
	{
		static float amp = 0; // Hardware::MUX::Max;
		switch (_status)
		{
		case STATUS_START:
			if (!Device::Options::GetWaveAmp())
				Software::Data::SetWaveAmp(Device::Options::GetWaveAmp());
			else
				Software::Data::SetWaveAmp(amp);
			// Hardware::MUX::Set(&amp);
			Software::Data::SetWaveFilter(Device::Options::GetWaveFilter());
			Software::Data::SetWaveStep(Device::Options::GetWaveStep());
			Software::Data::SetWaveLength(Device::Options::GetWaveLength());
			Hardware::ADC::Start(Software::Data::GetWaveBuffer(), Software::Data::GetWaveLength());
Hardware::Debug::TP(1)			;
Hardware::Debug::TP(0)			;
			_status = STATUS_MEASURE;
			break;
		case STATUS_MEASURE:
			if (Hardware::ADC::Ready())
			{
				Hardware::ADC::Stop();
				for(int i = 0; i < Software::Data::GetSpectrumChannels(); i++)//!!!
				{	
					int value = Software::Data::GetWave()[i];//(i * 60) << 4; //Software::Data::GetWave()[i];
				//	value = ((value & 0xFF00FF00) >> 8) | ((value & 0x00FF00FF) << 8);
					Software::Data::GetSpectrum()[i] = (unsigned short)((int)value >> (Hardware::ADC::Resolution - 16));
				}	
					
				/*int max = 1 << Hardware::ADC::Resolution;
				int min = (-1) << Hardware::ADC::Resolution;
				int mid = 0;
				int norm = 0;
				unsigned short size = Software::Data::GetWaveLength();
				int *data = Software::Data::GetWave();
				for (int i = 0; i < size; i++)
					norm += data[i] >> (Hardware::ADC::Resolution - 16);
				norm /= size >> (Hardware::ADC::Resolution - 16);
				for (int i = 0; i < size; i++)
				{
					volatile int x = data[i] - norm;
					if (x > max)
						max = x;
					if (x < min)
						min = x;
					mid += abs(x);
				}
				max = max - min;
				mid /= size;
				if (max > (((Hardware::ADC::Max - Hardware::ADC::Min) * 3) / 4))
					amp++;
				if (max < (((Hardware::ADC::Max - Hardware::ADC::Min) * 1) / 4))
					amp--;
				//if (amp > Hardware::MUX::Max)
				//	amp = Hardware::MUX::Max;
				//if (amp <= Hardware::MUX::Min)
				//	amp = Hardware::MUX::Min;
				Software::Data::SetMeasureAmplitudeMax(max >> (Hardware::ADC::Resolution - 16));
				Software::Data::SetMeasureAmplitudeMid(mid >> (Hardware::ADC::Resolution - 16));*/
				_status = STATUS_MATH;
			}
			break;
		case STATUS_MATH:
		/*	unsigned int channels;
			channels = Software::Options::GetSpectrumChannels();
			float partition;
			float overlapp;
			switch (_math)
			{
			case Software::Spectrum::MATH_AVERAGE:
				overlapp = Software::Options::GetSpectrumOverlapp();
				break;
			case Software::Spectrum::MATH_MAXIMUM: // принудительно убираем перекрытие окон
			default:
				overlapp = 0;
				break;
			}
			channels = Software::Options::GetSpectrumChannels();
			Software::Spectrum::Calculate((Software::Spectrum::math_type)_math, Software::Data::GetNoise(), Software::Data::GetSpectrumLength(), Software::Data::GetSpectrum(), &channels, &partition, overlapp);
			Software::Data::SetSpectrumChannels(channels);
			Software::Data::SetSpectrumPartition(partition);
			Software::Data::SetSpectrumOverlapp(overlapp);*/
			_status = STATUS_READY;
			break;
		case STATUS_READY:
			_status = STATUS_WAIT;
			break;
		case STATUS_WAIT:
		default:
			break;
		}
		
	}

} // namespace Device
