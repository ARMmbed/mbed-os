#pragma once
#include <stdlib.h>

template< typename T >
class Median {

public:
	
	static T compute(T * buffer, int size) {
	
		qsort(buffer, size, sizeof(T), compare);
	
		if(size % 2 == 0)
		{
			return (buffer[(size / 2) - 1] + buffer[size / 2]) / 2;
		}
		else
		{
			return buffer[(size - 1) / 2];
		}
	
	}
	
private:
	Median(){};
	
	static T compare(const void * a, const void * b) { return ( *(T*)a - *(T*)b ); };
	
};