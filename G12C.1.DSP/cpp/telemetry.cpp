#include "common.h"
#include "main.h"
#include "telemetry.h"
#include "options.h"
#include "debug.h"
#include "rs485.h"
#include "data.h"
#include "control.h"

namespace Device
{
	unsigned short Telemetry::Parse(unsigned short *receive_data, unsigned short receive_count, unsigned short *transceive_data)
	{
		if ((receive_count != sizeof(receive_type)) && (receive_count != sizeof(receive_extended_type)))
			return 0;
		unsigned int result = 0;
		switch (((receive_type *)receive_data)->request)
		{
		case 0x0600:
			if (receive_count != sizeof(receive_type))
				break;
			((transceive_identification_type *)(transceive_data))->number = Device::Options::GetNumber();
			((transceive_identification_type *)(transceive_data))->version = Software::Main::GetVersion();
			result = (sizeof(transceive_identification_type));
			break;
		case 0x0610:
			if (receive_count != sizeof(receive_type))
				break;
			((transceive_options_type *)transceive_data)->amp = Device::Options::GetWaveAmp();
			((transceive_options_type *)transceive_data)->step = Device::Options::GetWaveStep();
			((transceive_options_type *)transceive_data)->length = Device::Options::GetWaveLength();
			((transceive_options_type *)transceive_data)->filter = Device::Options::GetWaveFilter();
			((transceive_options_type *)transceive_data)->channels = Device::Options::GetSpectrumChannels();
			((transceive_options_type *)transceive_data)->overlapp = Device::Options::GetSpectrumOverlapp();
			result = sizeof(transceive_options_type);
			break;
		case 0x0620:
		case 0x0621:
			if (receive_count != sizeof(receive_type))
				break;
			((transceive_measure_type *)transceive_data)->options.amp = Software::Data::GetWaveAmp();
			((transceive_measure_type *)transceive_data)->options.step = Software::Data::GetWaveStep();
			((transceive_measure_type *)transceive_data)->options.length = Software::Data::GetWaveLength();
			((transceive_measure_type *)transceive_data)->options.filter = Software::Data::GetWaveFilter();
			((transceive_measure_type *)transceive_data)->options.channels = Software::Data::GetSpectrumChannels();
			{
				float partition = Software::Data::GetSpectrumPartition();
				((transceive_measure_type *)transceive_data)->options.partition[0] = (unsigned short)(*(unsigned int *)(&partition)) & 0xFFFF;
				((transceive_measure_type *)transceive_data)->options.partition[1] = (unsigned short)((*(unsigned int *)(&partition)) >> 16) & 0xFFFF;
			}
			((transceive_measure_type *)transceive_data)->options.overlapp = Software::Data::GetSpectrumOverlapp();
			((transceive_measure_type *)transceive_data)->measure.amplitude_max = Software::Data::GetMeasureAmplitudeMax();
			((transceive_measure_type *)transceive_data)->measure.amplitude_mid = Software::Data::GetMeasureAmplitudeMid();
			((transceive_measure_type *)transceive_data)->measure.temperature = Device::Control::GetTemperature();
			for (unsigned int i = 0; i < ((transceive_measure_type *)transceive_data)->options.channels; i++)
				((transceive_measure_type *)transceive_data)->spectrum[i] = Software::Data::GetSpectrum()[i];
			Device::Control::Set((((receive_type *)receive_data)->request) & 0x000F);
			result = sizeof(transceive_measure_type) + ((transceive_measure_type *)transceive_data)->options.channels;
			break;
		case 0x0680:
			if (receive_count != sizeof(receive_extended_type))
				break;
			switch (((receive_extended_type *)receive_data)->data[0])
			{
			case 0x0001:
				result = sizeof(transceive_type);
				Device::Options::SetNumber(((receive_extended_type *)receive_data)->data[1]);
				break;
			case 0x0002:
				result = sizeof(transceive_type);
				break;
			}
			break;
		case 0x0690:
			if (receive_count != sizeof(receive_extended_type))
				break;
			switch (((receive_extended_type *)receive_data)->data[0])
			{
			case 0x0001:
				result = sizeof(transceive_type);
				Device::Options::SetWaveAmp(((receive_extended_type *)receive_data)->data[1]); 
				break;
			case 0x0002:
				result = sizeof(transceive_type);
				Device::Options::SetWaveStep(((receive_extended_type *)receive_data)->data[1]);
				break;
			case 0x0003:
				result = sizeof(transceive_type);
				Device::Options::SetWaveLength(((receive_extended_type *)receive_data)->data[1]);
				break;
			case 0x0004:
				result = sizeof(transceive_type);
				Device::Options::SetWaveFilter(((receive_extended_type *)receive_data)->data[1]);
				break;
			case 0x0005:
				result = sizeof(transceive_type);
				Device::Options::SetSpectrumChannels(((receive_extended_type *)receive_data)->data[1]);
				break;
			case 0x0006:
				result = sizeof(transceive_type);
				Device::Options::SetSpectrumOverlapp(((receive_extended_type *)receive_data)->data[1]);
				break;
			}
			break;
		case 0x06F0:
			if (receive_count != sizeof(receive_type))
				break;
			result = sizeof(transceive_type);
			Device::Options::Save();
			break;
		default:
			result = 0;
			break;
		}
		if (result)
			((transceive_type *)(transceive_data))->response = ((receive_type *)receive_data)->request;
		return result;
	}

