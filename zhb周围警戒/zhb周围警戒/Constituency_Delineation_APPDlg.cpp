
// Constituency_Delineation_APPDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "Constituency_Delineation_APP.h"
#include "Constituency_Delineation_APPDlg.h"
#include "afxdialogex.h"
#include "Constituency_Delineation.h"
#include "DNN_Detect.h"
#include "HIK_Flow_In.h"
#include "Transboundary_Detection.h"
#include "Shellapi.h"
#include "tchar.h"
#pragma comment(lib,"shell32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern HIK_CAMERA Camera;
extern Mat frame_detect;
extern Mat mask;
extern Mat current_frame;
extern String Record_Path;
extern String WindowName;
extern vector<vector<Point>> Polys;
extern bool flag_created;
extern bool Paint_Flag;
extern bool Warning_Flag;

const char* ConfigFile = "config.txt";
CFont font;
void Video_Play_Thread(void*);
void Developer_Play_Thread(void*);
DNN_Detector Detector;
bool Develop_Mode = 0;
CConstituencyDelineationAPPDlg* g_pDlg = NULL;

// CConstituencyDelineationAPPDlg 对话框



CConstituencyDelineationAPPDlg::CConstituencyDelineationAPPDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CONSTITUENCY_DELINEATION_APP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CConstituencyDelineationAPPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, m_sliderc);
}

BEGIN_MESSAGE_MAP(CConstituencyDelineationAPPDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT3, &CConstituencyDelineationAPPDlg::OnEnChangeEdit3)
	ON_BN_CLICKED(IDC_BUTTON2, &CConstituencyDelineationAPPDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CConstituencyDelineationAPPDlg::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT4, &CConstituencyDelineationAPPDlg::OnEnChangeEdit4)
	ON_EN_CHANGE(IDC_EDIT5, &CConstituencyDelineationAPPDlg::OnEnChangeEdit5)
	ON_BN_CLICKED(IDC_BUTTON4, &CConstituencyDelineationAPPDlg::OnBnClickedButton4)
	ON_EN_CHANGE(IDC_EDIT1, &CConstituencyDelineationAPPDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON5, &CConstituencyDelineationAPPDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON3, &CConstituencyDelineationAPPDlg::OnBnClickedButton3)
	ON_COMMAND(IDOK, &CConstituencyDelineationAPPDlg::OnIdok)
	ON_COMMAND(IDCANCEL, &CConstituencyDelineationAPPDlg::OnIdcancel)
	ON_WM_CLOSE()
	ON_EN_CHANGE(IDC_EDIT6, &CConstituencyDelineationAPPDlg::OnEnChangeEdit6)
	ON_BN_CLICKED(IDC_BUTTON6, &CConstituencyDelineationAPPDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CConstituencyDelineationAPPDlg::OnBnClickedButton7)
//	ON_STN_CLICKED(IDC_Label5, &CConstituencyDelineationAPPDlg::OnClickedLabel5)
//ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CConstituencyDelineationAPPDlg::OnNMCustomdrawSlider1)
ON_WM_HSCROLL()
ON_BN_CLICKED(IDC_BUTTON8, &CConstituencyDelineationAPPDlg::OnBnClickedButton8)
ON_BN_CLICKED(IDC_BUTTON9, &CConstituencyDelineationAPPDlg::OnBnClickedButton9)
END_MESSAGE_MAP()


// CConstituencyDelineationAPPDlg 消息处理程序

BOOL CConstituencyDelineationAPPDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	g_pDlg = this;
	static CFont InitFont1;
	InitFont1.CreatePointFont(240, _T("黑体"));
	GetDlgItem(IDC_Label1)->SetFont(&InitFont1);
	GetDlgItem(IDC_Label10)->SetFont(&InitFont1);
	static CFont InitFont2;
	InitFont2.CreatePointFont(100, _T("黑体"));
	GetDlgItem(IDC_Label3)->SetFont(&InitFont2);
	GetDlgItem(IDC_Label4)->SetFont(&InitFont2);
	GetDlgItem(IDC_Label5)->SetFont(&InitFont2);
	GetDlgItem(IDC_BUTTON1)->SetFont(&InitFont2);
	GetDlgItem(IDC_BUTTON2)->SetFont(&InitFont2);
	GetDlgItem(IDC_BUTTON4)->SetFont(&InitFont2);
	static CFont InitFont3;
	InitFont3.CreatePointFont(120, _T("黑体"));
	GetDlgItem(IDC_BUTTON3)->SetFont(&InitFont3);
	GetDlgItem(IDC_Label2)->SetFont(&InitFont3);
	GetDlgItem(IDC_BUTTON6)->SetFont(&InitFont3);
	GetDlgItem(IDC_BUTTON7)->SetFont(&InitFont3);
	GetDlgItem(IDC_BUTTON8)->SetFont(&InitFont3);
	GetDlgItem(IDC_Label6)->SetFont(&InitFont3);
	GetDlgItem(IDC_Label7)->SetFont(&InitFont3);
	GetDlgItem(IDC_Label8)->SetFont(&InitFont3);
	GetDlgItem(IDC_Label9)->SetFont(&InitFont3);

	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON5)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON6)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON7)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON8)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
	GetDlgItem(IDC_SLIDER1)->EnableWindow(FALSE);

	ifstream Pathin(ConfigFile);
	getline(Pathin, Record_Path);

	if (Record_Path.length() == 0)
	{
		ofstream Pathout(ConfigFile);
		Pathout << "C:/Record/";
		Record_Path = "C:/Record/";
		Pathout.close();
	}

	if (_access(Record_Path.c_str(), 0))
		createDirectory(Record_Path);

	GetDlgItem(IDC_VIDEO)->ShowWindow(FALSE);
	namedWindow(WindowName, WINDOW_AUTOSIZE);
	setMouseCallback(WindowName, mouseHandler, NULL);
	HWND MainHPic = (HWND)cvGetWindowHandle(WindowName.c_str());
	HWND MainHParent = ::GetParent(MainHPic);
	::SetParent(MainHPic, GetDlgItem(IDC_VIDEO)->m_hWnd);
	::ShowWindow(MainHParent, SW_HIDE);

	GetDlgItem(IDC_WARNING)->ShowWindow(FALSE);
	namedWindow("Warning!!!", WINDOW_AUTOSIZE);
	HWND WarHWnd = (HWND)cvGetWindowHandle("Warning!!!");
	HWND WarHParent = ::GetParent(WarHWnd);
	::SetParent(WarHWnd, GetDlgItem(IDC_WARNING)->m_hWnd);
	::ShowWindow(WarHParent, SW_HIDE);

	GetDlgItem(IDC_BUTTON9)->ShowWindow(FALSE);
	GetDlgItem(IDC_Label6)->ShowWindow(FALSE);
	GetDlgItem(IDC_Label7)->ShowWindow(FALSE);
	GetDlgItem(IDC_Label8)->ShowWindow(FALSE);
	GetDlgItem(IDC_Label9)->ShowWindow(FALSE);

	m_sliderc.SetRange(25, 90);
	m_sliderc.SetTicFreq(1);
	m_sliderc.SetPos(70);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CConstituencyDelineationAPPDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CConstituencyDelineationAPPDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CConstituencyDelineationAPPDlg::OnEnChangeEdit1()
{
	CString Message;
	Message = "输入路径后点击“更改路径”";
	font.DeleteObject();
	font.CreatePointFont(90, _T("黑体"));
	GetDlgItem(IDC_Label2)->SetFont(&font);
	GetDlgItem(IDC_Label2)->SetWindowTextW(Message);
}

void CConstituencyDelineationAPPDlg::OnEnChangeEdit3()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

void CConstituencyDelineationAPPDlg::OnEnChangeEdit4()
{
	CEdit* pBox;
	pBox = (CEdit*)GetDlgItem(IDC_EDIT4);
	CString Developer_ID;
	pBox->GetWindowText(Developer_ID);
	if (Developer_ID == "Developer")
		GetDlgItem(IDC_EDIT6)->ShowWindow(TRUE);
}

void CConstituencyDelineationAPPDlg::OnEnChangeEdit5()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

