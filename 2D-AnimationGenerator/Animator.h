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

class Animator2D
{
public:
   enum { FILL=0, WIRED };

   struct Animation
   {
      vec3 Color;
      float TranslationX, TranslationY;
      float ScaleX, ScaleY;
      float RotationAngle;
      Animation() : Color{ 0.0f, 0.0f, 0.0f }, TranslationX( 0.0f ), TranslationY( 0.0f ), 
      ScaleX( 0.0f ), ScaleY( 0.0f ), RotationAngle( 0.0f ) {}
   };

   // The object in a key frame is defined in the coordinates whose origin is at the TOP-LEFT.
   struct KeyFrame
   {
      int FillType;
      int TopLeftX, TopLeftY;
      int ObjectWidth, ObjectHeight;
      float AnchorX, AnchorY;
      float Duration;
      Animation Start, End;
   };

   uint TotalKeyFramesNum;
   int ScreenWidth, ScreenHeight;

   Animator2D();
   Animator2D(const int& screen_width, const int& screen_height);

   void setScreenSize(const int& width, const int& height);
   void addKeyFrame(const KeyFrame& key_frame);
   void getOriginalPosition(int& top_left_x, int& top_left_y, int& width, int& height, const uint& key_frame_index);
   void getAnchor(float& anchor_x, float& anchor_y, const uint& key_frame_index);
   void getFillType(GLenum& fill_type, const uint& key_frame_index);
   void getAnimationNow(Animation& animation, const uint& key_frame_index, const float& current_time);


private:
   vector<KeyFrame> KeyFrames;
};