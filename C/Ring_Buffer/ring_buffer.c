/*
------------------------
Made by Damian Chor¹¿y.
Created 02.2019
Last change 26.03.2019
------------------------

Simple Ring Buffer initialized via RingBuffer_Init(), with dynamical-allocated memory.

TODO:
	-check all sizeof(char) and edit it to make more elastic
	-Look at FirstData=-1; it'll be better if you make "Max_Size+1"
	-Beautify?
	-Make some better comments
*/


/* Includes ------------------------------------------------------------------*/
#include <assert.h>
#include "ring_buffer.h"
#include <stdlib.h>


bool RingBuffer_Init(RingBuffer *ringBuffer, char *dataBuffer, size_t dataBufferSize)
{
	assert(ringBuffer);
	assert(dataBuffer);
	assert(dataBufferSize > 0);

	if ((ringBuffer) && (dataBuffer) && (dataBufferSize > 0)) {
	
		ringBuffer->dataBufferSize = dataBufferSize;
		ringBuffer->dataBuffer = (char*)malloc(ringBuffer->dataBufferSize * sizeof(char));
		ringBuffer->increment = 0;
		ringBuffer->FirstData = -1;

		assert(ringBuffer->dataBuffer);

		return true;
	}

	return false;
}

bool RingBuffer_Clear(RingBuffer *ringBuffer)
{
	assert(ringBuffer);

	if (ringBuffer) {
		ringBuffer->FirstData = -1;
		ringBuffer->increment = 0; // just make default values.
		return true;
	}
	return false;
}

bool RingBuffer_IsEmpty(const RingBuffer *ringBuffer)
{
	assert(ringBuffer);
	if (RingBuffer_GetLen(ringBuffer) != 0) { return false; }
	else {
		return true;
	}
}

size_t RingBuffer_GetLen(const RingBuffer *ringBuffer)
{
	assert(ringBuffer);

	if (ringBuffer) {
		if (ringBuffer->increment > ringBuffer->FirstData) {  // normal state, without recyrcling
			return (ringBuffer->increment - ringBuffer->FirstData) * sizeof(char);
		}
		else if (ringBuffer->FirstData < ringBuffer->dataBufferSize) { // when recyrcled
			return (ringBuffer->dataBufferSize - (ringBuffer->FirstData - ringBuffer->increment)) * sizeof(char);
		}
	}
	return 0; // initial state, there is no first data.

}

size_t RingBuffer_GetCapacity(const RingBuffer *ringBuffer)
{
	assert(ringBuffer);

	if (ringBuffer) {
		return ringBuffer->dataBufferSize * sizeof(char);
	}
	return 0;
}


bool RingBuffer_PutChar(RingBuffer *ringBuffer, char c)
{
	assert(ringBuffer);

	if (ringBuffer) {

		if (RingBuffer_GetLen(ringBuffer) == RingBuffer_GetCapacity(ringBuffer)) { return false; } // when buffor is full
		if (ringBuffer->increment == ringBuffer->dataBufferSize) { ringBuffer->increment = 0; } // recyrcling

		ringBuffer->dataBuffer[(ringBuffer->increment)] = c;  // putting data into buffer
		ringBuffer->increment = ringBuffer->increment + sizeof(char); // increment ringbuffer data pointer;

		if (ringBuffer->FirstData >= ringBuffer->dataBufferSize) { ringBuffer->FirstData = 0; } // setting the Firstdata if unset
		return true;
	}
	return false;
}

bool RingBuffer_GetChar(RingBuffer *ringBuffer, char *c)
{
	assert(ringBuffer);
	assert(c);

	if ((ringBuffer) && (c)) {
		if (RingBuffer_GetLen(ringBuffer) == 0) {  // If buffor is empty, return false
			return false;
		}
		else {

			if (ringBuffer->FirstData >= ringBuffer->dataBufferSize) { ringBuffer->FirstData = 0; } // after recyrculation, first_data pointer
			*c = ringBuffer->dataBuffer[ringBuffer->FirstData++];

			return true;
		}
	}
	return false;
}

