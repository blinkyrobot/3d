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
#include <vector>
#include <algorithm>

typedef std::pair<vec3f, vec3f> Line; 

int Main(int argc, char **argv)
{
    // Initialization
		vec3f cameraPosition(1, 1, 5);
    vec3f Target(0, 0, 0);
    plot p(300, 300);
    float canvasWidth = 1.00, canvasHeight = 1.00;
    uint32_t imageWidth = 300, imageHeight = 300;
    bool redraw = true;

    // game loop: listen to events and render graphics
    while (1) { 
      // get event messages
      tagMSG lpMsg;
      if (::PeekMessageA(&lpMsg, NULL, 0, 0, PM_REMOVE)) {
        redraw = true;
        if (lpMsg.message == WM_KEYDOWN) { 
          switch (lpMsg.wParam) {
            case VK_LEFT:
              cameraPosition.x -= 0.1;
              break;
            case VK_RIGHT:
              cameraPosition.x += 0.1;
              break;
            case VK_UP:
              if (cameraPosition.z > 4.1) {
                cameraPosition.z -= 0.1;
              }
              break;
            case VK_DOWN:
              cameraPosition.z += 0.1;
              break;
            default:
              break;
          }
        } else if (lpMsg.message == WM_CHAR) {
          switch (lpMsg.wParam) {
            case 'o':
              cameraPosition.x = 1;   
              cameraPosition.y = 1;   
              cameraPosition.z = 5;
              break;
            default:
              break;
          }
        }  
      }

      if (redraw) {
        p.plot_clear(200, 200, 200);
      } else {
        ::Sleep(10);
        continue;
      } 

		  matrix4f cameraToWorld = computeCameraToWorldMatrix(cameraPosition,
                                                          Target);
      matrix4f worldToCamera=invert(cameraToWorld);

      // draw the axes
      worldToPixel transformer(worldToCamera, canvasWidth, canvasHeight,
                              imageWidth, imageHeight);
      vec3f point0(0, 0, 0);
      vec2i point0R = transformer(point0);
      std::vector<vec3f> pointVec = { {1,0,0}, {0,1,0}, {0,0,1} };
      std::vector<vec2i> pointRVec; 
      pointRVec.resize(pointVec.size()); // allocate space 
      std::transform(pointVec.begin(), pointVec.end(), 
                     pointRVec.begin(), transformer);
      p.plot_set_color(0, 255, 0);
      p.plot_line(point0R, pointRVec[0]);
      p.plot_line(point0R, pointRVec[1]);
      p.plot_line(point0R, pointRVec[2]);

      p.plot_set_color(255, 255, 255);
      char buffer[30];
      sprintf(buffer, "(%.1f, %.1f, %.1f)", cameraPosition.x, cameraPosition.y,
              cameraPosition.z);
      p.plot_text(0, 0, buffer); 

      // draw the xz-plane 
      float step = 0.5; // step size
 
      Line u({-cameraPosition.z, 0, -step}, 
            {cameraPosition.z, 0, -step});   // x-direction grid lines
      Line vr({0, 0, 0},                     // z-direciton grid lines 
              {0, 0, cameraPosition.z - 1}); // on right half of plane 
      Line vl({0, 0, 0},                     // z-direction grid lines
              {0, 0, cameraPosition.z - 1}); // on left half of plane 

      for (float t = 0; t <= cameraPosition.z - 1; t += step) {
        u.first.z = t; 
        u.second.z = t; 
        vr.first.x = t;
        vr.second.x = t;
        vl.first.x = -t;
        vl.second.x = -t;

        std::vector<vec3f> vec = {u.first, u.second, vl.first, vl.second,
                                  vr.first, vr.second}; 
        std::vector<vec2i> rVec; 
        rVec.resize(vec.size()); // allocate space 
        std::transform(vec.begin(), vec.end(), 
                       rVec.begin(), transformer);
        p.plot_set_color(255, 255, 255);
        p.plot_line(rVec[0], rVec[1]);
        p.plot_line(rVec[2], rVec[3]);
        p.plot_line(rVec[4], rVec[5]);
      } 

      // render 
      p.plot_flush(); 
      Sleep(10);
      redraw = false;
    } // end game loop

    return 1;

}
