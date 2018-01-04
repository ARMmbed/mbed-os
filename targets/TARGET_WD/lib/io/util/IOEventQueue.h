#pragma once

#include "platform/Callback.h"
#include "mbed_events.h"
#include "rtos.h"

#define IO_EVENT_QUEUE_SIZE		32

class IOEventQueue {
	
	public:
		static IOEventQueue & getInstance() {
			static IOEventQueue instance;
			return instance;
		}
	
		template <typename R, typename A0>
		Event<void(A0)> event(mbed::Callback<R(A0)> cb) {
			return _queue.event(cb);
		}

		template <typename R, typename A0, typename A1>
		Event<void(A0, A1)> event(mbed::Callback<R(A0, A1)> cb) {
			return _queue.event(cb);
		}
	
		template <typename R, typename A0, typename A1, typename A2>
		Event<void(A0, A1, A2)> event(mbed::Callback<R(A0, A1, A2)> cb) {
			return _queue.event(cb);
		}
		
		template <typename F, typename A0>
		int call(F f, A0 a0) {
			return _queue.call(f, a0);
		}
		
	private:
		IOEventQueue() : _queue(IO_EVENT_QUEUE_SIZE * EVENTS_EVENT_SIZE) {
			_eventThread.start(callback(&_queue, &EventQueue::dispatch_forever));
		}

		~IOEventQueue(){};	
	
		EventQueue _queue;
		Thread _eventThread;

	public:
		// make deleted function public due to better error messages
		IOEventQueue(const IOEventQueue &) = delete;	
		IOEventQueue & operator = (const IOEventQueue &) = delete;
};