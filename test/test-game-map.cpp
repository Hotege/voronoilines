
#define UNICODE
#define _UNICODE

#include "../src/randomize.h"
#include "../src/poisson.h"
#include "../src/game.h"

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
    vl::voronoi voronoi(vertices, triangles, W, H);
    vl::map map(voronoi);
    auto& V = map.get_distribution();

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
        int FH = 8;
        Gdiplus::Font font(Gdiplus::FontFamily::GenericSansSerif(), FH);
        Gdiplus::SolidBrush brush(Gdiplus::Color::DarkGray);
        for (size_t i = 0; i < V.get_points().size(); i++)
        {
            auto& vt = V.get_points()[i];
            int32_t weight = map.get_weights()[i];
            TCHAR str[10] = { 0 };
            _itot(weight, str, 10);
            Gdiplus::REAL x = (Gdiplus::REAL)vt.x;
            Gdiplus::REAL y = (Gdiplus::REAL)vt.y;
            graphics.DrawString(str, -1, &font,
                Gdiplus::PointF(x - FH / 4, y - FH / 2), &brush);
        }

        Gdiplus::Pen pen_line(Gdiplus::Color::Green);
        for (size_t i = 0; i < V.get_borders().size(); i++)
        {
            auto& border = voronoi.get_borders()[i];
            if (!border.get_vertical_valid())
                continue;
            auto& vertical = border.get_vertical();
            auto c0 = vertical.first, c1 = vertical.second;
            Gdiplus::REAL x0((Gdiplus::REAL)c0.x), y0((Gdiplus::REAL)c0.y);
            Gdiplus::REAL x1((Gdiplus::REAL)c1.x), y1((Gdiplus::REAL)c1.y);
            graphics.DrawLine(&pen_line, x0, y0, x1, y1);
        }

        CLSID png;
        GetEncoderClsid(_T("image/png"), &png);
        bitmap.Save(_T("game.png"), &png, NULL);
        DeleteObject(hdc);
    }
    Gdiplus::GdiplusShutdown(token);

    return 0;
}
