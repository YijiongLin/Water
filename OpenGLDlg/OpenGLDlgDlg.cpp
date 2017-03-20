
// OpenGLDlgDlg.cpp : 实现文件
////
//  OpenGLDlgDlg.cpp
//  From : http://blog.csdn.net/linyijiong/article/details/53647250
//
//  Created by linyijiong on 2016/12/14.
//  Copyright  2016年 linyijiong. All rights reserved.
//

#include "stdafx.h"
#include "OpenGLDlg.h"
#include "OpenGLDlgDlg.h"
#include "afxdialogex.h"
#include <stdio.h>
#include <GL/glut.h>
#include <math.h> 
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include<iso646.h>
#include <windows.h>  
#include "Resource.h"


#define FREEGLUT_STATIC
#define MAX_CHAR 128  // 申请MAX_CHAR个连续的显示列表编号，用于字符显示

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
using namespace std;
const GLfloat Pi = 3.1415926536f;
bool getTxtData(float** b, const int maxrow, const int maxcol, char* cwd);//const经常用来修饰一个值不能改变的量，安全，并且能消除存储操作
//编写函数getData()将键盘输入的一行任意个数以空格分隔的数值数据读到一个数组中。
//(提示：可事先输入数据的个数.getData()函数应返回2个值，一个是存放输入的一组数的堆内存首地址，另一个是数据的个数)
char*cwd;
int const  getMaxRow(char* cwd);

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)

END_MESSAGE_MAP()


// COpenGLDlgDlg 对话框



COpenGLDlgDlg::COpenGLDlgDlg(CWnd* pParent /*=NULL*/) //COpenGLDlgDlg类的构造函数
	: CDialogEx(COpenGLDlgDlg::IDD, pParent)
	, m_nTimer(0)

	, m_speed1(0)
	, m_speed2(0)
	, speed1(0)
	, speed2(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nTimer = 0; //数据初始化为0

}

void COpenGLDlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_editMultiLine);


	DDX_Text(pDX, IDC_EDIT4, m_speed1); //给编辑框添加变量（在资源视图中的编辑框中右击，然后添加变量）
	DDX_Text(pDX, IDC_EDIT5, m_speed2); 
}

BEGIN_MESSAGE_MAP(COpenGLDlgDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT1, &COpenGLDlgDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON2, &COpenGLDlgDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &COpenGLDlgDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON4, &COpenGLDlgDlg::OnBnClickedButton4)
	ON_EN_CHANGE(IDC_LOCALTIMER, &COpenGLDlgDlg::OnEnChangeLocaltimer)
	ON_WM_TIMER()

	ON_BN_CLICKED(IDC_BUTTON9, &COpenGLDlgDlg::OnBnClickedButton9)
END_MESSAGE_MAP()


// COpenGLDlgDlg 消息处理程序

BOOL COpenGLDlgDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	SetWindowText(_T("水刀切割功能软件")); //设置对话框标题
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	m_editMultiLine.SetWindowText(_T("txt文件路径"));//设置只读编辑框的默认文字
	CEdit*  pEdit4 = (CEdit*)GetDlgItem(IDC_EDIT4);//获取相应的编辑框ID
	pEdit4->SetWindowText(_T("")); //设置默认显示的内容 
	CEdit*  pEdit5 = (CEdit*)GetDlgItem(IDC_EDIT5);//获取相应的编辑框ID
	pEdit5->SetWindowText(_T("")); //设置默认显示的内容 
	// TODO:  在此添加额外的初始化代码
	//设置设备描述表与颜色描述表
	CWnd *pWnd = GetDlgItem(IDC_STATIC);
	hDC = ::GetDC(pWnd->GetSafeHwnd());       //获得指定的设备描述表的手柄

	if (SetWindowPixelFormat(hDC) == FALSE)
		return 0;
	if (CreateViewGLRC(hDC) == FALSE)
		return 0;

	SetTimer(1, 1000, NULL); //启动ID为1的定时器，定时时间为1秒
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void COpenGLDlgDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void COpenGLDlgDlg::OnPaint()
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
	//设置视角，并使得都窗口改变时图形不变
	Reshape();

}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR COpenGLDlgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void selectFont(int size, int charset, const char* face) //字体选择函数的声明，字体大小、字符集、字体名称，可查书看参数定义
{
	HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0,
		charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
	HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
	DeleteObject(hOldFont);
}

