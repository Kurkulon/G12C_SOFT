#include "common.h"
#include "rs485.h"
#include "control.h"

namespace Hardware
{
	void RS485::Init()
	{
		SRU(UART0_TX_O, DPI_PB04_I);
		SRU(UART0_TX_PBEN_O, DPI_PBEN04_I);
		SRU(LOW, DPI_PBEN08_I);
		SRU(DPI_PB08_O, UART0_RX_I);
		SRU(LOW, DPI_PB06_I); // DE
		SRU(HIGH, DPI_PBEN06_I);

		*pUART0TXCTL = 0;
		*pUART0RXCTL = 0;
		*pUART0LCR = UARTDLAB;
		*pUART0DLL = ((unsigned int)(PCLK / (16 * Frequency)));
		*pUART0DLH = (((unsigned int)(PCLK / (16 * Frequency))) >> 8);
		*pUART0LCR = UARTWLS8 | UARTSTB | UARTPEN | UARTEPS;
		*pUART0MODE = UARTPST0 | UARTPST1; // пробовал с упаковкой по 2 байта, проблема возникает когда приходит всего один байт, тогда невозможно его отследить, можно сделать на RX без упаковки, на TX с упаковкой и экономить память
	}

	unsigned short RS485::Pack(unsigned short *data, unsigned short size) // смотри описание на SHARC, там сказано на хрена это нужно
	{
		for (short s = size - 1; s >= 0; s--)
		{
			data[s * 2 + 1] = (data[s] >> 8) & 0xFF;
			data[s * 2] = data[s] & 0xFF;
		}
		return size * 2;
	}
	

	unsigned short RS485::UnPack(unsigned short *data, unsigned short size) // смотри описание на SHARC, там сказано на хрена это нужно
	{
		for (unsigned short s = 0; s < size / 2; s++)
			data[s] = ((data[s * 2 + 1] << 8) & 0xFF00) | (data[s * 2] & 0x00FF);
		return size / 2;
	}

	void RS485::StartTX(unsigned char *data, unsigned short size)
	{
		SRU(HIGH, DPI_PB06_I);
		*pIMUART0TX = 1;
		*pIIUART0TX = (unsigned int)data;
		*pCUART0TX = size;
		*pUART0TXCTL |= UARTEN | UARTDEN;
	}

	void RS485::StartRX(unsigned char *data, unsigned short size)
	{
		SRU(LOW, DPI_PB06_I);
		*pIMUART0RX = 1;
		*pIIUART0RX = (unsigned int)data;
		*pCUART0RX = size;
		*pUART0RXCTL |= UARTEN | UARTDEN;
	}

	void RS485::StopTX()
	{
		*pUART0TXCTL &= ~(UARTEN | UARTDEN);
		SRU(LOW, DPI_PB06_I);
	}

	void RS485::StopRX()
	{
		*pUART0RXCTL &= ~(UARTEN | UARTDEN);
	}

	bool RS485::ReadyTX()
	{
		if (*pCUART0TX)
			return false;
		if (!((*pUART0LSR) & UARTTEMT))
			return false;

		return true;
	}

	bool RS485::ReadyRX()
	{
		if (*pCUART0RX)
			return false;
		if ((*pUART0LSR) & UARTDR)
			return false;
		return true;
	}

	unsigned short RS485::GetTX()
	{
		return (*pCUART0TX);
	}

	unsigned short RS485::GetRX()
	{
		return (*pCUART0RX);
	}

} // namespace Hardware
