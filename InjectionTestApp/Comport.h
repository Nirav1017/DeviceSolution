
#pragma once
#ifndef __COMPORT_LIB_H__
#define __COMPORT_LIB_H__

#ifdef __cplusplus 
extern "C" {
#endif 

#define COMPORT_LIB __declspec(dllexport)

	typedef enum _PORT_LOG_LEVEL
	{
		/** log level 0 - no logs - default */
		LEVEL_LOG_OFF = 0,
		/** log level 1 - for error messages */
		LEVEL_LOG_ON = 1,
	} PORT_LOG_LEVEL;

	typedef enum _COMPORT_LIB_LIB_ERROR_CODE_ {
		COMPORT_SUCCESS = 0,
		COMPORT_NO_DEVICE = -101,
		COMPORT_FAILED_TO_GET_COM_PORT_STATE = -102,//Failed to get COM port state
		COMPORT_READ_FAILED = -103,
		COMPORT_DATA_NOT_FOUND = -104,
		COMPORT_MEMORY_INTILIZE_FAIL = -105,
		COMPORT_EXECPTION_ACCURED = -106,
		COMPORT_WRITE_FAILED = -107,
		COMPORT_INVALID_HANDLE = -108,
		COMPORT_INVALID_INPUT_PARAM = -109

	}COMPORT_LIB_LIB_ERROR_CODE;

	typedef void(*DATA_FEEDBACK_CALLBACK)(int nErrorCode, char* buffer, unsigned int buff_size);

	COMPORT_LIB int comportInit();

	COMPORT_LIB int IsDeviceConnected(CHAR* pVid, CHAR* pPid, HANDLE& hComHandle);
	
	COMPORT_LIB int ComportDeviceDisConnect(HANDLE& hComHandle);

	COMPORT_LIB int startdatapreview();

	COMPORT_LIB int CommRegisterCallback(const DATA_FEEDBACK_CALLBACK* pEventCallBack);
	
	COMPORT_LIB int PortEnableLogs(const PORT_LOG_LEVEL eLogLevel, const char* pDirectoryPath);

#ifdef __cplusplus 
}
#endif 

#endif
