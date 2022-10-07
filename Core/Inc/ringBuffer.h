/*
 * ringBuffer.h
 *
 *  Created on: 11 мар. 2021 г.
 *      Author: Rumyantzev_D
 */

#ifndef INC_RINGBUFFER_H_
#define INC_RINGBUFFER_H_

#include "sys/_stdint.h"

template<int SIZE, class DATA_T=unsigned char>
class RingBuffer
{
public:
	typedef uint16_t INDEX_T;

private:
	DATA_T _data[SIZE];
	volatile INDEX_T _readCount;
	volatile INDEX_T _writeCount;
	static const INDEX_T _mask = SIZE - 1;

public:
	inline bool Write(DATA_T value)
	{
		if(IsFull())
				return false;
		_data[_writeCount++ & _mask] = value;
		return true;
	}

	inline bool Read(DATA_T &value)
	{
		if(IsEmpty())
				return false;
		value = _data[_readCount++ & _mask];
		return true;
	}

	inline DATA_T First()const
	{
		return operator[](0);
	}

	inline DATA_T Last()const
	{
		return operator[](Count());
	}

	inline DATA_T& operator[] (INDEX_T i)
	{
		if(IsEmpty() || i > Count())
			return DATA_T();
		return _data[(_readCount + i) & _mask];
	}

	inline const DATA_T operator[] (INDEX_T i)const
	{
		if(IsEmpty())
			return DATA_T();
		return _data[(_readCount + i) & _mask];
	}

	inline bool IsEmpty()const
	{
		return _writeCount == _readCount;
	}

	inline bool IsFull()const
	{
		return ((INDEX_T)(_writeCount - _readCount) & (INDEX_T)~(_mask)) != 0;
	}

	INDEX_T Count()const
	{
		return (_writeCount - _readCount) & _mask;
	}

	inline void Clear()
	{
		_readCount=0;
		_writeCount=0;
	}

	inline unsigned Size()
	{return SIZE;}
};

#endif /* INC_RINGBUFFER_H_ */
