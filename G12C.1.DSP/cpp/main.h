#ifndef MAIN_H__INCLUDED
#define MAIN_H__INCLUDED

#ifndef __cplusplus
#error C++ compilation required.
#endif

namespace Software
{
	class Main
	{
	private:
		static const unsigned int _version = 0x0200;

	public:
		static unsigned int GetVersion()
		{
			return _version;
		}
	};
} // namespace Software

namespace Hardware
{
	class Main
	{
	public:
		static void Init()
		{
			sysreg_write(sysreg_IMASKP, 0);
			sysreg_write(sysreg_IRPTL, 0);
			sysreg_write(sysreg_LIRPTL, 0);
			sysreg_write(sysreg_IMASK, 0);
			//----- Init PLL ----------------------------------------
			unsigned int i, pmctl;
			pmctl = *pPMCTL;
			pmctl &= ~(PLLM63 | INDIV | PLLD16);
			pmctl |= ((CCLK / CLKIN) & (PLLM63)) | PLLD2 | DIVEN;
			*pPMCTL = pmctl;
			pmctl |= PLLBP;			   // Setting the Bypass bit
			*pPMCTL = pmctl;		   // Putting the PLL into bypass mode
			for (i = 0; i < 4096; i++) // Wait for around 4096 cycles for the pll to lock.
				NOP;
			pmctl = *pPMCTL;
			pmctl &= ~PLLBP; // Clear Bypass Mode
			*pPMCTL = pmctl;
			for (i = 0; i < 15; i++) // Wait for around 15 cycles for the output dividers to stabilize.
				NOP;
		};
		static void inline Idle(){};
	};
} // namespace Hardware

#endif
