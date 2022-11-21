#include<windows.h>
#include<stdlib.h>
#include <mmsystem.h> //for sound effects
#include<stdio.h>
#include<time.h>
#include"helper.h"


//#pragma comment(lib, "Winmm.lib")

#define VK_KEY_S 0x53
#define VK_KEY_W 0x57

typedef struct {
    double x, y;
}Point;

int OnKeyDown(HWND, int, LPRECT, LPRECT);
void OnTimer(HWND);
void ReFillRects(HWND, LPRECT, LPRECT);
void SetCoords(LPRECT, LPRECT);
void setScores(HWND);
BOOL doOverlap(Point l1, Point r1, Point l2, Point r2);
BOOL isCollide(LPRECT);
BOOL isleftCollide(LPRECT);
BOOL isrightCollide(LPRECT);
void resetAll(HWND);
void autoMoveRect(HWND, LPRECT);

int x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6;
long long timecount = 0;
double dx = 10, dy = 10;
int xlowerlim, xupperlim, ylowerlim, yupperlim, randx, randy;
int player1_score = 0, player2_score = 0, serving_palyer = 1;
BOOL isStarted = FALSE;
RECT ballRect;     // rectangle to represent ball
RECT leftrect;       // rectangle that encloses bitmap 1
RECT rightrect;       // rectangle that encloses bitmap 2

