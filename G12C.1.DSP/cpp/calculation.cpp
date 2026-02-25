#include "common.h"
#include "control.h"
#include "calculation.h"

namespace Software
{
	complex_float Calculation::_coef[];
	complex_float Calculation::_data[Software::Calculation::Length];
	/*const float Calculation::_partition[1024] =
		{
#include "partition.table"
	};*/

	bool Calculation::FFT(complex_float *data, unsigned int *size, bool inverse)
	{
		if ((*size > (1u << 13)) || (*size < (1u << 1)))
			return false;
		volatile unsigned int size_pow = 0;
		while ((1u << (size_pow + 1)) <= *size)
			size_pow++;
		*size = (1u << size_pow);
		volatile int i, j, n, k, io, ie, in, nn;
		volatile float ru, iu, rtp, itp, rtq, itq, rw, iw, sr;
		nn = *size >> 1;
		ie = *size;
		for (n = 1; n <= size_pow; n++)
		{
			rw = _coef[size_pow - n].re;
			iw = _coef[size_pow - n].im;
			if (inverse)
				iw = -iw;
			in = ie >> 1;
			ru = 1.0f;
			iu = 0.0f;
			for (j = 0; j < in; j++)
			{
				for (i = j; i < *size; i += ie)
				{
					io = i + in;
					rtp = data[i].re + data[io].re;
					itp = data[i].im + data[io].im;
					rtq = data[i].re - data[io].re;
					itq = data[i].im - data[io].im;
					data[io].re = rtq * ru - itq * iu;
					data[io].im = itq * ru + rtq * iu;
					data[i].re = rtp;
					data[i].im = itp;
				}
				sr = ru;
				ru = ru * rw - iu * iw;
				iu = iu * rw + sr * iw;
			}
			ie >>= 1;
		}
		for (j = i = 1; i < *size; i++)
		{
			if (i < j)
			{
				io = i - 1;
				in = j - 1;
				rtp = data[in].re;
				itp = data[in].im;
				data[in].re = data[io].re;
				data[in].im = data[io].im;
				data[io].re = rtp;
				data[io].im = itp;
			}
			k = nn;
			while (k < j)
			{
				j = j - k;
				k >>= 1;
			}
			j = j + k;
		}
		if (!inverse)
			return true;
		rw = 1.0f / (*size);
		for (i = 0; i < (*size); i++)
		{
			data[i].re *= rw;
			data[i].im *= rw;
		}
		return true;
	}

	bool Calculation::Partition(math_type math, complex_float *data, unsigned int size, unsigned short *out, unsigned int *out_size, float *partition, float overlapping)
	{
	/*	overlapping /= 100.0f; // переводим проценты
		overlapping /= 2;	   // делим пополам чтобы применить к 2-м окнам
		if (*out_size > sizeof(_partition))
			*out_size = sizeof(_partition);
		if (*out_size < 1)
			*out_size = 1;
		if (*out_size > size)
			*out_size = size;
		float index = 0;
		float k = _partition[*out_size - 1];
		for (int i = 0; i < size; i++) // переводим из комплексного числа в вещественное
			data[i].re = sqrt(data[i].re * data[i].re + data[i].im * data[i].im);
		for (int i = 0; i < *out_size; i++)
		{
			float value = 0;
			int count = 0;
			int index_start = (int)(index - (overlapping * (k / _partition[*out_size - 1])));
			int index_end = (int)((index + k) + (overlapping * (k * _partition[*out_size - 1])));
			if (index_start < 0)
				index_start = 0;
			if (index_end > size)
				index_end = size;
			switch (math)
			{
			case MATH_AVERAGE: // усреднение в окне
				for (int j = index_start; j < index_end; j++)
					value += data[j].re;
				value *= (1u << (16 - Hardware::ADCS7476::Resolution)); //  т.к. входной сигнал 12бит, а передать можем 16 бит. короче растянем шкалу
				count = index_end - index_start;
				break;
			case MATH_MAXIMUM:
				for (int j = index_start; j < index_end; j++)
				{
					if (value < data[j].re) // и ищем максимум
						value = data[j].re;
				}
				count = 1;
				break;
			default:
				break;
			}

			if (count)
			{
				value /= count;
				value /= (size / 2);
				if (value > 0xFFFF)
					value = 0xFFFF;
				if (value < 0)
					value = 0;
			}
			out[i] = value;
			index += k;
			k = k * _partition[*out_size - 1];
		}
		*partition = _partition[*out_size - 1];*/
		return true;
	}

	bool Calculation::Calculate(math_type math, unsigned short *in, unsigned int in_size, unsigned short *out, unsigned int *out_size, float *partition, float overlapping)
	{
		if (in_size > sizeof(_data) / sizeof(complex_float))
			return false;
		for (int i = 0; i < in_size; i++)
		{
			_data[i].re = (float)in[i];
			_data[i].im = 0;
		}
		if (!FFT(_data, &in_size, false))
			return false;
		//if (!Partition(math, _data, in_size / 2, out, out_size, partition, overlapping))
		//	return false;
		return true;
	}

	void Calculation::Init()
	{
		for (int i = 0; i < sizeof(_coef) / sizeof(complex_float); i++)
		{
			_coef[i].re = cos(-PI / (1u << i));
			_coef[i].im = sin(-PI / (1u << i));
		}
	}

	
}
