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


int main(int argc,char* argv[])
{	
    bool setup = false;
    int counter=0;
    TCHAR title[100];
    HWND hwnd;
    //windows uses \ not /
    wstring img_dir = L"images/" + to_wstring(stoi(argv[1])) + L"_";
    string img_path = "images/" + to_string(stoi(argv[1])) + "_"; 
    wstring imgStr;

    //array of key presses, bool? int? (skipping strife?)
    // [SPACE],1,2,3,4,5,6,7,[left_arrow], [right_arrow], [up_arrow], [down_arrow], [shift], [ctrl]
    //int keystate[14]; 
    int keystate[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0}; 

    ofstream outputFile;
    string filename = "trainingData.csv";

    // create and open the .csv file
    //fs.open(outputFile,filename);
    outputFile.open(filename.c_str(), ios::out | ios::app);

    // write header for .csv
    //outputFile << "A" << "," << "B" << "," << "C" << endl;
    
    //capture and skip the first keypresses
    GetAsyncKeyState(VK_LEFT);
    GetAsyncKeyState(VK_RIGHT);
    GetAsyncKeyState(VK_UP);
    GetAsyncKeyState(VK_DOWN);
    GetAsyncKeyState(VK_CONTROL);
    GetAsyncKeyState(VK_SPACE);
    GetAsyncKeyState(VK_RETURN);
    GetAsyncKeyState(0x53);
    GetAsyncKeyState(VK_ESCAPE);
   
    //insteat of calling get key 12 times, store in variable
    while(true)
    {
        if(GetAsyncKeyState(VK_ESCAPE))
        {
            cout << "\nScreen recording stop : " << endl;
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

            // remove console msg later
           // GetWindowText(hwnd, title, 99);

            cout << "\nScreen recording start : " << endl;
            //wcout << "\n" << (img + to_wstring(counter));
            //wcout << "\n" << img;
            
        }
        if(GetAsyncKeyState(VK_SPACE))
        {
            //cout << "[SPACE]" << endl;
            keystate[0] = 1;
        }
        if(GetAsyncKeyState(0x31))
        {
            //cout << "1" << endl;
            keystate[1] = 1;
        }
        if(GetAsyncKeyState(0x32))
        {
            //cout << "2" << endl;
            keystate[2] = 1;
        }
        if(GetAsyncKeyState(0x33))
        {
            //cout << "3" << endl;
            keystate[3] = 1;
        }
        if(GetAsyncKeyState(0x34))
        {
            //cout << "4" << endl;
            keystate[4] = 1;
        }
        if(GetAsyncKeyState(0x35))
        {
            //cout << "5" << endl;
            keystate[5] = 1;
        }
        if(GetAsyncKeyState(0x36))
        {
            //cout << "6" << endl;
            keystate[6] = 1;
        }
        if(GetAsyncKeyState(0x37))
        {
            //cout << "7" << endl;
            keystate[7] = 1;
        }
        if(GetAsyncKeyState(VK_LEFT))
        {
            //cout << "[ARROW_LEFT]" << endl;
            keystate[8] = 1;
        }
        if(GetAsyncKeyState(VK_RIGHT))
        {
            //cout << "[ARROW_RIGHT]" << endl;
            keystate[9] = 1;
        }
        if(GetAsyncKeyState(VK_UP))
        {
            //cout << "[ARROW_UP]" << endl;
            keystate[10] = 1;
        }
        if(GetAsyncKeyState(VK_DOWN))
        {
            //cout << "[ARROW_DOWN]" << endl;
            keystate[11] = 1;
        }
        if(GetAsyncKeyState(VK_SHIFT))
        {
            //cout << "[SHIFT]" << endl;
            keystate[12] = 1;
        }
        if(GetAsyncKeyState(VK_CONTROL))
        {
            //cout << "[CTRL]" << endl;
            keystate[13] = 1;
        }
       

        if(setup)
        {
            //add image file path
            //may want to make path a fixed size.

            //add previous images to file - for velocity, elc
            if(counter == 0)
            {
                 outputFile << img_path + to_string(counter) + ".jpeg";
                 outputFile << ",";
                outputFile << img_path + to_string(counter) + ".jpeg";
                 outputFile << ",";
                outputFile << img_path + to_string(counter) + ".jpeg";
                 outputFile << ",";
                outputFile << img_path + to_string(counter) + ".jpeg";
            }
            else if(counter ==1)
            {
                 outputFile << img_path + to_string(counter - 1) + ".jpeg";
                 outputFile << ",";
                outputFile << img_path + to_string(counter - 1) + ".jpeg";
                 outputFile << ",";
                outputFile << img_path + to_string(counter - 1) + ".jpeg";
                 outputFile << ",";
                outputFile << img_path + to_string(counter) + ".jpeg";
            }
            else if(counter ==2)
            {
                 outputFile << img_path + to_string(counter - 2) + ".jpeg";
                 outputFile << ",";
                outputFile << img_path + to_string(counter - 2) + ".jpeg";
                 outputFile << ",";
                outputFile << img_path + to_string(counter - 1) + ".jpeg";
                 outputFile << ",";
                outputFile << img_path + to_string(counter) + ".jpeg";
            }
            else
            {
                outputFile << img_path + to_string(counter - 3) + ".jpeg";
                 outputFile << ",";
                outputFile << img_path + to_string(counter - 2) + ".jpeg";
                 outputFile << ",";
                outputFile << img_path + to_string(counter - 1) + ".jpeg";
                 outputFile << ",";
                outputFile << img_path + to_string(counter) + ".jpeg";
            }

            //outputFile << img_path + to_string(counter) + ".jpeg";

            //read keys

            //cout << "{";
            for(int i=0; i<14; i++)
            {
                //cout << keystate[i];
                outputFile << "," << keystate[i];
                keystate[i] = 0;
            }
            //cout << "}" << endl;
            outputFile << endl;

            //reset keys

            //screen capture
            //For later - create the file name and pass in
        
            imgStr = img_dir + to_wstring(counter) + L".jpeg";
            gdiscreen(hwnd, imgStr);
            counter++;
        }
        
        Sleep(250); //capture screen 4 times per second
    }

    // close the output file
    outputFile.close();

    return 0;
}

//define your .csv format
// image_location, key1_down, key2_down, ..., elc.

