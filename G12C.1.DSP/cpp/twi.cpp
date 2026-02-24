#include "common.h"
#include "TWI.h"
#include "debug.h"

namespace Hardware
{
	TWI::Descriptor *TWI::_descriptor = 0;
	TWI::Descriptor *TWI::_descriptor_last = 0;

	bool TWI::Handle(TWI::Descriptor *d)
	{
		if (d == 0)
			return false;
		if ((d->command.dimersion < 1) || (d->command.dimersion > 4))
			return false;
		if ((d->write.dimersion < 1) || (d->write.dimersion > 4))
			return false;
		if ((d->read.dimersion < 1) || (d->read.dimersion > 4))
			return false;
		if (d->command.count + d->write.count >= 0xFF) // ограничение TWI
			return false;
		if (d->read.count >= 0xFF)
			return false;

		d->next = 0;
		d->ready = false;
		d->error = false;

		if (_descriptor_last == 0)
		{
			_descriptor_last = d;
			if (_descriptor != 0)
				return false;
			_descriptor = d;
		}
		else
		{
			_descriptor_last->next = d;
			_descriptor_last = d;
		}
		return true;
	}

	void TWI::Idle()
	{
		bool next = false;
		if (_descriptor == 0)
			return;
		if (*pTWIMSTAT & (TWILOST | TWIANAK | TWIDNAK | TWIRERR | TWIWERR)) // break
		{
			*pTWIMCTL = TWIFAST | TWIMEN | TWISTOP;
			*pTWIMSTAT = (TWILOST | TWIANAK | TWIDNAK | TWIRERR | TWIWERR);
			_descriptor->error = true;
			_descriptor->command.count = 0;
			_descriptor->write.count = 0;
			_descriptor->read.count = 0;
		}
		else if ((!(*pTWIMSTAT & TWIMPROG)) && (!(*pTWIMCTL & TWIDCNT))) // start
		{
			if (_descriptor->command.count + _descriptor->write.count != 0)
			{
				*pTWIFIFOCTL = TWITXFLUSH | TWIRXFLUSH;
				*pTWIMADDR = _descriptor->address;
				*pTWIFIFOCTL = 0;
				*pTWIMCTL = TWIFAST | TWIMEN | (TWIDCNT & ((_descriptor->command.count + _descriptor->write.count) << 6));
			}
			else if (_descriptor->read.count != 0)
			{
				*pTWIFIFOCTL = TWITXFLUSH | TWIRXFLUSH;
				*pTWIMADDR = _descriptor->address;
				*pTWIFIFOCTL = 0;
				*pTWIMCTL = TWIFAST | TWIMEN | TWIMDIR | (TWIDCNT & ((_descriptor->read.count) << 6));
			}
			else
			{
				next = true;
				if (!_descriptor->error)
					_descriptor->ready = true;
			}
		}

		if (_descriptor->command.count > 0)
		{
			if ((*pTWIFIFOSTAT & TWITXS) != TWITXS)
			{
				_descriptor->command.count--;
				*pTXTWI8 = (*_descriptor->command.data) >> (8 * (_descriptor->command.count % _descriptor->command.dimersion));
				if ((_descriptor->command.count % _descriptor->command.dimersion) == 0)
					_descriptor->command.data++;
			}
		}
		else if (_descriptor->write.count > 0)
		{
			if ((*pTWIFIFOSTAT & TWITXS) != TWITXS)
			{
				_descriptor->write.count--;
				*pTXTWI8 = (*_descriptor->write.data) >> (8 * (_descriptor->write.count % _descriptor->write.dimersion));
				if ((_descriptor->write.count % _descriptor->write.dimersion) == 0)
					_descriptor->write.data++;
			}
		}
		if (_descriptor->read.count != 0)
		{
			if ((*pTWIFIFOSTAT & TWIRXS) != 0)
			{
				_descriptor->read.count--;
				if ((_descriptor->read.count % _descriptor->read.dimersion) + 1 == _descriptor->read.dimersion)
					*_descriptor->read.data = 0;
				*_descriptor->read.data |= (unsigned int)(*pRXTWI8) << (8 * (_descriptor->read.count % _descriptor->read.dimersion));
				if ((_descriptor->read.count % _descriptor->read.dimersion) == 0)
					_descriptor->read.data++;
			}
		}

		if (next)
		{
			Descriptor *n = _descriptor->next;
			if (n != 0)
			{
				_descriptor->next = 0;
				_descriptor = n;
			}
			else
			{
				_descriptor_last = 0;
				_descriptor = 0;
			}
		}
	}

	void TWI::Init()
	{
		SRU(LOW, DPI_PB13_I);
		SRU(TWI_DATA_PBEN_O, DPI_PBEN13_I);
		SRU(DPI_PB13_O, TWI_DATA_I);

		SRU(LOW, DPI_PB14_I);
		SRU(TWI_CLK_PBEN_O, DPI_PBEN14_I);
		SRU(DPI_PB14_O, TWI_CLK_I);

		*pTWIMCTL = 0;
		*pTWIMSTAT = (TWILOST | TWIANAK | TWIDNAK | TWIRERR | TWIWERR);
		*pTWIFIFOCTL = TWITXFLUSH | TWIRXFLUSH;
		*pTWIMITR = (PRESCALE & (unsigned int)(PCLK / 10000000.0f));
		*pTWIDIV = (8 << 8) | (17 << 0);
		*pTWIMITR |= TWIEN;
		*pTWIFIFOCTL = 0;
	}
}
