#include "Renderer.h"

void getKeyFrame1(Animator2D::KeyFrame& key_frame)
{
   key_frame.FillType = Animator2D::FILL_TYPE::FILL;
   key_frame.TopLeft = { 100, 100 };
   key_frame.ObjectWidth = 200;
   key_frame.ObjectHeight = 200;
   key_frame.Anchor = { 0.5f, 0.5f };
   key_frame.Duration = 1000.0f;

   key_frame.Start.Color = { 0.0f, 1.0f, 0.0f };
   key_frame.Start.Translation = { 0.0f, 0.0f };
   key_frame.Start.Scale = { 1.0f, 1.0f };
   key_frame.Start.RotationAngle = 0.0f;

   key_frame.End.Color = { 0.0f, 1.0f, 0.0f };
   key_frame.End.Translation = { 500.0f, 500.0f };
   key_frame.End.Scale = { 2.0f, 2.0f };
   key_frame.End.RotationAngle = 0.0f;
}

void getKeyFrame2(Animator2D::KeyFrame& key_frame)
{
   key_frame.FillType = Animator2D::FILL_TYPE::FILL;
   key_frame.TopLeft = { 1500, 100 };
   key_frame.ObjectWidth = 200;
   key_frame.ObjectHeight = 500;
   key_frame.Anchor = { 0.5f, 0.5f };
   key_frame.Duration = 2000.0f;

   key_frame.Start.Color = { 0.0f, 0.0f, 1.0f };
   key_frame.Start.Translation = { 0.0f, 0.0f };
   key_frame.Start.Scale = { 1.0f, 1.0f };
   key_frame.Start.RotationAngle = 0.0f;

   key_frame.End.Color = { 0.0f, 1.0f, 1.0f };
   key_frame.End.Translation = { 0.0f, 0.0f };
   key_frame.End.Scale = { 1.0f, 1.0f };
   key_frame.End.RotationAngle = 90.0f;
}

int main()
{
   std::vector<Animator2D::KeyFrame> key_frames(2);
   getKeyFrame1( key_frames[0] );
   getKeyFrame2( key_frames[1] );

   RendererGL renderer;
   renderer.play( key_frames );
   return 0;
}