void drawString(const char* str) //屏幕显示字体  
{
	static int isFirstCall = 1;
	static GLuint lists;

	if (isFirstCall) {
		isFirstCall = 0;
		// 申请MAX_CHAR个连续的显示列表编号  
		lists = glGenLists(MAX_CHAR);
		// 把每个字符的绘制命令都装到对应的显示列表中  
		wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
	}
	// 调用每个字符对应的显示列表，绘制每个字符  
	for (; *str != '\0'; ++str) {
		glCallList(lists + *str);
	}
}



void COpenGLDlgDlg::Reshape()
{
	//获取窗口大小
	CRect rect;
	GetDlgItem(IDC_STATIC)->GetWindowRect(rect);//GetDlgItem返回窗口中指定参数ID的子元素的句柄，可以通过返回的句柄对窗口内的子元素进行操作。
	//根据窗口大小，调整视口大小
	glViewport(0, 0, rect.Width(), rect.Height());// 这里要设置opengl在窗口的绘图区域，指定视口大小与对话框客户区大小一致
	glMatrixMode(GL_PROJECTION); 	// 一般只在窗口reshape时才重新设定下投影矩阵和模型矩阵
	glLoadIdentity();               //先初始化窗口，然后重置投影矩阵，最后回到模型矩阵

	//正射投影	 	
	if (rect.Width() <= rect.Height())
		gluOrtho2D(-70.0, 870.0, -80.0*(GLfloat)rect.Height() / (GLfloat)rect.Width(),
		880.0*(GLfloat)rect.Height() / (GLfloat)rect.Width());
	else
		gluOrtho2D(-70.0*(GLfloat)rect.Width() / (GLfloat)rect.Height(),
		870.0*(GLfloat)rect.Width() / (GLfloat)rect.Height(), -80.0, 880.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}




//设置、选择像素格式描述表

bool COpenGLDlgDlg::SetWindowPixelFormat(HDC hDC) 
{
	static PIXELFORMATDESCRIPTOR pixelDesc;          //typedef struct tagPIXELFORMATDESCRIPTOR是一个数据结构，已经有windows95定义好，直接用即可 
	pixelDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR); //初始化像素格式结构
	pixelDesc.nVersion = 1;
	pixelDesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL  | PFD_STEREO_DONTCARE;
	pixelDesc.iPixelType = PFD_TYPE_RGBA;
	pixelDesc.cColorBits = 32;
	pixelDesc.cRedBits = 8;
	pixelDesc.cRedShift = 16;
	pixelDesc.cGreenBits = 8;
	pixelDesc.cGreenShift = 8;
	pixelDesc.cBlueBits = 8;
	pixelDesc.cBlueShift = 0;
	pixelDesc.cAlphaBits = 0;
	pixelDesc.cAlphaShift = 0;
	pixelDesc.cAccumBits = 64;
	pixelDesc.cAccumRedBits = 16;
	pixelDesc.cAccumGreenBits = 16;
	pixelDesc.cAccumBlueBits = 16;
	pixelDesc.cAccumAlphaBits = 0;
	pixelDesc.cDepthBits = 32;
	pixelDesc.cStencilBits = 8;
	pixelDesc.cAuxBuffers = 0;
	pixelDesc.iLayerType = PFD_MAIN_PLANE;
	pixelDesc.bReserved = 0;
	pixelDesc.dwLayerMask = 0;
	pixelDesc.dwVisibleMask = 0;
	pixelDesc.dwDamageMask = 0;

	m_GLPixelIndex = ChoosePixelFormat(hDC, &pixelDesc);//为设备描述表选择最匹配的像素格式 ，返回其索引。m_GLPixelIndex作为该像素表格的索引（选择符）  
	                                                    //https://msdn.microsoft.com/en-us/library/dd318284(VS.85).aspx
	                                                    //Return Value :If the function succeeds, the return value is a pixel format index (one-based) 
	                                                    //that is the closest match to the given pixel format descriptor.
	                                                    //If the function fails, the return value is zero.To get extended error information, call GetLastError.
	if (m_GLPixelIndex == 0)
	{
		m_GLPixelIndex = 1;
		if (DescribePixelFormat(hDC, m_GLPixelIndex, sizeof(PIXELFORMATDESCRIPTOR), &pixelDesc) == 0)// 当 ChoosePixelFormat失效时，使用DescribePixelFormat获得最匹配的绘图像素格式
			                                                                                         //https://msdn.microsoft.com/zh-cn/office/ms537573
		{
			return FALSE;//不能选择一种相匹配的像素格式
		}
	}
	if (SetPixelFormat(hDC, m_GLPixelIndex, &pixelDesc) == FALSE)//将指定的DC设置为由ChoosePixelFormat确定的像素格式
	{
		return FALSE;//不能设置匹配的像素格式
	}

	return TRUE;
}

