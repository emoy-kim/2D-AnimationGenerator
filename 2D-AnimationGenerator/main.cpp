#include "Renderer.h"

void getKeyFrame1(Animator2D::KeyFrame& key_frame)
{
   key_frame.FillType = Animator2D::FILL;
   key_frame.TopLeftX = 100;
   key_frame.TopLeftY = 100;
   key_frame.ObjectWidth = 200;
   key_frame.ObjectHeight = 200;
   key_frame.AnchorX = 0.5f;
   key_frame.AnchorY = 0.5f;
   key_frame.Duration = 1000.0f;

   key_frame.Start.Color = { 0.0f, 1.0f, 0.0f };
   key_frame.Start.TranslationX = 0.0f;
   key_frame.Start.TranslationY = 0.0f;
   key_frame.Start.ScaleX = 1.0f;
   key_frame.Start.ScaleY = 1.0f;
   key_frame.Start.RotationAngle = 0.0f;

   key_frame.End.Color = { 0.0f, 1.0f, 0.0f };
   key_frame.End.TranslationX = 500.0f;
   key_frame.End.TranslationY = 500.0f;
   key_frame.End.ScaleX = 2.0f;
   key_frame.End.ScaleY = 2.0f;
   key_frame.End.RotationAngle = 0.0f;
}

void getKeyFrame2(Animator2D::KeyFrame& key_frame)
{
   key_frame.FillType = Animator2D::FILL;
   key_frame.TopLeftX = 1500;
   key_frame.TopLeftY = 100;
   key_frame.ObjectWidth = 200;
   key_frame.ObjectHeight = 500;
   key_frame.AnchorX = 0.5f;
   key_frame.AnchorY = 0.5f;
   key_frame.Duration = 2000.0f;

   key_frame.Start.Color = { 0.0f, 0.0f, 1.0f };
   key_frame.Start.TranslationX = 0.0f;
   key_frame.Start.TranslationY = 0.0f;
   key_frame.Start.ScaleX = 1.0f;
   key_frame.Start.ScaleY = 1.0f;
   key_frame.Start.RotationAngle = 0.0f;

   key_frame.End.Color = { 0.0f, 1.0f, 1.0f };
   key_frame.End.TranslationX = 0.0f;
   key_frame.End.TranslationY = 0.0f;
   key_frame.End.ScaleX = 1.0f;
   key_frame.End.ScaleY = 1.0f;
   key_frame.End.RotationAngle = 90.0f;
}

int main()
{
   vector<Animator2D::KeyFrame> key_frames(2);
   getKeyFrame1( key_frames[0] );
   getKeyFrame2( key_frames[1] );

   RendererGL renderer;
   renderer.play( key_frames );
   return 0;
}