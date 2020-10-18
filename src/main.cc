#include <plot.h>
#include <iostream>

int Main(int argc, char* argrv[]) 
{
	std::cout << "begin main" << std::endl;

	// initialize the plotter and variables
  plot p(300, 300);
	int x = 0, y = 0;
	bool stop = false;
	p.plot_set_color(0, 255, 0);
	p.plot_line(point2d(40, 40), point2d(100, 100));
	p.plot_set_color(255,0, 0);
  p.plot_arc(100, 100, 5, 0, 360);
	p.plot_flush();

	std::cout << "end main" << std::endl;
	return 1;
}

	/* p.plot_set_color(255, 0, 0);
 	for (int i = 0; i < 300; i++) {
 		for (int j = 0; j < 300; j++) {
			p.plot_point(i, j);
		}
	}    
	p.plot_set_color(0, 255, 0);
	p.plot_line(0, 300, 300, 0); */
