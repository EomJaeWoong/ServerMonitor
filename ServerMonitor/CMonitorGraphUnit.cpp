#include "stdafx.h"

CMonitorGraphUnit::stHWNDtoTHIS CMonitorGraphUnit::stThis;
int CMonitorGraphUnit::iClassNum = 1;

//-------------------------------------------------------------------------------------------------------
// 생성자
// 생성자에서는 자식 윈도우 클래스 등록, 생성후
// 테이블에 클래스 포인터와 윈도우 핸들을 등록한다.
//-------------------------------------------------------------------------------------------------------
CMonitorGraphUnit::CMonitorGraphUnit(HINSTANCE hInstance, HWND hWndParent, WCHAR *szName, COLORREF BackColor, GRAPHTYPE enType, int iPosX, int iPosY, int iWidth, int iHeight)
	: _hWndParent(hWndParent)
{
	WNDCLASSEX wcex;

	//-------------------------------------------------------------------------------------------------------
	// Window Class 이름
	//-------------------------------------------------------------------------------------------------------
	StringCchPrintf(_szName, (wcslen(szName) + 1) * 2, szName);
	CMonitorGraphUnit::iClassNum++;
	//-------------------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------------------------------
	// 자식 윈도우 클래스 등록
	//-------------------------------------------------------------------------------------------------------
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(BackColor);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = _szName;
	wcex.hIconSm = NULL;
	RegisterClassEx(&wcex);
	//-------------------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------------------------------
	// 자식 윈도우 생성
	//-------------------------------------------------------------------------------------------------------
	_hWndChild = CreateWindow(_szName, _szName, WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE
		, iPosX, iPosY, iWidth, iHeight, hWndParent, NULL, hInstance, NULL);


	//-------------------------------------------------------------------------------------------------------
	// 데이터 큐 생성
	//-------------------------------------------------------------------------------------------------------
	qDataQueue = new CQueue<int>(MAXDATA);

	_enGraphType = enType;
	GetClientRect(_hWndChild, &_rChildRect);
	
	HDC hdc = GetDC(_hWndChild);

	_hMemBitmap = CreateCompatibleBitmap(hdc, _rChildRect.right, _rChildRect.bottom);
	_hMemDC = CreateCompatibleDC(hdc);
	_hMemBitmapOld = (HBITMAP)SelectObject(_hMemDC, _hMemBitmap);

	ReleaseDC(_hWndChild, hdc);

	//-------------------------------------------------------------------------------------------------------
	// 테이블에 클래스와 핸들 등록
	//-------------------------------------------------------------------------------------------------------
	PutThis();
}

CMonitorGraphUnit::~CMonitorGraphUnit()
{

}

