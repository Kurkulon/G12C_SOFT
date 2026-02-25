#include "common.h"
#include "options.h"
#include "twi.h"
#include "debug.h"
#include "mux.h"
#include "calculation.h"


namespace Device
{
	unsigned int Options::_options_buffer[];
	Options::options_type *Options::_options = (Options::options_type *)((unsigned int)_options_buffer << 1); // pack16
	bool Options::_save = false;
	bool Options::_ready = false;

	unsigned short Options::GetNumber()
	{
		return _options->special.number;
	}

	void Options::SetNumber(unsigned short number)
	{
		_options->special.number = number;
	}
 
	unsigned short Options::GetWaveAmp()
	{
		if (_options->wave.amp > Hardware::MUX::Max + 1)
			_options->wave.amp = Hardware::MUX::Max + 1;
		return _options->wave.amp;
	}

	void Options::SetWaveAmp(unsigned short amp)
	{
		_options->wave.amp = amp;
	}

	unsigned short Options::GetWaveStep()
	{
		_options->wave.step = Software::Calculation::Step; // оставили один шаг
		return _options->wave.step;
	}

	void Options::SetWaveStep(unsigned short step)
	{
		_options->wave.step = step; 
	}

	unsigned short Options::GetWaveLength()
	{
		_options->wave.length = Software::Calculation::Length; // оставили одну длину
		return _options->wave.length;
	}

	void Options::SetWaveLength(unsigned short length)
	{
		_options->wave.length = length; 
	}

	unsigned short Options::GetWaveFilter()
	{
		return _options->wave.filter;
	}

	void Options::SetWaveFilter(unsigned short filter)
	{
		_options->wave.filter = filter;
	}

	unsigned short Options::GetSpectrumChannels()
	{
		if (_options->spectrum.channels <= Software::Calculation::MinChannels)
			_options->spectrum.channels = Software::Calculation::MinChannels;
		if (_options->spectrum.channels > Software::Calculation::MaxChannels)
			_options->spectrum.channels = Software::Calculation::MaxChannels;
		return _options->spectrum.channels;
	}

	void Options::SetSpectrumChannels(unsigned short channels)
	{
		_options->spectrum.channels = channels;
	}

	unsigned short Options::GetSpectrumOverlapp()
	{
		if (_options->spectrum.overlapp <= Software::Calculation::MinOverlapp)
			_options->spectrum.overlapp = Software::Calculation::MinOverlapp;
		if (_options->spectrum.overlapp > Software::Calculation::MaxOverlapp)
			_options->spectrum.overlapp = Software::Calculation::MaxOverlapp;
		return _options->spectrum.overlapp;
	}

	void Options::SetSpectrumOverlapp(unsigned short overlapp)
	{
		_options->spectrum.overlapp = overlapp;
	}

	void Options::Init()
	{
		_save = false;
		_ready = false;
	}

	void Options::Idle()
	{
		static unsigned char state = 0;
		static Hardware::TWI::Descriptor d;
		static unsigned short offset;
		static unsigned short count;

		switch (state)
		{
		case 0:
			offset = 0;
			count = 0;
			state = 1;
			break;
		case 1:
			d.address = 0x50;
			d.command.dimersion = 2;
			d.command.data = (unsigned int *)&offset;
			d.command.count = sizeof(offset) * d.command.dimersion;
			d.write.dimersion = 2;
			d.write.data = 0;
			d.write.count = 0;
			d.read.dimersion = 2;
			d.read.data = (unsigned int *)((unsigned int)_options + offset / d.read.dimersion);
			count = sizeof(options_type) * d.read.dimersion - offset;
			if (count > 64)
				count = 64;
			d.read.count = count;
			Hardware::TWI::Handle(&d);
			state = 2;
			break;
		case 2:
			if (d.ready)
			{
				offset += count;
				if (offset < sizeof(options_type) * d.read.dimersion)
					state = 1;
				else
				{
					state = 3;
					_ready = true;
				}
			}
			if (d.error)
				state = 1;
			break;
		case 3:
			if (!_save)
				break;
			offset = 0x0000;
			state = 4;
			break;
		case 4:
			d.address = 0x50;
			d.command.dimersion = 2;
			d.command.data = (unsigned int *)&offset;
			d.command.count = sizeof(offset) * d.command.dimersion;
			d.write.dimersion = 2;
			d.write.data = (unsigned int *)((unsigned int)_options + offset / d.write.dimersion);
			count = sizeof(options_type) * d.write.dimersion - offset;
			if (count > 64)
				count = 64;
			d.write.count = count;
			d.read.dimersion = 2;
			d.read.data = 0;
			d.read.count = 0;
			Hardware::TWI::Handle(&d);
			state = 5;
			break;
		case 5:
			if (d.ready)
			{
				offset += count;
				if (offset < sizeof(options_type) * d.write.dimersion)
					state = 4;
				else
				{
					_save = false;
					state = 3;
				}
			}
			if (d.error)
			{
				state = 4;
			}
			break;
		default:
			state = 0;
			break;
		}
	}

}
