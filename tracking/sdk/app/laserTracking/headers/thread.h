
#pragma once

#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <process.h>


template <class TargetClass, typename DataType> 
struct generic_thread_data { 
	TargetClass *self; 
	void (TargetClass::*func)(DataType*); 
	DataType *data; 
}; 

template <class TargetClass, typename DataType> 
void generic_thread_starter(void *ptr) { 
	generic_thread_data<TargetClass, DataType> *data = (generic_thread_data<TargetClass, DataType> *)ptr; 
	((data->self)->*(data->func))(data->data); delete data; 
} 

template <class TargetClass, typename DataType> uintptr_t begin_thread_on_object(TargetClass *obj, void (TargetClass::*func)(DataType*), DataType *data) { 
	generic_thread_data<TargetClass, DataType> *generic_data = new generic_thread_data<TargetClass, DataType>(); 
	generic_data->self = obj; 
	generic_data->func = func; 
	generic_data->data = data; 
	return _beginthread(generic_thread_starter<TargetClass, DataType>, 0, generic_data); 
}

/*
	The Runnable interface is an abstraction level for using a thread.
	Every Thread has to be initialized with an instance of the Runnable interface.
	in the run() method should be implemented the runnable code.
*/

class Runnable {
public:
	virtual void run() = 0;
};

/*
	The Thread class is for running a task in the background.
	It has to be initialized with an instance of a class tha inherits from the Runnable interface.
	And with the method start(), the runnable code from the Runnable interface is beeing executed in the background.
*/

class Thread {
	Runnable *runnable;
	static LPDWORD dwThreadID;
	static Runnable *tempRunnable;
	HANDLE hThread;

public:
	Thread(Runnable *runnable) {
		this->runnable = runnable;
	}
	void start() {
		begin_thread_on_object(this, &Thread::run, runnable);
	}

	void stop() {
		_endthread();
	}

	static Runnable *getRunnable() {
		return tempRunnable;
	}

	void run(Runnable* r) {
		r->run();
	}
};