void CConstituencyDelineationAPPDlg::OnEnChangeEdit6()
{
	CEdit* pBox;
	pBox = (CEdit*)GetDlgItem(IDC_EDIT6);
	CString Developer_Key;
	pBox->GetWindowText(Developer_Key);
	if (Developer_Key == "Hawthorn")
	{
		Develop_Mode = 1;
		CString Message;
		Message = "开发者模式！";
		font.DeleteObject();
		font.CreatePointFont(120, _T("黑体"));
		GetDlgItem(IDC_Label2)->SetFont(&font);
		GetDlgItem(IDC_Label2)->SetWindowTextW(Message);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON5)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON6)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON7)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON8)->EnableWindow(TRUE);
		GetDlgItem(IDC_SLIDER1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT4)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT5)->EnableWindow(FALSE);
		GetDlgItem(IDC_Label6)->ShowWindow(TRUE);
		GetDlgItem(IDC_Label7)->ShowWindow(TRUE);
		GetDlgItem(IDC_Label8)->ShowWindow(TRUE);
		GetDlgItem(IDC_Label9)->ShowWindow(TRUE);
	}
}

void CConstituencyDelineationAPPDlg::OnBnClickedButton1()
{
	CEdit* pBox;
	pBox = (CEdit*)GetDlgItem(IDC_EDIT3);
	CString IP;
	pBox->GetWindowText(IP);
	string Inter_Str_IP;
	Inter_Str_IP = CT2A(IP.GetString());
	Camera.UserIP = (char*)Inter_Str_IP.c_str();

	pBox = (CEdit*)GetDlgItem(IDC_EDIT4);
	CString ID;
	pBox->GetWindowText(ID);
	string Inter_Str_ID;
	Inter_Str_ID = CT2A(ID.GetString());
	Camera.UserID = (char*)Inter_Str_ID.c_str();

	pBox = (CEdit*)GetDlgItem(IDC_EDIT5);
	CString Key;
	pBox->GetWindowText(Key);
	string Inter_Str_Key;
	Inter_Str_Key = CT2A(Key.GetString());
	Camera.UserKey = (char*)Inter_Str_Key.c_str();

	CString Message;

	if (IP.GetLength() <= 0)
	{
		Message = "请输入IP地址";
		font.DeleteObject();
		font.CreatePointFont(120, _T("黑体"));
		GetDlgItem(IDC_Label2)->SetFont(&font);
		GetDlgItem(IDC_Label2)->SetWindowTextW(Message);
		return;
	}
	else if (ID.GetLength() <= 0 || Key.GetLength() <= 0)
	{
		Message = "请输入用户名与密码";
		font.DeleteObject();
		font.CreatePointFont(100, _T("黑体"));
		GetDlgItem(IDC_Label2)->SetFont(&font);
		GetDlgItem(IDC_Label2)->SetWindowTextW(Message);
		return;
	}
	else
	{
		int Error_Code;

		Error_Code = Camera.Camera_Init();
		switch (Error_Code)
		{
			case 0: break;
			case 41:
			{
				Message = "SDK资源分配错误！";
				font.DeleteObject();
				font.CreatePointFont(100, _T("黑体"));
				GetDlgItem(IDC_Label2)->SetFont(&font);
				GetDlgItem(IDC_Label2)->SetWindowTextW(Message);
				GetDlgItem(IDC_EDIT5)->SetWindowText(_T(""));
				return;
			}
			case 53:
			{
				Message = "本地IP地址失效！";
				font.DeleteObject();
				font.CreatePointFont(100, _T("黑体"));
				GetDlgItem(IDC_Label2)->SetFont(&font);
				GetDlgItem(IDC_Label2)->SetWindowTextW(Message);
				GetDlgItem(IDC_EDIT5)->SetWindowText(_T(""));
				return;
			}
			default:
			{
				Message.Format(_T("其它初始化错误：%d"), Error_Code);
				font.DeleteObject();
				font.CreatePointFont(100, _T("黑体"));
				GetDlgItem(IDC_Label2)->SetFont(&font);
				GetDlgItem(IDC_Label2)->SetWindowTextW(Message);
				return;
			}
		}

		Error_Code = Camera.Camera_Login();
		switch (Error_Code)
		{
			case 0:
			{
				Message = "登录成功！";
				font.DeleteObject();
				font.CreatePointFont(120, _T("黑体"));
				GetDlgItem(IDC_Label2)->SetFont(&font);
				GetDlgItem(IDC_Label2)->SetWindowTextW(Message);
				GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON5)->EnableWindow(TRUE);
				GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
				GetDlgItem(IDC_EDIT3)->EnableWindow(FALSE);
				GetDlgItem(IDC_EDIT4)->EnableWindow(FALSE);
				GetDlgItem(IDC_EDIT5)->EnableWindow(FALSE);
				GetDlgItem(IDC_Label6)->ShowWindow(TRUE);
				GetDlgItem(IDC_Label7)->ShowWindow(TRUE);
				GetDlgItem(IDC_Label8)->ShowWindow(TRUE);
				GetDlgItem(IDC_Label9)->ShowWindow(TRUE);
				GetDlgItem(IDC_SLIDER1)->ShowWindow(TRUE);
				break;
			}
			case 1:
			{
				Message = "用户名或密码错误！";
				font.DeleteObject();
				font.CreatePointFont(100, _T("黑体"));
				GetDlgItem(IDC_Label2)->SetFont(&font);
				GetDlgItem(IDC_Label2)->SetWindowTextW(Message);
				GetDlgItem(IDC_EDIT5)->SetWindowText(_T(""));
				return;
			}
			case 7:
			{
				Message = "设备IP错误或不在线！";
				font.DeleteObject();
				font.CreatePointFont(100, _T("黑体"));
				GetDlgItem(IDC_Label2)->SetFont(&font);
				GetDlgItem(IDC_Label2)->SetWindowTextW(Message);
				GetDlgItem(IDC_EDIT5)->SetWindowText(_T(""));
				return;
			}
			default:
			{
				Message.Format(_T("其它登录错误：%d"), Error_Code);
				font.DeleteObject();
				font.CreatePointFont(100, _T("黑体"));
				GetDlgItem(IDC_Label2)->SetFont(&font);
				GetDlgItem(IDC_Label2)->SetWindowTextW(Message);
				GetDlgItem(IDC_EDIT5)->SetWindowText(_T(""));
				return;
			}
		}
	}
}

