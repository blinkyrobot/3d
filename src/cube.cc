// 1. define each object in its local space
// 2. position each object in the world space
// 3. position the camera in the world space  
//    Specify the cameraPosition, Target, UP paramenets
// 4. translate the world space to camera space (view transform) 


#include <plot.h>
#include <geometry.h>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <cmath>
#include <windows.h>
#include <algorithm>

const vec3f verts[8] = { {0,0,0}, {0,0,1}, {0,1,0}, {0,1,1}, 
                         {1,0,0}, {1,0,1}, {1,1,0}, {1,1,1} };

const uint32_t numCube = 1;

const uint32_t cube[numCube * 8] = { 0, 4, 5, 1, 3, 2, 6, 7 };

int Main(int argc, char **argv)
{
		vec3f cameraPosition(0,0, 3);
		vec3f Target(0,0, 0);
    plot p(300, 300);
    p.plot_set_color(0, 0, 0);
    float canvasWidth = 2, canvasHeight = 2;
    uint32_t imageWidth = 300, imageHeight = 300;
		const vec3f vWorld[8] = {verts[cube[0]], verts[cube[1]],
	                           verts[cube[2]], verts[cube[3]],
	                           verts[cube[4]], verts[cube[5]],
	                           verts[cube[6]], verts[cube[7]]};
		vec2i vRaster[8];

    bool redraw = true;

    while (1) { 
    // get event messages
    tagMSG lpMsg;
    if (::PeekMessageA(&lpMsg, NULL, 0, 0, PM_REMOVE)) {
      redraw = true;
      if (lpMsg.message == WM_LBUTTONDOWN) {
        p.plot_set_color(0, 0, 255);
      } else if (lpMsg.message == WM_KEYDOWN) { 
        switch (lpMsg.wParam) {
          case VK_LEFT:
            cameraPosition.x -= 0.1;
            break;
          case VK_RIGHT:
            cameraPosition.x += 0.1;
            break;
          case VK_UP:
            cameraPosition.y += 0.1;
            break;
          case VK_DOWN:
            cameraPosition.y -= 0.1;
            break;
          default:
            break;
        }
      }  
    }

    if (redraw) {
      p.plot_clear(300, 300);
    } else {
      ::Sleep(10);
      continue;
    } 

		matrix4f cameraToWorld = computeCameraToWorldMatrix(cameraPosition, 
                                                        Target);
    matrix4f worldToCamera = invert(cameraToWorld);
    std::cerr << worldToCamera << std::endl;

    worldToPixel transformer(worldToCamera, canvasWidth, canvasHeight,
                             imageWidth, imageHeight);

		for (int i=0; i < 8; ++i) {
    	vRaster[i] = transformer(vWorld[i]);
		} 

		 auto const svgLine = [&p, &vRaster] (auto i1, auto i2) {
      p.plot_line(vRaster[i1], vRaster[i2]);
		};	

		for (int i=0; i < 7; ++i) { svgLine(i, i+1); }
		svgLine(3, 0);
		svgLine(1, 6);
		svgLine(0, 5);
		svgLine(2, 7);
		svgLine(4, 7);

    // render 
    p.plot_flush(); 
    Sleep(10);
    redraw = false;
  }
    return 1;
}
