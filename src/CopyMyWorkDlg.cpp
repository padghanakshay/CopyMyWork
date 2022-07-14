
// CopyMyWorkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CopyMyWork.h"
#include "CopyMyWorkDlg.h"
#include "afxdialogex.h"
#include "Util.h"
#include <algorithm>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

//================================================================================
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

//================================================================================
CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

//================================================================================
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

//================================================================================
BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCopyMyWorkDlg dialog


//================================================================================

CCopyMyWorkDlg::CCopyMyWorkDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCopyMyWorkDlg::IDD, pParent)
	, m_ipFolder(_T(""))
	, m_opFolder(_T(""))
	, m_extensions(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
//================================================================================

void CCopyMyWorkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MFCEDITBROWSE1, m_ipFolder);
	DDX_Text(pDX, IDC_MFCEDITBROWSE2, m_opFolder);
	DDX_Text(pDX, IDC_RICHEDIT21, m_extensions);
	DDX_Control(pDX, IDC_RICHEDIT21, m_extensionsCtrl);
}
//================================================================================

BEGIN_MESSAGE_MAP(CCopyMyWorkDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CCopyMyWorkDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CCopyMyWorkDlg message handlers

//================================================================================

BOOL CCopyMyWorkDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	m_extensions = L".cpp .c .rc .rch .h .xml .vcxproj .filters .sln";
	m_ipFolder = L"D:\\GitHub\\CopyMyWork\\wrk\\res";
	m_opFolder = L"E:\\New folder";

	UpdateData(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}
//================================================================================
void CCopyMyWorkDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

//================================================================================
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCopyMyWorkDlg::OnPaint()
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

//================================================================================

bool CCopyMyWorkDlg::Algorithm(CString ip, CString op, std::vector<std::string>& ipPaths, std::vector<std::string>& acceptedEXTENSION,
	CString& update)
{
	try
	{
		std::string ips = CT2A(ip);
		std::string ops = CT2A(op);

		for (const std::string& pathS : ipPaths)
		{
			size_t isf = pathS.find_last_of(".");
			if (isf == std::string::npos)
				continue;

			std::string extension = pathS.substr(isf);

			bool acceptedFileExt = false;

			for (size_t eNum = 0; eNum < acceptedEXTENSION.size(); ++eNum)
			{
				auto EXT = acceptedEXTENSION[eNum];
				std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
				std::transform(EXT.begin(), EXT.end(), EXT.begin(), ::tolower);
				if (extension == EXT)
				{
					acceptedFileExt = true;
					break;
				}
			}

			if (!acceptedFileExt)
				continue;

			if (Util::IsFileWritable(pathS))
			{
				auto destination = ops;
				std::string fileDestination = ops;
				size_t idx = pathS.find(ips);
				if (idx != std::string::npos)
				{
					fileDestination = destination + pathS.substr(idx + ips.length());
				}
				std::string folderName = fileDestination.substr(0, fileDestination.find_last_of("\\"));
				try
				{
					Util::CreateFolder(folderName);
					std::wstring pathSW = L"", fileDestinationW = L"";
					Util::StringToWstring(pathS, pathSW);
					Util::StringToWstring(fileDestination, fileDestinationW);

					COPYFILE2_EXTENDED_PARAMETERS extendedParameters;
					extendedParameters.dwSize = sizeof(extendedParameters);
					extendedParameters.dwCopyFlags = 1; // bFailIfExists ? COPY_FILE_FAIL_IF_EXISTS : 0;
					extendedParameters.pfCancel = FALSE;
					extendedParameters.pProgressRoutine = NULL;
					extendedParameters.pvCallbackContext = NULL;

					HRESULT hr = CopyFile2(pathSW.c_str(), fileDestinationW.c_str(), &extendedParameters);
					BOOL isCopied = SUCCEEDED(hr);

					/*std::ifstream  src("source_filename", std::ios::binary);
					std::ofstream  dst("dest_filename", std::ios::binary);
					dst << src.rdbuf();*/

					//TODO
					//std::filesystem::create_directory(folderName);
					//std::filesystem::copy(pathS, fileDestination);

					update = CString(pathS.c_str());
				}
				catch (std::exception ex)
				{

				}
			}
		}

		AfxMessageBox(L"Congrats, All Files Copied!", MB_OK, MB_ICONINFORMATION);

	}
	catch (std::exception ex)
	{
		AfxMessageBox(CString(ex.what()), MB_OK, MB_ICONERROR);
		return false;
	}
	return true;
}
//================================================================================
// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCopyMyWorkDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//================================================================================

void CCopyMyWorkDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	CString ip = m_ipFolder, op = m_opFolder;
	std::vector<std::string> ipPaths, acceptedEXTENSION;

	try
	{
		std::string ips = CT2A(ip);
		Util::GetAllFiles(ips, ipPaths);
		Util::GetAcceptedExtension(m_extensions, acceptedEXTENSION);
	}
	catch (std::exception ex)
	{
		AfxMessageBox(CString(ex.what()), MB_OK, MB_ICONERROR);
		return;
	}

	CString m_update;
	Algorithm(ip, op, ipPaths, acceptedEXTENSION, m_update);
	
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}

//================================================================================