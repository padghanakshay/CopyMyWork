
// CopyMyWorkDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include<vector>
#include<string>

// CCopyMyWorkDlg dialog
class CCopyMyWorkDlg : public CDialogEx
{
// Construction
public:
	CCopyMyWorkDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_COPYMYWORK_DIALOG };

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
	DECLARE_MESSAGE_MAP()
public:
	CString m_ipFolder;
	CString m_opFolder;
	CString m_extensions;
	afx_msg void OnBnClickedOk();
	CRichEditCtrl m_extensionsCtrl;

	bool Algorithm(CString ip, CString op, std::vector<std::string>& ipPaths, 
		std::vector<std::string>& acceptedEXTENSION, CString& update);

};
