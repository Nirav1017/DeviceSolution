#include <windows.h>
#include "Comport.h"
#include"SerialPort.h"


SerailPort *gpSerialPort = NULL;


int comportInit()
{
    int returnValue = COMPORT_SUCCESS;
    char szOutputString[256] = { 0 };
    delete gpSerialPort;
    gpSerialPort = NULL;

    try
    {
        gpSerialPort = new SerailPort();
        LOG_DEBUG("*~*~*~*~*~*~*~*~*~*~*~*~*~*~* Library initialized  *~*~*~*~*~*~*~*~*~*~*~*~*~*~*");
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

int IsDeviceConnected(CHAR* pVid, CHAR* pPid, HANDLE& hComHandle)
{
    int returnValue = COMPORT_NO_DEVICE;

    returnValue = gpSerialPort->GetComPort(pVid, pPid, hComHandle);
    if (returnValue == COMPORT_SUCCESS)
    {         
        return COMPORT_SUCCESS;
    }
    else
    {      
        return COMPORT_NO_DEVICE;
    } 
}

int ComportDeviceDisConnect(HANDLE& hComHandle)
{
    int returnValue = COMPORT_NO_DEVICE;

    returnValue = gpSerialPort->DisConnectedDevice(hComHandle);

    if (returnValue == COMPORT_SUCCESS)
    {
        return COMPORT_SUCCESS;
    }
    else
    {
        return COMPORT_NO_DEVICE;
    }
}

int startdatapreview()
{
    int returnValue = COMPORT_INVALID_HANDLE;

    if (gpSerialPort)
        returnValue = gpSerialPort->DataPreview();
    else
        returnValue = COMPORT_INVALID_HANDLE;
    
    return returnValue;
}

int CommRegisterCallback(const DATA_FEEDBACK_CALLBACK* pEventCallBack)
{
    int returnValue = COMPORT_SUCCESS;

    if (!pEventCallBack)
    {
        returnValue = COMPORT_INVALID_INPUT_PARAM;
    }
    else
    {
        if (gpSerialPort)
            returnValue = gpSerialPort->RegisterCallback(pEventCallBack);
        else
            returnValue = COMPORT_INVALID_HANDLE;
    }

    return returnValue;
}

int PortEnableLogs(const PORT_LOG_LEVEL eLogLevel, const char* pDirectoryPath)
{
    int returnValue = COMPORT_SUCCESS;

    LogLevel eLogLevelInternal = LOG_LEVEL_ERROR;
    switch (eLogLevel)
    {
    case LEVEL_LOG_ON:
        eLogLevelInternal = LOG_LEVEL_DEBUG;
        break;
    }

    if (eLogLevel != LEVEL_LOG_OFF)
    {
        if (pDirectoryPath != NULL)
        {
            LOG_DEBUG("File opened successfully on given directory path.");
        }

        CLogger::getInstance()->updateLogLevel(eLogLevelInternal);
        LOG_SETPATH(pDirectoryPath);
    }
    else
    {
        LOG_DEBUG("File logging will be disabled now.");

        CLogger::getInstance()->disableLog();
    }


    return returnValue;
}

