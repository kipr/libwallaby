#include "kipr/graphics/graphics.h"

#include <vector>
#include <algorithm>
#include "PixelToaster.h"

#include <iostream>

#include <string.h>
#include <math.h>

using namespace PixelToaster;
using namespace std;

namespace Kiss
{
	struct Graphics
	{
		Display display;
		vector<Pixel> pixels;
		
		inline int width() const
		{
			return display.width();
		}
		
		inline int height() const
		{
			return display.height();
		}
		
		inline int size() const
		{
			return width() * height();
		}
		
		inline bool isValidPoint(const int x, const int y) const
		{
			return x >= 0 && y >= 0 && x < width() && y < height();
		}
	} g_graphics;
	
	class PixelToasterListener : public Listener
	{
	public:
		PixelToasterListener()
		{
			memset(m_keyStates, false, sizeof(bool) * 0xFF);
		}
		
		bool isMiddleButtonDown() const
		{
			return m_mouse.buttons.middle;
		}
		
		bool isLeftButtonDown() const
		{
			return m_mouse.buttons.left;
		}
		
		bool isRightButtonDown() const
		{
			return m_mouse.buttons.right;
		}
		
		int mouseX() const
		{
			return m_mouse.x;
		}
		
		int mouseY() const
		{
			return m_mouse.y;
		}
		
		bool isKeyPressed(const int key)
		{
			return m_keyStates[key];
		}
		
	protected:
		void onKeyDown(DisplayInterface& display, Key key)
		{
			m_keyStates[(int)key] = true;
		}

		void onKeyPressed(DisplayInterface& display, Key key)
		{
		}

		void onKeyUp(DisplayInterface& display, Key key)
		{
			m_keyStates[(int)key] = false;
		}

		void onMouseButtonDown(DisplayInterface& display, Mouse mouse)
		{
			m_mouse = mouse;
		}

		void onMouseButtonUp( DisplayInterface & display, Mouse mouse )
		{
			m_mouse = mouse;
		}

		void onMouseMove(DisplayInterface & display, Mouse mouse)
		{
			m_mouse = mouse;
		}

		void onActivate(DisplayInterface & display, bool active)
		{
		}

		void onOpen(DisplayInterface& display)
		{
		}

		bool onClose(DisplayInterface& display)
		{
			exit(0);
			return true;
		}
		
	private:
		Mouse m_mouse;
		bool m_keyStates[0xFF];
	} g_listener;
}

using namespace Kiss;

inline Pixel fromTrueColor(Encoding enc, const int _0, const int _1, const int _2)
{
	switch(enc) {
		case RGB: return Pixel(_0 / 255.0, _1 / 255.0, _2 / 255.0);
		case BGR: return Pixel(_2 / 255.0, _1 / 255.0, _0 / 255.0);
	}
}

int graphics_open(int width, int height)
{
	if(!g_graphics.display.open("KISS Graphics Window", width, height)) return false;
	g_graphics.pixels.assign(g_graphics.size(), Pixel());
	g_graphics.display.listener(&g_listener);
	return true;
}

void graphics_close()
{
	g_graphics.display.close();
}

void graphics_update()
{
	if(!g_graphics.display.update(g_graphics.pixels)) {
		cerr << "Warning: graphics_update failed." << endl;
	}
}

void graphics_clear()
{
    graphics_fill(0, 0, 0);
}

void graphics_blit(const unsigned char *data, int x, int y, int width, int height)
{
	graphics_blit_enc(data, RGB, x, y, width, height);
}

void graphics_blit_region(const unsigned char *data, int sx, int sy, int ex, int ey, int width, int height, int dx, int dy)
{
	graphics_blit_region_enc(data, RGB, sx, sy, ex, ey, width, height, dx, dy);
}

void graphics_blit_enc(const unsigned char *data, Encoding enc, int x, int y, int width, int height)
{
	graphics_blit_region_enc(data, enc, 0, 0, width - 1, height - 1, width, height, x, y);
}

void graphics_blit_section(const unsigned char *data, Encoding enc, const int index, const int dindex, const int length)
{
	int actualLength = length + dindex < g_graphics.size()
		? length : g_graphics.size() - dindex;
	
	if(actualLength > g_graphics.size()) actualLength = g_graphics.size();
	if(actualLength <= 0) return;
	
	for(int i = 0; i < actualLength; ++i) {
		const unsigned int offset = (i + index) * 3;
		g_graphics.pixels[dindex + i] = fromTrueColor(enc, data[offset],
			data[offset + 1], data[offset + 2]);
	}
}

void graphics_blit_region_enc(const unsigned char *data, Encoding enc, int sx, int sy, int ex, int ey, int width, int height, int dx, int dy)
{
	if(sx >= ex || sy >= ey) return;
	if(dx >= g_graphics.width() || dy >= g_graphics.height()) return;
	ey = ey > height - 1 ? height - 1 : ey;

	const int hsize = ey - sy + 1;
	for(int i = 0; i < hsize; ++i) {
		int cols = ex - sx + 1;
		if(cols + dx > g_graphics.width()) cols -= dx;
		const int index = (sy + i) * width + sx;
		const int dindex = (dy + i) * g_graphics.width() + dx;
		graphics_blit_section(data, enc,
			index < 0 ? 0 : index,
			dindex < 0 ? 0 : dindex,
			cols);
	}
}