int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow) {

    MSG msg;
    /*perform application initalization*/
    InitInstance(hInstance, nCmdShow, "Pong Game");

    /*message Loop*/
    while (GetMessage(&msg, 0, 0, 0)) {
        DispatchMessage(&msg);
    }
    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

    static HBITMAP hbmp;
    static HDC hdcCompat;    // DC for copying bitmap 
    static COLORREF crBkgnd; // color of client-area background 
    static HBRUSH hbrBkgnd;  // handle of background-color brush
    static HPEN hpenDot;     // handle of dotted pen

    HFONT font, oldfont;
    LOGFONT lf;

    HDC hdc;
    RECT rect;
    PAINTSTRUCT ps;

    switch (message) {

        case WM_CREATE:

            GetClientRect(hWnd, &rect);

            x1 = 1;
            y1 = ((rect.bottom - rect.top) / 2) - 40;
            x2 = 20;
            y2 = ((rect.bottom - rect.top) / 2) + 40;
            x3 = (rect.right - rect.left) - 20;
            y3 = ((rect.bottom - rect.top) / 2) - 40;
            x4 = (rect.right - rect.left) - 1;
            y4 = ((rect.bottom - rect.top) / 2) + 40;
            x5 = (rect.right - rect.left) / 2 - 15;
            y5 = ((rect.bottom - rect.top) / 2) - 35;
            x6 = (rect.right - rect.left) / 2;
            y6 = ((rect.bottom - rect.top) / 2) - 20;

            SetTimer(hWnd, 1, 40, NULL);

            xlowerlim = 1;
            ylowerlim = 1;
            xupperlim = 1000;
            yupperlim = 1000;

            dy = generateRandominRange(10, 50);

            setScores(hWnd);

            //DrawText(hdc, TEXT("Press Enter to continue"), -1 , &rect, DT_SINGLELINE | DT_CENTER);

        case WM_PAINT:

            hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            //SetCoords(&leftrect, &rightrect);
            ReFillRects(hWnd, &leftrect, &rightrect);
            if (!isStarted) {
                ZeroMemory(&lf, sizeof(lf));
                ZeroMemory(&font, sizeof(font));
                lf.lfHeight = 15;
                lf.lfWeight = FW_BOLD;
                lf.lfItalic = FALSE;
                lstrcpy(lf.lfFaceName, "04b03");
                font = CreateFontIndirect(&lf);
                oldfont = SelectObject(hdc, font);

                TextOut(hdc, (rect.right - rect.left) / 2 - 50, 30, TEXT("Press Enter to start"), strlen("Press Enter to continue"));

                SelectObject(hdc, oldfont);
            }

            EndPaint(hWnd, &ps);

            return 0;

         case WM_KEYDOWN:
             switch (wParam) {
                 case VK_RETURN:
                     isStarted = TRUE;
                     break;
                 default:
                     OnKeyDown(hWnd, wParam, &leftrect, &rightrect);
             }
             return 0;

         case WM_TIMER:

             OnTimer(hWnd);
             return 0;

         case WM_QUIT:

             KillTimer(hWnd, 1);
             PostQuitMessage(0);
             return 0;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

int OnKeyDown(HWND hWnd, int wParam, LPRECT leftrect, LPRECT rightrect) {

    RECT rect, backrect;

    HDC hdc;

    GetClientRect(hWnd, &rect);

    hdc = GetDC(hWnd);

    switch (wParam)
    {

    case VK_UP:

        // Process the UP ARROW key.
        if (leftrect->top - 10 >= 1) {

            FillRect(hdc, leftrect, CreateSolidBrush(Grey));

            leftrect->top -= 12;
            leftrect->bottom -= 12;

            SetCoords(leftrect, rightrect);

            Rectangle(hdc, leftrect->left, leftrect->top,
                leftrect->right, leftrect->bottom);
        }

        //InvalidateRect(hWnd, &rect, FALSE);

        break;

    case VK_DOWN:

        // Process the DOWN ARROW key.
        if (leftrect->bottom + 10 <= rect.bottom - 1) {

            FillRect(hdc, leftrect, CreateSolidBrush(Grey));

            leftrect->top += 12;
            leftrect->bottom += 12;

            SetCoords(leftrect, rightrect);

            Rectangle(hdc, leftrect->left, leftrect->top,
                leftrect->right, leftrect->bottom);
        }
        //InvalidateRect(hWnd, &rect, TRUE);
        break;

    case VK_KEY_W:

        // Process the W alphabet key.
        if (rightrect->top - 10 >= 1) {

            FillRect(hdc, rightrect, CreateSolidBrush(Grey));

            rightrect->top -= 12;
            rightrect->bottom -= 12;

            SetCoords(leftrect, rightrect);

            Rectangle(hdc, rightrect->left, rightrect->top,
                rightrect->right, rightrect->bottom);
        }
        //InvalidateRect(hWnd, &rect, TRUE);

        break;

    case VK_KEY_S:

        // Process the S alphabet key.
        if (rightrect->bottom + 10 <= rect.bottom - 1) {

            FillRect(hdc, rightrect, CreateSolidBrush(Grey));

            rightrect->top += 12;
            rightrect->bottom += 12;

            SetCoords(leftrect, rightrect);

            Rectangle(hdc, rightrect->left, rightrect->top,
                rightrect->right, rightrect->bottom);
        }
        //InvalidateRect(hWnd, &rect, TRUE);
        break;

    }

    SetCoords(leftrect, rightrect);

    ReleaseDC(hWnd, hdc);

    return 0;
}

void OnTimer(HWND hWnd) {

    RECT rect, oldrect;
    HDC hdc;

    GetClientRect(hWnd, &rect);

    hdc = GetDC(hWnd);

    oldrect = ballRect;

    timecount += 40;
    if (timecount >= 20000) {
        InvalidateRect(hWnd, &rect, TRUE);
        timecount = 0;
    }

    if (dx > 0 && dx < 20) {
        dx = 20;
    }else if(dx < 0 && abs(dx) < 20){
        dx = -20;
    }

    if (isStarted) {

        /*Rectangle(hdc, ballRect.left, ballRect.top,
            ballRect.right, ballRect.bottom);*/

        oldrect = ballRect;
        FillRect(hdc, &oldrect, CreateSolidBrush(Grey));

        Point leftrectptl1 = { x1, y1 };
        Point leftrectptr1 = { x2, y2 };

        Point ballrectptl1 = { x5, y5 };
        Point ballrectptr1 = { x6, y6 };

        Point rightrectptl1 = { x3, y3 };
        Point rightrectptr1 = { x4, y4 };

        if (isleftCollide(&leftrect)) {

            dx = -dx;
            dx = dx > 0 ? dx + generateRandominRange(0, 12) : dx - generateRandominRange(0, 12);
            //x5 = leftrect.left + 5;
            PlaySound(TEXT("sounds/paddle_hit.wav"), NULL, SND_FILENAME | SND_ASYNC);
            oldrect = ballRect;
            FillRect(hdc, &oldrect, CreateSolidBrush(Grey));
            //ReFillRects(hWnd, &leftrect, &rightrect);
            if (dy < 0) {
                dy = -generateRandominRange(0, 15);
            }
            else {
                dy = generateRandominRange(0, 15);
            }
            if (dx > 0 && dx > 30) {

                dx = generateRandominRange(20, 30);

            }
            if(dx < 0 && abs(dx) > 30) {
                dx =  -generateRandominRange(20, 30);
            }
            SetCoords(&leftrect, &rightrect);
        }
        else if (isrightCollide(&rightrect)) {
            
            dx = -dx;
            dx = dx > 0 ? dx + generateRandominRange(0, 8) : dx - generateRandominRange(0, 8);
            //x5 = rightrect.left - 4;
            PlaySound(TEXT("sounds/paddle_hit.wav"), NULL, SND_FILENAME | SND_ASYNC);
            //ReFillRects(hWnd, &leftrect, &rightrect);
            if (dy < 0) {
                dy = -generateRandominRange(0, 15);
            }
            else {
                dy = generateRandominRange(0, 15);
            }
            if (dx > 0 && dx > 30) {

                dx = generateRandominRange(20, 30);

            }
            if (dx < 0 && abs(dx) > 30) {
                dx = -generateRandominRange(20, 30);
            }
            SetCoords(&leftrect, &rightrect);
        }
        else if (ballRect.left <= rect.left + 2) {
            dy = -dy;
            serving_palyer = serving_palyer == 1 ? 2 : 1;
            player2_score++;
            setScores(hWnd);
            PlaySound(TEXT("sounds/score.wav"), NULL, SND_FILENAME | SND_ASYNC);
            Sleep(300);
            SetCoords(&leftrect, &rightrect);
            resetAll(hWnd);

        }
        else if (ballRect.right >= rect.right - 2) {
            dy = -dy;
            serving_palyer = serving_palyer == 1 ? 2 : 1;
            SetCoords(&leftrect, &rightrect);
            player1_score++;
            setScores(hWnd);
            PlaySound(TEXT("sounds/score.wav"), NULL, SND_FILENAME | SND_ASYNC);
            Sleep(300);
            //dx = -dx;
            resetAll(hWnd);
        }
        else if (ballRect.top <= 0) {
            dy = -dy;
            SetCoords(&leftrect, &rightrect);
            PlaySound(TEXT("sounds/wall_hit.wav"), NULL, SND_FILENAME | SND_ASYNC);

        }
        else if (ballRect.bottom >= rect.bottom - 5) {
            dy = -dy;
            SetCoords(&leftrect, &rightrect);
            PlaySound(TEXT("sounds/wall_hit.wav"), NULL, SND_FILENAME | SND_ASYNC);
        }
        else if (ballRect.left <= leftrect.right && ballRect.bottom <= leftrect.top - 3) {

            if (dy > 0) {
                dy = -dy;
            }

            SetCoords(&leftrect, &rightrect);
        }
        else if (ballRect.left <= leftrect.right && ballRect.top >= leftrect.bottom + 3) {

            if (dy < 0) {
                dy = -dy;
            }
            SetCoords(&leftrect, &rightrect);
        }
        else if (ballRect.right >= rightrect.left && ballRect.bottom <= rightrect.top - 3) {
            
            if (dy > 0) {
                dy = -dy;
            }

            SetCoords(&leftrect, &rightrect);
        }
        else if (ballRect.right >= rightrect.left && ballRect.top >= rightrect.bottom + 3) {

            if (dy < 0) {
                dy = -dy;
            }
            SetCoords(&leftrect, &rightrect);

        }

        oldrect = ballRect;
        FillRect(hdc, &oldrect, CreateSolidBrush(Grey));

        ballRect.left += dx;
        ballRect.right += dx;
        ballRect.top += dy;
        ballRect.bottom += dy;

        if (player1_score == 10 || player2_score == 10) {
            
            char scoreMessage[50];
            snprintf(scoreMessage, 50, "Player %d won the match !", player1_score == 10 ? 1 : 2);

            MessageBox(hWnd, scoreMessage, "Round completed", 0);

            player1_score = 0;
            player2_score = 0;
            setScores(hWnd);
            resetAll(hWnd);
        }

        SetCoords(&leftrect, &rightrect);
        autoMoveRect(hWnd, &rightrect);
        //autoMoveRect(hWnd, &leftrect);

    }
    else {
        if (serving_palyer == 1) {
            dx = generateRandominRange(10, 18);
        }
        else {
            dx = -generateRandominRange(10, 18);
        }
    }

        //InvalidateRect(hWnd, &rect, TRUE);
    ReFillRects(hWnd, &leftrect, &rightrect);

    ReleaseDC(hWnd, hdc);
}

void ReFillRects(HWND hWnd, LPRECT leftrect, LPRECT rightrect) {

    HDC hdc;
    hdc = GetDC(hWnd);

    SetRect(leftrect, x1, y1, x2, y2);
    SetRect(rightrect, x3, y3, x4, y4);
    SetRect(&ballRect, x5, y5, x6, y6);

    RECT rect;

    GetClientRect(hWnd, &rect);

    Rectangle(hdc, leftrect->left, leftrect->top,
        leftrect->right, leftrect->bottom);

    Rectangle(hdc, rightrect->left, rightrect->top,
        rightrect->right, rightrect->bottom);

    Rectangle(hdc, ballRect.left, ballRect.top,
        ballRect.right, ballRect.bottom);

    setScores(hWnd);

    SetCoords(leftrect, rightrect);

    ReleaseDC(hWnd, hdc);

    //InvalidateRect(hWnd, &rect, TRUE);
}

void SetCoords(LPRECT leftrect, LPRECT rightrect) {

    x1 = leftrect->left;
    y1 = leftrect->top;
    x2 = leftrect->right;
    y2 = leftrect->bottom;
    x3 = rightrect->left;
    y3 = rightrect->top;
    x4 = rightrect->right;
    y4 = rightrect->bottom;
    x5 = ballRect.left;
    y5 = ballRect.top;
    x6 = ballRect.right;
    y6 = ballRect.bottom;
}

void setScores(HWND hWnd) {


    HDC hdc;
    RECT rect, fontrect;
    PAINTSTRUCT ps;
    HFONT font, oldfont;
    LOGFONT lf;
    char score1[5];
    char score2[5];

    hdc = GetDC(hWnd);

    GetClientRect(hWnd, &rect);

    SetBkMode(hdc, TRANSPARENT);

    SetTextColor(hdc, RGB(255, 255, 255));

    snprintf(score1, 5, "%d", player1_score);
    snprintf(score2, 5, "%d", player2_score);

    GetClientRect(hWnd, &rect);

    ZeroMemory(&lf, sizeof(lf));
    ZeroMemory(&font, sizeof(font));
    lf.lfHeight = 70;
    lf.lfWeight = FW_BOLD;
    lf.lfItalic = FALSE;
    lstrcpy(lf.lfFaceName, "04b03");
    font = CreateFontIndirect(&lf);
    oldfont = SelectObject(hdc, font);

    //InvalidateRect(hWnd, &rect, TRUE);

    fontrect.left = (rect.right - rect.left) / 2 - 310;
    fontrect.top = 40;
    fontrect.right = (rect.right - rect.left) / 2 - 310 + 70;
    fontrect.bottom = 110;
    FillRect(hdc, &fontrect, CreateSolidBrush(Grey));
    fontrect.left = (rect.right - rect.left) / 2 + 310;
    fontrect.top = 40;
    fontrect.right = (rect.right - rect.left) / 2 + 310 + 70;
    fontrect.bottom = 110;
    FillRect(hdc, &fontrect, CreateSolidBrush(Grey));
    TextOut(hdc, (rect.right - rect.left) / 2 - 310, 40, TEXT(score1), strlen(score1));
    TextOut(hdc, (rect.right - rect.left) / 2 + 310, 40, TEXT(score2), strlen(score2));

    SetBkMode(hdc, TRANSPARENT);

    SelectObject(hdc, oldfont);

    DeleteObject(font);

    ReleaseDC(hWnd, hdc);
}

BOOL doOverlap(Point l1, Point r1, Point l2, Point r2)
{
    // if rectangle has area 0, no overlap
    if (l1.x == r1.x || l1.y == r1.y || r2.x == l2.x || l2.y == r2.y)
        return FALSE;

    // If one rectangle is on left side of other
    if (l1.x > r2.x || l2.x > r1.x)
        return FALSE;

    // If one rectangle is above other
    if (r1.y > l2.y || r2.y > l1.y)
        return FALSE;

    return TRUE;
}

BOOL isleftCollide(LPRECT paddle_rect) {

    if (ballRect.left <= (paddle_rect->right + 8) && ballRect.bottom >= paddle_rect->top && ballRect.top <= paddle_rect->bottom) {
        return TRUE;
    }
    return FALSE;
}

BOOL isrightCollide(LPRECT paddle_rect) {

    if (ballRect.right >= (paddle_rect->left - 8) && ballRect.bottom >= paddle_rect->top && ballRect.top <= paddle_rect->bottom) {
        return TRUE;
    }

    return FALSE;
}

void resetAll(HWND hWnd) {

    //KillTimer(hWnd, 1);

    isStarted = 0;

    RECT rect;
    HDC hdc;

    GetClientRect(hWnd, &rect);

    hdc = GetDC(hWnd);

    //MessageBox(NULL, "edge 2", "colld", 0);
    //InvalidateRect(hWnd, &rect, TRUE);

    FillRect(hdc, &leftrect, CreateSolidBrush(Grey));
    FillRect(hdc, &rightrect, CreateSolidBrush(Grey));
    FillRect(hdc, &ballRect, CreateSolidBrush(Grey));

    leftrect.left = 1;
    leftrect.top = ((rect.bottom - rect.top) / 2) - 40;
    leftrect.right = 20;
    leftrect.bottom = ((rect.bottom - rect.top) / 2) + 40;
    rightrect.left = (rect.right - rect.left) - 20;
    rightrect.top = ((rect.bottom - rect.top) / 2) - 40;
    rightrect.right = (rect.right - rect.left) - 1;
    rightrect.bottom = ((rect.bottom - rect.top) / 2) + 40;
    ballRect.left = (rect.right - rect.left) / 2 - 15;
    ballRect.top = ((rect.bottom - rect.top) / 2) - 35;
    ballRect.right = (rect.right - rect.left) / 2;
    ballRect.bottom = ((rect.bottom - rect.top) / 2) - 20;

    SetCoords(&leftrect, &rightrect);

    Rectangle(hdc, leftrect.left, leftrect.top, leftrect.right, leftrect.bottom);
    Rectangle(hdc, rightrect.left, rightrect.top, rightrect.right, rightrect.bottom);
    Rectangle(hdc, ballRect.left, ballRect.top, ballRect.right, ballRect.bottom);

    FillRect(hdc, &leftrect, CreateSolidBrush(RGB(255, 255, 255)));
    FillRect(hdc, &rightrect, CreateSolidBrush(RGB(255, 255, 255)));
    FillRect(hdc, &ballRect, CreateSolidBrush(RGB(255, 255, 255)));
    
    ReleaseDC(hWnd, hdc);

}

void autoMoveRect(HWND hWnd, LPRECT rect) {
    
    RECT winrect;
    HDC hdc;
    hdc = GetDC(hWnd);
    GetClientRect(hWnd, &winrect);
    if (rect->top + 30 < ballRect.top) {
        //SendMessage(hWnd, WM_KEYDOWN, VK_KEY_S, 0);
        if (rect->bottom + 20 <= winrect.bottom - 4) {
            FillRect(hdc, rect, CreateSolidBrush(Grey));

            rect->top += 20;
            rect->bottom += 20;

            SetCoords(&leftrect, &rightrect);

            Rectangle(hdc, rect->left, rect->top,
                rect->right, rect->bottom);
        }
    }
    else if (rect->top + 30 > ballRect.top) {
        //SendMessage(hWnd, WM_KEYDOWN, VK_KEY_W, 0);
        if (rect->top - 20 >= 1) {

            FillRect(hdc, rect, CreateSolidBrush(Grey));

            rect->top -= 20;
            rect->bottom -= 20;

            SetCoords(&leftrect, &rightrect);

            Rectangle(hdc, rect->left, rect->top,
                rect->right, rect->bottom);
        }
    }
    ReleaseDC(hWnd, hdc);
}

