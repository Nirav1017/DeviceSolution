
// InjectionTestAppDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "InjectionTestApp.h"
#include "InjectionTestAppDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About
#define WM_IMAGE_PREVIEW        WM_USER + 2000
std::ofstream m_CSVFile;
static int EnabledisableCnt = 0;
CInjectionTestAppDlg* myDilogObj;
static char VID_STR[] = "2047";//"0483";				///< MFS110 OEM Vendor ID.
static char PID_STR[] = "03F0";//"5740";
float out_value;
CString newValueStr, formatString;
bool isPhasefalut = false;
bool istripdata = false;
bool isOvFault = false;
bool isOCFault = false;
bool istripFault = false;
bool isCageDoorFault = false;
bool isEarthFault = false;
bool isHTflag = false;
bool IsWriteFile = FALSE;
static int htcnt = 0;
void  CommEventCallback(int nErrorCode, char* buffer, unsigned int buff_size);
DWORD WINAPI WriteDataThread(LPVOID lpParam);

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CInjectionTestAppDlg dialog



CInjectionTestAppDlg::CInjectionTestAppDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_INJECTIONTESTAPP_DIALOG, pParent)
	, m_intrvealvalue(1)
	, m_filepath(_T(""))
	, m_ACVoltageValue0(_T(""))
{
	hHandle = NULL;
	myDilogObj = this;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CInjectionTestAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_APP_NAME, m_testappname);
	DDX_Control(pDX, IDC_EDIT_AC_VOLTAGE_0, m_editAcVoltage0);
	DDX_Control(pDX, IDC_EDIT_AC_VOLTAGE_1, m_editACVoltage1);
	DDX_Control(pDX, IDC_EDIT_IP_FREQ, m_editIpFreq);
	DDX_Control(pDX, IDC_EDIT_OP_FREQ, m_editoutfreq);
	DDX_Control(pDX, IDC_EDIT_AC_CURRENT_0, m_editAccurrent0);
	DDX_Control(pDX, IDC_EDIT_AC_CURRENT_1, m_editAcCurrent1);
	DDX_Control(pDX, IDC_EDIT_AC_CURRENT_2, m_editACCurrent2);
	DDX_Control(pDX, IDC_EDIT_DC_VOLTAGE_0, m_editDCVoltage0);
	DDX_Control(pDX, IDC_EDIT_DC_VOLTAGE_1, m_editDCVoltage1);
	DDX_Control(pDX, IDC_EDIT_PHASE_ANGLE, m_editphaseangle);
	DDX_Control(pDX, IDC_EDIT_POWER_FACTOR, m_editpowerfactor);
	DDX_Control(pDX, IDC_EDIT_TRIP, m_edittripdata);
	DDX_Control(pDX, IDC_EDIT_KILOWATT, m_editkilowatt);
	DDX_Control(pDX, IDC_EDIT_RUN_TIME, m_edittimervalue);
	DDX_Control(pDX, IDC_EDIT_INTERVAl, m_editintrveal);
	DDX_Text(pDX, IDC_EDIT_INTERVAl, m_intrvealvalue);
	DDX_Control(pDX, IDC_EDIT_SAVE_AS, m_editfilepath);
	DDX_Text(pDX, IDC_EDIT_SAVE_AS, m_filepath);
	DDX_Text(pDX, IDC_EDIT_AC_VOLTAGE_0, m_ACVoltageValue0);
	DDX_Control(pDX, IDC_STATIC_HT_FLAG_P, m_HtflagP);
	DDX_Control(pDX, IDC_STATIC_LOG_STATUS_P, m_logstatus);
	DDX_Control(pDX, IDC_STATIC_PHASE_P, m_Phase);
	DDX_Control(pDX, IDC_STATIC_OV_P, m_ovstatus);
	DDX_Control(pDX, IDC_STATIC_OL_OC_P, m_olstatus);
	DDX_Control(pDX, IDC_STATIC_EARTH_P, m_earthstatus);
	DDX_Control(pDX, IDC_STATIC_DOOR_P, m_doorstatus);
	DDX_Control(pDX, IDC_STATIC_TRIP_P, m_tripstatus);
	DDX_Control(pDX, IDC_STATIC_VOLTAGE_0, m_SaticText[0]);
	DDX_Control(pDX, IDC_STATIC_VOLTAGE_1, m_SaticText[1]);
	DDX_Control(pDX, IDC_STATIC_IP_FREQ, m_SaticText[2]);
	DDX_Control(pDX, IDC_STATIC_OP_FREQ, m_SaticText[3]);
	DDX_Control(pDX, IDC_STATIC_AC_VOLTS_0, m_SaticText[4]);
	DDX_Control(pDX, IDC_STATIC_AC_VOLTS_1, m_SaticText[5]);
	DDX_Control(pDX, IDC_STATIC_IP_FREQ_TEXT, m_SaticText[6]);
	DDX_Control(pDX, IDC_STATIC_IP_FREQ_TEXT2, m_SaticText[7]);
	DDX_Control(pDX, IDC_STATIC_AC_VOLTAGE_FREQ, m_SaticText[8]);
	DDX_Control(pDX, IDC_STATIC_AC_CURRENT, m_SaticText[9]);
	DDX_Control(pDX, IDC_STATIC_AC_CURRENT_0, m_SaticText[10]);
	DDX_Control(pDX, IDC_STATIC_AC_CURRENT_1, m_SaticText[11]);
	DDX_Control(pDX, IDC_STATIC_AC_CURRENT_2, m_SaticText[12]);
	DDX_Control(pDX, IDC_STATIC_AMPS_0, m_SaticText[13]);
	DDX_Control(pDX, IDC_STATIC_AMPS_1, m_SaticText[14]);
	DDX_Control(pDX, IDC_STATIC_AMPS_2, m_SaticText[15]);
	DDX_Control(pDX, IDC_STATIC_DC_VOLTAGE, m_SaticText[16]);
	DDX_Control(pDX, IDC_STATIC_DC_VOLTAGE_0, m_SaticText[17]);
	DDX_Control(pDX, IDC_STATIC_DC_VOLTAGE_1, m_SaticText[18]);
	DDX_Control(pDX, IDC_STATIC_RUNTIME, m_SaticText[19]);
	DDX_Control(pDX, IDC_STATIC_DC_VOLTS_0, m_SaticText[20]);
	DDX_Control(pDX, IDC_STATIC_DC_VOLTS_1, m_SaticText[21]);
	DDX_Control(pDX, IDC_STATIC_TIME, m_SaticText[22]);
	DDX_Control(pDX, IDC_STATIC_STATUS, m_SaticText[23]);
	DDX_Control(pDX, IDC_STATIC_APP_NAME, m_testappname);
	DDX_Control(pDX, IDC_STATIC_PHASE_ANGLE, m_SaticText[24]);
	DDX_Control(pDX, IDC_STATIC_POWER_FACTOR, m_SaticText[25]);
	DDX_Control(pDX, IDC_STATIC_TRIP, m_SaticText[26]);
	DDX_Control(pDX, IDC_STATIC_KILOWATT, m_SaticText[27]);
	DDX_Control(pDX, IDC_STATIC_DEG, m_SaticText[28]);
	DDX_Control(pDX, IDC_STATIC_TRIP_TEXT, m_SaticText[29]);
	DDX_Control(pDX, IDC_STATIC_KW, m_SaticText[30]);	
	DDX_Control(pDX, IDC_STATIC_LOGS, m_SaticText[31]);
	DDX_Control(pDX, IDC_STATIC_INTERVAL, m_SaticText[32]);
	DDX_Control(pDX, IDC_STATIC_SAVE_AS, m_SaticText[33]);
	DDX_Control(pDX, IDC_STATIC_SECS, m_SaticText[34]);
	DDX_Control(pDX, IDC_STATIC_LOG_STATUS, m_SaticText[35]);
	DDX_Control(pDX, IDC_STATIC_PHASE, m_SaticText[36]);
	DDX_Control(pDX, IDC_STATIC_OV, m_SaticText[37]);
	DDX_Control(pDX, IDC_STATIC_OC_OL, m_SaticText[38]);
	DDX_Control(pDX, IDC_STATIC_EARTH, m_SaticText[39]);
	DDX_Control(pDX, IDC_STATIC_DOOR, m_SaticText[40]);
	DDX_Control(pDX, IDC_STATIC_TRIP_S, m_SaticText[41]);
	DDX_Control(pDX, IDC_STATIC_HT_FLAG, m_SaticText[42]);
	DDX_Control(pDX, IDC_STATIC_FAULTS, m_SaticText[43]);

	DDX_Control(pDX, IDC_STATIC_AC_VOLTAGE_VALUE_0, m_SaticText[44]);
	DDX_Control(pDX, IDC_STATIC_AC_VOLTAGE_VALUE_1, m_SaticText[45]);
	DDX_Control(pDX, IDC_STATIC_IP_FREQ_VALUE, m_SaticText[46]);
	DDX_Control(pDX, IDC_STATIC_OP_FREQ_VALUE, m_SaticText[47]);
	DDX_Control(pDX, IDC_STATIC_DC_VOLTAGE_VALUE_0, m_SaticText[48]);
	DDX_Control(pDX, IDC_STATIC_DC_VOLTAGE_VALUE_1, m_SaticText[49]);
	DDX_Control(pDX, IDC_STATIC_TIMER_VALUE, m_SaticText[50]);
	DDX_Control(pDX, IDC_STATIC_AC_CURRENT_VALUE_0, m_SaticText[51]);
	DDX_Control(pDX, IDC_STATIC_AC_CURRENT_VALUE_1, m_SaticText[52]);
	DDX_Control(pDX, IDC_STATIC_AC_CURRENT_VALUE_2, m_SaticText[53]);
	DDX_Control(pDX, IDC_STATIC_PHASE_ANGLE_VALUE, m_SaticText[54]);
	DDX_Control(pDX, IDC_STATIC_POWER_FACTOR_VALUE, m_SaticText[55]);
	DDX_Control(pDX, IDC_STATIC_TRIP_VALUE, m_SaticText[56]);
	DDX_Control(pDX, IDC_STATIC_KILOWATT_VALUE, m_SaticText[57]);

	DDX_Control(pDX, IDC_BUTTON_LOG_ENABLE, m_btnenable);
}

