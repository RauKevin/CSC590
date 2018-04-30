// https://www.daniweb.com/programming/software-development/threads/335547/take-screenshot-and-save-it-as-jpg

#include <iostream>
#include <windows.h>
#include <GdiPlus.h>
#include <string>
#include <fstream>

#pragma comment( lib, "gdiplus" )
#pragma comment(lib, "Gdi32.lib")
#pragma comment(lib, "User32.lib")

using namespace std;
using namespace Gdiplus;

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
void gdiscreen(HWND hwnd, wstring imgStr);

//may want to pass in the HWND and have error return
void gdiscreen(HWND hwnd, wstring imgStr)
{
    //wstring imgStr = L"images/screen.jpeg";
    const WCHAR * img = imgStr.c_str();

	RECT rcClient;
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	
	{//how does this make a difference
        if (hwnd == NULL)
        {
            cout << "Application window not found" << endl;
            return;
        }
        GetWindowRect(hwnd, &rcClient);
		HDC scrdc, memdc;
		HBITMAP membit;
		scrdc = GetDC(hwnd);
		int Height = GetSystemMetrics(SM_CYSCREEN);
		int Width = GetSystemMetrics(SM_CXSCREEN);
		memdc = CreateCompatibleDC(scrdc);
		membit = CreateCompatibleBitmap(scrdc, Width, Height);
		HBITMAP hOldBitmap =(HBITMAP) SelectObject(memdc, membit);
		BitBlt(memdc, 0, 0, Width, Height, scrdc, 0, 0, SRCCOPY);
		Gdiplus::Bitmap bitmap(membit, NULL);
		CLSID clsid;
		GetEncoderClsid(L"image/jpeg", &clsid);
        bitmap.Save(img, &clsid);
		SelectObject(memdc, hOldBitmap);
		DeleteObject(memdc);
		DeleteObject(membit);
		::ReleaseDC(0,scrdc);
	}
	GdiplusShutdown(gdiplusToken);
}
int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	using namespace Gdiplus;
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes
	ImageCodecInfo* pImageCodecInfo = NULL;
	GetImageEncodersSize(&num, &size);
	if(size == 0)
		return -1;  // Failure
	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo == NULL)
		return -1;  // Failure
	GetImageEncoders(num, size, pImageCodecInfo);
	for(UINT j = 0; j < num; ++j)
	{
		if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}    
	}
	free(pImageCodecInfo);
	return 0;
}


//int main(int argc,char* argv[])
int main()
{	
    bool setup = false;
    int counter=0;
    TCHAR title[100];
    HWND hwnd;
    //windows uses \ not /
    //wstring img_dir = L"images/" + to_wstring(stoi(argv[1])) + L"_";
    //string img_path = "images/" + to_string(stoi(argv[1])) + "_"; 
    wstring imgStr;

    //capture and skip the first keypresses
    GetAsyncKeyState(0x53);
    GetAsyncKeyState(VK_ESCAPE);

    cout << "\nProgram Start " << endl;
   
    //instead of calling get key 12 times, store in variable
    while(true)
    {
        if(GetAsyncKeyState(VK_ESCAPE))
        {
            cout << "\nImage capture stop : " << endl;
            break;
        }
        //start recording
        if(GetAsyncKeyState(0x53))
        {
           if(!setup)
            {
                hwnd = GetForegroundWindow();
                setup = true;
            }

           // GetWindowText(hwnd, title, 99);

            cout << "\nImage capture start : " << endl;

            
        }
        

        if(setup)
        {
            //screen capture
        
            //imgStr = img_dir + to_wstring(counter) + L".jpeg";
            imgStr = to_wstring(counter) + L".jpeg";    //saves images in main folder bc im lazy
            gdiscreen(hwnd, imgStr);
            counter++;
            if(counter == 4)
            {
                counter = 0;
            }
        }
        
        Sleep(250); //capture screen 4 times per second
    }

    return 0;
}