void CConstituencyDelineationAPPDlg::OnBnClickedButton2()
{
	if (_access(Record_Path.c_str(), 0))
		createDirectory(Record_Path);
	CString cmd,dir;
	cmd = "explore";
	dir = Record_Path.c_str();
	::ShellExecute(NULL, cmd, dir, NULL, NULL, SW_SHOWDEFAULT);
}

void CConstituencyDelineationAPPDlg::OnBnClickedButton3()
{
	CString Message;
	if (!Camera.Play_Flag)
	{
		GetDlgItem(IDC_VIDEO)->ShowWindow(TRUE);

		int Error_Code;

		if (Develop_Mode)
			Error_Code = 0;
		else
			Error_Code = Camera.Get_Flow(1, 0);

		switch (Error_Code)
		{
			case 0:
			{
				Message = "摄像头启动成功！";
				font.DeleteObject();
				font.CreatePointFont(120, _T("黑体"));
				GetDlgItem(IDC_Label2)->SetFont(&font);
				GetDlgItem(IDC_Label2)->SetWindowTextW(Message);
				static CFont tempfont;
				Message = "摄像头：开启";
				tempfont.CreatePointFont(120, _T("黑体"));
				GetDlgItem(IDC_Label8)->SetFont(&tempfont);
				GetDlgItem(IDC_Label8)->SetWindowTextW(Message);
				GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON5)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON6)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON7)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON8)->EnableWindow(TRUE);
				GetDlgItem(IDC_SLIDER1)->EnableWindow(TRUE);
				GetDlgItem(IDC_Label1)->ShowWindow(FALSE);
				break; 
			}
			default:
			{
				Message.Format(_T("其它播放错误：%d"), Error_Code);
				font.DeleteObject();
				font.CreatePointFont(100, _T("黑体"));
				GetDlgItem(IDC_Label2)->SetFont(&font);
				GetDlgItem(IDC_Label2)->SetWindowTextW(Message);
				GetDlgItem(IDC_EDIT5)->SetWindowText(_T(""));
				return;
			}
		}
		if (Develop_Mode)
		{
			Camera.Play_Flag = 1;
			_beginthread(Developer_Play_Thread, 0, NULL);
			while (frame_detect.empty()) waitKey(50);
			Camera.Flow_Width = frame_detect.cols;
			Camera.Flow_Height = frame_detect.rows;
			Camera.Scale_Width = (float)Video_ShowSize.width / Camera.Flow_Width;
			Camera.Scale_Height = (float)Video_ShowSize.height / Camera.Flow_Height;
		}
		mask = Mat(Size(Camera.Flow_Width, Camera.Flow_Height), CV_8UC1, Scalar(1));

		_beginthread(Video_Play_Thread, 0, NULL);
	}
	else
	{
		Polys.clear();
		mask = Mat(Size(Camera.Flow_Width, Camera.Flow_Height), CV_8UC1, Scalar(1));
		flag_created = 1;
		if (Develop_Mode)
			Camera.Play_Flag = 0;
		else
			Camera.Camera_Stop();
		GetDlgItem(IDC_VIDEO)->ShowWindow(FALSE);
		Message = "摄像头已关闭";
		font.DeleteObject();
		font.CreatePointFont(120, _T("黑体"));
		GetDlgItem(IDC_Label2)->SetFont(&font);
		GetDlgItem(IDC_Label2)->SetWindowTextW(Message);
		static CFont tempfont;
		Message = "摄像头：关闭";
		tempfont.CreatePointFont(120, _T("黑体"));
		GetDlgItem(IDC_Label8)->SetFont(&tempfont);
		GetDlgItem(IDC_Label8)->SetWindowTextW(Message);
		GetDlgItem(IDC_BUTTON5)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
		GetDlgItem(IDC_Label1)->ShowWindow(TRUE);
	}
}

