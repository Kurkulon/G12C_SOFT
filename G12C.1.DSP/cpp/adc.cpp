#include "common.h"
#include "adc.h"
#include "control.h"

namespace Hardware
{
	void ADC::Init()
	{
		SRU(HIGH, DAI_PBEN07_I);
		SRU(HIGH, DAI_PB07_I);

		SRU(HIGH, DAI_PBEN02_I);
		SRU(SPORT0_FS_O, DAI_PB02_I);
		SRU(SPORT0_FS_O, MISCA4_I);
		SRU(HIGH, INV_MISCA4_I);

		SRU(SPORT0_CLK_O, DAI_PB19_I);
		SRU(SPORT0_CLK_O, SPORT0_CLK_I);
		SRU(HIGH, DAI_PBEN19_I);//!!!!SRU(MISCA4_O, DAI_PBEN19_I);

		SRU(SPORT0_DA_O, DAI_PB01_I);
		SRU(SPORT0_DA_PBEN_O, DAI_PBEN01_I);
		SRU(DAI_PB01_O, SPORT0_DA_I);

		/*	SRU(SPORT0_CLK_O, DAI_PB19_I);
			SRU(HIGH, DAI_PBEN19_I);
			SRU(SPORT0_CLK_O, SPORT0_CLK_I);

			SRU(SPORT0_FS_O, DAI_PB02_I);
			SRU(HIGH, DAI_PBEN02_I);
			SRU(DAI_PB02_O, SPORT0_FS_I);
		*/
	}

	void ADC::Start(int *data, unsigned short size)
	{
		*pSPCTL0 = 0;
		*pDIV0 = (((Clock / Frequency) - 1) << 16) | ((PCLK / (4 * Clock) - 1) << 1);
		*pSPCTL0 = SLEN20 | FSR | IFS | LFS | LAFS | CKRE | ICLK | IFS | DTYPE1;
		*pIMSP0A = 1;
		*pIISP0A = (unsigned int)data;
		*pCSP0A = size;
		*pSPCTL0 |= SPEN_A | SDEN_A;
	}

	void ADC::Stop()
	{
		*pSPCTL0 &= ~(SPEN_A | SDEN_A);
	}

	bool ADC::Ready()
	{
		if (*pCSP0A)
			return false;
		return true;
	}

} // namespace Hardware
