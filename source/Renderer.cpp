#include "Renderer.h"

RendererGL::RendererGL() : 
   Window( nullptr ), FrameWidth( 1920 ), FrameHeight( 1080 ), StartTiming( 0.0 ),
   MainCamera( std::make_unique<CameraGL>() ), ObjectShader( std::make_unique<ShaderGL>() ),
   Lights( std::make_unique<LightGL>() ), Animator( std::make_unique<Animator2D>() )
{
   Renderer = this;

   initialize();
   printOpenGLInformation();
}

void RendererGL::printOpenGLInformation()
{
   std::cout << "****************************************************************\n";
   std::cout << " - GLFW version supported: " << glfwGetVersionString() << "\n";
   std::cout << " - OpenGL renderer: " << glGetString( GL_RENDERER ) << "\n";
   std::cout << " - OpenGL version supported: " << glGetString( GL_VERSION ) << "\n";
   std::cout << " - OpenGL shader version supported: " << glGetString( GL_SHADING_LANGUAGE_VERSION ) << "\n";
   std::cout << "****************************************************************\n\n";
}

void RendererGL::initialize()
{
   if (!glfwInit()) {
      std::cout << "Cannot Initialize OpenGL...\n";
      return;
   }
   glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
   glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 6 );
   glfwWindowHint( GLFW_DOUBLEBUFFER, GLFW_TRUE );
   glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

   Window = glfwCreateWindow( FrameWidth, FrameHeight, "Main Camera", nullptr, nullptr );
   glfwMakeContextCurrent( Window );

   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cout << "Failed to initialize GLAD" << std::endl;
      return;
   }
   
   registerCallbacks();
   
   //glEnable( GL_DEPTH_TEST );
   glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );

   MainCamera->updateWindowSize( FrameWidth, FrameHeight );

   const std::string shader_directory_path = std::string(CMAKE_SOURCE_DIR) + "/shaders";
   ObjectShader->setShader(
      std::string(shader_directory_path + "/BasicPipeline.vert").c_str(),
      std::string(shader_directory_path + "/BasicPipeline.frag").c_str()
   );
}

void RendererGL::error(int error, const char* description) const
{
   puts( description );
}

void RendererGL::errorWrapper(int error, const char* description)
{
   Renderer->error( error, description );
}

void RendererGL::cleanup(GLFWwindow* window)
{
   glfwSetWindowShouldClose( window, GLFW_TRUE );
}

void RendererGL::cleanupWrapper(GLFWwindow* window)
{
   Renderer->cleanup( window );
}

void RendererGL::keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   if (action != GLFW_PRESS) return;

   switch (key) {
      case GLFW_KEY_R:
         StartTiming = glfwGetTime() * 1000.0;
         std::cout << "Replay Animation!\n";
         break;
      case GLFW_KEY_L:
         Lights->toggleLightSwitch();
         std::cout << "Light Turned " << (Lights->isLightOn() ? "On!\n" : "Off!\n");
         break;
      case GLFW_KEY_Q:
      case GLFW_KEY_ESCAPE:
         cleanupWrapper( window );
         break;
      default:
         return;
   }
}

void RendererGL::keyboardWrapper(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   Renderer->keyboard( window, key, scancode, action, mods );
}

void RendererGL::reshape(GLFWwindow* window, int width, int height) const
{
   MainCamera->updateWindowSize( width, height );
   glViewport( 0, 0, width, height );
}

void RendererGL::reshapeWrapper(GLFWwindow* window, int width, int height)
{
   Renderer->reshape( window, width, height );
}

void RendererGL::registerCallbacks() const
{
   glfwSetErrorCallback( errorWrapper );
   glfwSetWindowCloseCallback( Window, cleanupWrapper );
   glfwSetKeyCallback( Window, keyboardWrapper );
   glfwSetFramebufferSizeCallback( Window, reshapeWrapper );
}

void RendererGL::setKeyFrames(const std::vector<Animator2D::KeyFrame>& key_frames) const
{
   for (const auto& key_frame : key_frames) {
      Animator->addKeyFrame( key_frame );
   }
}

void RendererGL::setLights() const
{  
   const glm::vec4 light_position(0.0f, 0.0f, 1.0f, 0.0f);
   const glm::vec4 ambient_color(0.3f, 0.3f, 0.3f, 1.0f);
   const glm::vec4 diffuse_color(0.7f, 0.7f, 0.7f, 1.0f);
   const glm::vec4 specular_color(0.9f, 0.9f, 0.9f, 1.0f);
   Lights->addLight( light_position, ambient_color, diffuse_color, specular_color );
}

void RendererGL::setObjects()
{
   Objects.clear();
   const int key_frame_num = Animator->getTotalKeyFramesNum();
   for (int i = 0; i < key_frame_num; ++i) {
      Objects.emplace_back( std::make_unique<ObjectGL>() );
      Objects[i]->setSquareObject( GL_TRIANGLES );
   }
}

void RendererGL::drawObject() const
{
   glUseProgram( ObjectShader->getShaderProgram() );
   
   const auto current_time = static_cast<float>(glfwGetTime() * 1000.0 - StartTiming);
   const int key_frame_num = Animator->getTotalKeyFramesNum();
   for (int i = 0; i < key_frame_num; ++i) {
      GLenum fill_type;
      Animator->getFillType( fill_type, i );
      glPolygonMode( GL_FRONT_AND_BACK, fill_type );

      Animator2D::Animation animation;
      Animator->getAnimationNow( animation, i, current_time );

      glm::mat4 to_world;
      Animator->getWorldMatrix( to_world, animation, i );
     
      ObjectShader->transferBasicTransformationUniforms( to_world, MainCamera.get(), false );
      Objects[i]->setDiffuseReflectionColor( glm::vec4( animation.Color, 1.0f) );
      Objects[i]->transferUniformsToShader( ObjectShader.get() );
      Lights->transferUniformsToShader( ObjectShader.get() );


      glBindVertexArray( Objects[i]->getVAO() );
      glDrawArrays( Objects[i]->getDrawMode(), 0, Objects[i]->getVertexNum() );
   }
}

void RendererGL::render() const
{
   glClear( OPENGL_COLOR_BUFFER_BIT | OPENGL_DEPTH_BUFFER_BIT );

   drawObject();

   glBindVertexArray( 0 );
   glUseProgram( 0 );
}

void RendererGL::play(const std::vector<Animator2D::KeyFrame>& key_frames)
{
   if (glfwWindowShouldClose( Window )) initialize();

   setKeyFrames( key_frames );
   setLights();
   setObjects();
   ObjectShader->setUniformLocations( Lights->getTotalLightNum() );

   StartTiming = glfwGetTime() * 1000.0;
   while (!glfwWindowShouldClose( Window )) {
      render();

      glfwSwapBuffers( Window );
      glfwPollEvents();
   }
   glfwDestroyWindow( Window );
}