void graphics_fill(int r, int g, int b)
{
	g_graphics.pixels.assign(g_graphics.size(), fromTrueColor(RGB, r, g, b));
}

void graphics_pixel(int x, int y, int r, int g, int b)
{
	if(!g_graphics.isValidPoint(x, y)) return;
	g_graphics.pixels[y * g_graphics.width() + x] = fromTrueColor(RGB, r, g, b);
}

void graphics_line(int x1, int y1, int x2, int y2, int r, int g, int b)
{
	int steep = abs(y2 - y1) > abs(x2 - x1) ? 1 : 0;

	if(steep) {
		swap(x1, y1);
		swap(x2, y2);
	}
	
	if(x1 > x2) {
		swap(x1, x2);
		swap(y1, y2);
	}

	int deltax = x2 - x1;
	int deltay = abs(y2 - y1);
	int error = -(deltax + 1) >> 1;
	int y = y1;

	int ystep = y1 < y2 ? 1 : -1;

	for(int x = x1; x <= x2; ++x) {
		if(steep) graphics_pixel(y, x, r, g, b);
		else graphics_pixel(x, y, r, g, b);
		error += deltay;
		if(error >= 0) {
			y += ystep;
			error -= deltax;
		}
	}
}

void graphics_circle(int cx, int cy, int radius, int r, int g, int b)
{
	int f = 1 - radius;
	int ddF_x = 0;
	int ddF_y = -2 * radius;

	graphics_pixel(cx, cy + radius, r, g, b);
	graphics_pixel(cx, cy - radius, r, g, b);
	graphics_pixel(cx + radius, cy, r, g, b);
	graphics_pixel(cx - radius, cy, r, g, b);

	int x = 0;
	int y = radius;
	while(x < y) {
		if(f >= 0) {
			--y;
			ddF_y += 2;
			f += ddF_y;
		}
		++x;
		ddF_x += 2;
		f += ddF_x + 1;
		graphics_pixel(cx + x, cy + y, r, g, b);
		graphics_pixel(cx - x, cy + y, r, g, b);
		graphics_pixel(cx + x, cy - y, r, g, b);
		graphics_pixel(cx - x, cy - y, r, g, b);
		graphics_pixel(cx + y, cy + x, r, g, b);
		graphics_pixel(cx - y, cy + x, r, g, b);
		graphics_pixel(cx + y, cy - x, r, g, b);
		graphics_pixel(cx - y, cy - x, r, g, b);
	}
}

void graphics_circle_fill(int cx, int cy, int radius, int r, int g, int b)
{
	const long c_squared = (long)radius * (long)radius;
	
	for(int i = cx - radius; i < cx + radius; ++i) {
		for(int j = cy - radius; j < cy + radius; ++j) {
			long dx = cx - i;
			dx *= dx;
			
			long dy = cy - j;
			dy *= dy;
			
			if(dx + dy <= c_squared) {
				graphics_pixel(i, j, r, g, b);
			}
		}
	}
}

void graphics_rectangle(int x1, int y1, int x2, int y2, int r, int g, int b)
{
	graphics_line(x1, y1, x1, y2, r, g, b);
	graphics_line(x1, y1, x2, y1, r, g, b);
	graphics_line(x2, y2, x2, y1, r, g, b);
	graphics_line(x2, y2, x1, y2, r, g, b);
}

void graphics_rectangle_fill(int x1, int y1, int x2, int y2, int r, int g, int b)
{
	if(x1 > x2) swap(x1, x2);
	if(y1 > y2) swap(y1, y2);

	for(int i = x1; i <= x2; ++i) {
		for(int j = y1; j <= y2; ++j) {
			graphics_pixel(i, j, r, g, b);
		}
	}
}

void graphics_triangle(int x1, int y1, int x2, int y2, int x3, int y3, int r, int g, int b)
{
	graphics_line(x1, y1, x2, y2, r, g, b);
	graphics_line(x2, y2, x3, y3, r, g, b);
	graphics_line(x3, y3, x1, y1, r, g, b);
}

void graphics_triangle_fill(int x1, int y1, int x2, int y2, int x3, int y3, int r, int g, int b)
{
	const double l = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
	const double dx = (x2 - x1) / l;
	const double dy = (y2 - y1) / l;

	for(int i = 0; i <= l; ++i) graphics_line(x3, y3, x1 + i * dx, y1 + i * dy, r, g, b);
}

int get_key_state(enum KeyCode key)
{
	return g_listener.isKeyPressed((int)key) ? 1 : 0;
}

void get_mouse_position(int *x, int *y)
{
	*x = g_listener.mouseX();
	*y = g_listener.mouseY();
}

int get_mouse_middle_button()
{
	return g_listener.isMiddleButtonDown();
}

int get_mouse_left_button()
{
	return g_listener.isLeftButtonDown();
}

int get_mouse_right_button()
{
	return g_listener.isRightButtonDown();
}
