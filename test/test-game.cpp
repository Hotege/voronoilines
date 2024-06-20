
#define UNICODE
#define _UNICODE

#include <cstdio>

#include "../src/game.h"

#include <Windows.h>
#include <tchar.h>
#include <gdiplus.h>

double W = 320, H = 200;
double R = 40;
size_t K = 50;
Gdiplus::REAL radius = 2.5;
double prec = 1e-7;

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
    vl::game g(0x19950723, vl::game::init_data{ W, H, R, K });
    auto& map = g.get_data();
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

        int FH = 8;
        Gdiplus::Font font(Gdiplus::FontFamily::GenericSansSerif(), FH);
        Gdiplus::SolidBrush brush(Gdiplus::Color::DarkGray);
        for (size_t i = 0; i < V.get_points().size(); i++)
        {
            auto& vt = V.get_points()[i];
            int32_t weight = map.get_weights()[i];
            TCHAR str[10] = { 0 };
            _itot(weight, str, 10);
            str[0] = str[1] = str[2] = 0;
            _itot(i, str, 10);

            auto bs = V.get_borders_by_point(i);
            std::vector<vl::segment> lines;
            for (auto& bi : bs)
            {
                auto& b = V.get_borders()[bi];
                if (!b.get_vertical_valid())
                    continue;
                auto segment = b.get_vertical();
                segment = vl::validate_segment(segment, W, H);
                lines.push_back(segment);
            }
            std::vector<vl::vertex> poles;
            if (3 == vl::sort(poles, lines))
            {
                auto v0 = poles[0], v1 = poles[poles.size() - 1];
                if (abs(v0.x - v1.x) > prec && abs(v0.y - v1.y) > prec)
                {
                    vl::vertex corner = { W, H };
                    if (abs(v0.x) < prec || abs(v1.x) < prec)
                        corner.x = 0;
                    if (abs(v0.y) < prec || abs(v1.y) < prec)
                        corner.y = 0;
                    poles.push_back(corner);
                }
            }
            vl::vertex center = { 0, 0 };
            for (auto& p : poles)
            {
                center.x += p.x;
                center.y += p.y;
            }
            center.x /= poles.size();
            center.y /= poles.size();

            Gdiplus::REAL x = (Gdiplus::REAL)center.x;
            Gdiplus::REAL y = (Gdiplus::REAL)center.y;
            Gdiplus::StringFormat format;
            Gdiplus::RectF bounds;
            graphics.MeasureString(str, -1, &font,
                Gdiplus::PointF(0, 0), &format, &bounds);
            Gdiplus::PointF L(x - bounds.Width / 2, y - bounds.Height / 2);
            graphics.DrawString(str, -1, &font, L, &brush);
        }

        Gdiplus::Pen pen_line(Gdiplus::Color::Green);
        for (size_t i = 0; i < V.get_borders().size(); i++)
        {
            auto& border = V.get_borders()[i];
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
