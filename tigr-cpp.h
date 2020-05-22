/*
USAGE:

    auto t = std::make_shared<tigr::Window>(200, 200, "Example", (int)tigr::WindowFlag::AUTO);
    auto image = std::make_shared<tigr::Bitmap>(200, 200);

    while (!t->isClosed()) {
        t->clear(0, 0, 0);

        for (int y = 0; y < 200; ++y) {
            for (int x = 0; x < 200; ++x) {
                if ((x % 2 == 0 && y % 2 != 0) || (x % 2 != 0 && y % 2 == 0))
                    image->setPixel(x, y, tigr::RGBA{ 255, 255, 255 });
                else
                    image->setPixel(x, y, tigr::RGBA{ 0, 0, 0 });
            }
        }

        t->blitTint(image, tigr::RGBA{ 255, 0, 0 });
        t->blitTint(image, 10, 10, 0, 0, 180, 180, tigr::RGBA{ 0, 0, 255 });
        t->blitTint(image, 20, 20, 0, 0, 160, 160, tigr::RGBA{ 0, 255, 0 });
        t->print(0, 0, tigr::RGBA{ 255, 255, 255 }, "Hello world");
        t->update();
    }


Not yet implemented functions:
    tigrPlot
    tigrPosition
    tigrEnforceScale
    tigrResize
    tigrPrint
*/

#pragma once
#include <tigr.h>
#include <algorithm>
#include <memory>
#include <cassert>
#include <cstdarg>

namespace tigr {

enum class WindowFlag : int {
    FIXED = 0,
    AUTO = 1,
    A2X = 2,
    A3X = 4,
    A4X = 8,
    RETINA = 16,
};

struct RGBA {
    unsigned char r, g, b, a;

    RGBA(TPixel pixel) : r(pixel.r), g(pixel.g), b(pixel.b), a(pixel.a) {
    }

    explicit RGBA(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0, unsigned char a = 255) : r(r), g(g), b(b), a(a) {
    }

    operator TPixel() const {
        return tigrRGBA(r, g, b, a);
    }
};

class Bitmap {
   protected:
    Tigr* b = nullptr;

   public:
    Bitmap() {
    }

    Bitmap(int w, int h) {
        b = tigrBitmap(w, h);
    }

    virtual ~Bitmap() {
        tigrFree(b);
        b = nullptr;
    }

    TPixel& at(int x, int y) {
        return b->pix[y * b->w + x];
    }

    void clear(unsigned char r, unsigned char g, unsigned char w) {
        clear(tigrRGB(r, g, w));
    }

    void clear(const RGBA& color) {
        tigrClear(b, color);
    }

    void setPixel(int x, int y, const RGBA& color) {
        at(x, y) = color;
    }

    void setRectWH(int x, int y, int w, int h, const RGBA& color) {
        tigrRect(b, x, y, w, h, color);
    }

    void setRect(int x0, int y0, int x1, int y1, const RGBA& color) {
        assert(x1 > x0 && y1 > y0);
        tigrRect(b, x0, y0, x1 - x0, y1 - y0, color);
    }

    void setFillWH(int x, int y, int w, int h, const RGBA& color) {
        tigrFill(b, x, y, w, h, color);
    }

    void setFill(int x0, int y0, int x1, int y1, const RGBA& color) {
        assert(x1 > x0 && y1 > y0);
        tigrFill(b, x0, y0, x1 - x0, y1 - y0, color);
    }

    void setLine(int x0, int y0, int x1, int y1, const RGBA& color) {
        tigrLine(b, x0, y0, x1, y1, color);
    }

    void blit(std::shared_ptr<Bitmap> source, int dx, int dy, int sx, int sy, int w, int h) {
        tigrBlit(b, source->b, dx, dy, sx, sy, w, h);
    }

    void blit(std::shared_ptr<Bitmap> source) {
        int minW = std::min(b->w, source->b->w);
        int minH = std::min(b->h, source->b->h);
        blit(source, 0, 0, 0, 0, minW, minH);
    }

    void blitTint(std::shared_ptr<Bitmap> source, int dx, int dy, int sx, int sy, int w, int h, const RGBA& tint) {
        tigrBlitTint(b, source->b, dx, dy, sx, sy, w, h, tint);
    }

    void blitTint(std::shared_ptr<Bitmap> source, const RGBA& tint) {
        int minW = std::min(b->w, source->b->w);
        int minH = std::min(b->h, source->b->h);
        tigrBlitTint(b, source->b, 0, 0, 0, 0, minW, minH, tint);
    }

    void blitAlpha(std::shared_ptr<Bitmap> source, int dx, int dy, int sx, int sy, int w, int h, float alpha) {
        tigrBlitAlpha(b, source->b, dx, dy, sx, sy, w, h, alpha);
    }

    void blitAlpha(std::shared_ptr<Bitmap> source, float alpha) {
        int minW = std::min(b->w, source->b->w);
        int minH = std::min(b->h, source->b->h);
        tigrBlitAlpha(b, source->b, 0, 0, 0, 0, minW, minH, alpha);
    }

    void print(TigrFont* font, int x, int y, const RGBA& color, const char* text, ...) {
        va_list args;
        va_start(args, text);
        tigrPrint(b, font, x, y, color, text, args);
        va_end(args);
    }

    void print(int x, int y, const RGBA& color, const char* text, ...) {
        va_list args;
        va_start(args, text);
        tigrPrint(b, tfont, x, y, color, text, args);
        va_end(args);
    }

    void update() {
        tigrUpdate(b);
    }
};

class Window final : public Bitmap {
   public:
    Window(int w, int h, const char* title, int flags) {
        b = tigrWindow(w, h, title, flags);
    }

    bool isClosed() {
        return tigrClosed(b);
    }
};

}  // namespace tigr
