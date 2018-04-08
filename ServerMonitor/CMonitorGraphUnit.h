#ifndef __CMONITORGRAPHUNIT__H__
#define __CMONITORGRAPHUNIT__H__

#define dfMAXCHILD		100
#define CLASSNAME		20
#define MAXDATA			100

class CMonitorGraphUnit
{
public:
	/*-------------------------------------------------------------------*/
	// Graph Type
	/*-------------------------------------------------------------------*/
	typedef enum e_GRAPH_TYPE
	{
		eBAR_SINGLE_VERT,
		eBAR_SINGLE_HORZ,
		eBAR_MULTI_VERT,
		eBAR_MULTI_HORZ,
		eLINE_SINGLE,
		eLINE_MULTI,
		eNUMBER,
		eONOFF,
		ePIE
	} GRAPHTYPE;
	
	/*-------------------------------------------------------------------*/
	// Color Type
	/*-------------------------------------------------------------------*/
	typedef enum e_COLOR_TYPE
	{
		eBACKGROUND_GRAY	= RGB(120, 120, 120),
		eBACKGROUND_BLUE	= RGB(100, 140, 255),
		eBACKGROUND_PURPLE	= RGB(220, 80, 220),
		eBACKGROUND_GRREN	= RGB(50, 150, 50),
		eBACKGROUND_YELLOW	= RGB(255, 180, 50),

		eTITLE_GRAY			= RGB(70, 70, 70),
		eTITLE_BLUE			= RGB(50, 50, 255),
		eTITLE_PURPLE		= RGB(170, 30, 170),
		eTITLE_GREEN		= RGB(0, 100, 0),
		eTITLE_YELLOW		= RGB(255, 130, 0),

		eLINE_WHITE			= RGB(255, 255, 255),
		eLINE_BLUE			= RGB(50, 50, 255),
		eLINE_GREEN			= RGB(150, 245, 110),
		eLINE_PINK			= RGB(255, 20, 147),
		eLINE_YELLOW		= RGB(255, 220, 55),
		eLINE_WOOD			= RGB(210, 105, 30),
		eLINE_RED			= RGB(255, 0, 0),
		eLINE_PURPLE		= RGB(153, 50, 204),
		eLINE_SKY			= RGB(70, 255, 255),
		eLINE_CYAN			= RGB(185, 255, 255),

		eBAR_GREEN			= RGB(203, 255, 117),
		eBAR_YELLOW			= RGB(255, 215, 0),
		eBAR_PURBLE			= RGB(255, 130, 255),
		eBAR_RED			= RGB(255, 0, 0)

		// PIE ����� �� �߰��ϱ�
	} COLORTYPE;

	/*-------------------------------------------------------------------*/
	// Child Windows ����
	/*-------------------------------------------------------------------*/
	typedef struct ST_HWNDtoTHIS
	{
		HWND				hWnd[dfMAXCHILD];
		CMonitorGraphUnit	*pThis[dfMAXCHILD];
	} stHWNDtoTHIS;
	
public:

	CMonitorGraphUnit(HINSTANCE hInstance, HWND hWndParent, WCHAR *szName, COLORREF BackColor, GRAPHTYPE enType, int iPosX, int iPosY, int iWidth, int iHeight);
	~CMonitorGraphUnit();

	/////////////////////////////////////////////////////////
	// ������ ���ν���
	/////////////////////////////////////////////////////////
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	/////////////////////////////////////////////////////////
	// ������ �ֱ�.
	/////////////////////////////////////////////////////////
	BOOL	InsertData(int iData);

protected:

	//------------------------------------------------------
	// ������ �ڵ�, this ������ ��Ī ���̺� ����.
	//------------------------------------------------------
	BOOL						PutThis(void);
	static BOOL					RemoveThis(HWND hWnd);
	static CMonitorGraphUnit	*GetThis(HWND hWnd);

	static void					PaintGraph(HWND hWnd, GRAPHTYPE eGraphType);

private:
	
	//------------------------------------------------------
	// �θ� ������ �ڵ�, �� ������ �ڵ�, �ν��Ͻ� �ڵ� ��...
	//------------------------------------------------------
	HWND					_hWndParent;
	HWND					_hWndChild;

	HDC						_hMemDC;
	HBITMAP					_hMemBitmap;
	HBITMAP					_hMemBitmapOld;

	//------------------------------------------------------
	// ������ ��ġ,ũ��,����, �׷��� Ÿ�� ��.. �ڷ�
	//------------------------------------------------------
	WCHAR					_szName[CLASSNAME];

	RECT					_rChildRect;
	GRAPHTYPE				_enGraphType;
	COLORTYPE				_enBackGround;

	//------------------------------------------------------
	// ������ ť
	//------------------------------------------------------
	CQueue<int>				*qDataQueue;

		// static �ɹ� �Լ��� ���ν������� This �����͸� ã�� ����
		// HWND + Class Ptr �� ���̺�

	static stHWNDtoTHIS		stThis;
	static int				iClassNum;
};

#endif