//bool COpenGLDlgDlg::CreatViewGLRC(HDC hDC)
//{
//	return false;
//}


//将设备描述表和渲染描述表联系
bool COpenGLDlgDlg::CreateViewGLRC(HDC hDC)//HDC为设备描述表句柄。
                                           //在Windows环境中，句柄是用来标识被应用程序所建立或使用的对象的唯一整数
{
	m_hGLRC = wglCreateContext(hDC); //函数创建一个新的OpenGL渲染描述表RC,此描述表必须适用于绘制到由hdc返回的设备。
	                                   //这个渲染描述表将有和设备上下文(dc)一样的像素格式。
	                                   //hdc用于函数将要创建的适合的OpenGL渲染描述表的设备上下文(dc)句柄.
		                               //Return Values如果函数调用成功, 返回有效的OpenGL渲染描述表句柄.否则返回NULL.
	if (m_hGLRC == NULL)
		return FALSE;
	if (wglMakeCurrent(hDC, m_hGLRC) == FALSE)//wglMakeCurrent将RC设置成当前RC
		return FALSE;

	return TRUE;
}


void COpenGLDlgDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void COpenGLDlgDlg::OnBnClickedButton2()//Draw 静态画图，坐标轴
{
	//glClear(GL_COLOR_BUFFER_BIT);

	selectFont(26, ANSI_CHARSET, "Comic Sans MS"); //字体大小、字符集、字体名称，
	glColor3f(0.0f, 1.0f, 0.0f);     //设置字体颜色  
	glRasterPos2i(-30, 850);  //起始位置  
	drawString("Y");   //输出的字符串  
	glColor3f(0.0f, 1.0f, 0.0f);     //设置字体颜色  
	glRasterPos2i(850, -35);  //起始位置  
	drawString("X");   //输出的字符串  

	for (int l = 0; l < 9; l++) //坐标刻度
	{
		glColor3f(0, 1, 0);
		glLineWidth(2);
		glBegin(GL_LINES);
		glVertex2d(-10, l * 100);
		glVertex2d(0, l * 100);
		glVertex2d(l * 100, -10);
		glVertex2d(l * 100, 0);
		glEnd();

	}



	glColor3f(0.0f, 1.0f, 0.0f);     //设置字体颜色  
	glRasterPos2i(-25, -20);  //起始位置  
	drawString("0");   //输出的字符串  

	glRasterPos2i(-55, 95);  //起始位置  
	drawString("100");   //输出的字符串  

	glRasterPos2i(-55, 195);  //起始位置  
	drawString("200");   //输出的字符串  

	glRasterPos2i(-55, 295);  //起始位置  
	drawString("300");   //输出的字符串  

	glRasterPos2i(-55, 395);  //起始位置  
	drawString("400");   //输出的字符串  

	glRasterPos2i(-55, 495);  //起始位置  
	drawString("500");   //输出的字符串  

	glRasterPos2i(-55, 595);  //起始位置  
	drawString("600");   //输出的字符串  

	glRasterPos2i(-55, 695);  //起始位置  
	drawString("700");   //输出的字符串  

	glRasterPos2i(-55, 795);  //起始位置  
	drawString("800");   //输出的字符串  

	glRasterPos2i(80, -35);  //起始位置  
	drawString("100");   //输出的字符串  

	glRasterPos2i(180, -35);  //起始位置  
	drawString("200");   //输出的字符串  

	glRasterPos2i(280, -35);  //起始位置  
	drawString("300");   //输出的字符串  

	glRasterPos2i(380, -35);  //起始位置  
	drawString("400");   //输出的字符串  

	glRasterPos2i(480, -35);  //起始位置  
	drawString("500");   //输出的字符串  

	glRasterPos2i(580, -35);  //起始位置  
	drawString("600");   //输出的字符串  

	glRasterPos2i(680, -35);  //起始位置  
	drawString("700");   //输出的字符串  

	glRasterPos2i(780, -35);  //起始位置  
	drawString("800");   //输出的字符串  

	glColor3f(0, 1, 0);//以下为坐标轴箭头
	glLineWidth(2);
	glBegin(GL_LINES);
	glVertex2d(0, 0);
	glVertex2d(0, 850);
	glVertex2d(0, 0);
	glVertex2d(850, 0);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex2d(-6, 835);
	glVertex2d(6, 835);
	glVertex2d(0, 850);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex2d(835, -6);
	glVertex2d(835, 6);
	glVertex2d(850, 0);
	glEnd();



	//为了方便，将当前工作地址用cwd来代替，这样方便后续的编程
	//将文件数据读入二维数组中
	const int maxrow = getMaxRow(cwd);//实现对矩阵的各行取最大值，对角线取最大值，并用函数返回指针值
	const int maxcol = 6;

	float** b = new float*[maxrow];   //对于生成二维及更高维的数组，应使用多维指针。
	//以二维指针为例
	//int **p=new int* [row]; //row是二维数组的行，*p是指向一个指针数组的指针
	//for(int i=0; i<row; i++)
	//p[i]=new int [col]; //col是二维数组的列，p是指向一个int数组的指针
	for (int i = 0; i<maxrow; i++){
		b[i] = new float[maxcol];
	}

	//这个for语句是用new申请完数组加的一段初始化，若无，则无数据的数组元素位会显示乱码
	for (int i = 0; i < maxrow; i++){  
		for (int j = 0; j < maxcol; j++)
			b[i][j] = 0;
	}
	//开始读取txt数据到二维数组b
	if (getTxtData(b, maxrow, maxcol, cwd)){

	}

	double x[1000], y[1000];
	int  j, n;



	for (n = 0; n<maxrow; n++)
	{

		glPointSize(5);


		//若为空行程，虚线段表示
		if (b[n][0] == 4){

			glLineWidth(2);             //静态虚线线粗
			glColor3f(1, 1, 1);
			glEnable(GL_LINE_STIPPLE);
			glLineStipple(2, 0x0F0F);//设置虚线的样式  
			glBegin(GL_LINES);//在下面2个点之间画一条虚线
			glVertex2d(b[n][1], b[n][2]);
			glVertex2d(b[n][3], b[n][4]);
			glEnd();
			glDisable(GL_LINE_STIPPLE);

			float a[1000][5];//给空行程的虚线段箭头建立一个二维数组，用于储存箭头几何信息

			if (b[n][4] < b[n][2] && b[n][3] < b[n][1] or b[n][4] > b[n][2] && b[n][3] < b[n][1]) //若线段为第三象限
			{
				a[n][4] = atan((b[n][4] - b[n][2]) / (b[n][3] - b[n][1]));//虚线的倾斜角度
				a[n][1] = b[n][4] + 20 * sin(a[n][4] - 15 * Pi / 180);  //左边箭头点
				a[n][0] = b[n][3] + 20 * cos(a[n][4] - 15 * Pi / 180);
				glColor3f(1, 0, 1);
				glLineWidth(4);
				glBegin(GL_LINES);
				glVertex2d(b[n][3], b[n][4]);
				glVertex2d(a[n][0], a[n][1]);
				glEnd();
				a[n][3] = b[n][4] + 20 * sin(a[n][4] + 15 * Pi / 180);  //右边箭头点
				a[n][2] = b[n][3] + 20 * cos(a[n][4] + 15 * Pi / 180);
				glColor3f(1, 0, 1);
				glBegin(GL_POLYGON);
				glVertex2d(b[n][3], b[n][4]);
				glVertex2d(a[n][2], a[n][3]);
				glVertex2d(a[n][2], a[n][3]);
				glVertex2d(a[n][0], a[n][1]);
				glVertex2d(b[n][3], b[n][4]);
				glVertex2d(a[n][0], a[n][1]);
				glEnd();
			}
			else
			{
				a[n][4] = atan((b[n][4] - b[n][2]) / (b[n][3] - b[n][1]));//虚线的倾斜角度
				a[n][1] = b[n][4] - 20 * sin(a[n][4] - 15 * Pi / 180);  //左边箭头点
				a[n][0] = b[n][3] - 20 * cos(a[n][4] - 15 * Pi / 180);
				glColor3f(1, 0, 1);
				glLineWidth(2);
				glBegin(GL_LINES);
				glVertex2d(b[n][3], b[n][4]);
				glVertex2d(a[n][0], a[n][1]);
				glEnd();
				a[n][3] = b[n][4] - 20 * sin(a[n][4] + 15 * Pi / 180);  //右边箭头点
				a[n][2] = b[n][3] - 20 * cos(a[n][4] + 15 * Pi / 180);
				glColor3f(1, 0, 1);
				glLineWidth(2);
				glBegin(GL_POLYGON);
				glVertex2d(b[n][3], b[n][4]);
				glVertex2d(a[n][2], a[n][3]);
				glVertex2d(a[n][2], a[n][3]);
				glVertex2d(a[n][0], a[n][1]);
				glVertex2d(b[n][3], b[n][4]);
				glVertex2d(a[n][0], a[n][1]);
				glEnd();
			}

		}

		//若为直线段
		else if (b[n][0] == 1)
		{
			glColor3f(1, 1, 0);
			glLineWidth(2);
			glBegin(GL_LINES);//在下面2个点之间画一条虚线
			glVertex2d(b[n][1], b[n][2]);
			glVertex2d(b[n][3], b[n][4]);
			glEnd();
		}

		//若为圆弧段
		else if (b[n][0] == 2)
		{

			glLineWidth(2);  //静态圆弧线粗
			for (j = 1; j < 1000; j++)
			{
				glColor3f(1, 1, 0);

				x[0] = b[n][4] + b[n][3] * cos(b[n][1] * Pi / 180);
				y[0] = b[n][5] + b[n][3] * sin(b[n][1] * Pi / 180);
				x[j] = b[n][4] + b[n][3] * cos((b[n][1] + (b[n][2])  * j / 1000)*Pi / 180);
				y[j] = b[n][5] + b[n][3] * sin((b[n][1] + (b[n][2]) * j / 1000)*Pi / 180);
				glBegin(GL_LINES);
				glVertex2d(x[j - 1], y[j - 1]);
				glVertex2d(x[j], y[j]);
				glEnd();

			}

		}
	}
	for (int i = 0; i < maxrow; i++){ delete[]b[i]; }
	delete[]b;


	glFlush();
}



