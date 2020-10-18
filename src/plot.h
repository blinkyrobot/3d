// Provides an interface to the low level GDI draw functions
#include <types.h>

class plot
{
public:
	plot(int width, int height);
	~plot();

	// accessors

	// manipulators
	void plot_set_color(int red, int green, int blue);
	void plot_point(const point2d& p);
	void plot_line(const point2d& p1, const point2d& p2);
	void plot_square(int x, int y, int length);
	void plot_triangle(const point2d&, const point2d&, const point2d&);
  void plot_arc(int x, int y, float r, float startAngle, float sweepAngle);
  void plot_text(int x, int y, char* string);
  void plot_clear(int red = 255, int green = 255, int blue = 255);
	void plot_flush();

private:
	int _width;
	int _height;
	int _red;
	int _green;
	int _blue;
};