void CConstituencyDelineationAPPDlg::OnBnClickedButton4()
{
	CString Message, Title;
	Message = "是否确定注销？";
	Title = "注销";
	if (MessageBox(Message, Title, MB_ICONQUESTION | MB_OKCANCEL) == IDOK)
	{
		GetDlgItem(IDC_VIDEO)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT3)->SetWindowText(_T(""));
		GetDlgItem(IDC_EDIT4)->SetWindowText(_T(""));
		GetDlgItem(IDC_EDIT5)->SetWindowText(_T(""));
		GetDlgItem(IDC_EDIT3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT4)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT5)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON6)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON7)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON8)->EnableWindow(FALSE);
		GetDlgItem(IDC_SLIDER1)->EnableWindow(FALSE);
		GetDlgItem(IDC_Label1)->ShowWindow(TRUE);
		GetDlgItem(IDC_Label6)->ShowWindow(FALSE);
		GetDlgItem(IDC_Label7)->ShowWindow(FALSE);
		GetDlgItem(IDC_Label8)->ShowWindow(FALSE);
		GetDlgItem(IDC_Label9)->ShowWindow(FALSE);
		Camera.Camera_Logout();
		Camera.Camera_Clear();
		Message = "已注销!";
		MessageBox(Message, Title, MB_ICONASTERISK | MB_OK);
	}
	else
		return;
}

void CConstituencyDelineationAPPDlg::OnBnClickedButton5()
{
	CEdit* pBox;
	pBox = (CEdit*)GetDlgItem(IDC_EDIT1);
	CString Path;
	pBox->GetWindowText(Path);
	if (Path.Right(1) != '/' || Path.Right(1) != '\\')
		Path += '/';
	CString Message, Title;
	Message = "是否确定更改保存路径为";
	Title = "更改记录保存路径";
	Message.Format(_T("%s%s"), Message, Path);
	if (MessageBox(Message, Title, MB_ICONQUESTION | MB_OKCANCEL) == IDOK)
	{
		String Origin_Path;
		Origin_Path.assign(Record_Path);

		Record_Path = CT2A(Path.GetString());
		Record_Path = Record_Path + "Record/";

		if (_access(Record_Path.c_str(), 0))
			createDirectory(Record_Path);
		ofstream Pathout(ConfigFile);
		Pathout << Record_Path;
		Pathout.close();

		Message = "路径已更改!";
		MessageBox(Message, Title, MB_ICONASTERISK | MB_OK);

		CString cmd, dir;
		cmd = "explore";
		dir = Origin_Path.c_str();
		::ShellExecute(NULL, cmd, dir, NULL, NULL, SW_SHOWDEFAULT);
		dir = Record_Path.c_str();
		::ShellExecute(NULL, cmd, dir, NULL, NULL, SW_SHOWDEFAULT);
	}
	GetDlgItem(IDC_EDIT1)->SetWindowText(_T(""));
}