BEGIN_MESSAGE_MAP(CInjectionTestAppDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_DEVICE_CONNECT, &CInjectionTestAppDlg::OnBnClickedButtonDeviceConnect)
	ON_BN_CLICKED(IDC_BUTTON_DISCONNECT_DEVICE, &CInjectionTestAppDlg::OnBnClickedButtonDisconnectDevice)
	ON_BN_CLICKED(IDC_BUTTON_LOG_ENABLE, &CInjectionTestAppDlg::OnBnClickedButtonLogEnable)
	ON_BN_CLICKED(IDC_BUTTON_CREATE_LOG_FILE, &CInjectionTestAppDlg::OnBnClickedButtonCreateLogFile)
	ON_MESSAGE(WM_IMAGE_PREVIEW, &CInjectionTestAppDlg::OnMessageImagePreview)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CInjectionTestAppDlg message handlers

BOOL CInjectionTestAppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	PortEnableLogs(LEVEL_LOG_ON, "");
	// TODO: Add extra initialization here
	int ret = comportInit();
	SetWindowBackGroundColor();
	if (ret != COMPORT_SUCCESS)
	{
		AfxMessageBox(L"Device was not init");
	}

	m_testappname.SetWindowTextW(L"Test Application");

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CInjectionTestAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CInjectionTestAppDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CInjectionTestAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CInjectionTestAppDlg::SetReadData(char* input_data, comm_tx_db_t* pcomm_tx_db)
{
#define SLAVE_ADDRESS            (0x40)

	//char input_data[76] = { 0x40,0x40,0x00,0x00,0x1F,0x00,0x03,0x11,0xC0,0x12,0x00,0x00,0xFF,0x01,0x00,0x00,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xB6,0x04,0x00,0x00,0xCD,0x09,0x00,0x00,0xF3,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x02,0x02,0x00,0x00,0x01,0x00,0x00,0x01,0x00,0x00 };
	//check if received data byte matches slave address
	if (SLAVE_ADDRESS == input_data[0])
	{
		pcomm_tx_db->dev_address = (SLAVE_ADDRESS) | (SLAVE_ADDRESS << 8);

		//Compute values
		pcomm_tx_db->status_db.gen_faults = input_data[4];
		pcomm_tx_db->status_db.phase_fault = input_data[5];
		pcomm_tx_db->status_db.trip_data = input_data[6];
		pcomm_tx_db->status_db.ht_flag = input_data[7];

		//comm_tx_db.timer_val = system_timer.act_timer | (((uint32_t)system_timer.temp) << 16); // this code does not run correctly

		/*input_data[8] = 0xc0;
		input_data[9] = 0x12;
		input_data[11] = 0x00;
		input_data[10] = 0x00;*/
		//0x000012C0
		//0x000076A1
		pcomm_tx_db->timer_val = ((input_data[9] & 0xFF) << 8) | (input_data[8] & 0xFF);
		pcomm_tx_db->timer_val <<= 16;
		pcomm_tx_db->timer_val |= ((input_data[11] & 0xFF) << 8) | (input_data[10] & 0xFF);

		pcomm_tx_db->voltage1 = ((input_data[15] & 0xFF) << 24) | ((input_data[14] & 0xFF) << 16) | ((input_data[13] & 0xFF) << 8) | (input_data[12] & 0xFF);
		pcomm_tx_db->voltage2 = ((input_data[19] & 0xFF) << 24) | ((input_data[18] & 0xFF) << 16) | ((input_data[17] & 0xFF) << 8) | (input_data[16] & 0xFF);

		pcomm_tx_db->input_freq = ((input_data[23] & 0xFF) << 24) | ((input_data[22] & 0xFF) << 16) | ((input_data[21] & 0xFF) << 8) | (input_data[20] & 0xFF);
		pcomm_tx_db->output_freq = ((input_data[27] & 0xFF) << 24) | ((input_data[26] & 0xFF) << 16) | ((input_data[25] & 0xFF) << 8) | (input_data[24] & 0xFF);

		pcomm_tx_db->current1 = ((input_data[31] & 0xFF) << 24) | ((input_data[30] & 0xFF) << 16) | ((input_data[29] & 0xFF) << 8) | (input_data[28] & 0xFF);
		pcomm_tx_db->current2 = ((input_data[35] & 0xFF) << 24) | ((input_data[34] & 0xFF) << 16) | ((input_data[33] & 0xFF) << 8) | (input_data[32] & 0xFF);
		pcomm_tx_db->current3 = ((input_data[39] & 0xFF) << 24) | ((input_data[38] & 0xFF) << 16) | ((input_data[37] & 0xFF) << 8) | (input_data[36] & 0xFF);

		pcomm_tx_db->dc_chan1 = ((input_data[43] & 0xFF) << 24) | ((input_data[42] & 0xFF) << 16) | ((input_data[41] & 0xFF) << 8) | (input_data[40] & 0xFF);
		pcomm_tx_db->dc_chan2 = ((input_data[47] & 0xFF) << 24) | ((input_data[46] & 0xFF) << 16) | ((input_data[45] & 0xFF) << 8) | (input_data[44] & 0xFF);

		pcomm_tx_db->phase_angle = ((input_data[51] & 0xFF) << 24) | ((input_data[50] & 0xFF) << 16) | ((input_data[49] & 0xFF) << 8) | (input_data[48] & 0xFF);
		pcomm_tx_db->power_factor = ((input_data[55] & 0xFF) << 24) | ((input_data[54] & 0xFF) << 16) | ((input_data[53] & 0xFF) << 8) | (input_data[52] & 0xFF);
		pcomm_tx_db->power_kw = ((input_data[59] & 0xFF) << 24) | ((input_data[58] & 0xFF) << 16) | ((input_data[57] & 0xFF) << 8) | (input_data[56] & 0xFF);
		pcomm_tx_db->model_no = ((input_data[63] & 0xFF) << 24) | ((input_data[62] & 0xFF) << 16) | ((input_data[61] & 0xFF) << 8) | (input_data[60] & 0xFF);//81; // for PCITS4D-3P

		pcomm_tx_db->dp_db.voltage1 = input_data[64];
		pcomm_tx_db->dp_db.voltage2 = input_data[65];

		pcomm_tx_db->dp_db.input_freq = input_data[66];
		pcomm_tx_db->dp_db.output_freq = input_data[67];

		pcomm_tx_db->dp_db.current1 = input_data[68];
		pcomm_tx_db->dp_db.current2 = input_data[69];
		pcomm_tx_db->dp_db.current3 = input_data[70];

		pcomm_tx_db->dp_db.dc_chan1 = input_data[71];
		pcomm_tx_db->dp_db.dc_chan2 = input_data[72];

		pcomm_tx_db->dp_db.phase_angle = input_data[73];
		pcomm_tx_db->dp_db.power_factor = input_data[74];
		pcomm_tx_db->dp_db.power_kw = input_data[75];

		//USBCDC_sendDataInBackground((uint8_t*)&comm_tx_db.dev_address, sizeof(comm_tx_db), CDC0_INTFNUM, 1);
	}
}//End of UsbRxPrcs

