/* -*- coding: utf-8 -*- */

/** 
 * test-021-anttweak.cpp
 *
 * Simple GLFWWindow + triangle + AntTweakBar
 */
#include <AntTweakBar.h>
#include <GLFW/glfw3.h>
#include <string>
#include <stdlib.h>
#include <iostream>

// ***************************************************************************
// **************************************************************** TwCallback
// void TwEventMouseButtonGLFW3( GLFWwindow *win, /*not used*/
//                                      int glfwButton, int glfwAction,
//                                      int mods /* not used */)
// {
//   TwEventMouseButtonGLFW( glfwButton, glfwAction );
// }
// int TW_CALL TwEventMousePosGLFW3( GLFWwindow *win, /*not used*/
//                                   double xpos, double ypos )
// {
//   //std::cout << "++Pos x=" << xpos << " y=" << ypos << std::endl;
//   return TwEventMousePosGLFW( (int) (xpos+0.5), (int) (ypos+0.5) );
// }
// int TW_CALL TwEventMouseWheelGLFW3( GLFWwindow *win, /*not used*/
//                                     double xoffset, double yoffset )
// {
//   std::cout << "++Wheel x=" << xoffset << " y=" << yoffset << std::endl;
//   /* TODO yoffset is only +1/0/-1, AntTWeak need sth else ? */
//   return TwEventMouseWheelGLFW( (int) (yoffset+0.5) );
// }
// int TW_CALL TwEventKeyGLFW3( GLFWwindow *win, /*not used*/
//                              int glfwKey, int glfwScanCode,
//                              int glfwAction, int glfwMods)
// {
//   /* TODO rewrite using glfwMods, etc... [see TwEventGLFW.c] */
//   return TwEventKeyGLFW( glfwKey, glfwAction);
// }
// int TW_CALL TwEventCharGLFW3( GLFWwindow *win, /*not used*/
//                               int glfwChar )
// {
//   return TwEventCharGLFW( glfwChar, GLFW_PRESS );
// }
// ******************************************************************** Window
/**
 *
 */
