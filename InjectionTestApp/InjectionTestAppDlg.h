
// InjectionTestAppDlg.h : header file
//

#pragma once
#include "ColorStatic.h"
#include "Comport.h"

#include <fstream>
#include <string>
#define BCKCOLOR RGB(241, 230, 184)
#define BLACKCOLOR RGB(0,0,0)
#define REDCOLOR RGB(255,0,0)
#define GREENCOLOR RGB(0,255,0)

typedef struct
{
	uint8_t gen_faults;
	uint8_t phase_fault;
	uint8_t trip_data;
	uint8_t ht_flag;
} status_db_t;


typedef struct
{
	uint8_t voltage1; 			//0
	uint8_t voltage2;			//1
	uint8_t input_freq;			//2
	uint8_t output_freq;		//3

	uint8_t current1;			//4
	uint8_t current2;			//5
	uint8_t current3;			//6
	uint8_t dc_chan1;			//7

	uint8_t dc_chan2;			//8
	uint8_t phase_angle;		//9
	uint8_t power_factor;		//10
	uint8_t power_kw;			//11
}dp_db_t;

typedef struct
{
	uint32_t dev_address;
	status_db_t status_db;
	uint32_t timer_val;

	uint32_t voltage1; 		//0
	uint32_t voltage2;			//1

	uint32_t input_freq;		//2
	uint32_t output_freq;		//3

	uint32_t current1;			//4
	uint32_t current2;			//5
	uint32_t current3;			//6

	uint32_t dc_chan1;			//7
	uint32_t dc_chan2;			//8

	uint32_t phase_angle;		//9
	uint32_t power_factor;		//10
	uint32_t power_kw;			//11
	uint32_t model_no;			//12

	dp_db_t dp_db;

} comm_tx_db_t;

// CInjectionTestAppDlg dialog
class CInjectionTestAppDlg : public CDialogEx
{
// Construction
public:
	CInjectionTestAppDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INJECTIONTESTAPP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()
private:	
	
	void SetWindowBackGroundColor();
	void SetDefaultValue();

	std::string generateTimestamp();
public:
	comm_tx_db_t comm_tx_db;
	HANDLE hHandle;
	DATA_FEEDBACK_CALLBACK m_cbCommEventCallback;
	afx_msg LRESULT OnMessageImagePreview(WPARAM wParam, LPARAM lParam);
	void SetReadData(char* input_data, comm_tx_db_t* pcomm_tx_db);
	void DisplayValue(uint32_t Value, int dp_postion, float* out_value);
	void SetColourFormat(CColorStatic& t_obj, COLORREF t_back_color, COLORREF t_text_color);
public:
	afx_msg void OnBnClickedButtonDeviceConnect();
	afx_msg void OnBnClickedButtonDisconnectDevice();
	afx_msg void OnBnClickedButtonLogEnable();
	afx_msg void OnBnClickedButtonCreateLogFile();
	CColorStatic m_testappname;
	CEdit m_editAcVoltage0;
	CEdit m_editACVoltage1;
	CEdit m_editIpFreq;
	CEdit m_editoutfreq;
	CEdit m_editAccurrent0;
	CEdit m_editAcCurrent1;
	CEdit m_editACCurrent2;
	CEdit m_editDCVoltage0;
	CEdit m_editDCVoltage1;
	CEdit m_editphaseangle;
	CEdit m_editpowerfactor;
	CEdit m_edittripdata;
	CEdit m_editkilowatt;
	CEdit m_edittimervalue;
	CEdit m_editintrveal;
	int m_intrvealvalue;
	CEdit m_editfilepath;
	CString m_filepath;
	CString m_ACVoltageValue0;
	CColorStatic m_HtflagP;
	CColorStatic m_SaticText[58];
	CColorStatic m_logstatus;
	CColorStatic m_Phase;
	CColorStatic m_ovstatus;
	CColorStatic m_olstatus;
	CColorStatic m_earthstatus;
	CColorStatic m_doorstatus;
	CColorStatic m_tripstatus;
	CButton m_btnenable;
	afx_msg void OnClose();
};
