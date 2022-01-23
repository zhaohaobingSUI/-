
// Constituency_Delineation_APPDlg.h: 头文件
//

#pragma once


// CConstituencyDelineationAPPDlg 对话框
class CConstituencyDelineationAPPDlg : public CDialogEx
{
// 构造
public:
	CConstituencyDelineationAPPDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONSTITUENCY_DELINEATION_APP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnEnChangeEdit4();
	afx_msg void OnEnChangeEdit5();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnIdok();
	afx_msg void OnIdcancel();
	afx_msg void OnClose();
	afx_msg void OnEnChangeEdit6();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
//	afx_msg void OnClickedLabel5();
//	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_sliderc;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
};
