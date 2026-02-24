#include "common.h"
#include "main.h"
#include "mux.h"
#include "control.h"

namespace Hardware
{
    void MUX::Init() // 74HCT595
    {
        SRU(SPORT2_CLK_O, DAI_PB06_I);
        SRU(HIGH, DAI_PBEN06_I);
        SRU(SPORT2_CLK_O, SPORT2_CLK_I);

        SRU(SPORT2_FS_O, DAI_PB05_I);
        SRU(HIGH, DAI_PBEN05_I);
        SRU(DAI_PB05_O, SPORT2_FS_I);

        SRU(SPORT2_DA_O, DAI_PB02_I);
        SRU(SPORT2_DA_PBEN_O, DAI_PBEN02_I);
        SRU(DAI_PB02_O, SPORT2_DA_I);

        *pSPCTL2 = 0;
        *pDIV2 = (((Clk / Frequency) - 1) << 16) | ((PCLK / (4 * Clk) - 1) << 1);
		*pSPCTL2 = SLEN8 | FSR | LFS | SPTRAN | LAFS | CKRE | ICLK | IFS; 
        *pSPCTL2 |= SPEN_A; 
    }

    void MUX::Set(unsigned char data)
    {      
        *pTXSP2A = data;
    }

} // namespace Hardware
