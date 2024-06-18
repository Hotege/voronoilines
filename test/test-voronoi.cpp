
#define _UNICODE

#include "../src/randomize.h"
#include "../src/poisson.h"
#include "../src/voronoi.h"

#include <Windows.h>
#include <tchar.h>
#include <gdiplus.h>

INT W = 640, H = 480;
double R = 32;
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
    auto vertices = vl::poisson_distribute(W, H, R);
    auto triangles = vl::triangulate(vertices, W, H);

    ULONG_PTR token;
    Gdiplus::GdiplusStartupInput input;
    Gdiplus::GdiplusStartup(&token, &input, NULL);
    HDC hdc = CreateCompatibleDC(NULL);
    if (hdc)
    {
        Gdiplus::Bitmap bitmap(W, H, PixelFormat32bppARGB);
        Gdiplus::Graphics graphics(&bitmap);
        graphics.Clear(Gdiplus::Color::White);
        Gdiplus::Pen pen_vertex(Gdiplus::Color::Red);
        for (auto& v : vertices)
        {
            Gdiplus::REAL x = (Gdiplus::REAL)v.x;
            Gdiplus::REAL y = (Gdiplus::REAL)v.y;
            graphics.DrawEllipse(&pen_vertex, x, y, radius, radius);
        }
        Gdiplus::Pen pen_line(Gdiplus::Color::Blue);
        for (auto& t : triangles)
        {
            auto i0 = t.vertices[0], i1 = t.vertices[1], i2 = t.vertices[2];
            auto p0 = vertices[i0], p1 = vertices[i1], p2 = vertices[i2];
            Gdiplus::REAL x0 = (Gdiplus::REAL)p0.x, y0 = (Gdiplus::REAL)p0.y;
            Gdiplus::REAL x1 = (Gdiplus::REAL)p1.x, y1 = (Gdiplus::REAL)p1.y;
            Gdiplus::REAL x2 = (Gdiplus::REAL)p2.x, y2 = (Gdiplus::REAL)p2.y;
            graphics.DrawLine(&pen_line, x0, y0, x1, y1);
            graphics.DrawLine(&pen_line, x1, y1, x2, y2);
            graphics.DrawLine(&pen_line, x2, y2, x0, y0);
        }
        CLSID png;
        GetEncoderClsid(_T("image/png"), &png);
        bitmap.Save(_T("voronoi.png"), &png, NULL);
        DeleteObject(hdc);
    }
    Gdiplus::GdiplusShutdown(token);

    return 0;
}