class Window
{
public:
  /**  
   * Création avec titre et taille de fenetre.
   */
  Window(const std::string& title = "GLFW Window", int width=640, int height=400)
  {
    std::cout << "Window creation" << std::endl;
    GLFWwindow* window;

    /* Variable set using AntTweakBar */
    double angle_rot = 0;
    double time = 0;
    int wire = 0;       // Draw model in wireframe?
    float bgColor[] = { 0.1f, 0.2f, 0.4f };         // Background color
    
    glfwSetErrorCallback(error_callback);

    std::cout << "__GLFW Init" << std::endl;
    if (!glfwInit())
        exit(EXIT_FAILURE);
    
    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (!window) {
      glfwTerminate();
      exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    std::cout << "__AntTWEAK" << std::endl;
    // Initialize AntTweakBar
    TwInit(TW_OPENGL, NULL);
    TwWindowSize( width, height);

    // Create AntTweakBar
    TwBar* _bar = TwNewBar( "TweakBar" );
    TwDefine(" GLOBAL help='Example to integrate AntTweakBar with GLFW.' ");

    // Add 'speed' to 'bar': it is a modifable (RW) variable of type TW_TYPE_DOUBLE. Its key shortcuts are [r] and [R]. */
    TwAddVarRW(_bar, "rotation", TW_TYPE_DOUBLE, &angle_rot,
               " label='Rot angle' min=-360 max=360 step=5.0 keyIncr=r keyDecr=R help='Rotation angle (degres)' ");
    // Add 'wire' to 'bar': it is a modifable variable of type TW_TYPE_BOOL32 (32 bits boolean). Its key shortcut is [w].
    TwAddVarRW(_bar, "wire", TW_TYPE_BOOL32, &wire,
               " label='Wireframe mode' key=w help='Toggle wireframe display mode.' ");
    // Add 'time' to 'bar': it is a read-only (RO) variable of type TW_TYPE_DOUBLE, with 1 precision digit */
    TwAddVarRO(_bar, "time", TW_TYPE_DOUBLE, &time,
               " label='Time' precision=1 help='Time (in seconds).' ");
    // Add 'bgColor' to 'bar': it is a modifable variable of type TW_TYPE_COLOR3F (3 floats color) */
    TwAddVarRW(_bar, "bgColor", TW_TYPE_COLOR3F, &bgColor,
               " label='Background color' ");
    
    std::cout << "__SET Callback" << std::endl;
    // - Directly redirect GLFW mouse button events to AntTweakBar
    glfwSetMouseButtonCallback( window,
                                (GLFWmousebuttonfun) TwEventMouseButtonGLFW3 );
    std::cout << "  MouseButton" << std::endl;
    // - Directly redirect GLFW mouse position events to AntTweakBar
    glfwSetCursorPosCallback( window,
                              (GLFWcursorposfun) TwEventCursorPosGLFW3 );
    std::cout << "  CursorPos" << std::endl;
    // - Directly redirect GLFW mouse wheel events to AntTweakBar
    glfwSetScrollCallback( window,
                           (GLFWscrollfun) TwEventScrollGLFW3 );
    std::cout << "  Scroll" << std::endl;
    // - Directly redirect GLFW key events to AntTweakBar
    glfwSetKeyCallback( window,
                        (GLFWkeyfun) TwEventKeyGLFW3 );
    std::cout << "  Key" << std::endl;
    // - Directly redirect GLFW key events to AntTweakBar */
    glfwSetCharModsCallback( window,
                             (GLFWcharmodsfun) TwEventCharModsGLFW3 );
    std::cout << "  EventChar" << std::endl;

    // Initialize time
    time = glfwGetTime();
    
    while (!glfwWindowShouldClose(window) &&
           !glfwGetKey(window, GLFW_KEY_ESCAPE)) {
      float ratio;
      int width, height;
      time = glfwGetTime();
      
      glfwGetFramebufferSize(window, &width, &height);
      ratio = width / (float) height;
      
      glViewport(0, 0, width, height);
      glClearColor(bgColor[0], bgColor[1], bgColor[2], 1);
      glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
      
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
      glMatrixMode(GL_MODELVIEW);
      
      glLoadIdentity();
      glRotatef((float) angle_rot, 0.f, 0.f, 1.f);

      if( wire ) {
        glBegin(GL_LINE_STRIP);
      }
      else {
        glBegin(GL_TRIANGLES);
      }
      glColor3f(1.f, 0.f, 0.f);
      glVertex3f(-0.6f, -0.4f, 0.f);
      glColor3f(0.f, 1.f, 0.f);
      glVertex3f(0.6f, -0.4f, 0.f);
      glColor3f(0.f, 0.f, 1.f);
      glVertex3f(0.f, 0.6f, 0.f);
      glEnd();

      // Draw tweak bars
      TwDraw();
      
      glfwSwapBuffers(window);
      glfwPollEvents();
    }
    std::cout << "__RENDER ended" << std::endl;

    TwDeleteBar( _bar );
    std::cout << "__BAR deleted" << std::endl;
    TwTerminate();
    std::cout << "__TW terminated" << std::endl;

    
    glfwDestroyWindow(window);
    std::cout << "__GLFW window detroyed" << std::endl;
    
    glfwTerminate();
    std::cout << "__GLFW terminated" << std::endl;
    exit(EXIT_SUCCESS);
  }
private:
  //******************************************************************************
  /**
   * Callback qui gère les événements 'key'
   */
  static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
  {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GL_TRUE);
  }
  // ***************************************************************************
  /**
   * Callback pour gérer les messages d'erreur de GLFW
   */
  static void error_callback(int error, const char* description)
  {
    std::cerr <<  description << std::endl;
    //fputs(description, stderr);
  }
};

//******************************************************************************
int main( int argc, char *argv[] )
{
  Window win("GFFW + AntTweakBar", 600, 600);
  return 0;
}
