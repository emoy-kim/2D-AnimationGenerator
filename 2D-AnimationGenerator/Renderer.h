/*
 * Author: Emoy Kim
 * E-mail: emoy.kim_AT_gmail.com
 * 
 * This code is a free software; it can be freely used, changed and redistributed.
 * If you use any version of the code, please reference the code.
 * 
 */

#pragma once

#include "_Common.h"

#include "Camera.h"
#include "Object.h"
#include "Animator.h"

class RendererGL
{
   static RendererGL* Renderer;
   GLFWwindow* Window;

   double StartTiming;

   CameraGL MainCamera;
   ShaderGL ObjectShader;
   
   LightGL Lights;
   vector<ObjectGL> Objects;

   Animator2D Animator;
 
   void registerCallbacks() const;
   void initialize();

   void printOpenGLInformation() const;

   void error(int error, const char* description) const;
   void cleanup(GLFWwindow* window);
   void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
   void reshape(GLFWwindow* window, int width, int height);
   static void errorWrapper(int error, const char* description);
   static void cleanupWrapper(GLFWwindow* window);
   static void keyboardWrapper(GLFWwindow* window, int key, int scancode, int action, int mods);
   static void reshapeWrapper(GLFWwindow* window, int width, int height);

   void setKeyFrames(const vector<Animator2D::KeyFrame>& key_frames);
   void setLights();
   void setObject();

   void getWorldMatrix(mat4& to_world, const Animator2D::Animation& animation, const uint& key_frame_index);
   void drawObject();
   void render();


public:
   RendererGL(const RendererGL&) = delete;
   RendererGL(const RendererGL&&) = delete;
   RendererGL& operator=(const RendererGL&) = delete;
   RendererGL& operator=(const RendererGL&&) = delete;


   RendererGL();
   ~RendererGL();

   void play(const vector<Animator2D::KeyFrame>& key_frames);
};