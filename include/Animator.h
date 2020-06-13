#pragma once

#include "_Common.h"

class Animator2D
{
public:
   enum class FILL_TYPE { FILL=0, WIRED };

   struct Animation
   {
      glm::vec3 Color;
      glm::vec2 Translation;
      glm::vec2 Scale;
      float RotationAngle;
      Animation() :
         Color{ 0.0f, 0.0f, 0.0f }, Translation( 0.0f, 0.0f ), Scale( 0.0f, 0.0f ), RotationAngle( 0.0f ) {}
   };

   // The object in a key frame is defined in the coordinates whose origin is at the TOP-LEFT.
   struct KeyFrame
   {
      FILL_TYPE FillType;
      int ObjectWidth;
      int ObjectHeight;
      glm::ivec2 TopLeft;
      glm::vec2 Anchor;
      float Duration;
      Animation Start;
      Animation End;

      KeyFrame() :
         FillType( FILL_TYPE::FILL ), ObjectWidth( 0 ), ObjectHeight( 0 ), TopLeft( 0, 0 ), Anchor( 0.0f, 0.0f ),
         Duration( 0.0f ) {}
   };

   
   Animator2D();
   Animator2D(int screen_width, int screen_height);

   void setScreenSize(int width, int height);
   void addKeyFrame(const KeyFrame& key_frame);
   void getOriginalPosition(int& top_left_x, int& top_left_y, int& width, int& height, int key_frame_index);
   void getAnchor(float& anchor_x, float& anchor_y, int key_frame_index);
   void getFillType(GLenum& fill_type, int key_frame_index);
   void getAnimationNow(Animation& animation, int key_frame_index, float current_time);
   void getWorldMatrix(glm::mat4& to_world, const Animation& animation, int key_frame_index);
   [[nodiscard]] int getTotalKeyFramesNum() const { return TotalKeyFramesNum; }


private:
   int TotalKeyFramesNum;
   int ScreenWidth;
   int ScreenHeight;
   std::vector<KeyFrame> KeyFrames;
};