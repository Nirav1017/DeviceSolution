#pragma once

#define WAIT_TIME 2000
#define INPUTDATA 76

#include<Windows.h>
#include<stdlib.h>
#include<stdio.h>
#include <Setupapi.h>
#include <tchar.h>
#include <initguid.h>
#include <wchar.h>
#include "Comport.h"
#include "Logger.h"
using namespace Logging;
#pragma comment (lib, "Setupapi.lib")
#define BUFF_LEN 20
#define READ_DATA_LEN                 76		///< Length in Bytes.
class  SerailPort
{
public:
	// SerailPort(char *portname);
	 SerailPort();
	~ SerailPort();

	int RegisterCallback(const DATA_FEEDBACK_CALLBACK* pFeedbackCallBack);
	int GetComPort(CHAR* pVid, CHAR* pPid, HANDLE& hComHandle);
	int DisConnectedDevice(HANDLE& hComHandle);
	int WriteComPort(HANDLE& hndl, char* pCmd, int pCmdSize);
	int ReadComPort(HANDLE& hndl, char* pOutBuff, int iNumByteToRead);
	int WriteReadData();
	int DataPreview();
private:
	bool IsDeviceConnected;
	HANDLE m_comportHandle;
	HANDLE  m_hBulkTransferThreadHandle;
	static DWORD WINAPI BulkTransferThreadFunction(LPVOID lpParam);
	const DATA_FEEDBACK_CALLBACK* m_pdatafeedbackCallback;
};

