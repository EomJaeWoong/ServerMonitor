#ifndef HEAD_QUEUE_H
#define HEAD_QUEUE_H

template <class DATA>
class CQueue
{
public :
	CQueue(int MaxSize)
	{
		_ptrQueue = new DATA[MaxSize];
		_maxSize = MaxSize;
		_read = 0;
		_write = 0;
	}

	~CQueue()
	{
		delete[] _ptrQueue;
	}

private :
	DATA* _ptrQueue;
	int _read;
	int _write;
	int _maxSize;

public :
	bool read(DATA *outData)
	{
		if (_read == _write)	return false;

		*outData = _ptrQueue[_read];
		_read = (_read + 1) % _maxSize;

		return true;
	}

	bool write(DATA data)
	{
		if (((_write + 1) % _maxSize) == _read)	return false;

		_ptrQueue[_write] = data;
		_write = (_write + 1) % _maxSize;

		return true;
	}

	int getQueueSize()
	{
		if (_write < _read)	return (_write + _maxSize) - _read;
		else				return _write - _read;
	}

	bool peak(int position, DATA *outData)
	{
		if (isEmpty() || (position > getQueueSize()) || position < 0)	return false;

		*outData = _ptrQueue[(_read + position) % _maxSize];

		return true;
	}

	bool isFull()
	{
		if (((_write + 1) % _maxSize) == _read)	return true;
		else									return false;
	}

	bool isEmpty()
	{
		if (_read == _write) return true;
		else				 return false;
	}
};

#endif