#include <windows.h>
#include <iostream>

#include <array>
#include <vector>
#include <cmath>
#include <random>
#include <chrono>
#include <algorithm>
#include <iterator>

typedef std::array<int,2> golworldsize;
typedef std::vector<byte> golworld;
typedef unsigned char uchar;

golworldsize worldsize = {600,400};
golworld world = golworld(worldsize[0]*worldsize[1]);
golworld nworld = golworld(worldsize[0]*worldsize[1]);
uchar* image = new unsigned char [worldsize[0]*worldsize[1]*4];


std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
std::chrono::system_clock::time_point end = std::chrono::system_clock::now();

void mkimg(golworld w);
void paintscreen(HWND);
void first();
void next();

LRESULT CALLBACK WndProc(HWND hwnd , UINT msg , WPARAM wp , LPARAM lp) {
	TCHAR tcStr[128];

	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
		//std::cout << "\033[2K\033[1G" << "keydown " << wp << std::endl;
		bool a;
		switch (wp)
		{
			default:
			break;
		}
		return 0;
	case WM_CREATE:
   		std::cout << "Game of Life" << std::endl;
        first();
		return 0;
	case WM_PAINT:
		paintscreen(hwnd);
		return 0;
	}
	return DefWindowProc(hwnd,msg,wp,lp);
}

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,PSTR lpCmdLine,int nCmdShow) {
	HWND hwnd;
	MSG msg;
	WNDCLASS winc;

	winc.style = CS_HREDRAW | CS_VREDRAW;
	winc.lpfnWndProc = WndProc;
	winc.cbClsExtra	= winc.cbWndExtra	= 0;
	winc.hInstance = hInstance;
	winc.hIcon = LoadIcon(NULL , IDI_APPLICATION);
	winc.hCursor = LoadCursor(NULL , IDC_ARROW);
	winc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	winc.lpszMenuName = NULL;
	winc.lpszClassName = TEXT("app");

	if (!RegisterClass(&winc)) return -1;

	hwnd = CreateWindow(
		TEXT("app") , TEXT("Game of Life") ,
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX | WS_VISIBLE ,
		CW_USEDEFAULT , CW_USEDEFAULT ,
		worldsize[0] , worldsize[1] , // window size
		NULL , NULL , hInstance , NULL
	);

	if (hwnd == NULL) return -1;

	while(GetMessage(&msg,NULL,0,0)) {DispatchMessage(&msg);}
	return msg.wParam;
}
void paintscreen(HWND hwnd) {
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	static BITMAPINFO bmpInfo;
	GetClientRect(hwnd, &rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	bmpInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biWidth=width;
	bmpInfo.bmiHeader.biHeight=height;
	bmpInfo.bmiHeader.biPlanes=1;
	bmpInfo.bmiHeader.biBitCount=32;
	bmpInfo.bmiHeader.biCompression=BI_RGB;
    next();
	hdc = GetDC(hwnd);
	{ // 3d
		{
			end = std::chrono::system_clock::now();
			int msec = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
			std::cout << "\033[2K\033[1G" << msec << "ms fps" << (double)1000/msec;
			start = std::chrono::system_clock::now();
		}
		SetDIBitsToDevice(hdc,0,0,width,height,0,0,0,height,image,&bmpInfo,DIB_RGB_COLORS);
	}
	return;
}

void first() {
    for (int cr=0;cr<worldsize[0]*worldsize[1];cr++) {
        if (rand()%100>50) {
            world[cr] = 1;
        }
        else {
            world[cr] = 0;
        }
    }
    return;
}
void next() {
    for (int iy = 0; iy < worldsize[1]; iy++) {
        for (int ix = 0; ix < worldsize[0]; ix++) {
            int ar = 0;
            int ii = (iy*worldsize[0]+ix);
            nworld[ii] = 0;
            for (int by=-1;by<=1;by++) {
                for (int bx=-1;bx<=1;bx++) {
                    int bi = ((iy+by)*worldsize[0]+ix+bx);
                    if (!(by==0&bx==0)&&(iy+by>=0)&&(ix+bx>=0)&&(ix+bx<worldsize[0])&&(iy+by<worldsize[1])) {
                        if (world[bi]==1) {
                            ar++;
                        }
                    }
                }
            }
            if (world[ii]==0) {
                if (ar==3) {
                    nworld[ii] = 1;
                }
            }
            else if (ar==2|ar==3) {
                nworld[ii] = 1;
            }
        }
    }
    for (int i=0;i<worldsize[0]*worldsize[1];i++) {
        world[i] = nworld[i];
    }
	mkimg(world);
    return;
}
void mkimg(golworld w) {
    for (int i=0;i<worldsize[0]*worldsize[1];i++) {
        uchar bright = w[i]*255;
        image[i*4+0] = bright;
        image[i*4+1] = bright;
        image[i*4+2] = bright;
        image[i*4+3] = 255;
    }
}
