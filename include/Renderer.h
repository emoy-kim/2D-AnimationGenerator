#pragma once

#include "_Common.h"
#include "Animator.h"
#include "Light.h"
#include "Object.h"

class RendererGL
{
public:
   RendererGL(const RendererGL&) = delete;
   RendererGL(const RendererGL&&) = delete;
   RendererGL& operator=(const RendererGL&) = delete;
   RendererGL& operator=(const RendererGL&&) = delete;


   RendererGL();
   ~RendererGL();

   void play(const std::vector<Animator2D::KeyFrame>& key_frames);

private:
   inline static RendererGL* Renderer = nullptr;
   GLFWwindow* Window;
   int FrameWidth;
   int FrameHeight;
   double StartTiming;
   std::unique_ptr<CameraGL> MainCamera;
   std::unique_ptr<ShaderGL> ObjectShader;
   std::vector<std::unique_ptr<ObjectGL>> Objects;
   std::unique_ptr<LightGL> Lights;
   std::unique_ptr<Animator2D> Animator;
 
   void registerCallbacks() const;
   void initialize();

   static void printOpenGLInformation();

   void error(int error, const char* description) const;
   void cleanup(GLFWwindow* window);
   void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
   void reshape(GLFWwindow* window, int width, int height) const;
   static void errorWrapper(int error, const char* description);
   static void cleanupWrapper(GLFWwindow* window);
   static void keyboardWrapper(GLFWwindow* window, int key, int scancode, int action, int mods);
   static void reshapeWrapper(GLFWwindow* window, int width, int height);

   void setKeyFrames(const std::vector<Animator2D::KeyFrame>& key_frames) const;
   void setLights() const;
   void setObjects();
   void drawObject() const;
   void render() const;
};