#include "windows.h"
#include <oleacc.h>
#pragma comment ( lib,"oleacc.lib" )  
// #include <OleAuto.h>
#include <iostream>


using namespace std;
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);

// HINSTANCE hDll; // 动态库句柄

// hDll = LoadLibrary("oleacc.dll"); // 加载动态库

// typedef int (* lpFunction)(UINT);//声明函数指针类型
// lpFunction AccessibleObjectFromEvent; // 定义一个函数指针

// AccessibleObjectFromEvent = (lpFunction) GetProcAddress(hDll,"AccessibleObjectFromEvent");//得到所加载DLL模块中函数的

//6、处理消息（窗口过程）
    //CALLBACK代表__stdcall参数传递顺序  从右往左依次入栈，并且在函数返回前自动清空堆栈内容
LRESULT CALLBACK WindowProc(
    HWND hwnd,              //消息所属窗口句柄
    UINT uMsg,               //具体消息名称，WM_XXX
    WPARAM wparam,           //键盘附加消息
    LPARAM lparam                //鼠标附加消息
)
{

    switch (uMsg)
    {
    case WM_CLOSE:
        DestroyWindow(hwnd);                //触发WM_DESTROY
        break;
    case WM_DESTROY:
        PostQuitMessage(0);                  //发送退出消息
        break;
    case WM_LBUTTONDOWN:                      //鼠标左键按下消息
    {
        int xPos;
        int yPos;
        xPos = LOWORD(lparam);
        yPos = HIWORD(lparam);
        //打印操作，通常同MessageBox
        TCHAR buf[1024];
        wsprintf(buf, TEXT("x=%d,y=%d"), xPos, yPos);
        //所属窗口、显示内容、标题内容、关联按键类型
        MessageBox(hwnd, buf, TEXT("鼠标按下"), MB_OK);
        break;
    }
    case WM_KEYDOWN:       //键盘消息
        MessageBox(hwnd, TEXT("键盘按下"), TEXT("键盘消息"), MB_OK);
        break;
    case WM_PAINT:     //绘图
    {
        PAINTSTRUCT ps;             //绘图结构体
        HDC hdc = BeginPaint(hwnd, &ps);
        //绘制文字
        // TextOut(hdc, 100, 100, TEXT("helloworld"), strlen("helloworld"));
        EndPaint(hwnd, &ps);
        break;
    }
    default:
        break;
    }
    //默认方式处理
    return DefWindowProc(hwnd, uMsg, wparam, lparam);
}

void _Wineventproc(
    HWINEVENTHOOK hWinEventHook,
    DWORD event,
    HWND hwnd,
    LONG idObject,
    LONG idChild,
    DWORD idEventThread,
    DWORD dwmsEventTime
)
{
    cout << "钩子！" << endl;
    cout << event << endl;
    IAccessible* pAcc = NULL;
    VARIANT cc;
    HRESULT hr{};
    hr = AccessibleObjectFromEvent(hwnd, idObject, idChild, &pAcc, &cc);
    if ((hr == S_OK) && (pAcc != NULL))
    {
        long bb;
         (*pAcc).get_accChildCount(&bb);

        pAcc->Release();
    }

}
//int main() {
//    return 0;
//}
int WINAPI WinMain(
    HINSTANCE hInstance,           //应用程序实例句柄
    HINSTANCE hPrevInstance,       //前一个应用程序实例句柄，在win32环境下基本值NULL，不起作用
    LPSTR lpCmdLine,             //char *argv[]  命令行参数数组
    int nShowCmd)                //窗口显示命令，最大化最小化
{
    //实现底层窗口  6步骤
    //1、设计窗口类
    //2、注册窗口类
    //3、创建窗口类
    //4、显示和更新窗口
    //5、通过循环取消息
    //6、处理消息（窗口过程）

    cout << "开始！" << endl;

    AllocConsole();  //create console
    SetConsoleTitle("SHMRenderDebugConsole"); //set console title   
    FILE* tempFile = nullptr;
    freopen_s(&tempFile, "conin$", "r+t", stdin); //reopen the stdin, we can use std::cout.
    freopen_s(&tempFile, "conout$", "w+t", stdout);
    std::cout << "开始！" << std::endl;  //print ddd to console window

    //1、设计窗口
    WNDCLASS wc;
    wc.cbClsExtra = 0;//类额外的内存，通常为零
    wc.cbWndExtra = 0;//窗口额外的内存，通常为零
    // wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//设置背景
    wc.hCursor = LoadCursor(NULL, IDC_HAND);//设置光标，如果第一个参数为NULL，代表使用系统提供的默认光标
    wc.hIcon = LoadIcon(NULL, IDI_WARNING);
    wc.hInstance = hInstance;//当前实例句柄，WinMain函数中形参即可
    wc.lpfnWndProc = WindowProc;//窗口过程函数，回调函数，名称可以随便起
    wc.lpszClassName = TEXT("WINDOW");//指定窗口类名
    wc.lpszMenuName = NULL;//菜单名，没有填NULL
    wc.style = 0;//0代表默认风格

    //2、注册窗口类
    RegisterClass(&wc);

    //3、创建窗口
    /*
    lpClassName,    //类名
    lpWindowName,    //窗口名
    dwStyle,        //显示风格  WS_OVERLAPPEDWINDOW
    x, y,\    //创建窗口的起始坐标  CW_USEDEFAULT
    nWidth,nHeight,    //创建窗口默认宽高
    hWndParent,        //父窗口，NULL
    hMenu,       菜单  NULL
    hInstance,    实例句柄，hInstance
    lpParam       其他参数   NULL
    */
    HWND hwnd = CreateWindow(wc.lpszClassName, TEXT("testcpp"), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, hInstance, NULL);
    //4、显示和更新
    ShowWindow(hwnd, SW_SHOWNORMAL);
    UpdateWindow(hwnd);
    // #define int GWL_EXSTYLE = -20;
    // #define int WS_EX_TRANSPARENT = 0x20;
    // #define int WS_EX_LAYERED = 0x80000;
    // #define int WS_EX_TOOLWINDOW = 0x80;
    // #define int WS_EX_TOPMOST = 0x8;
    // #define int LWA_ALPHA = 2;
    // #define int WS_EX_NOACTIVATE = 0x08000000;
    // SetWindowLong(hwnd, GWL_EXSTYLE,  WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_NOACTIVATE);
    SetWindowLong(hwnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_NOACTIVATE);
    HWINEVENTHOOK _hook = SetWinEventHook(0x800C, 0x800C, nullptr, _Wineventproc, 0, 0, WINEVENT_OUTOFCONTEXT);
    cout << _hook << endl;
    cout << "注册钩子！" << endl;
    //5、通过循环取消息

    /*
    HWND        hwnd;                  //主窗口
    UINT        message;               //消息名称   WM_XXXXX（WINDOW MESSAGE)
    WPARAM      wParam;                //附加消息    键盘
    LPARAM      lParam;                //附加消息    鼠标左右键
    DWORD       time;                  //消息产生时间，
    POINT       pt;                    //附加消息   鼠标   x，y坐标
    */
    MSG msg;
    while (1)
    {
        /*
        参数
        _Out_ LPMSG lpMsg,               消息结构体
        _In_opt_ HWND hWnd,            NULL  捕获所有窗口消息
        _In_ UINT wMsgFilterMin,            过滤最小消息数量
        _In_ UINT wMsgFilterMax             过滤最大消息数量       都写0代表捕获所有消息
        */
        if (GetMessage(&msg, NULL, 0, 0) == FALSE)
        {
            break;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}