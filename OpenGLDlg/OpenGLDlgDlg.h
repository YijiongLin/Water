
// OpenGLDlgDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// COpenGLDlgDlg 对话框
class COpenGLDlgDlg : public CDialogEx
{
// 构造
public:
	COpenGLDlgDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_OPENGLDLG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
protected:
	HDC hDC;
	HGLRC m_hGLRC;
	int m_GLPixelIndex;
	void Reshape();
	void DrawOpenGL();
	bool SetWindowPixelFormat(HDC hDC);
//	bool CreatViewGLRC(HDC hDC);
	bool CreateViewGLRC(HDC hDC);
public:
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	CEdit m_editMultiLine;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnEnChangeLocaltimer();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	int m_nTimer;

	int m_speed1;
	int m_speed2;
	afx_msg void OnBnClickedButton9();
	int speed1;
	int speed2;
};