void CInjectionTestAppDlg::DisplayValue(uint32_t Value, int dp_postion, float* out_value)
{
	if (dp_postion == 0)
	{
		*out_value = (float)Value;
	}
	else {
		// Calculate the divisor to position the decimal point correctly
		int divisor = 1;
		for (uint8_t i = 0; i < dp_postion; i++) {
			divisor *= 10;
		}
		float displayed_value = static_cast<float>(Value) / divisor;
		//printf("%.2f\n", displayed_value); // Assuming 2 decimal places
		*out_value = displayed_value;
	}
}

void CInjectionTestAppDlg::SetColourFormat(CColorStatic& t_obj, COLORREF t_back_color, COLORREF t_text_color)
{
	t_obj.SetBkColor(t_back_color);     // Change to your desired Background Color
	t_obj.SetTextColor(t_text_color); // Change it to your desired Foreground Color
	//t_obj.SetFont(&t_Font);
}

void CInjectionTestAppDlg::SetWindowBackGroundColor()
{
	for (int i = 0; i < 44; i++)
	{
		SetColourFormat(m_SaticText[i], BCKCOLOR, RGB(0, 0, 0));
	}
	SetColourFormat(m_testappname, BCKCOLOR, RGB(0, 0, 0));
	
	CFont font;
	font.CreateFontW(23, 0, 0, 0, 500, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS
		, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	m_editfilepath.SetFont(&font);
	m_testappname.SetFont(&font);
	m_editintrveal.SetFont(&font);
	for (int i = 44; i < 58; i++)
	{
		m_SaticText[i].SetFont(&font);
	}
	SetColourFormat(m_logstatus, RGB(255, 0, 0), RGB(255, 255, 255));
	SetColourFormat(m_Phase, RGB(0, 255, 0), RGB(255, 255, 255));
	SetColourFormat(m_ovstatus, RGB(0, 255, 0), RGB(255, 255, 255));
	SetColourFormat(m_olstatus, RGB(0, 255, 0), RGB(255, 255, 255));
	SetColourFormat(m_earthstatus, RGB(0, 255, 0), RGB(255, 255, 255));
	SetColourFormat(m_doorstatus, RGB(0, 255, 0), RGB(255, 255, 255));
	SetColourFormat(m_tripstatus, RGB(0, 255, 0), RGB(255, 255, 255));
	SetColourFormat(m_HtflagP, RGB(103, 103, 103), RGB(255, 255, 255));

}

void CInjectionTestAppDlg::OnBnClickedButtonDeviceConnect()
{
	int returnValue = COMPORT_SUCCESS;
	returnValue = IsDeviceConnected(VID_STR, PID_STR, hHandle);
	if (returnValue == COMPORT_SUCCESS)
	{
		AfxMessageBox(L"Device connected successfully");
		m_cbCommEventCallback = (DATA_FEEDBACK_CALLBACK)&CommEventCallback;
		CommRegisterCallback(&m_cbCommEventCallback);

		returnValue = startdatapreview();

	}
	else
	{
		AfxMessageBox(L"Device Not Connected");
	}
}

void CInjectionTestAppDlg::OnBnClickedButtonDisconnectDevice()
{

	int returnValue = COMPORT_SUCCESS;
	returnValue = ComportDeviceDisConnect(hHandle);
	if (returnValue == COMPORT_SUCCESS)
	{
		AfxMessageBox(L"The device has been disconnected successfully.");
		SetColourFormat(m_logstatus, RGB(255, 0, 0), RGB(255, 255, 255));
		SetColourFormat(m_Phase, RGB(0, 255, 0), RGB(255, 255, 255));
		SetColourFormat(m_ovstatus, RGB(0, 255, 0), RGB(255, 255, 255));
		SetColourFormat(m_olstatus, RGB(0, 255, 0), RGB(255, 255, 255));
		SetColourFormat(m_earthstatus, RGB(0, 255, 0), RGB(255, 255, 255));
		SetColourFormat(m_doorstatus, RGB(0, 255, 0), RGB(255, 255, 255));
		SetColourFormat(m_tripstatus, RGB(0, 255, 0), RGB(255, 255, 255));
		SetColourFormat(m_HtflagP, RGB(103, 103, 103), RGB(255, 255, 255));
	}
	else
	{
		AfxMessageBox(L"Device disconnection failed.");
	}
}

void CInjectionTestAppDlg::OnBnClickedButtonLogEnable()
{
	UpdateData(TRUE);
	EnabledisableCnt++;
	if (EnabledisableCnt % 2 != 0)
	{
		IsWriteFile = TRUE;
		m_btnenable.SetWindowTextW(L"Log Disable");
		SetColourFormat(m_logstatus, RGB(0, 255, 0), RGB(255, 255, 255));
		HANDLE  m_hAutoWriteDataHandle;
		m_hAutoWriteDataHandle = ::CreateThread(NULL, 0, &WriteDataThread, 0, 0, NULL);
	}
	else
	{
		m_btnenable.SetWindowTextW(L"Log Enable");
		IsWriteFile = FALSE;
		SetColourFormat(m_logstatus, RGB(255, 0, 0), RGB(255, 255, 255));
		int m_EntryCount = 0;
	}
	UpdateData(FALSE);
}

void CInjectionTestAppDlg::OnBnClickedButtonCreateLogFile()
{
	UpdateData(TRUE);
	CFileDialog dlg(FALSE);
	dlg.m_ofn.nMaxFile = MAX_PATH;
	dlg.m_ofn.lpstrFilter = _T("Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0\0");
	dlg.m_ofn.lpstrTitle = _T("Save File As");
	CString filename;
	if (dlg.DoModal() == IDOK)
	{
		filename = dlg.GetPathName(); // return full path and filename
		//write your sample code here to save the file to the user selected path
	}
	UpdateData(TRUE);
	m_filepath.SetString(filename);
	UpdateData(FALSE);

	int m_EntryCount = 0;
	m_CSVFile.open(filename, std::ios::out | std::ios::app);

	m_CSVFile << "Voltage 0" << "," << "Voltage 1" << "," << "I/p Frequency"
		<< "," << "O/P Frequency" << "," << "Current 0" << "," << "Current 1" << ","
		<< "Current 2" << "," << "DC Voltage 0" << "," << "DC Voltage 1" << "," << "Phase Angle" << "," << "Power Factor" << "," << "KiloWatt" << "," << "Timer" << "," << "Trip Data" <<
		"," << "Phase_fault" << "," << "OV Fault" << "," << "OC Fault" << "," << "TRIP Fault" << "," << "CageDoor Fault"
		<< "," << "Earth Fault" << "," << "HT_FLAG" << std::endl;
	
}

void CommEventCallback(int nErrorCode, char* buffer, unsigned int buff_size)
{
	if (COMPORT_SUCCESS == nErrorCode)
	{
		char* pDataPreviewParams = reinterpret_cast<char*> (buffer);
		if (pDataPreviewParams)
		{
			// Pass wParam = 1 for error and wParam = 0 for image
			::PostMessage(myDilogObj->m_hWnd, WM_IMAGE_PREVIEW, 0,
				reinterpret_cast<LPARAM>(pDataPreviewParams));
		}
	}
	else
	{
		if (myDilogObj)
		{
			// Pass wParam = 1 for error and wParam = 0 for image
			::PostMessage(myDilogObj->m_hWnd, WM_IMAGE_PREVIEW, 1,
				reinterpret_cast<LPARAM>("data was not found"));
		}
	}

}

LRESULT CInjectionTestAppDlg::OnMessageImagePreview(WPARAM wParam, LPARAM lParam)
{
	int returnValue = 0;

	try
	{
		if (wParam == 1) // wParam = 1 displays error message
		{
			AfxMessageBox(L"Data was not found ...");
		}
		else
		{
			char* pDataPreviewParams = reinterpret_cast<char*> (lParam);

			if (pDataPreviewParams)
			{
				SetReadData(pDataPreviewParams, &comm_tx_db);
				//if (comm_tx_db.model_no == 1)
				if (true)
				{
					DisplayValue(comm_tx_db.voltage1, comm_tx_db.dp_db.voltage1, &out_value);
					formatString.Format(_T("%%.%df"), comm_tx_db.dp_db.voltage1); // Format specifier for the desired decimal places
					newValueStr.Format(formatString, out_value);
					SetDlgItemText(IDC_STATIC_AC_VOLTAGE_VALUE_0, newValueStr);
					
					DisplayValue(comm_tx_db.voltage2, comm_tx_db.dp_db.voltage2, &out_value);
					formatString.Format(_T("%%.%df"), comm_tx_db.dp_db.voltage2); // Format specifier for the desired decimal places
					newValueStr.Format(formatString, out_value);
					SetDlgItemText(IDC_STATIC_AC_VOLTAGE_VALUE_1, newValueStr);

					DisplayValue(comm_tx_db.input_freq, comm_tx_db.dp_db.input_freq, &out_value);
					formatString.Format(_T("%%.%df"), comm_tx_db.dp_db.input_freq); // Format specifier for the desired decimal places
					newValueStr.Format(formatString, out_value);
					SetDlgItemText(IDC_STATIC_IP_FREQ_VALUE, newValueStr);

					DisplayValue(comm_tx_db.output_freq, comm_tx_db.dp_db.output_freq, &out_value);
					formatString.Format(_T("%%.%df"), comm_tx_db.dp_db.output_freq); // Format specifier for the desired decimal places
					newValueStr.Format(formatString, out_value);
					SetDlgItemText(IDC_STATIC_OP_FREQ_VALUE, newValueStr);

					DisplayValue(comm_tx_db.current1, comm_tx_db.dp_db.current1, &out_value);
					formatString.Format(_T("%%.%df"), comm_tx_db.dp_db.current1); // Format specifier for the desired decimal places
					newValueStr.Format(formatString, out_value);
					SetDlgItemText(IDC_STATIC_AC_CURRENT_VALUE_0, newValueStr);

					DisplayValue(comm_tx_db.current2, comm_tx_db.dp_db.current2, &out_value);
					formatString.Format(_T("%%.%df"), comm_tx_db.dp_db.current2); // Format specifier for the desired decimal places
					newValueStr.Format(formatString, out_value);
					SetDlgItemText(IDC_STATIC_AC_CURRENT_VALUE_1, newValueStr);

					DisplayValue(comm_tx_db.current3, comm_tx_db.dp_db.current3, &out_value);
					formatString.Format(_T("%%.%df"), comm_tx_db.dp_db.current3); // Format specifier for the desired decimal places
					newValueStr.Format(formatString, out_value);
					SetDlgItemText(IDC_STATIC_AC_CURRENT_VALUE_2, newValueStr);

					DisplayValue(comm_tx_db.dc_chan1, comm_tx_db.dp_db.dc_chan1, &out_value);
					formatString.Format(_T("%%.%df"), comm_tx_db.dp_db.dc_chan1); // Format specifier for the desired decimal places
					newValueStr.Format(formatString, out_value);
					SetDlgItemText(IDC_STATIC_DC_VOLTAGE_VALUE_0, newValueStr);

					DisplayValue(comm_tx_db.dc_chan2, comm_tx_db.dp_db.dc_chan2, &out_value);
					formatString.Format(_T("%%.%df"), comm_tx_db.dp_db.dc_chan2); // Format specifier for the desired decimal places
					newValueStr.Format(formatString, out_value);
					SetDlgItemText(IDC_STATIC_DC_VOLTAGE_VALUE_1, newValueStr);

					DisplayValue(comm_tx_db.phase_angle, comm_tx_db.dp_db.phase_angle, &out_value);
					formatString.Format(_T("%%.%df"), comm_tx_db.dp_db.phase_angle); // Format specifier for the desired decimal places
					newValueStr.Format(formatString, out_value);
					SetDlgItemText(IDC_STATIC_PHASE_ANGLE_VALUE, newValueStr);

					DisplayValue(comm_tx_db.power_factor, comm_tx_db.dp_db.power_factor, &out_value);
					formatString.Format(_T("%%.%df"), comm_tx_db.dp_db.power_factor); // Format specifier for the desired decimal places
					newValueStr.Format(formatString, out_value);
					SetDlgItemText(IDC_STATIC_POWER_FACTOR_VALUE, newValueStr);

					DisplayValue(comm_tx_db.power_kw, comm_tx_db.dp_db.power_kw, &out_value);
					formatString.Format(_T("%%.%df"), comm_tx_db.dp_db.power_kw); // Format specifier for the desired decimal places
					newValueStr.Format(formatString, out_value);
					SetDlgItemText(IDC_STATIC_KILOWATT_VALUE, newValueStr);

					if (comm_tx_db.status_db.trip_data == 0)
					{
						newValueStr.Format(_T("%d"), 25);
						SetDlgItemText(IDC_STATIC_TRIP_VALUE, newValueStr);
					}
					if (comm_tx_db.status_db.trip_data == 1)
					{
						newValueStr.Format(_T("%d"), 50);
						SetDlgItemText(IDC_STATIC_TRIP_VALUE, newValueStr);
					}
					if (comm_tx_db.status_db.trip_data == 2)
					{
						newValueStr.Format(_T("%d"), 75);
						SetDlgItemText(IDC_STATIC_TRIP_VALUE, newValueStr);
					}
					if (comm_tx_db.status_db.trip_data == 3)
					{
						newValueStr.Format(_T("%d"), 100);
						SetDlgItemText(IDC_STATIC_TRIP_VALUE, newValueStr);
					}

					int seconds, minutes, hours;

					//unsigned int seconds = comm_tx_db.timer_val % 60;
					//comm_tx_db.timer_val /= 60;

					//unsigned int minutes = comm_tx_db.timer_val % 60;
					//comm_tx_db.timer_val /= 60;

					//unsigned int hours = comm_tx_db.timer_val;

					seconds = comm_tx_db.timer_val & 0x00FF;
					minutes = (comm_tx_db.timer_val >> 16) & 0xFFFF;

					// Check if minutes exceed 59 and convert to "hh::mm::ss" format
					if (minutes > 59) {
						hours = minutes / 60;
						minutes %= 60;
					}
					else {
						hours = 0;
					}

					newValueStr.Format(_T("%02d:%02d:%02d"), hours, minutes, seconds);
					SetDlgItemText(IDC_STATIC_TIMER_VALUE, newValueStr);

					if (comm_tx_db.status_db.ht_flag == 16)
					{
						SetColourFormat(m_HtflagP, RGB(103, 103, 103), RGB(255, 255, 255));
						isHTflag = false;
					}
					else if (comm_tx_db.status_db.ht_flag == 17)
					{
						isHTflag = true;
						if (htcnt % 2 == 0)
						{
							SetColourFormat(m_HtflagP, RGB(255, 255, 255), RGB(255, 255, 255));
						}
						else
						{
							SetColourFormat(m_HtflagP, RGB(255, 115, 0), RGB(255, 255, 255));
						}
						htcnt++;
					}

					if (comm_tx_db.status_db.gen_faults & 0x01) {
						isOvFault = true;
						SetColourFormat(m_ovstatus, RGB(255, 0, 0), RGB(255, 255, 255));
					}
					else {
						isOvFault = false;
						SetColourFormat(m_ovstatus, RGB(0, 255, 0), RGB(255, 255, 255));
					}

					if (comm_tx_db.status_db.gen_faults & 0x02) {
						isOCFault = true;
						SetColourFormat(m_olstatus, RGB(255, 0, 0), RGB(255, 255, 255));
				
					}
					else {
						isOCFault = false;					
						SetColourFormat(m_olstatus, RGB(0, 255, 0), RGB(255, 255, 255));
			
					}

					if (comm_tx_db.status_db.gen_faults & 0x04) {
						istripFault = true;
						SetColourFormat(m_tripstatus, RGB(255, 0, 0), RGB(255, 255, 255));
					}
					else {
						istripFault = false;
						SetColourFormat(m_tripstatus, RGB(0, 255, 0), RGB(255, 255, 255));
					}

					if (comm_tx_db.status_db.gen_faults & 0x08) {
						isCageDoorFault = true;
						SetColourFormat(m_doorstatus, RGB(255, 0, 0), RGB(255, 255, 255));
					}
					else {
						isCageDoorFault = false;
						SetColourFormat(m_doorstatus, RGB(0, 255, 0), RGB(255, 255, 255));
					}

					if (comm_tx_db.status_db.gen_faults & 0x10) {
						isEarthFault = true;
						SetColourFormat(m_earthstatus, RGB(255, 0, 0), RGB(255, 255, 255));
					}
					else {
						isEarthFault = false;
						SetColourFormat(m_earthstatus, RGB(0, 255, 0), RGB(255, 255, 255));
					}

				/*	if (comm_tx_db.status_db.gen_faults == 1)
					{
						isOvFault = true;
						isOCFault = false;
						istripFault = false;
						isCageDoorFault = false;
						isEarthFault = false;
						SetColourFormat(m_ovstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_olstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_tripstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_doorstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_earthstatus, RGB(0, 255, 0), RGB(255, 255, 255));
					}
					else if (comm_tx_db.status_db.gen_faults == 2)
					{
						isOCFault = true;
						isOvFault = false;
						istripFault = false;
						isCageDoorFault = false;
						isEarthFault = false;
						SetColourFormat(m_ovstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_olstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_tripstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_doorstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_earthstatus, RGB(0, 255, 0), RGB(255, 255, 255));
					}
					else if (comm_tx_db.status_db.gen_faults == 3)
					{
						isOvFault = true;
						isOCFault = true;
						istripFault = false;
						isCageDoorFault = false;
						isEarthFault = false;
						SetColourFormat(m_ovstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_olstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_tripstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_doorstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_earthstatus, RGB(0, 255, 0), RGB(255, 255, 255));

					}
					else if (comm_tx_db.status_db.gen_faults == 4)
					{
						isOvFault = false;
						isOCFault = false;
						istripFault = true;
						isCageDoorFault = false;
						isEarthFault = false;
						SetColourFormat(m_ovstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_olstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_tripstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_doorstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_earthstatus, RGB(0, 255, 0), RGB(255, 255, 255));

					}
					else if (comm_tx_db.status_db.gen_faults == 5)
					{
						istripFault = true;
						isOvFault = true;
						isOCFault = false;
						isCageDoorFault = false;
						isEarthFault = false;
						SetColourFormat(m_ovstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_olstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_tripstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_doorstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_earthstatus, RGB(0, 255, 0), RGB(255, 255, 255));

					}
					else if (comm_tx_db.status_db.gen_faults == 6)
					{
						istripFault = true;
						isOCFault = true;
						isOvFault = false;
						isCageDoorFault = false;
						isEarthFault = false;
						SetColourFormat(m_ovstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_olstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_tripstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_doorstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_earthstatus, RGB(0, 255, 0), RGB(255, 255, 255));

					}
					else if (comm_tx_db.status_db.gen_faults == 7)
					{
						istripFault = true;
						isOCFault = true;
						isOvFault = true;
						isCageDoorFault = false;
						isEarthFault = false;
						SetColourFormat(m_ovstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_olstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_tripstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_doorstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_earthstatus, RGB(0, 255, 0), RGB(255, 255, 255));
					}
					else if (comm_tx_db.status_db.gen_faults == 8)
					{
						isCageDoorFault = true;
						isOvFault = false;
						isOCFault = false;
						istripFault = false;
						isEarthFault = false;
						SetColourFormat(m_ovstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_olstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_tripstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_doorstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_earthstatus, RGB(0, 255, 0), RGB(255, 255, 255));
					}
					else if (comm_tx_db.status_db.gen_faults == 9)
					{
						isCageDoorFault = true;
						isOvFault = true;
						isOCFault = false;
						istripFault = false;
						isEarthFault = false;
						SetColourFormat(m_ovstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_olstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_tripstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_doorstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_earthstatus, RGB(0, 255, 0), RGB(255, 255, 255));

					}
					else if (comm_tx_db.status_db.gen_faults == 10)
					{
						isCageDoorFault = true;
						isOCFault = true;
						isOvFault = false;
						istripFault = false;
						isEarthFault = false;
						SetColourFormat(m_ovstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_olstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_tripstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_doorstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_earthstatus, RGB(0, 255, 0), RGB(255, 255, 255));
					}
					else if (comm_tx_db.status_db.gen_faults == 11)
					{
						isCageDoorFault = true;
						isOCFault = true;
						isOvFault = true;
						istripFault = false;
						isEarthFault = false;
						SetColourFormat(m_ovstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_olstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_tripstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_doorstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_earthstatus, RGB(0, 255, 0), RGB(255, 255, 255));
					}
					else if (comm_tx_db.status_db.gen_faults == 12)
					{
						isCageDoorFault = true;
						istripFault = true;
						isOvFault = false;
						isOCFault = false;
						isEarthFault = false;
						SetColourFormat(m_ovstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_olstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_tripstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_doorstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_earthstatus, RGB(0, 255, 0), RGB(255, 255, 255));

					}
					else if (comm_tx_db.status_db.gen_faults == 13)
					{
						isCageDoorFault = true;
						istripFault = true;
						isOvFault = true;
						isOCFault = false;
						isEarthFault = false;
						SetColourFormat(m_ovstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_olstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_tripstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_doorstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_earthstatus, RGB(0, 255, 0), RGB(255, 255, 255));
					}
					else if (comm_tx_db.status_db.gen_faults == 14)
					{
						isCageDoorFault = true;
						istripFault = true;
						isOvFault = false;
						isOCFault = true;
						isEarthFault = false;
						SetColourFormat(m_ovstatus, RGB(0, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_olstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_tripstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_doorstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_earthstatus, RGB(0, 255, 0), RGB(255, 255, 255));
					}
					else if (comm_tx_db.status_db.gen_faults == 15)
					{
						isCageDoorFault = true;
						istripFault = true;
						isOvFault = true;
						isOCFault = true;
						isEarthFault = false;
						SetColourFormat(m_ovstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_olstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_tripstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_doorstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_earthstatus, RGB(0, 255, 0), RGB(255, 255, 255));
					}
					else if (comm_tx_db.status_db.gen_faults == 16)
					{
						isEarthFault = true;
						isCageDoorFault = false;
						istripFault = false;
						isOvFault = false;
						isOCFault = false;
						SetColourFormat(m_ovstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_olstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_tripstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_doorstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_earthstatus, RGB(255, 0, 0), RGB(255, 255, 255));
					}
					else if (comm_tx_db.status_db.gen_faults == 17)
					{
						isEarthFault = true;
						isCageDoorFault = false;
						istripFault = false;
						isOvFault = true;
						isOCFault = false;
						SetColourFormat(m_ovstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_olstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_tripstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_doorstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_earthstatus, RGB(255, 0, 0), RGB(255, 255, 255));
					}
					else if (comm_tx_db.status_db.gen_faults == 18)
					{
						isEarthFault = true;
						isCageDoorFault = false;
						istripFault = false;
						isOvFault = false;
						isOCFault = true;
						SetColourFormat(m_ovstatus, RGB(0, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_olstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_tripstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_doorstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_earthstatus, RGB(255, 0, 0), RGB(255, 255, 255));
					}
					else if (comm_tx_db.status_db.gen_faults == 19)
					{
						isEarthFault = true;
						isCageDoorFault = false;
						istripFault = false;
						isOvFault = true;
						isOCFault = true;
						SetColourFormat(m_ovstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_olstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_tripstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_doorstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_earthstatus, RGB(255, 0, 0), RGB(255, 255, 255));
					}
					else if (comm_tx_db.status_db.gen_faults == 20)
					{
						isEarthFault = true;
						isCageDoorFault = false;
						istripFault = true;
						isOvFault = false;
						isOCFault = false;
						SetColourFormat(m_ovstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_olstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_tripstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_doorstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_earthstatus, RGB(255, 0, 0), RGB(255, 255, 255));
					}
					else if (comm_tx_db.status_db.gen_faults == 21)
					{
						isEarthFault = true;
						isCageDoorFault = false;
						istripFault = true;
						isOvFault = true;
						isOCFault = false;
						SetColourFormat(m_ovstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_olstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_tripstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_doorstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_earthstatus, RGB(255, 0, 0), RGB(255, 255, 255));
					}
					else if (comm_tx_db.status_db.gen_faults == 22)
					{
						isEarthFault = true;
						isCageDoorFault = false;
						istripFault = true;
						isOvFault = false;
						isOCFault = true;
						SetColourFormat(m_ovstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_olstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_tripstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_doorstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_earthstatus, RGB(255, 0, 0), RGB(255, 255, 255));
					}
					else if (comm_tx_db.status_db.gen_faults == 23)
					{
						isEarthFault = true;
						isCageDoorFault = false;
						istripFault = true;
						isOvFault = true;
						isOCFault = true;
						SetColourFormat(m_ovstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_olstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_tripstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_doorstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_earthstatus, RGB(255, 0, 0), RGB(255, 255, 255));
					}
					else if (comm_tx_db.status_db.gen_faults == 24)
					{
						isEarthFault = true;
						isCageDoorFault = true;
						istripFault = false;
						isOvFault = false;
						isOCFault = false;
						SetColourFormat(m_ovstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_olstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_tripstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_doorstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_earthstatus, RGB(255, 0, 0), RGB(255, 255, 255));
					}
					else if (comm_tx_db.status_db.gen_faults == 25)
					{
						isEarthFault = true;
						isCageDoorFault = true;
						istripFault = false;
						isOvFault = true;
						isOCFault = false;
						SetColourFormat(m_ovstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_olstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_tripstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_doorstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_earthstatus, RGB(255, 0, 0), RGB(255, 255, 255));
					}
					else if (comm_tx_db.status_db.gen_faults == 26)
					{
						isEarthFault = true;
						isCageDoorFault = true;
						istripFault = false;
						isOvFault = false;
						isOCFault = true;
						SetColourFormat(m_ovstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_olstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_tripstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_doorstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_earthstatus, RGB(255, 0, 0), RGB(255, 255, 255));
					}
					else if (comm_tx_db.status_db.gen_faults == 27)
					{
						isEarthFault = true;
						isCageDoorFault = true;
						istripFault = false;
						isOvFault = true;
						isOCFault = true;
						SetColourFormat(m_ovstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_olstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_tripstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_doorstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_earthstatus, RGB(255, 0, 0), RGB(255, 255, 255));
					}
					else if (comm_tx_db.status_db.gen_faults == 28)
					{
						isEarthFault = true;
						isCageDoorFault = true;
						istripFault = true;
						isOvFault = false;
						isOCFault = false;
						SetColourFormat(m_ovstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_olstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_tripstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_doorstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_earthstatus, RGB(255, 0, 0), RGB(255, 255, 255));
					}
					else if (comm_tx_db.status_db.gen_faults == 29)
					{
						isEarthFault = true;
						isCageDoorFault = true;
						istripFault = true;
						isOvFault = true;
						isOCFault = false;
						SetColourFormat(m_ovstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_olstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_tripstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_doorstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_earthstatus, RGB(255, 0, 0), RGB(255, 255, 255));
					}
					else if (comm_tx_db.status_db.gen_faults == 30)
					{
						isEarthFault = true;
						isCageDoorFault = true;
						istripFault = true;
						isOvFault = false;
						isOCFault = true;
						SetColourFormat(m_ovstatus, RGB(0, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_olstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_tripstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_doorstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_earthstatus, RGB(255, 0, 0), RGB(255, 255, 255));
					}
					else if (comm_tx_db.status_db.gen_faults == 31)
					{
						isEarthFault = true;
						isCageDoorFault = true;
						istripFault = true;
						isOvFault = true;
						isOCFault = true;
						SetColourFormat(m_ovstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_olstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_tripstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_doorstatus, RGB(255, 0, 0), RGB(255, 255, 255));
						SetColourFormat(m_earthstatus, RGB(255, 0, 0), RGB(255, 255, 255));
					}
					else
					{
						isEarthFault = false;
						isCageDoorFault = false;
						istripFault = false;
						isOvFault = false;
						isOCFault = false;
						SetColourFormat(m_ovstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_olstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_tripstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_doorstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						SetColourFormat(m_earthstatus, RGB(0, 255, 0), RGB(255, 255, 255));
						}*/

					if (comm_tx_db.status_db.phase_fault == 0)
					{
						SetColourFormat(m_Phase, RGB(0, 255, 0), RGB(255, 255, 255));
						isPhasefalut = false;
					}
					else
					{
						SetColourFormat(m_Phase, RGB(255, 0, 0), RGB(255, 255, 255));
						isPhasefalut = true;
					}
				}

			}
		}
	}
	catch (const std::exception& /* ex */)
	{
		returnValue = -2;
	}
	catch (...)
	{
		returnValue = -2;
	}

	return *reinterpret_cast<LRESULT const*>(&returnValue);
}

BOOL CInjectionTestAppDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);

	CBrush brush(RGB(241, 230, 184)); // Create a red brush
	pDC->FillRect(&rect, &brush); // Fill the background with the brush

	return TRUE; // Indicate that you have handled erasing the background
}

