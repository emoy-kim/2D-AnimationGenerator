#include "Renderer.h"

RendererGL* RendererGL::Renderer = nullptr;
RendererGL::RendererGL() : Window( nullptr ), StartTiming( 0.0 )
{
   Renderer = this;

   initialize();
   printOpenGLInformation();
}

RendererGL::~RendererGL()
{
   glfwTerminate();
}

void RendererGL::printOpenGLInformation() const
{
   cout << "****************************************************************" << endl;
   cout << " - GLFW version supported: " << glfwGetVersionString() << endl;
   cout << " - GLEW version supported: " << glewGetString( GLEW_VERSION ) << endl;
   cout << " - OpenGL renderer: " << glGetString( GL_RENDERER ) << endl;
   cout << " - OpenGL version supported: " << glGetString( GL_VERSION ) << endl;
   cout << " - OpenGL shader version supported: " << glGetString( GL_SHADING_LANGUAGE_VERSION ) << endl  ;
   cout << "****************************************************************" << endl << endl;
}

void RendererGL::initialize()
{
   if (!glfwInit()) {
      cout << "Cannot Initialize OpenGL..." << endl;
      return;
   }
   glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
   glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 5 );
   glfwWindowHint( GLFW_DOUBLEBUFFER, GLFW_TRUE );
   glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

   const int width = 1920;
   const int height = 1080;
   Window = glfwCreateWindow( width, height, "Main Camera", nullptr, nullptr );
   glfwMakeContextCurrent( Window );
   glewInit();
   
   registerCallbacks();
   
   glEnable( GL_DEPTH_TEST );
   glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );

   MainCamera.updateWindowSize( width, height );
   ObjectShader.setShader(
      "Shaders/VertexShaderForObject.glsl",
      "Shaders/FragmentShaderForObject.glsl"
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
   glDeleteProgram( ObjectShader.ShaderProgram );
   for (auto& object : Objects) {
      glDeleteVertexArrays( 1, &object.ObjVAO );
      glDeleteBuffers( 1, &object.ObjVBO );
   }
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
         cout << "Replay Animation!" << endl;
         break;
      case GLFW_KEY_L:
         Lights.toggleLightSwitch();
         cout << "Light Turned " << (Lights.isLightOn() ? "On!" : "Off!") << endl;
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

void RendererGL::reshape(GLFWwindow* window, int width, int height)
{
   MainCamera.updateWindowSize( width, height );
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

void RendererGL::setKeyFrames(const vector<Animator2D::KeyFrame>& key_frames)
{
   for (const auto& key_frame : key_frames) {
      Animator.addKeyFrame( key_frame );
   }
}

void RendererGL::setLights()
{  
   const vec4 light_position(0.0f, 0.0f, 1.0f, 0.0f);
   const vec4 ambient_color(0.3f, 0.3f, 0.3f, 1.0f);
   const vec4 diffuse_color(0.7f, 0.7f, 0.7f, 1.0f);
   const vec4 specular_color(0.9f, 0.9f, 0.9f, 1.0f);
   Lights.addLight( light_position, ambient_color, diffuse_color, specular_color );
}

void RendererGL::setObject()
{
   for (auto& object: Objects) {
      if (object.ObjVAO != 0) {
         glDeleteVertexArrays( 1, &object.ObjVAO );
         glDeleteBuffers( 1, &object.ObjVBO );
      }
   }

   vector<vec3> square_vertices;
   square_vertices.emplace_back( 1.0f, 0.0f, 0.0f );
   square_vertices.emplace_back( 1.0f, 1.0f, 0.0f );
   square_vertices.emplace_back( 0.0f, 1.0f, 0.0f );
   
   square_vertices.emplace_back( 1.0f, 0.0f, 0.0f );
   square_vertices.emplace_back( 0.0f, 1.0f, 0.0f );
   square_vertices.emplace_back( 0.0f, 0.0f, 0.0f );

   vector<vec3> square_normals;
   square_normals.emplace_back( 0.0f, 0.0f, 1.0f );
   square_normals.emplace_back( 0.0f, 0.0f, 1.0f );
   square_normals.emplace_back( 0.0f, 0.0f, 1.0f );
   
   square_normals.emplace_back( 0.0f, 0.0f, 1.0f );
   square_normals.emplace_back( 0.0f, 0.0f, 1.0f );
   square_normals.emplace_back( 0.0f, 0.0f, 1.0f );
      
   Objects.resize( Animator.TotalKeyFramesNum );
   for (uint i = 0; i < Animator.TotalKeyFramesNum; ++i) {
      Objects[i].setObject( GL_TRIANGLES, square_vertices, square_normals );
   }
}

void RendererGL::getWorldMatrix(mat4& to_world, const Animator2D::Animation& animation, const uint& key_frame_index)
{
   int top_left_x, top_left_y, width, height;
   Animator.getOriginalPosition( top_left_x, top_left_y, width, height, key_frame_index );

   float anchor_x, anchor_y;
   Animator.getAnchor( anchor_x, anchor_y, key_frame_index );

   const float scale_x = static_cast<float>(width) * animation.ScaleX;
   const float scale_y = static_cast<float>(height) * animation.ScaleY;
   const mat4 scale_matrix = scale( mat4(1.0f), vec3(scale_x, scale_y, 1.0f) );

   const float to_origin_x = scale_x * anchor_x;
   const float to_origin_y = scale_y * anchor_y;
   const mat4 to_origin = translate( mat4(1.0f), vec3(-to_origin_x, -to_origin_y, 0.0f) );
   const mat4 rotation = rotate( mat4(1.0f), radians( animation.RotationAngle ), vec3(0.0f, 0.0f, 1.0f) );
 
   const float translate_x = static_cast<float>(top_left_x) + animation.TranslationX;
   const float translate_y = static_cast<float>(Animator.ScreenHeight - top_left_y) - scale_y - animation.TranslationY;
   const mat4 translation = translate( mat4(1.0f), vec3(translate_x, translate_y, 0.0f) );

   to_world = translation * inverse( to_origin ) * rotation * to_origin * scale_matrix;
}

void RendererGL::drawObject()
{
   glUseProgram( ObjectShader.ShaderProgram );

   const auto current_time = static_cast<float>(glfwGetTime() * 1000.0 - StartTiming);
   for (uint i = 0; i < Animator.TotalKeyFramesNum; ++i) {
      GLenum fill_type;
      Animator.getFillType( fill_type, i );
      glPolygonMode( GL_FRONT_AND_BACK, fill_type );

      Animator2D::Animation animation;
      Animator.getAnimationNow( animation, i, current_time );

      mat4 to_world;
      getWorldMatrix( to_world, animation, i );
     
      const mat4 model_view_projection = MainCamera.ProjectionMatrix * MainCamera.ViewMatrix * to_world;
      glUniformMatrix4fv( ObjectShader.Location.World, 1, GL_FALSE, &to_world[0][0] );
      glUniformMatrix4fv( ObjectShader.Location.View, 1, GL_FALSE, &MainCamera.ViewMatrix[0][0] );
      glUniformMatrix4fv( ObjectShader.Location.Projection, 1, GL_FALSE, &MainCamera.ProjectionMatrix[0][0] );
      glUniformMatrix4fv( ObjectShader.Location.ModelViewProjection, 1, GL_FALSE, &model_view_projection[0][0] );

      Objects[i].setDiffuseReflectionColor( vec4( animation.Color, 1.0f) );
      Objects[i].transferUniformsToShader( ObjectShader );
      Lights.transferUniformsToShader( ObjectShader );

      glBindVertexArray( Objects[i].ObjVAO );
      glDrawArrays( Objects[i].DrawMode, 0, Objects[i].VerticesCount );
   }
}

void RendererGL::render()
{
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   drawObject();

   glBindVertexArray( 0 );
   glUseProgram( 0 );
}

void RendererGL::play(const vector<Animator2D::KeyFrame>& key_frames)
{
   if (glfwWindowShouldClose( Window )) initialize();

   setKeyFrames( key_frames );
   setLights();
   setObject();

   ObjectShader.setUniformLocations( Lights.TotalLightNum );

   StartTiming = glfwGetTime() * 1000.0;
   while (!glfwWindowShouldClose( Window )) {
      render();

      glfwSwapBuffers( Window );
      glfwPollEvents();
   }
   glfwDestroyWindow( Window );
}