void COpenGLDlgDlg::OnBnClickedButton1()
{

	CFileDialog fDlg(TRUE, _TEXT("txt"), NULL, 4 | 2,
		_TEXT("文本文件(*.txt,*.ini,*.log)|*.txt;*.ini;*.log|全部文件(*.*)|*.*||"));//定义一个对象，用于打开文件(此时并没打开)，主要是获得文件路径

	// 调用 DoModal() 方法来打开对话框
	if (fDlg.DoModal() == IDOK)
	{
		// 设置编辑框1的内容为选择文件的路径  
		// GetPathName() --> 返回选定文件的完整路径  
		m_editMultiLine.SetSel(1, 20);                            // 选择起始索引为1，终止索引为20（不包括在选择范围内）的正文，即“博客”   
		m_editMultiLine.ReplaceSel(_T(""));  // 将文件路径显示框设置默认内容为空   
		SetDlgItemText(IDC_EDIT1, fDlg.GetPathName());
		int len = WideCharToMultiByte(CP_ACP, 0, fDlg.GetPathName(), -1, NULL, 0, NULL, NULL);// Unicode编码模式 CString-->char *
		cwd = new char[len + 1];
		WideCharToMultiByte(CP_ACP, 0, fDlg.GetPathName(), -1, cwd, len, NULL, NULL);


	}


}