DWORD WINAPI WriteDataThread(LPVOID lpParam)
{
	CString CSVoltage0,CSVoltage1,CSIPFreq,CSOPFreq,CSACCurrent0,CSACCurrent1,CSACCurrent2,CSDCVoltage0
	,CSDCVoltage1,CPhaseangle, CKilowatt, CPowerFactor, CSTimer,CSTrip,Cphase_fault,COvFault,COCFault, CTripFault, CCageDoorFault, CEarthFault, CHtFlag;
	while (IsWriteFile)
	{
		myDilogObj->m_SaticText[44].GetWindowTextW(CSVoltage0);
		myDilogObj->m_SaticText[45].GetWindowTextW(CSVoltage1);
		myDilogObj->m_SaticText[46].GetWindowTextW(CSIPFreq);
		myDilogObj->m_SaticText[47].GetWindowTextW(CSOPFreq);
		myDilogObj->m_SaticText[51].GetWindowTextW(CSACCurrent0);
		myDilogObj->m_SaticText[52].GetWindowTextW(CSACCurrent1);
		myDilogObj->m_SaticText[53].GetWindowTextW(CSACCurrent2);
		myDilogObj->m_SaticText[48].GetWindowTextW(CSDCVoltage0);
		myDilogObj->m_SaticText[49].GetWindowTextW(CSDCVoltage1);
		myDilogObj->m_SaticText[54].GetWindowTextW(CPhaseangle);
		myDilogObj->m_SaticText[55].GetWindowTextW(CPowerFactor);
		myDilogObj->m_SaticText[57].GetWindowTextW(CKilowatt);
		myDilogObj->m_SaticText[50].GetWindowTextW(CSTimer);
		myDilogObj->m_SaticText[56].GetWindowTextW(CSTrip);

		const char* charStr = CT2A(CSTimer);
		std::string stdStr(charStr);

		if (isPhasefalut)
		{
			Cphase_fault = "1";
		}
		else
		{
			Cphase_fault = "0";
		}
		if (isOvFault)
		{
			COvFault = "1";
		}
		else
		{
			COvFault = "0";
		}
		if (isOCFault)
		{
			COCFault = "1";
		}
		else
		{
			COCFault = "0";
		}
		if (istripFault)
		{
			CTripFault = "1";
		}
		else
		{
			CTripFault = "0";
		}
		if (isCageDoorFault)
		{
			CCageDoorFault = "1";
		}
		else
		{
			CCageDoorFault = "0";
		}
		if (isEarthFault)
		{
			CEarthFault = "1";
		}
		else
		{
			CEarthFault = "0";
		}
		if (isHTflag)
		{
			CHtFlag = "1";
		}
		else
		{
			CHtFlag = "0";
		}


		m_CSVFile << _tstof(CSVoltage0) << "," << _tstof(CSVoltage1) << "," << _tstof(CSIPFreq)
			<< "," << _tstof(CSOPFreq) << "," << _tstof(CSACCurrent0) << "," << _tstof(CSACCurrent1) << ","
			<< _tstof(CSACCurrent2) << "," << _tstof(CSDCVoltage0) << "," << _tstof(CSDCVoltage1) << "," << _tstof(CPhaseangle) << "," << _tstof(CPowerFactor) << "," << _tstof(CKilowatt)
			<< "," << stdStr << "," << _tstof(CSTrip) << "," << _tstof(Cphase_fault) << "," << _tstof(COvFault) << "," << _tstof(COCFault) <<
			"," << _tstof(CTripFault) << "," << _tstof(CCageDoorFault) << "," << _tstof(CEarthFault) << "," << _tstof(CHtFlag) << std::endl;

		// Sleep for 5 seconds
		Sleep(myDilogObj->m_intrvealvalue * 1000); // 5 seconds in microseconds

	}
	return 0;
}