LRESULT CALLBACK CMonitorGraphUnit::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT			ps;
	HDC					hdc = NULL;
	CMonitorGraphUnit	*This = NULL;

	int					iPreVal = 0;

	This = GetThis(hWnd);

	switch (message)
	{
	case WM_CREATE :
		////////////////////////////////////////////////////////////////////////
		// 타이머 등록
		////////////////////////////////////////////////////////////////////////
		SetTimer(hWnd, 2, 100, NULL);

		break;

	case WM_TIMER :
		PaintGraph(hWnd, This->_enGraphType);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 그리기 코드를 추가합니다.

		BitBlt(hdc, 0, 0, This->_rChildRect.right, This->_rChildRect.bottom, This->_hMemDC, 0, 0, SRCCOPY);
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

BOOL CMonitorGraphUnit::InsertData(int iData)
{
	int iTrashValue;

	//false인 경우 생각
	if (!qDataQueue->isFull())
		qDataQueue->write(iData);
	else
	{
		qDataQueue->read(&iTrashValue);
		qDataQueue->write(iData);
	}
	
	return true;
}

void CMonitorGraphUnit::PaintGraph(HWND hWnd, GRAPHTYPE eGraphType)
{
	CMonitorGraphUnit	*This = GetThis(hWnd);

	HFONT				hFont, hOldFont;
	HBRUSH				hBrush, hOldBrush;
	HPEN				hPen, hOldPen;

	int					iTempVal = 0, iPreVal = 0;

	////////////////////////////////////////////////////////////////////////
	// Title 그리기
	////////////////////////////////////////////////////////////////////////
	hBrush = CreateSolidBrush(CMonitorGraphUnit::eTITLE_GRAY);
	hOldBrush = (HBRUSH)SelectObject(This->_hMemDC, hBrush);

	hFont = CreateFont(14, 0, 0, 0, 1000, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("굴림"));
	hOldFont = (HFONT)SelectObject(This->_hMemDC, hFont);
	SetTextColor(This->_hMemDC, RGB(255, 255, 255));
	SetBkMode(This->_hMemDC, TRANSPARENT);

	PatBlt(This->_hMemDC, 0, 0, This->_rChildRect.right, 30, PATCOPY);
	TextOut(This->_hMemDC, This->_rChildRect.left + 25, 8, This->_szName, wcslen(This->_szName));

	SelectObject(This->_hMemDC, hOldBrush);
	SelectObject(This->_hMemDC, hOldFont);
	DeleteObject(hFont);
	DeleteObject(hBrush);

	////////////////////////////////////////////////////////////////////////
	// 그래프 별로 그리기
	////////////////////////////////////////////////////////////////////////
	switch (eGraphType)
	{
	case eLINE_SINGLE :
		hBrush = CreateSolidBrush(CMonitorGraphUnit::eBACKGROUND_GRAY);
		hOldBrush = (HBRUSH)SelectObject(This->_hMemDC, hBrush);
		PatBlt(This->_hMemDC, 0, 30, This->_rChildRect.right, This->_rChildRect.bottom, PATCOPY);

		//-------------------------------------------------------------------------------------------------------
		// Queue에 있는 데이터를 그림
		//-------------------------------------------------------------------------------------------------------
		hPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
		hOldPen = (HPEN)SelectObject(This->_hMemDC, hPen);

		if (This->qDataQueue->peak(0, &iPreVal))
			iPreVal = This->_rChildRect.bottom - (iPreVal * ((double)(This->_rChildRect.bottom - 30) / (double)100));

		else
			iPreVal = This->_rChildRect.bottom;

		for (int iCnt = 0; iCnt < This->qDataQueue->getQueueSize(); iCnt++){
			int iTempVal;
			if (This->qDataQueue->peak(iCnt, &iTempVal))
			{
				MoveToEx(This->_hMemDC, (iCnt - 1) * (This->_rChildRect.right / 100), iPreVal, NULL);
				LineTo(This->_hMemDC, iCnt * (This->_rChildRect.right / 100), This->_rChildRect.bottom - (iTempVal * ((double)(This->_rChildRect.bottom - 30) / (double)100)));
				iPreVal = This->_rChildRect.bottom - (iTempVal * ((double)(This->_rChildRect.bottom - 30) / (double)100));
			}
		}
		//-------------------------------------------------------------------------------------------------------
		SelectObject(This->_hMemDC, hOldBrush);
		SelectObject(This->_hMemDC, hOldPen);

		DeleteObject(hBrush);
		DeleteObject(hPen);

		InvalidateRect(hWnd, NULL, FALSE);

		break;

	case eLINE_MULTI :
		break;

	case eBAR_SINGLE_HORZ :
		hBrush = CreateSolidBrush(CMonitorGraphUnit::eBACKGROUND_GRAY);
		hOldBrush = (HBRUSH)SelectObject(This->_hMemDC, hBrush);
		PatBlt(This->_hMemDC, 0, 30, This->_rChildRect.right, This->_rChildRect.bottom, PATCOPY);

		SelectObject(This->_hMemDC, hOldBrush);
		DeleteObject(hBrush);

		This->qDataQueue->read(&iTempVal);

		hBrush = CreateSolidBrush(CMonitorGraphUnit::eBAR_GREEN);
		hOldBrush = (HBRUSH)SelectObject(This->_hMemDC, hBrush);
		hPen = (HPEN)GetStockObject(NULL_PEN);
		hOldPen = (HPEN)SelectObject(This->_hMemDC, hPen);

		Rectangle(This->_hMemDC,
			0,
			(double)(This->_rChildRect.bottom - 30) / (double)5 + 30,
			iTempVal * ((double)(This->_rChildRect.right - 30) / (double)100),
			(double)(This->_rChildRect.bottom - 30) / (double)5 * (double)4 + 30);

		SelectObject(This->_hMemDC, hOldBrush);
		SelectObject(This->_hMemDC, hOldPen);
		DeleteObject(hBrush);
		DeleteObject(hPen);

		InvalidateRect(hWnd, NULL, FALSE);

		break;

	case eBAR_SINGLE_VERT :
		hBrush = CreateSolidBrush(CMonitorGraphUnit::eBACKGROUND_GRAY);
		hOldBrush = (HBRUSH)SelectObject(This->_hMemDC, hBrush);
		PatBlt(This->_hMemDC, 0, 30, This->_rChildRect.right, This->_rChildRect.bottom, PATCOPY);

		SelectObject(This->_hMemDC, hOldBrush);
		DeleteObject(hBrush);
		
		This->qDataQueue->read(&iTempVal);

		hBrush = CreateSolidBrush(CMonitorGraphUnit::eBAR_GREEN);
		hOldBrush = (HBRUSH)SelectObject(This->_hMemDC, hBrush);
		hPen = (HPEN)GetStockObject(NULL_PEN);
		hOldPen = (HPEN)SelectObject(This->_hMemDC, hPen);

		Rectangle(This->_hMemDC,
			(double)This->_rChildRect.right / (double)5,
			This->_rChildRect.bottom - (iTempVal * ((double)(This->_rChildRect.bottom - 30) / (double)100)),
			(double)This->_rChildRect.right / (double)5 * (double)4,
			This->_rChildRect.bottom);

		SelectObject(This->_hMemDC, hOldBrush);
		SelectObject(This->_hMemDC, hOldPen);
		DeleteObject(hBrush);
		DeleteObject(hPen);
		
		InvalidateRect(hWnd, NULL, FALSE);

		break;

	case eBAR_MULTI_VERT :
		break;

	case eBAR_MULTI_HORZ :
		break;

	case eNUMBER :
		hBrush = CreateSolidBrush(CMonitorGraphUnit::eBACKGROUND_GRAY);
		hOldBrush = (HBRUSH)SelectObject(This->_hMemDC, hBrush);
		PatBlt(This->_hMemDC, 0, 30, This->_rChildRect.right, This->_rChildRect.bottom, PATCOPY);

		SelectObject(This->_hMemDC, hOldBrush);
		DeleteObject(hBrush);

		This->qDataQueue->read(&iTempVal);

		
		hFont = CreateFont(
			(double)This->_rChildRect.bottom / (double)7 * (double)4, 
			0,
			0,
			0,
			1000,
			0,
			0,
			0,
			HANGEUL_CHARSET, 
			0,
			0,
			0,
			VARIABLE_PITCH | FF_ROMAN, 
			TEXT("굴림")
			);

		hOldFont = (HFONT)SelectObject(This->_hMemDC, hFont);
		SetTextColor(This->_hMemDC, RGB(255, 255, 255));
		SetBkMode(This->_hMemDC, TRANSPARENT);
		
		WCHAR szNumber[4];
		StringCchPrintf(szNumber, 4, L"%d", iTempVal);
		DrawText(This->_hMemDC, szNumber, wcslen(szNumber), &This->_rChildRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		SelectObject(This->_hMemDC, hOldBrush);
		SelectObject(This->_hMemDC, hOldFont);
		DeleteObject(hFont);
		DeleteObject(hBrush);
		
		InvalidateRect(hWnd, NULL, FALSE);
		
		break;

	case eONOFF :
		hBrush = CreateSolidBrush(CMonitorGraphUnit::eBACKGROUND_GRAY);
		hOldBrush = (HBRUSH)SelectObject(This->_hMemDC, hBrush);
		PatBlt(This->_hMemDC, 0, 30, This->_rChildRect.right, This->_rChildRect.bottom, PATCOPY);

		SelectObject(This->_hMemDC, hOldBrush);
		DeleteObject(hBrush);

		This->qDataQueue->read(&iTempVal);


		hFont = CreateFont(
			(double)This->_rChildRect.bottom / (double)7 * (double)3,
			0,
			0,
			0,
			1000,
			0,
			0,
			0,
			HANGEUL_CHARSET,
			0,
			0,
			0,
			VARIABLE_PITCH | FF_ROMAN,
			TEXT("굴림")
			);

		hOldFont = (HFONT)SelectObject(This->_hMemDC, hFont);
		SetTextColor(This->_hMemDC, RGB(255, 255, 255));
		SetBkMode(This->_hMemDC, TRANSPARENT);

		if (iTempVal > 50)
			DrawText(This->_hMemDC, L"ON", 2, &This->_rChildRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		else
			DrawText(This->_hMemDC, L"OFF", 3, &This->_rChildRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		SelectObject(This->_hMemDC, hOldBrush);
		SelectObject(This->_hMemDC, hOldFont);
		DeleteObject(hFont);
		DeleteObject(hBrush);

		InvalidateRect(hWnd, NULL, FALSE);

		break;
		break;

	default :
		break;
	}
}

BOOL CMonitorGraphUnit::PutThis(void)
{
	if (_hWndChild != NULL){
		for (int iCnt = 0; iCnt < dfMAXCHILD; iCnt++){
			if (CMonitorGraphUnit::stThis.hWnd[iCnt] == NULL){
				CMonitorGraphUnit::stThis.hWnd[iCnt] = _hWndChild;
				CMonitorGraphUnit::stThis.pThis[iCnt] = this;
				return true;
			}
		}
	}

	return false;
}

CMonitorGraphUnit *CMonitorGraphUnit::GetThis(HWND hWnd)
{
	CMonitorGraphUnit	*This = NULL;

	for (int iCnt = 0; iCnt < dfMAXCHILD; iCnt++){
		if (hWnd == CMonitorGraphUnit::stThis.hWnd[iCnt]){
			This = stThis.pThis[iCnt];
			break;
		}
	}

	return This;
}

BOOL CMonitorGraphUnit::RemoveThis(HWND hWnd)
{
	CMonitorGraphUnit *This = GetThis(hWnd);

	return true;
}