void CConstituencyDelineationAPPDlg::OnBnClickedButton6()
{
	CString Message;
	if (Detector.Detect_Mode)
	{
		Detector.Detect_Mode = 0;
		Message = "检测模式关闭！";
		font.DeleteObject();
		font.CreatePointFont(120, _T("黑体"));
		GetDlgItem(IDC_Label2)->SetFont(&font);
		GetDlgItem(IDC_Label2)->SetWindowTextW(Message);
		static CFont tempfont;
		Message = "检测模式：关闭";
		tempfont.CreatePointFont(120, _T("黑体"));
		GetDlgItem(IDC_Label7)->SetFont(&tempfont);
		GetDlgItem(IDC_Label7)->SetWindowTextW(Message);
		GetDlgItem(IDC_SLIDER1)->EnableWindow(FALSE);
	}
	else
	{
		Detector.Detect_Mode = 1;
		Message = "检测模式开启！";
		font.DeleteObject();
		font.CreatePointFont(120, _T("黑体"));
		GetDlgItem(IDC_Label2)->SetFont(&font);
		GetDlgItem(IDC_Label2)->SetWindowTextW(Message);
		static CFont tempfont;
		Message = "检测模式：开启";
		tempfont.CreatePointFont(120, _T("黑体"));
		GetDlgItem(IDC_Label7)->SetFont(&tempfont);
		GetDlgItem(IDC_Label7)->SetWindowTextW(Message);
		GetDlgItem(IDC_SLIDER1)->EnableWindow(TRUE);
	}
}

void CConstituencyDelineationAPPDlg::OnBnClickedButton7()
{
	CString Message;
	if (Paint_Flag)
	{
		Paint_Flag = 0;
		Message = "选区绘制关闭！";
		font.DeleteObject();
		font.CreatePointFont(120, _T("黑体"));
		GetDlgItem(IDC_Label2)->SetFont(&font);
		GetDlgItem(IDC_Label2)->SetWindowTextW(Message);
		static CFont tempfont;
		Message = "选区绘制：关闭";
		tempfont.CreatePointFont(120, _T("黑体"));
		GetDlgItem(IDC_Label6)->SetFont(&tempfont);
		GetDlgItem(IDC_Label6)->SetWindowTextW(Message);
	}
	else
	{
		Paint_Flag = 1;
		Message = "选区绘制开启！";
		font.DeleteObject();
		font.CreatePointFont(120, _T("黑体"));
		GetDlgItem(IDC_Label2)->SetFont(&font);
		GetDlgItem(IDC_Label2)->SetWindowTextW(Message);
		static CFont tempfont;
		Message = "选区绘制：开启";
		tempfont.CreatePointFont(120, _T("黑体"));
		GetDlgItem(IDC_Label6)->SetFont(&tempfont);
		GetDlgItem(IDC_Label6)->SetWindowTextW(Message);
	}
}

void CConstituencyDelineationAPPDlg::OnBnClickedButton8()
{
	CString Message;
	if (Detector.Ability_Mode)
	{
		Message = "高抗干扰检测模式";
		font.DeleteObject();
		font.CreatePointFont(120, _T("黑体"));
		GetDlgItem(IDC_Label2)->SetFont(&font);
		GetDlgItem(IDC_Label2)->SetWindowTextW(Message);
		static CFont tempfont;
		Message = "性能偏好：高抗干扰";
		tempfont.CreatePointFont(120, _T("黑体"));
		GetDlgItem(IDC_BUTTON8)->SetFont(&tempfont);
		GetDlgItem(IDC_BUTTON8)->SetWindowTextW(Message);
		Detector.Detect_Mode = 0;
		Detector.Ability_Mode = 0;
		Detector.Ability_Set();
		Detector.Detect_Mode = 1;
	}
	else
	{
		Message = "高识别率检测模式";
		font.DeleteObject();
		font.CreatePointFont(120, _T("黑体"));
		GetDlgItem(IDC_Label2)->SetFont(&font);
		GetDlgItem(IDC_Label2)->SetWindowTextW(Message);
		static CFont tempfont;
		Message = "性能偏好：高识别率";
		tempfont.CreatePointFont(120, _T("黑体"));
		GetDlgItem(IDC_BUTTON8)->SetFont(&tempfont);
		GetDlgItem(IDC_BUTTON8)->SetWindowTextW(Message);
		Detector.Detect_Mode = 0;
		Detector.Ability_Mode = 1;
		Detector.Ability_Set();
		Detector.Detect_Mode = 1;
	}
}

