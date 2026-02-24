#include "common.h"
#include "debug.h"

namespace Hardware
{

	void Debug::Init()
	{
		sysreg_bit_clr(sysreg_FLAGS, FLG0);
		sysreg_bit_set(sysreg_FLAGS, FLG0O);
	}

	void Debug::TP(bool state)
	{
		if (state)
			sysreg_bit_set(sysreg_FLAGS, FLG0);
		else
			sysreg_bit_clr(sysreg_FLAGS, FLG0);
	}
	
}
