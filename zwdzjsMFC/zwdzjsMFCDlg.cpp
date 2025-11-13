
// zwdzjsMFCDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "zwdzjsMFC.h"
#include "zwdzjsMFCDlg.h"
#include "afxdialogex.h"
#include <tlhelp32.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CzwdzjsMFCDlg 对话框



CzwdzjsMFCDlg::CzwdzjsMFCDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ZWDZJSMFC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CzwdzjsMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CzwdzjsMFCDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CzwdzjsMFCDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CzwdzjsMFCDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CzwdzjsMFCDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CzwdzjsMFCDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CzwdzjsMFCDlg::OnBnClickedButton5)
END_MESSAGE_MAP()

HWND g_hwnd = 0;
uintptr_t g_dllBase = 0;
DWORD  g_pid = 0;
uintptr_t 获取DLL地址(HWND hwnd, LPCWCHAR dllName) {

	if (!hwnd || dllName == nullptr)
	{
		return 0;
	}
	DWORD pid;
	GetWindowThreadProcessId(hwnd, &pid);
	g_pid = pid;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
	if (hSnapshot == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	MODULEENTRY32W moduleEntry;
	moduleEntry.dwSize = sizeof(MODULEENTRY32W);
	if (Module32FirstW(hSnapshot, &moduleEntry))
	{
		do {
			LPCWCHAR dllN = moduleEntry.szModule;
			if (_wcsicmp(dllN, dllName) == 0)
			{
				CloseHandle(hSnapshot);
				return (uintptr_t)moduleEntry.modBaseAddr;
			}
		} while (Module32NextW(hSnapshot, &moduleEntry));


	}
	CloseHandle(hSnapshot);
	return 0;
};


VOID init() {
	g_hwnd = FindWindow(NULL, L"PlantsVsZombiesRH");
	if (!g_hwnd)
	{
		MessageBox(NULL, L"未找到进程", NULL, NULL);
		return;
	}
	g_dllBase = 获取DLL地址(g_hwnd, L"GameAssembly.dll");
	if (!g_dllBase)
	{
		MessageBox(NULL, L"未找到模块", NULL, NULL);
		return;
	}
};


// CzwdzjsMFCDlg 消息处理程序

BOOL CzwdzjsMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	init();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CzwdzjsMFCDlg::OnPaint()
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
HCURSOR CzwdzjsMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

DWORD64 获取阳光值() {
	//
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, g_pid);
	PDWORD addr = (PDWORD)(g_dllBase + 0x01F34728);
	DWORD64 cache = 0;
	ReadProcessMemory(handle, addr, &cache, sizeof(DWORD64), nullptr);
	ReadProcessMemory(handle, (PDWORD)(cache + 0xB8), &cache, sizeof(DWORD64), nullptr);
	ReadProcessMemory(handle, (PDWORD)(cache), &cache, sizeof(DWORD64), nullptr);
	ReadProcessMemory(handle, (PDWORD)(cache + 0xD8), &cache, sizeof(DWORD64), nullptr);
	return cache;
};
void CzwdzjsMFCDlg::OnBnClickedButton1()
{

	DWORD64 z = 获取阳光值();
	WCHAR buff[64];
	memset(buff, 0, 64);
	swprintf_s(buff, 64, L"%d", z);
	GetDlgItem(IDC_EDIT1)->SetWindowText(buff);
}
VOID 修改阳光值(DWORD64 z) {
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, g_pid);
	PDWORD addr = (PDWORD)(g_dllBase + 0x01F34728);
	DWORD64 cache = 0;
	ReadProcessMemory(handle, addr, &cache, sizeof(DWORD64), nullptr);
	ReadProcessMemory(handle, (PDWORD)(cache + 0xB8), &cache, sizeof(DWORD64), nullptr);
	ReadProcessMemory(handle, (PDWORD)(cache), &cache, sizeof(DWORD64), nullptr);
	WriteProcessMemory(handle, (PDWORD)(cache + 0xD8), &z, sizeof(DWORD64), nullptr);
	return;
};
void CzwdzjsMFCDlg::OnBnClickedButton2()
{
	WCHAR buff[64];
	memset(buff, 0, 64);
	GetDlgItem(IDC_EDIT1)->GetWindowText(buff, 64);
	DWORD64 z = 0;
	z = _wtoll(buff);
	修改阳光值(z);
}

VOID 无冷却() {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, g_pid);
	PDWORD addr = (PDWORD)(g_dllBase + 0x01F4AA10);
	DWORD64 cache = 0;
	FLOAT f = 0;
	ReadProcessMemory(hProcess, addr, &cache, sizeof(DWORD64), nullptr);
	ReadProcessMemory(hProcess, (PDWORD)(cache + 0xB8), &cache, sizeof(DWORD64), nullptr);
	ReadProcessMemory(hProcess, (PDWORD)cache, &cache, sizeof(DWORD64), nullptr);
	ReadProcessMemory(hProcess, (PDWORD)(cache + 0x30), &cache, sizeof(DWORD64), nullptr);
	DWORD64 tmp = 0;
	for (size_t i = 0; i < 14; i++)//14
	{
		
		ReadProcessMemory(hProcess, (PDWORD)(cache + 0x20 + i * 8), &tmp, sizeof(DWORD64), nullptr);
		//ReadProcessMemory(hProcess, (PDWORD)(cache + 0x48), &f, sizeof(FLOAT), nullptr);
		WriteProcessMemory(hProcess, (PDWORD)(tmp + 0x48), &f, sizeof(FLOAT), nullptr);//2C+1C
		tmp = 0;
	}
}
void CzwdzjsMFCDlg::OnBnClickedButton3()
{
	无冷却();

}
VOID 植物0消耗() {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, g_pid);
	PDWORD addr = (PDWORD)(g_dllBase + 0x01F4AA10);
	DWORD64 cache = 0;
	FLOAT f = 0;
	ReadProcessMemory(hProcess, addr, &cache, sizeof(DWORD64), nullptr);
	ReadProcessMemory(hProcess, (PDWORD)(cache + 0xB8), &cache, sizeof(DWORD64), nullptr);
	ReadProcessMemory(hProcess, (PDWORD)cache, &cache, sizeof(DWORD64), nullptr);
	ReadProcessMemory(hProcess, (PDWORD)(cache + 0x30), &cache, sizeof(DWORD64), nullptr);
	DWORD64 tmp = 0;
	for (size_t i = 0; i < 14; i++)//14
	{

		ReadProcessMemory(hProcess, (PDWORD)(cache + 0x20 + i * 8), &tmp, sizeof(DWORD64), nullptr);

		WriteProcessMemory(hProcess, (PDWORD)(tmp + 0x24), &f, sizeof(FLOAT), nullptr);//2C-8
		tmp = 0;
	}
}
void CzwdzjsMFCDlg::OnBnClickedButton4()
{
	植物0消耗();
}

void CzwdzjsMFCDlg::OnBnClickedButton5()
{
	init();
}