const int getMaxRow(char* cwd){  //此函数返回值为number，为txt文件中的总行数
	ifstream txtFile(cwd);//从当前文件地址中输入文件流到txrFile中
	if (!txtFile.is_open())//用来检查文件是否被打开：is_open方法。成功打开，其值为1
	{
		cout << "Error opening file"; exit(1);//退出该函数
	}
	string buffer; //访问字符操作string可以按数组方式，以下标来访问。还可以用at()函数访问指定的字符
	int number = 0;
	while (!txtFile.eof())//eof函数 ，它是ifstream 从类 ios 中继承过来的，当到达文件末尾时返回true 。(end of file）
	{
		getline(txtFile, buffer);//这里的getline通过和eof一起作用，作为一个最大行数的终止条件
		//（getline是从txtFile里面得到一行字符并输入到buffer中）
		number++;
	}
	return number;//若txt文档中最后一行为空行，则返回值为当前读取到的行数减一，
	//因为数组中的排序是从0而不是1开始，需要减一。
}

bool getTxtData(float** b, const int maxrow, const int maxcol, char* cwd){ //读取文本中的二维数组
	ifstream txtFile(cwd);//以输入方式打开文件，格式为ifstream file("地址")。char* cwd = "新建文本文档2.txt"
	if (!txtFile)return false;

	string buffer;       //访问字符操作string可以按数组方式，以下标来访问。还可以用at()函数访问指定的字符
	for (int i = 0; i<maxrow; i++){
		getline(txtFile, buffer);
		istringstream is(buffer);
		for (int j = 0; j<maxcol; j++){
			is >> b[i][j];

		}
	}

	return true;
}













