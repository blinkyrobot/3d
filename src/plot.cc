// This is the universal plot interface
// Can be configured to interact wtih winPlot, psPlot underneath
#include <plot.h>
#include <winMain.h>
#include <iostream>

// constructor
plot::plot(int width, int height) :
	_width(width), _height(height),
	_red(255), _green(255), _blue(255) 
{
	std::cout << "create plot" << std::endl;
}

plot::~plot() {
	std::cout << "destroy plot" << std::endl;
}

void
plot::plot_set_color(int red, int green, int blue)
{
	_red = red;
	_green = green;
	_blue = blue;
}
	
void
plot::plot_point(const point2d& p)
{
	winPlot(p.x, p.y, _red, _green, _blue);
}

void
plot::plot_line(const point2d& p1, const point2d& p2)
{
	winPlotLine(p1.x, p1.y, p2.x, p2.y, _red, _green, _blue);
}

void
plot::plot_square(int x, int y, int length)
{
  point2d a(x, y);
  point2d b(x + length, y);
  point2d c(x + length, y + length);
  point2d d(x, y + length);
	plot_line(a, b);
	plot_line(b, c);
	plot_line(c, d);
	plot_line(d, a);
}

void
plot::plot_triangle(const point2d& p1, const point2d& p2, const point2d& p3)
{
	plot_line(p1, p2);
	plot_line(p2, p3);
	plot_line(p3, p1);
}

void 
plot::plot_arc(int x, int y, float r, float startAngle, float sweepAngle)
{
  winPlotArc(x, y, r, startAngle, sweepAngle);
}

void
plot::plot_text(int x, int y, char* string)
{
  winPlotText(x, y, string);
}

void
plot::plot_clear(int red, int green, int blue)
{
  winClear(red, green, blue);
}

void
plot::plot_flush()
{
	winFlush();
}
