#include "Animator.h"

Animator2D::Animator2D() : Animator2D(1920, 1080)
{   
}

Animator2D::Animator2D(const int& screen_width, const int& screen_height) :
   TotalKeyFramesNum( 0 ), ScreenWidth( screen_width ), ScreenHeight( screen_height )
{
}

void Animator2D::setScreenSize(const int& width, const int& height)
{
   ScreenWidth = width;
   ScreenHeight = height;
}

void Animator2D::addKeyFrame(const KeyFrame& key_frame)
{
   KeyFrames.push_back( key_frame );
   TotalKeyFramesNum = KeyFrames.size();
}

void Animator2D::getOriginalPosition(int& top_left_x, int& top_left_y, int& width, int& height, const uint& key_frame_index)
{
   if (key_frame_index >= KeyFrames.size()) return;

   KeyFrame& key_frame = KeyFrames[key_frame_index];
   top_left_x = key_frame.TopLeftX;
   top_left_y = key_frame.TopLeftY;
   width = key_frame.ObjectWidth;
   height = key_frame.ObjectHeight;
}

void Animator2D::getAnchor(float& anchor_x, float& anchor_y, const uint& key_frame_index)
{
   if (key_frame_index >= KeyFrames.size()) return;

   KeyFrame& key_frame = KeyFrames[key_frame_index];
   anchor_x = key_frame.AnchorX;
   anchor_y = key_frame.AnchorY;
}

void Animator2D::getFillType(GLenum& fill_type, const uint& key_frame_index)
{
   if (key_frame_index >= KeyFrames.size()) return;

   KeyFrame& key_frame = KeyFrames[key_frame_index];
   fill_type = key_frame.FillType == FILL ? GL_FILL : GL_LINE;
}

void Animator2D::getAnimationNow(Animation& animation, const uint& key_frame_index, const float& current_time)
{
   if (key_frame_index >= KeyFrames.size()) return;

   KeyFrame& key_frame = KeyFrames[key_frame_index];
   float t = current_time / key_frame.Duration;
   t = std::max( t, 0.0f );
   t = std::min( t, 1.0f );

   animation.Color = key_frame.Start.Color * (1.0f - t) + key_frame.End.Color * t;
   animation.ScaleX = key_frame.Start.ScaleX * (1.0f - t) + key_frame.End.ScaleX * t;
   animation.ScaleY = key_frame.Start.ScaleY * (1.0f - t) + key_frame.End.ScaleY * t;
   animation.TranslationX = key_frame.Start.TranslationX * (1.0f - t) + key_frame.End.TranslationX * t;
   animation.TranslationY = key_frame.Start.TranslationY * (1.0f - t) + key_frame.End.TranslationY * t;
   animation.RotationAngle = key_frame.Start.RotationAngle * (1.0f - t) + key_frame.End.RotationAngle * t;
}