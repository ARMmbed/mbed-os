#pragma once
#include <list>
#include <iterator>
#include <cmath>

#define DEFAULT_MEASUREMENT_BUFFER_SIZE		9

template <typename T>
struct IMeasurementBuffer {
	IMeasurementBuffer(){}
	virtual ~IMeasurementBuffer(){}
	
	virtual void clear(void) = 0;
	virtual void add(T v) = 0;
	virtual T get(void) = 0;
	virtual T MAD(void) = 0;
};

template <typename T>
class MeasurementBuffer : public IMeasurementBuffer<T> {

public:
	
	MeasurementBuffer() : MeasurementBuffer(DEFAULT_MEASUREMENT_BUFFER_SIZE){};
	MeasurementBuffer(int size) : _size(size){};
	
	void clear() {
		buf.clear();
	};
	
	void add(T v){
		buf.push_back(v);
		if (buf.size() > _size) buf.pop_front();
	};
	
	T get() {
		std::list<T> bufCpy (buf);
		return _median(bufCpy);
	};
	
	T MAD() {
		T median = get();
		std::list<T> tmp;
		
		for (typename std::list<T>::const_iterator it = buf.begin(); it != buf.end(); it++) {
			tmp.push_back(abs(*it-median));
		}
		return _median(tmp);
	};

private:
	std::list<T> buf;
	int _size;
	
	T _median(std::list<T> & b) {
		
		if (b.size() > 0) {
			
			b.sort();
		
			if (b.size() % 2 == 0 ) {
				typename std::list<T>::const_iterator it1 = b.begin();
				std::advance(it1, (b.size() / 2) - 1);
				typename std::list<T>::const_iterator it2 = b.begin();
				std::advance(it2, b.size() / 2);
				
				return (*it1 + *it2) / 2;
			} else {
				typename std::list<T>::const_iterator it = b.begin();
				std::advance(it, (b.size() - 1) / 2);
				return *it;
			}
		}
		
		return T();
		
	};
	
};