void CConstituencyDelineationAPPDlg::OnBnClickedButton9()
{
	Warning_Flag = 0;
	CString Message;
	Message = "警报已解除";
	font.DeleteObject();
	font.CreatePointFont(120, _T("黑体"));
	GetDlgItem(IDC_Label2)->SetFont(&font);
	GetDlgItem(IDC_Label2)->SetWindowTextW(Message);
	GetDlgItem(IDC_BUTTON9)->ShowWindow(FALSE);
	GetDlgItem(IDC_WARNING)->ShowWindow(FALSE);
}

void Video_Play_Thread(void*)
{
	while (Camera.Play_Flag == 1)
	{
		if (frame_detect.data)
		{
			current_frame = Mat(Size(Camera.Flow_Width, Camera.Flow_Height), CV_8UC3, Scalar(255, 0, 0));
			frame_detect.copyTo(current_frame, mask);

			Detector.Detect_DrawRect();

			if (Detector.Detect_Mode)
			{
				Working_Area_Colored();

				Transboundary_Alert();
			}

			filtered.clear();

			resize(current_frame, current_frame, Video_ShowSize);
			cv::imshow(WindowName, current_frame);
		}
		cv::waitKey(10);
	}
	return;
}

void Developer_Play_Thread(void*)
{
	VideoCapture cap(0);
	while (Camera.Play_Flag == 1)
	{
		cap >> frame_detect;
		waitKey(1);
	}
}

unsigned _stdcall Warning_Play_Thread(void*)
{
	Mat warningf;
	VideoCapture warn("warning.mp4");
	CConstituencyDelineationAPPDlg* pDlg = (CConstituencyDelineationAPPDlg*)g_pDlg;
	pDlg->GetDlgItem(IDC_BUTTON9)->ShowWindow(TRUE); 
	pDlg->GetDlgItem(IDC_WARNING)->ShowWindow(TRUE);
	CString Message;
	Message = "警报！目标越界！";
	font.DeleteObject();
	font.CreatePointFont(120, _T("黑体"));
	pDlg->GetDlgItem(IDC_Label2)->SetFont(&font);
	pDlg->GetDlgItem(IDC_Label2)->SetWindowTextW(Message);
	PlaySound(TEXT("2.WAV"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);//拉起警报声
	while (Warning_Flag)
	{
		warn >> warningf;
		if (warningf.empty())
		{
			warn.set(CAP_PROP_POS_FRAMES, 0);
			warn >> warningf;
		}
		resize(warningf, warningf, Size(228,180));
		cv::imshow("Warning!!!", warningf);
		cv::waitKey(60);
	}
	PlaySound(NULL, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	return 0;
}

void CConstituencyDelineationAPPDlg::OnIdok()
{
	// TODO: 在此添加命令处理程序代码
}

void CConstituencyDelineationAPPDlg::OnIdcancel()
{
	// TODO: 在此添加命令处理程序代码
}

void CConstituencyDelineationAPPDlg::OnClose()
{
	CString Message, Title;
	Message = "确定退出程序？";
	Title = "退出程序";
	if (MessageBox(Message, Title, MB_ICONQUESTION | MB_OKCANCEL) == IDOK)
	{
		cv::destroyAllWindows();
		EndDialog(IDCANCEL);
		CDialogEx::OnClose();
	}
}

void CConstituencyDelineationAPPDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CSliderCtrl* slider = (CSliderCtrl*)pScrollBar;
	if (slider = &m_sliderc)
	{
		CString Message;
		int Sens = m_sliderc.GetPos();
		Detector.Sensitivity = (float)Sens / 100.0;
		static CFont tempfont;
		Message.Format(_T("检测灵敏度：%d"), Sens);
		tempfont.CreatePointFont(120, _T("黑体"));
		GetDlgItem(IDC_Label9)->SetFont(&tempfont);
		GetDlgItem(IDC_Label9)->SetWindowTextW(Message);
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
