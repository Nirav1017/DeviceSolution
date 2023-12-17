#include "SerialPort.h"
#include <iostream>
using namespace std;

SerailPort::SerailPort() :
	m_hBulkTransferThreadHandle(NULL),
	m_comportHandle(NULL),
	IsDeviceConnected(false)
{
	
	m_pdatafeedbackCallback = NULL;
}

SerailPort::~SerailPort()
{
	
}

int SerailPort::RegisterCallback(const DATA_FEEDBACK_CALLBACK* pFeedbackCallBack)
{
	int returnValue = COMPORT_SUCCESS;

	m_pdatafeedbackCallback = pFeedbackCallBack;

	return returnValue;
}

int SerailPort::GetComPort(CHAR* pVid, CHAR* pPid, HANDLE& hComHandle)
{
	int retVal = COMPORT_NO_DEVICE;
	char szOutputString[256] = { 0 };
	if (IsDeviceConnected == true)
	{
		return COMPORT_SUCCESS;
	}
	try
	{
		bool bIsDeviceFound = false;
		HDEVINFO DeviceInfoSet;
		DWORD DeviceIndex = 0;
		SP_DEVINFO_DATA DeviceInfoData;

		BYTE szBuffer[1024] = { 0 };
		DEVPROPTYPE ulPropertyType;
		DWORD dwSize = 0;
		DWORD Error = 0;
		char pszPortName[BUFF_LEN] = {};


		//create device hardware id

		//SetupDiGetClassDevs returns a handle to a device information set
		DeviceInfoSet = SetupDiGetClassDevs(
			NULL,
			"USB",
			NULL,
			DIGCF_ALLCLASSES | DIGCF_PRESENT);

		if (DeviceInfoSet == INVALID_HANDLE_VALUE)
		{
			_snprintf_s(szOutputString, sizeof(szOutputString), sizeof(szOutputString),
				"%s - Line# [%d] -INVALID_HANDLE_VALUE ", __FUNCTION__, __LINE__);
			LOG_DEBUG(szOutputString);
			retVal = COMPORT_NO_DEVICE;
			return retVal;
		}

		//Fills a block of memory with zeros
		ZeroMemory(&DeviceInfoData, sizeof(SP_DEVINFO_DATA));
		DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
		//Receive information about an enumerated device
		while (SetupDiEnumDeviceInfo(
			DeviceInfoSet,
			DeviceIndex,
			&DeviceInfoData))
		{
			DeviceIndex++;
			//Retrieves a specified Plug and Play device property
			if (SetupDiGetDeviceRegistryProperty(DeviceInfoSet, &DeviceInfoData, SPDRP_HARDWAREID,
				&ulPropertyType, (BYTE*)szBuffer,
				sizeof(szBuffer),   // The size, in bytes
				&dwSize))
			{
				char szVID_PID_String[64] = "";
				_snprintf_s(szVID_PID_String, sizeof(szVID_PID_String), sizeof(szVID_PID_String), "vid_%s&pid_%s", pVid, pPid);
				_strlwr_s(szVID_PID_String, 64);

				_strlwr_s((CHAR*)szBuffer, sizeof(szBuffer));

				if (strstr((CHAR*)szBuffer, szVID_PID_String))
				{
					HKEY hDeviceRegistryKey;
					//Get the key
					hDeviceRegistryKey = SetupDiOpenDevRegKey(DeviceInfoSet, &DeviceInfoData, DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_READ);
					if (hDeviceRegistryKey == INVALID_HANDLE_VALUE)
					{
						_snprintf_s(szOutputString, sizeof(szOutputString), sizeof(szOutputString),
							"%s - Line# [%d] -Not able to open registry ", __FUNCTION__, __LINE__);
						LOG_DEBUG(szOutputString);
						retVal = COMPORT_NO_DEVICE;
						Error = GetLastError();
						break; //Not able to open registry
					}
					else
					{
						// Read in the name of the port

						DWORD dwSize = sizeof(pszPortName);
						DWORD dwType = 0;
						if ((RegQueryValueEx(hDeviceRegistryKey, "PortName", NULL, &dwType, (LPBYTE)pszPortName, &dwSize) == ERROR_SUCCESS) && (dwType == REG_SZ))
						{
							// Check if it really is a com port
							if (_tcsnicmp(pszPortName, _T("COM"), 3) == 0)
							{
								int nPortNr = _ttoi(pszPortName + 3);
								if (nPortNr != 0)
								{
									bIsDeviceFound = true;
									_snprintf_s(szOutputString, sizeof(szOutputString), sizeof(szOutputString),
										"%s - Line# [%d] -Device found", __FUNCTION__, __LINE__);
									LOG_DEBUG(szOutputString);
									break;
								}
							}
						}
						// Close the key now that we are finished with it
						RegCloseKey(hDeviceRegistryKey);
					}
				}
				else
				{
					_snprintf_s(szOutputString, sizeof(szOutputString), sizeof(szOutputString),
						"%s - Line# [%d] -device vid pid not match [%s] ", __FUNCTION__, __LINE__, (CHAR*)szBuffer);
					LOG_DEBUG(szOutputString);
				}
			}
		}

		if (DeviceInfoSet)
		{
			SetupDiDestroyDeviceInfoList(DeviceInfoSet);
		}

		std::string comPortValue = "\\\\.\\" + std::string(pszPortName);

		if (bIsDeviceFound)
		{
			hComHandle = CreateFile(
				comPortValue.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL,
				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

			if (hComHandle == INVALID_HANDLE_VALUE)
			{
				_snprintf_s(szOutputString, sizeof(szOutputString), sizeof(szOutputString),
					"%s - Line# [%d] -comport handle not create", __FUNCTION__, __LINE__);
				LOG_DEBUG(szOutputString);
				retVal = COMPORT_NO_DEVICE;
			}
			else
			{
				IsDeviceConnected = true;
				m_comportHandle = hComHandle;
				_snprintf_s(szOutputString, sizeof(szOutputString), sizeof(szOutputString),
					"%s - Line# [%d] -comport handle  create", __FUNCTION__, __LINE__);
				LOG_DEBUG(szOutputString);
				retVal = COMPORT_SUCCESS;
			}
		}
	}
	catch (const std::exception& ex)
	{
		_snprintf_s(szOutputString, sizeof(szOutputString), sizeof(szOutputString),
			"%s - Line# [%d] - Exception occurred - [%s].", __FUNCTION__, __LINE__, ex.what());
		LOG_DEBUG(szOutputString);
		retVal = COMPORT_EXECPTION_ACCURED;
	}
	catch (...)
	{
		_snprintf_s(szOutputString, sizeof(szOutputString), sizeof(szOutputString),
			"%s - Line# [%d] - Exception occurred .", __FUNCTION__, __LINE__);
		LOG_DEBUG(szOutputString);
		retVal = COMPORT_EXECPTION_ACCURED;
	}
	return retVal;
}

int SerailPort::DisConnectedDevice(HANDLE& hComHandle)
{
	int retVal = COMPORT_SUCCESS;
	char szOutputString[256] = { 0 };
	try 
	{
		if (m_hBulkTransferThreadHandle)
		{
			DWORD dwWaitResult = WaitForSingleObject(m_hBulkTransferThreadHandle, 500);
			if (WAIT_OBJECT_0 == dwWaitResult)
			{
				CloseHandle(m_hBulkTransferThreadHandle);
				m_hBulkTransferThreadHandle = NULL;
			}
		}
		bool ret = CloseHandle(hComHandle);
		if (ret == true)
		{
			_snprintf_s(szOutputString, sizeof(szOutputString), sizeof(szOutputString),
				"%s - Line# [%d] -comport handle close", __FUNCTION__, __LINE__);
			LOG_DEBUG(szOutputString);
			IsDeviceConnected = false;
			retVal = COMPORT_SUCCESS;
		}
	}
	catch (const std::exception& ex)
	{
		_snprintf_s(szOutputString, sizeof(szOutputString), sizeof(szOutputString),
			"%s - Line# [%d] - Exception occurred - [%s].", __FUNCTION__, __LINE__, ex.what());
		LOG_DEBUG(szOutputString);
		retVal = COMPORT_EXECPTION_ACCURED;
	}
	catch (...)
	{
		_snprintf_s(szOutputString, sizeof(szOutputString), sizeof(szOutputString),
			"%s - Line# [%d] - Exception occurred .", __FUNCTION__, __LINE__);
		LOG_DEBUG(szOutputString);
		retVal = COMPORT_EXECPTION_ACCURED;
	}
	return retVal;
}

int SerailPort::WriteComPort(HANDLE& hndl, char* pCmd, int pCmdSize)
{
	DCB dcb;
	DWORD byteswritten;

	if (!GetCommState(hndl, &dcb))
		return false;

	dcb.BaudRate = CBR_115200; //9600 Baud
	dcb.ByteSize = 8; //8 data bits
	dcb.Parity = NOPARITY; //no parity
	dcb.StopBits = ONESTOPBIT; //1 stop

	if (!SetCommState(hndl, &dcb))
		return COMPORT_WRITE_FAILED;

	BOOL bWriteStatus = WriteFile(hndl, pCmd, pCmdSize, &byteswritten, NULL);

	return (bWriteStatus ? COMPORT_SUCCESS : COMPORT_WRITE_FAILED);
}

int SerailPort::ReadComPort(HANDLE& hndl, char* pOutBuff, int iNumByteToRead)
{
	DCB dcb;

	DWORD dwBytesTransferred = 0;

	if (!GetCommState(hndl, &dcb))
		return COMPORT_READ_FAILED;

	dcb.BaudRate = CBR_115200; //9600 Baud
	dcb.ByteSize = 8; //8 data bits
	dcb.Parity = NOPARITY; //no parity
	dcb.StopBits = ONESTOPBIT; //1 stop

	if (!SetCommState(hndl, &dcb))
		return COMPORT_READ_FAILED;

	BOOL bReadStatus = ReadFile(hndl, pOutBuff, iNumByteToRead, &dwBytesTransferred, 0); //read 1

	return (bReadStatus ? COMPORT_SUCCESS : COMPORT_READ_FAILED);
}

int SerailPort::WriteReadData()
{
	int returnValue = COMPORT_SUCCESS;
	char szOutputString[256] = { 0 };
	char* Bufferdata = NULL;
	try
	{
		while (IsDeviceConnected)
		{
		/*	static char kChipUniqueID[] = "40";
			returnValue = WriteComPort(m_comportHandle, kChipUniqueID, (int)(strlen(kChipUniqueID)));*/
			BYTE command = 0x40;
			DWORD bytesWritten, byteread;
			::Sleep(100);
			//bool set = WriteFile(m_comportHandle, &command, 1, &bytesWritten, NULL);
			if (m_comportHandle)
			{
				if (WriteFile(m_comportHandle, &command, 1, &bytesWritten, NULL))
				{
					_snprintf_s(szOutputString, sizeof(szOutputString), sizeof(szOutputString),
						"%s - Line# [%d] -Write command success [%d]", __FUNCTION__, __LINE__, bytesWritten);
					LOG_DEBUG(szOutputString);
					Bufferdata = (char*)calloc(READ_DATA_LEN, sizeof(char));
					//returnValue = ReadComPort(m_comportHandle, (char*)Bufferdata, READ_DATA_LEN);			
					if (ReadFile(m_comportHandle, Bufferdata, READ_DATA_LEN, &byteread, NULL))
					{
						_snprintf_s(szOutputString, sizeof(szOutputString), sizeof(szOutputString),
							"%s - Line# [%d] -Read Data success [%d] - [%s]", __FUNCTION__, __LINE__, byteread, Bufferdata);
						LOG_DEBUG(szOutputString);
						(*m_pdatafeedbackCallback)(COMPORT_SUCCESS, Bufferdata, READ_DATA_LEN);

					}
					else
					{
						_snprintf_s(szOutputString, sizeof(szOutputString), sizeof(szOutputString),
							"%s - Line# [%d] -Read Data fail [%d] - [%s]", __FUNCTION__, __LINE__, byteread, Bufferdata);
						LOG_DEBUG(szOutputString);
						(*m_pdatafeedbackCallback)(COMPORT_READ_FAILED, Bufferdata, READ_DATA_LEN);
					}
				}
				else
				{
					_snprintf_s(szOutputString, sizeof(szOutputString), sizeof(szOutputString),
						"%s - Line# [%d] -Write command fail [%d]", __FUNCTION__, __LINE__, bytesWritten);
					LOG_DEBUG(szOutputString);
					returnValue = COMPORT_WRITE_FAILED;
				}
			}
			else
			{
				_snprintf_s(szOutputString, sizeof(szOutputString), sizeof(szOutputString),
					"%s - Line# [%d] -Device Handle not Found ", __FUNCTION__, __LINE__);
				LOG_DEBUG(szOutputString);
				returnValue = COMPORT_WRITE_FAILED;
			}
		}
	}
	catch (const std::exception& ex)
	{
		_snprintf_s(szOutputString, sizeof(szOutputString), sizeof(szOutputString),
			"%s - Line# [%d] - Exception occurred - [%s].", __FUNCTION__, __LINE__, ex.what());
		LOG_DEBUG(szOutputString);
		returnValue = COMPORT_EXECPTION_ACCURED;
	}
	catch (...)
	{
		_snprintf_s(szOutputString, sizeof(szOutputString), sizeof(szOutputString),
			"%s - Line# [%d] - Exception occurred .", __FUNCTION__, __LINE__);
		LOG_DEBUG(szOutputString);
		returnValue = COMPORT_EXECPTION_ACCURED;
	}

	return returnValue;
}

DWORD WINAPI SerailPort::BulkTransferThreadFunction(LPVOID lpParam)
{
	unsigned int uRetVal = 0;
	char szOutputString[256] = { 0 };
	try
	{
		SerailPort* pMBSDeviceHandlerCore = reinterpret_cast<SerailPort*> (lpParam);
		if (pMBSDeviceHandlerCore)
		{
			uRetVal = pMBSDeviceHandlerCore->WriteReadData();
		}
	}
	catch (const std::exception& ex)
	{
		_snprintf_s(szOutputString, sizeof(szOutputString), sizeof(szOutputString),
			"%s - Line# [%d] - Exception occurred - [%s].", __FUNCTION__, __LINE__, ex.what());
		LOG_DEBUG(szOutputString);
		uRetVal = COMPORT_EXECPTION_ACCURED;
	}
	catch (...)
	{
		_snprintf_s(szOutputString, sizeof(szOutputString), sizeof(szOutputString),
			"%s - Line# [%d] - Exception occurred .", __FUNCTION__, __LINE__);
		LOG_DEBUG(szOutputString);
		uRetVal = COMPORT_EXECPTION_ACCURED;
	}
	return uRetVal;
}

int SerailPort::DataPreview()
{
	int returnValue = COMPORT_SUCCESS;
	char szOutputString[256] = { 0 };
	try {
		DWORD dwBulkTransferThreadID = 0;

		m_hBulkTransferThreadHandle = ::CreateThread(NULL, 0, &BulkTransferThreadFunction, this, 0, &dwBulkTransferThreadID);

		if (NULL == m_hBulkTransferThreadHandle)
		{
			_snprintf_s(szOutputString, sizeof(szOutputString), sizeof(szOutputString),
				"%s - Line# [%d] -COMPORT_MEMORY_INTILIZE_FAIL", __FUNCTION__, __LINE__);
			LOG_DEBUG(szOutputString);
			returnValue = COMPORT_MEMORY_INTILIZE_FAIL;
		}
	}
	catch (const std::exception& ex)
	{
		_snprintf_s(szOutputString, sizeof(szOutputString), sizeof(szOutputString),
			"%s - Line# [%d] - Exception occurred - [%s].", __FUNCTION__, __LINE__, ex.what());
		LOG_DEBUG(szOutputString);
		returnValue = COMPORT_EXECPTION_ACCURED;
	}
	catch (...)
	{
		_snprintf_s(szOutputString, sizeof(szOutputString), sizeof(szOutputString),
			"%s - Line# [%d] - Exception occurred .", __FUNCTION__, __LINE__);
		LOG_DEBUG(szOutputString);
		returnValue = COMPORT_EXECPTION_ACCURED;
	}
	return returnValue;
}