void COpenGLDlgDlg::OnBnClickedButton4()
{
	// TODO:  在此添加控件通知处理程序代码
	glClear(GL_COLOR_BUFFER_BIT); //擦黑板，清空画板像素
	glFlush();

}


void COpenGLDlgDlg::OnEnChangeLocaltimer()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void COpenGLDlgDlg::OnTimer(UINT_PTR nIDEvent)
{
	CTime t = CTime::GetCurrentTime();     //获取系统时间
	CString strTime = t.Format(_T("%Y-%m-%d %H:%M:%S"));    //显示系统时间
	//在界面上加一个编辑控件或者静态文本控件，再添加代码将 strTime 显示到控件中。
	//以上不能加在
	switch (nIDEvent)
	{
	case 1:

		SetDlgItemText(IDC_LOCALTIMER, strTime);
		break;

	default:
		break;
	}


	CDialogEx::OnTimer(nIDEvent);

}



void COpenGLDlgDlg::OnBnClickedButton9() //动态画图
{
	UpdateData(TRUE);
	speed1 = 60000 / m_speed1;
	speed2 = 60000 / m_speed2;
	UpdateData(FALSE);

	const int maxrow = getMaxRow(cwd);//实现对矩阵的各行取最大值，对角线取最大值，并用函数返回指针值
	const int maxcol = 6;

	float** b = new float*[maxrow];   //对于生成二维及更高维的数组，应使用多维指针。
	//以二维指针为例
	//int **p=new int* [row]; //row是二维数组的行，*p是指向一个指针数组的指针
	//for(int i=0; i<row; i++)
	//p[i]=new int [col]; //col是二维数组的列，p是指向一个int数组的指针
	for (int i = 0; i<maxrow; i++){
		b[i] = new float[maxcol];
	}
	for (int i = 0; i < maxrow; i++){  //这个for语句是用new申请完数组加的一段初始化，若无，则无数据的数组元素位会显示乱码（因为没定义)
		for (int j = 0; j < maxcol; j++)
			b[i][j] = 0;
	}

	if (getTxtData(b, maxrow, maxcol, cwd)){
		/*for (int i = 0; i<maxrow; i++){
			for (int j = 0; j<maxcol; j++)
				cout << b[i][j] << "  ";
			cout << "\n";
		}*/
	}


	int  n;



	for (n = 0; n<maxrow; n++)
	{

		glPointSize(5);


		//若为空行程，虚线段表示  蓝色
		if (b[n][0] == 4)
		{

			glLineWidth(2);
			glColor3f(0, 0, 1);                 //动态空行程的线粗

			if (b[n][1] != b[n][3])										//如果斜率存在
			{
				GLfloat k = (b[n][4] - b[n][2]) / (b[n][3] - b[n][1]);					//直线斜率 y=kx+c
				GLfloat c = (b[n][3] * b[n][2] - b[n][1] * b[n][4]) / (b[n][3] - b[n][1]);			//直线常数
				float zjb1 = b[n][3] - b[n][1];           //直角边x
				float zjb2 = b[n][4] - b[n][2];            //直角边y
				float xb = sqrt(zjb1*zjb1 + zjb2*zjb2);    //斜边

				//假定以A为原点建立二维坐标系，则下边4个if分别对应于：第一象限，第二象限，第三象限，第四象限


					//float cta = atan(zjb2 / zjb1);             //倾斜角
					for (int i = 0; i <= (xb+1); i++)			 	//若xb不+1，则因为for的判断语句中i为整型，缺失1
					{
						glEnable(GL_LINE_STIPPLE);
						glBegin(GL_LINES);
						glVertex2f(b[n][1], b[n][2]);
						glVertex2f(b[n][1] + i*zjb1 / xb, k*(b[n][1] + i*zjb1 / xb) + c);
						glEnd();
						glDisable(GL_LINE_STIPPLE);
						Sleep(speed1);
						glFlush();							//立刻开始处理保存在缓冲区中的OpenGL函数调用
					}
				

			}
			else
			{
				if (b[n][2]<b[n][4])									//沿y轴正方向画线
				{
					for (int i = 0; i <= b[n][4] - b[n][2]; i++)				//特别注意循环条件，既要避免死循环又要避免少循环
					{
						glBegin(GL_LINES);
						glVertex2f(b[n][1], b[n][2]);
						glVertex2f(b[n][1], b[n][2] + i);
						glEnd();
						Sleep(speed1);
						glFlush();							//立刻开始处理保存在缓冲区中的OpenGL函数调用
					}
				}
				else										//沿y轴负方向画线
				{
					for (int i = 0; i >= b[n][4] - b[n][2]; i--)				//特别注意循环条件，既要避免死循环又要避免少循环
					{
						glBegin(GL_LINES);
						glVertex2f(b[n][1], b[n][2]);
						glVertex2f(b[n][1], b[n][2] + i);
						glEnd();
						Sleep(speed1);
						glFlush();							//立刻开始处理保存在缓冲区中的OpenGL函数调用
					}
				}
			}

			float a[1000][5];//给空行程的虚线段箭头建立一个二维数组，用于储存箭头几何信息

			if (b[n][4] < b[n][2] && b[n][3] < b[n][1] or b[n][4] > b[n][2] && b[n][3] < b[n][1]) //若线段为第三象限
			{
				a[n][4] = atan((b[n][4] - b[n][2]) / (b[n][3] - b[n][1]));//虚线的倾斜角度
				a[n][1] = b[n][4] + 20 * sin(a[n][4] - 15 * Pi / 180);  //左边箭头点
				a[n][0] = b[n][3] + 20 * cos(a[n][4] - 15 * Pi / 180);
				glColor3f(0, 0, 1);
				glLineWidth(4);
				glBegin(GL_LINES);
				glVertex2d(b[n][3], b[n][4]);
				glVertex2d(a[n][0], a[n][1]);
				glEnd();
				a[n][3] = b[n][4] + 20 * sin(a[n][4] + 15 * Pi / 180);  //右边箭头点
				a[n][2] = b[n][3] + 20 * cos(a[n][4] + 15 * Pi / 180);
				glColor3f(0, 0, 1);
				glBegin(GL_POLYGON);
				glVertex2d(b[n][3], b[n][4]);
				glVertex2d(a[n][2], a[n][3]);
				glVertex2d(a[n][2], a[n][3]);
				glVertex2d(a[n][0], a[n][1]);
				glVertex2d(b[n][3], b[n][4]);
				glVertex2d(a[n][0], a[n][1]);
				glEnd();
			}
			else
			{
				a[n][4] = atan((b[n][4] - b[n][2]) / (b[n][3] - b[n][1]));//虚线的倾斜角度
				a[n][1] = b[n][4] - 20 * sin(a[n][4] - 15 * Pi / 180);  //左边箭头点
				a[n][0] = b[n][3] - 20 * cos(a[n][4] - 15 * Pi / 180);
				glColor3f(0, 0, 1);
				glLineWidth(2);
				glBegin(GL_LINES);
				glVertex2d(b[n][3], b[n][4]);
				glVertex2d(a[n][0], a[n][1]);
				glEnd();
				a[n][3] = b[n][4] - 20 * sin(a[n][4] + 15 * Pi / 180);  //右边箭头点
				a[n][2] = b[n][3] - 20 * cos(a[n][4] + 15 * Pi / 180);
				glColor3f(0, 0, 1);
				glLineWidth(2);
				glBegin(GL_POLYGON);
				glVertex2d(b[n][3], b[n][4]);
				glVertex2d(a[n][2], a[n][3]);
				glVertex2d(a[n][2], a[n][3]);
				glVertex2d(a[n][0], a[n][1]);
				glVertex2d(b[n][3], b[n][4]);
				glVertex2d(a[n][0], a[n][1]);
				glEnd();
			}

		}


		//实切直线段 红色
		else if (b[n][0] == 1)    
		{
			GLfloat k = (b[n][4] - b[n][2]) / (b[n][3] - b[n][1]);					//直线斜率 y=kx+c
			GLfloat c = (b[n][3] * b[n][2] - b[n][1] * b[n][4]) / (b[n][3] - b[n][1]);			//直线常数
			float zjb1 = b[n][3] - b[n][1];           //直角边x
			float zjb2 = b[n][4] - b[n][2];            //直角边y
			float xb = sqrt(zjb1*zjb1 + zjb2*zjb2);    //斜边

			glLineWidth(2);                         //动态直线的线粗
			glColor3f(1, 0, 0);                     

			if (b[n][1] != b[n][3])										//如果斜率存在
			{
				GLfloat k = (b[n][4] - b[n][2]) / (b[n][3] - b[n][1]);					//直线斜率 y=kx+c
				GLfloat c = (b[n][3] * b[n][2] - b[n][1] * b[n][4]) / (b[n][3] - b[n][1]);			//直线常数
				float zjb1 = b[n][3] - b[n][1];           //直角边x
				float zjb2 = b[n][4] - b[n][2];            //直角边y
				float xb = sqrt(zjb1*zjb1 + zjb2*zjb2);    //斜边

				//假定以A为原点建立二维坐标系，则下边4个if分别对应于：第一象限，第二象限，第三象限，第四象限


				//float cta = atan(zjb2 / zjb1);             //倾斜角
				for (int i = 0; i <= (xb+1); i++)			 	//注意循环条件
				{
					glBegin(GL_LINES);
					glVertex2f(b[n][1], b[n][2]);
					glVertex2f(b[n][1] + i*zjb1 / xb, k*(b[n][1] + i*zjb1 / xb) + c);
					glEnd();
					Sleep(speed2);
					glFlush();							//立刻开始处理保存在缓冲区中的OpenGL函数调用
				}


			}
			else
			{
				if (b[n][2]<b[n][4])									//沿y轴正方向画线
				{
					for (int i = 0; i <= b[n][4] - b[n][2]; i++)				//特别注意循环条件，既要避免死循环又要避免少循环
					{
						glBegin(GL_LINES);
						glVertex2f(b[n][1], b[n][2]);
						glVertex2f(b[n][1], b[n][2] + i);
						glEnd();
						Sleep(speed2);
						glFlush();							//立刻开始处理保存在缓冲区中的OpenGL函数调用
					}
				}
				else										//沿y轴负方向画线
				{
					for (int i = 0; i >= b[n][4] - b[n][2]; i--)				//特别注意循环条件，既要避免死循环又要避免少循环
					{
						glBegin(GL_LINES);
						glVertex2f(b[n][1], b[n][2]);
						glVertex2f(b[n][1], b[n][2] + i);
						glEnd();
						Sleep(speed2);
						glFlush();							//立刻开始处理保存在缓冲区中的OpenGL函数调用
					}
				}
			}


		}

		else if (b[n][0] == 2)    
		{
			int arci=1;
			int arc = fabs(b[n][2]) / 180 * Pi*b[n][3]; //该段弧长值，fabs取绝对值
			float * xarc = new float [arc+1]; //数组的个数为（arc+1）-1=arc个
			float * yarc = new float [arc+1];

			glLineWidth(2);              //动态圆弧线粗         
			glColor3f(1, 0, 0);

			for (arci = 1; arci <= arc; arci++) //因为这里arci最大值为arc,所以要用到arc个元素。
			                                  	//因此上面申请动态数组的时候，[]中必须为arc+1,这样申请到的元素总数为arc个才够用
			{
				

				xarc[0] = b[n][4] + b[n][3] * cos(b[n][1] * Pi / 180);
				yarc[0] = b[n][5] + b[n][3] * sin(b[n][1] * Pi / 180);
				xarc[arci] = b[n][4] + b[n][3] * cos((b[n][1] + (b[n][2])  * arci / arc)*Pi / 180);
				yarc[arci] = b[n][5] + b[n][3] * sin((b[n][1] + (b[n][2]) * arci / arc)*Pi / 180);
				glBegin(GL_LINES);
				glVertex2d(xarc[arci-1], yarc[arci-1]);
				glVertex2d(xarc[arci], yarc[arci]);
				glEnd();
				Sleep(speed2);
				glFlush();	
			}
			delete[]xarc;
			delete[]yarc;

		}
	}
	for (int i = 0; i < maxrow; i++){ delete[]b[i]; }
	delete[]b;


	glFlush();

}