	void Telemetry::Idle()
	{
		static unsigned char state = 0;
		static unsigned short receive_count = 0;
		static unsigned short receive_size = 0;
		static unsigned short receive_data[_receive_data_size * 2]; //однобайтная передача, для передачи 0xAA55 передаем 0x00AA + 0x0055
		static unsigned short transceive_count = 0;
		static unsigned short transceive_size = 0;
		static unsigned short transceive_data[_transceive_data_size * 2]; //однобайтная передача, для передачи 0xAA55 передаем 0x00AA + 0x0055
		switch (state)
		{
		case 0:
			if (!Device::Options::Ready())
				break;
			receive_count = 0;
			receive_size = sizeof(receive_data);
			Hardware::RS485::StartRX((unsigned char *)receive_data, receive_size);
			TimerStart(_receive_pause_before);
			state++;
			break;
		case 1:
			if (receive_count < receive_size - Hardware::RS485::GetRX())
			{
				receive_count = receive_size - Hardware::RS485::GetRX();
				TimerStop();
				TimerStart(_receive_pause_before);
			}
			if (TimerEvent())
			{
				TimerStop();
				Hardware::RS485::StopRX();
				receive_count = 0;
				receive_size = sizeof(receive_data);
				Hardware::RS485::StartRX((unsigned char *)receive_data, receive_size);
				state++;
			}
			break;
		case 2:
			if (receive_count < receive_size - Hardware::RS485::GetRX())
			{
				receive_count = receive_size - Hardware::RS485::GetRX();
				TimerStop();
				TimerStart(_receive_pause_after);
			}
			if (!receive_count)
				break;
			if (TimerEvent())
			{
				TimerStop();
				Hardware::RS485::StopRX();
				receive_count = Hardware::RS485::UnPack(receive_data, receive_count); // bytes -> words
				transceive_size = Parse(receive_data, receive_count, transceive_data);
				receive_count = 0;
				if (transceive_size > 0)
				{
					transceive_size = Hardware::RS485::Pack(transceive_data, transceive_size); // bytes
					Hardware::RS485::StartTX((unsigned char *)transceive_data, transceive_size);
					state++;
				}
				else
					state = 0;
			}
			break;
		case 3:
			if (Hardware::RS485::ReadyTX())
			{
				Hardware::RS485::StopTX();
				transceive_count = 0;
				transceive_size = 0;
				Device::Control::Start();
				state = 0;
			}
			break;
		}
	}
}
