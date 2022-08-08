/*
 * Ring_Buf.h
 *
 *  Created on: 2 июл. 2022 г.
 *      Author: admin
 */

#ifndef INC_RING_BUF_H_
#define INC_RING_BUF_H_

#include <iostream>
#include <locale.h>
#include <vector>

template <typename type>

class RingBuf
{
private:

	std::vector<type> m_ring_mem;
	int rd_ind = 0;
	int wr_ind = 0;

public:

	RingBuf(int n)
	{
		m_ring_mem.resize(n);
	}

	~RingBuf()
	{
		m_ring_mem.resize(0);
	}

	void write(type input)
	{
		if (wr_ind == m_ring_mem.size() - 1)
		{
			m_ring_mem[wr_ind] = input;
			wr_ind = 0;
		}
		else
		{
			m_ring_mem[wr_ind] = input;
			wr_ind++;
		}

	}

	int readLenght()
	{
		return wr_ind-rd_ind;
	}

	void bufReset()
	{
		rd_ind = 0;
		wr_ind = 0;
	}

	bool isEmpty()
	{
		return rd_ind == wr_ind;
	}

	type read()
	{

		if (isEmpty())
		{
			return 0;
		}
		else
		{
			if (rd_ind == m_ring_mem.size() - 1)
			{
				int rd_ind_temp = rd_ind;
				rd_ind = 0;
				return m_ring_mem.at(rd_ind_temp);
			}
			else
			{
				return m_ring_mem.at(rd_ind++);
			}
		}
	}

	void RingBuffer_Write(type *pData, uint16_t len)
	{
		for (uint16_t i=0; i < len; i++)
		{
			this -> write(pData[i]);
		}
	}


};






#endif /* INC_RING_BUF_H_ */
