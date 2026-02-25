#include "common.h"
#include "main.h"
#include "mux.h"
#include "control.h"

namespace Hardware
{
    void MUX::Init() 
    {
        
    	SRU(SPI_CLK_O, DPI_PB09_I);
		SRU(SPI_CLK_PBEN_O, DPI_PBEN09_I);
        SRU(SPI_CLK_O, SPI_CLK_I);

       	SRU(SPI_FLG0_O, DPI_PB10_I);
		SRU(SPI_FLG0_PBEN_O, DPI_PBEN10_I);
            
        SRU(SPI_MOSI_O, DPI_PB11_I);
		SRU(SPI_MOSI_PBEN_O, DPI_PBEN11_I);
        SRU(SPI_MOSI_O, SPI_MOSI_I);

        SRU(SPI_MISO_O, DPI_PB12_I);
		SRU(SPI_MISO_PBEN_O, DPI_PBEN12_I);
        SRU(SPI_MISO_O, SPI_MISO_I);

        *pSPIFLG = DS0EN | SPIFLG0;
        *pSPICTL = WL8 | MSBF | SPIMS | TIMOD1 | CLKPL;  
        *pSPIBAUD = ((PCLK / (4 * Clock)) << 1);
		*pSPICTL |= SPIEN;
    }

    void MUX::Set(unsigned char data)
    {      
        *pTXSPI = data;
    }

} // namespace Hardware
