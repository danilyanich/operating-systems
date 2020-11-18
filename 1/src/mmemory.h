#pragma once

#define OUT_OF_MEMORY -2
#define WRONG_PARAMETERS -1
#define SUCCESSFUL_CODE 0
#define UNKNOWN_ERROR 1

/**
    address of segment
**/
typedef char *VA;

/**
 	@func	_malloc	
 	@brief	Allocate memory for segment
	
	@param	[out] address address of segment
	@param	[in]  segmentSize	 size of segment
	
	@return	status code
	@retval	0  successful
	@retval	-1 wrong parameters
	@retval	-2 out of memory
	@retval	1  unknown error
 **/
int _malloc(VA *address, size_t segmentSize);


/**
 	@func	_free
 	@brief	Erase segment
	
	@param	[in] address address of segment
	
	@return	status code
	@retval	0  successful
	@retval	-1 wrong parameters
	@retval	1  unknown error
 **/
int _free(VA address);

/**
 	@func	_read
 	@brief	Read information from segment
	
	@param	[in] address	     address of segment
	@param	[in] addressOfBuffer address of buffer for containing
	@param	[in] bufferSize	     size of buffer
	
	@return	status code
	@retval	0  successful
	@retval	-1 wrong parameters
	@retval	-2 access outside the block
	@retval	1  unknown error
 **/
int _read(VA address, void *addressOfBuffer, size_t bufferSize);


/**
 	@func	_write
 	@brief	Write information in segment
	
	@param	[in] address		 address of segment
	@param	[in] addressOfBuffer address of buffer for containing
	@param	[in] bufferSize	     size of buffer
	
	@return	status code
	@retval	0  successful
	@retval	-1 wrong parameters
	@retval	-2 access outside the block
	@retval	1  unknown error
 **/
int _write(VA address, void *addressOfBuffer, size_t bufferSize);


/**
 	@func	_init
 	@brief	Initialization of memory manager
	
	@param	[in] segmentsAmount amount of segment
	@param	[in] segmentSize	size of segment

	@return	status code
	@retval	0  successful
	@retval	-1 wrong parameters
	@retval	1  unknown error
 **/
int _init(int segmentsAmount, int segmentSize);