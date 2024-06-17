
#include "../src/randomize.h"
#include "../src/poisson.h"

#include <Windows.h>
#include <tchar.h>
#include <gdiplus.h>

INT W = 640, H = 480;
Gdiplus::REAL radius = 2.5;

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
    UINT num = 0;          // number of image encoders
    UINT size = 0;         // size of the image encoder array in bytes
    Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;
    Gdiplus::GetImageEncodersSize(&num, &size);
    if (size == 0)
        return -1;  // Failure
    pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
    if (pImageCodecInfo == NULL)
        return -1;  // Failure
    GetImageEncoders(num, size, pImageCodecInfo);
    for (UINT j = 0; j < num; j++)
    {
        if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
        {
            *pClsid = pImageCodecInfo[j].Clsid;
            free(pImageCodecInfo);
            return j;  // Success
        }
    }
    free(pImageCodecInfo);
    return -1;  // Failure
}

int main(int argc, char* argv[])
{
    vl::randomize(0x19950723);
    if (argc > 1)
        vl::randomize(atoi(argv[1]));
    auto vertices = vl::poisson_distribute(640, 480, 10);

    ULONG_PTR token;
    Gdiplus::GdiplusStartupInput input;
    Gdiplus::GdiplusStartup(&token, &input, NULL);
    HDC hdc = CreateCompatibleDC(NULL);
    if (hdc)
    {
        Gdiplus::Bitmap bitmap(W, H, PixelFormat32bppARGB);
        Gdiplus::Graphics graphics(&bitmap);
        graphics.Clear(Gdiplus::Color::White);
        Gdiplus::Pen pen(Gdiplus::Color::Black);
        for (auto& v : vertices)
        {
            Gdiplus::REAL x = (Gdiplus::REAL)v.x;
            Gdiplus::REAL y = (Gdiplus::REAL)v.y;
            graphics.DrawEllipse(&pen, x, y, radius, radius);
        }
        CLSID png;
        GetEncoderClsid(_T("image/png"), &png);
        bitmap.Save(_T("poisson.png"), &png, NULL);
        DeleteObject(hdc);
    }
    Gdiplus::GdiplusShutdown(token);

    return 0;
}
