#include<windows.h>
#include"helper.h"

void OnDestroy(HWND);
void OnLButtonDown(HWND, int, int);
void OnLButtonUp();
void OnMouseMove(HWND, int, int, int);

int x1, y1, x2, y2;

int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow){
    
    MSG msg;
    /*perform application initalization*/
    InitInstance(hInstance, nCmdShow, "Draw Shapes");

    /*message Loop*/
    while(GetMessage(&msg, 0, 0, 0)){
        DispatchMessage(&msg);
    }
    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
    
    switch(message){
        
        case WM_DESTROY:
            OnDestroy(hWnd);
            break;
        case WM_LBUTTONDOWN:
            OnLButtonDown(hWnd, LOWORD(lParam), HIWORD(lParam));
            break;
        case WM_LBUTTONUP:
            OnLButtonUp();
            break;
        case WM_MOUSEMOVE:
            OnMouseMove(hWnd, wParam, LOWORD(lParam), HIWORD(lParam));
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

void OnDestroy(HWND hWnd){
    
    PostQuitMessage(0);
}

void OnLButtonDown(HWND hWnd, int x, int y){
    SetCapture(hWnd);
    x1 = x;
    y1 = y;
}

void OnMouseMove(HWND hWnd, int flag, int x, int y){
    
    HDC hdc;
    RECT rect;

    if(flag == MK_LBUTTON){ /* is left mouse button depressed*/
        hdc = GetDC(hWnd);
        GetClientRect(hWnd, &rect);
        if(x >= rect.left && x <= rect.right && y >= rect.top && y <= rect.bottom){
            
            x2 = x;
            y2 = y;

            MoveToEx(hdc, x1, y1, NULL);
            LineTo(hdc, x2, y2);
             
            ReleaseDC(hWnd, hdc);

            x1 = x2;
            y1 = y2;   
        }   
    }
}

void OnLButtonUp(){

    ReleaseCapture();
}
