#include "Animator.h"

Animator2D::Animator2D() : Animator2D(1920, 1080)
{   
}

Animator2D::Animator2D(int screen_width, int screen_height) : TotalKeyFramesNum( 0 ), ScreenWidth( screen_width ), ScreenHeight( screen_height )
{
}

void Animator2D::setScreenSize(int width, int height)
{
   ScreenWidth = width;
   ScreenHeight = height;
}

void Animator2D::addKeyFrame(const KeyFrame& key_frame)
{
   KeyFrames.push_back( key_frame );
   TotalKeyFramesNum = KeyFrames.size();
}

void Animator2D::getOriginalPosition(int& top_left_x, int& top_left_y, int& width, int& height, int key_frame_index)
{
   if (key_frame_index >= static_cast<int>(KeyFrames.size())) return;

   KeyFrame& key_frame = KeyFrames[key_frame_index];
   top_left_x = key_frame.TopLeft.x;
   top_left_y = key_frame.TopLeft.y;
   width = key_frame.ObjectWidth;
   height = key_frame.ObjectHeight;
}

void Animator2D::getAnchor(float& anchor_x, float& anchor_y, int key_frame_index)
{
   if (key_frame_index >= static_cast<int>(KeyFrames.size())) return;

   KeyFrame& key_frame = KeyFrames[key_frame_index];
   anchor_x = key_frame.Anchor.x;
   anchor_y = key_frame.Anchor.y;
}

void Animator2D::getFillType(GLenum& fill_type, int key_frame_index)
{
   if (key_frame_index >= static_cast<int>(KeyFrames.size())) return;

   KeyFrame& key_frame = KeyFrames[key_frame_index];
   fill_type = key_frame.FillType == FILL_TYPE::FILL ? GL_FILL : GL_LINE;
}

void Animator2D::getAnimationNow(Animation& animation, int key_frame_index, float current_time)
{
   if (key_frame_index >= static_cast<int>(KeyFrames.size())) return;

   KeyFrame& key_frame = KeyFrames[key_frame_index];
   float t = current_time / key_frame.Duration;
   t = std::max( t, 0.0f );
   t = std::min( t, 1.0f );

   animation.Color = key_frame.Start.Color * (1.0f - t) + key_frame.End.Color * t;
   animation.Scale.x = key_frame.Start.Scale.x * (1.0f - t) + key_frame.End.Scale.x * t;
   animation.Scale.y = key_frame.Start.Scale.y * (1.0f - t) + key_frame.End.Scale.y * t;
   animation.Translation.x = key_frame.Start.Translation.x * (1.0f - t) + key_frame.End.Translation.x * t;
   animation.Translation.y = key_frame.Start.Translation.y * (1.0f - t) + key_frame.End.Translation.y * t;
   animation.RotationAngle = key_frame.Start.RotationAngle * (1.0f - t) + key_frame.End.RotationAngle * t;
}

void Animator2D::getWorldMatrix(glm::mat4& to_world, const Animation& animation, int key_frame_index)
{
   int top_left_x, top_left_y, width, height;
   getOriginalPosition( top_left_x, top_left_y, width, height, key_frame_index );

   float anchor_x, anchor_y;
   getAnchor( anchor_x, anchor_y, key_frame_index );

   const float scale_x = static_cast<float>(width) * animation.Scale.x;
   const float scale_y = static_cast<float>(height) * animation.Scale.y;
   const glm::mat4 scale_matrix = scale( glm::mat4(1.0f), glm::vec3(scale_x, scale_y, 1.0f) );

   const float to_origin_x = scale_x * anchor_x;
   const float to_origin_y = scale_y * anchor_y;
   const glm::mat4 to_origin = translate( glm::mat4(1.0f), glm::vec3(-to_origin_x, -to_origin_y, 0.0f) );
   const glm::mat4 rotation = rotate( glm::mat4(1.0f), glm::radians( animation.RotationAngle ), glm::vec3(0.0f, 0.0f, 1.0f) );
 
   const float translate_x = static_cast<float>(top_left_x) + animation.Translation.x;
   const float translate_y = static_cast<float>(ScreenHeight - top_left_y) - scale_y - animation.Translation.y;
   const glm::mat4 translation = translate( glm::mat4(1.0f), glm::vec3(translate_x, translate_y, 0.0f) );

   to_world = translation * inverse( to_origin ) * rotation * to_origin * scale_matrix;
}