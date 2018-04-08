// ServerMonitor.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "ServerMonitor.h"

// ���� ����:
HINSTANCE hInst;								// ���� �ν��Ͻ��Դϴ�.
CMonitorGraphUnit *p1, *p2, *p3, *p4, *p5;
int iRand;

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: ���⿡ �ڵ带 �Է��մϴ�.
	MSG msg;
	WNDCLASSEX wcex;
	HWND hWnd;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SERVERMONITOR));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(RGB(180, 180, 180));
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"ServerMonitor";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	RegisterClassEx(&wcex);

	hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

	hWnd = CreateWindow(L"ServerMonitor", L"ServerMonitor", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	// �⺻ �޽��� �����Դϴ�.
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int) msg.wParam;
}

//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND	- ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT	- �� â�� �׸��ϴ�.
//  WM_DESTROY	- ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE :
		p1 = new CMonitorGraphUnit(hInst, hWnd, L"SINGLE LINE",CMonitorGraphUnit::eBACKGROUND_GRAY, CMonitorGraphUnit::eLINE_SINGLE, 50, 50, 600, 300);
		p2 = new CMonitorGraphUnit(hInst, hWnd, L"SINGLE BAR VERTICAL",CMonitorGraphUnit::eBACKGROUND_GRAY, CMonitorGraphUnit::eBAR_SINGLE_VERT, 700, 50, 300, 300);
		p3 = new CMonitorGraphUnit(hInst, hWnd, L"SINGLE BAR HORIZON", CMonitorGraphUnit::eBACKGROUND_GRAY, CMonitorGraphUnit::eBAR_SINGLE_HORZ, 1050, 50, 300, 300);
		p4 = new CMonitorGraphUnit(hInst, hWnd, L"NUMBER", CMonitorGraphUnit::eBACKGROUND_GRAY, CMonitorGraphUnit::eNUMBER, 50, 400, 300, 300);
		p5 = new CMonitorGraphUnit(hInst, hWnd, L"ON/OFF", CMonitorGraphUnit::eBACKGROUND_GRAY, CMonitorGraphUnit::eONOFF, 400, 400, 300, 300);
		SetTimer(hWnd, 1, 100, NULL);
		break;


	case WM_TIMER :
		if (iRand == 101) iRand = 0;
		p1->InsertData(iRand);
		p2->InsertData(iRand);
		p3->InsertData(iRand);
		p4->InsertData(iRand);
		p5->InsertData(iRand);
		iRand++;
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}