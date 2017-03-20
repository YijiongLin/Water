
// OpenGLDlgDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// COpenGLDlgDlg �Ի���
class COpenGLDlgDlg : public CDialogEx
{
// ����
public:
	COpenGLDlgDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_OPENGLDLG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
