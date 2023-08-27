#define OLC_PGE_APPLICATION
#define OLC_PGEX_GRAPHICS3D
#include "olcPixelGameEngine.h"
#include "extensions/olcPGEX_Graphics3D.h"
#include <random>

class Game : public olc::PixelGameEngine
{
public:
	Game()
	{
		sAppName = "Memory ?";
	}

	bool OnUserCreate()
	{
		
		if (cubeMesh.LoadOBJFile("content/meshes/cube.obj"))
		{
			std::cout << "Mesh loaded!";
		}
		else
		{
			std::cout << "Mesh load fail!";
		}
		spr_test = new olc::Sprite("content/textures/dirtblock.png");

		//cubeMesh.tris =
		//{
		//	// SOUTH
		//	{ 0.0f, 0.0f, 0.0f, 1.0f,	    0.0f, 1.0f, 0.0f, 1.0f,		 1.0f, 1.0f, 0.0f, 1.0f,	0.0f, 1.0f, 0.0f,	0.0f, 0.0f, 0.0f, 		1.0f, 0.0f, 0.0f, },
		//	{ 0.0f, 0.0f, 0.0f, 1.0f,  		1.0f, 1.0f, 0.0f, 1.0f,		 1.0f, 0.0f, 0.0f, 1.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f, 0.0f, 		1.0f, 1.0f, 0.0f, },

		//	// EAST
		//	{ 1.0f, 0.0f, 0.0f, 1.0f,		1.0f, 1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f, 0.0f, 		1.0f, 0.0f, 0.0f, },
		//	{ 1.0f, 0.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f, 1.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f, 0.0f, 		1.0f, 1.0f, 0.0f, },

		//	// NORTH
		//	{ 1.0f, 0.0f, 1.0f, 1.0f,		1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f, 0.0f, 		1.0f, 0.0f, 0.0f, },
		//	{ 1.0f, 0.0f, 1.0f, 1.0f,		0.0f, 1.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f, 1.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f, 0.0f, 		1.0f, 1.0f, 0.0f, },

		//	// WEST
		//	{ 0.0f, 0.0f, 1.0f, 1.0f,		0.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 0.0f, 1.0f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f, 0.0f, 		1.0f, 0.0f, 0.0f, },
		//	{ 0.0f, 0.0f, 1.0f, 1.0f,		0.0f, 1.0f, 0.0f, 1.0f,		0.0f, 0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f, 0.0f, 		1.0f, 1.0f, 0.0f, },

		//	// TOP             																 				    
		//	{ 0.0f, 1.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f, 1.0f,		1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f, 0.0f, 		1.0f, 0.0f, 0.0f, },
		//	{ 0.0f, 1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 1.0f, 0.0f, 1.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f, 0.0f, 		1.0f, 1.0f, 0.0f, },

		//	// BOTTOM         																 				   
		//	{ 1.0f, 0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f, 0.0f, 		1.0f, 0.0f, 0.0f, },
		//	{ 1.0f, 0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f, 0.0f, 		1.0f, 1.0f, 0.0f, },

		//};

		olc::GFX3D::ConfigureDisplay();
		// Projection stuff
		//vCamera = { 0.0f, 0.0f, -2.0f };
		//vLookDir = { 0.0f, 0.0f, 1.0f };
		
		pipe.SetProjection(90.0f, (float)ScreenHeight()/(float)ScreenWidth(), 0.1f, 1000.0f, 0.0f, 0.0f, ScreenWidth(), ScreenHeight());
		return true;
	}
	
	bool OnUserUpdate(float fElapsedTime)
	{
		fTheta += fElapsedTime * 10;
		/*for (int x = 0; x != ScreenWidth(); x++)
		{
			for (int y = 0; y != ScreenHeight(); y++)
			{
				Draw(x,y , olc::Pixel(rand() % 255, rand() % 255, rand() % 255));
			}
		}*/
		//olc::vf2d textPos = { 0.0f, 0.0f };
		//DrawString(textPos, "TEST!!!");

		Clear(olc::CYAN);
		olc::GFX3D::ClearDepth();

		if (GetKey(olc::Key::W).bHeld) vCamera.z += 5.0f * fElapsedTime;
		if (GetKey(olc::Key::S).bHeld) vCamera.z -= 5.0f * fElapsedTime;
		if (GetKey(olc::Key::D).bHeld) vCamera.x += 5.0f * fElapsedTime;
		if (GetKey(olc::Key::A).bHeld) vCamera.x -= 5.0f * fElapsedTime;
		if (GetKey(olc::Key::SPACE).bHeld) vCamera.y -= 5.0f * fElapsedTime;
		if (GetKey(olc::Key::C).bHeld) vCamera.y += 5.0f * fElapsedTime;
		if (GetKey(olc::Key::Q).bHeld) vLookDir.x -= 5.0f * fElapsedTime;
		if (GetKey(olc::Key::E).bHeld) vLookDir.x += 5.0f * fElapsedTime;
		

		olc::GFX3D::vec3d vLookTarget = olc::GFX3D::Math::Vec_Add(vCamera, vLookDir);

		pipe.SetCamera(vCamera, vLookTarget, vUp);

		//olc::GFX3D::mat4x4 matRotX = olc::GFX3D::Math::Mat_MakeRotationX(fTheta);
		//olc::GFX3D::mat4x4 matRotY = olc::GFX3D::Math::Mat_MakeRotationY(fTheta);
		//olc::GFX3D::mat4x4 matRotZ = olc::GFX3D::Math::Mat_MakeRotationZ(fTheta / 3.0f);


		olc::GFX3D::mat4x4 matWorld = olc::GFX3D::Math::Mat_MakeIdentity();

		pipe.SetTransform(matWorld);

		pipe.SetTexture(spr_test);

		pipe.Render(cubeMesh.tris);
		pipe.Render(cubeMesh.tris);

		//olc::GFX3D::vec3d vForward = olc::GFX3D::Math::Vec_Mul(vLookDir, 8.0f * fElapsedTime);
		 
		//if (GetKey(olc::Key::W).bHeld) vCamera = olc::GFX3D::Math::Vec_Add(vCamera, vForward);
		//if (GetKey(olc::Key::S).bHeld) vCamera = olc::GFX3D::Math::Vec_Sub(vCamera, vForward);
		////if (GetKey(olc::Key::W).bHeld) fElapsedTime * (vCamera.z += 1.0f);
		////if (GetKey(olc::Key::S).bHeld) fElapsedTime * (vCamera.z += -1.0f);
		//if (GetKey(olc::Key::A).bHeld) fYaw += -2.0f;
		//if (GetKey(olc::Key::D).bHeld) fYaw += 2.0f;

		////olc::GFX3D::vec3d camPos = { 0.0f, 0.0f, -2.0f };
		////olc::GFX3D::vec3d lookAt = { 0.0f, 0.0f, 1.0f };

		//olc::GFX3D::vec3d vecUp = { 0.0f, 1.0f, 0.0f };
		//olc::GFX3D::vec3d vecTarget = { 0, 0, 1.0f };
		//olc::GFX3D::mat4x4 matCamRot = olc::GFX3D::Math::Mat_MakeRotationY(fYaw);
		//vLookDir = olc::GFX3D::Math::Mat_MultiplyVector(matCamRot, vLookDir);
		//vecTarget = olc::GFX3D::Math::Vec_Add(vCamera, vLookDir);
		//olc::GFX3D::mat4x4 matTransform = olc::GFX3D::Math::Mat_MakeTranslation(0.0f, 0.0f, -2.0f);

		//pipe.SetTransform(matTransform);
		//// Camera stuff
		//pipe.SetCamera(vCamera, vecTarget, vecUp);
		//
		//// Render stuff
		////pipe.Render(cubeMesh.tris, olc::GFX3D::RENDERFLAGS::RENDER_WIRE);
		//olc::GFX3D::vec3d circle = { 0.0f, 0.0f, -2.0f };
		//pipe.RenderCircleXZ(circle, 4.0f);

		return true;
		
	}

	bool OnUserDestroy()
	{
		return false;
	}

private:
	olc::GFX3D::vec3d vUp = { 0, 1, 0 };
	olc::GFX3D::vec3d vCamera = { 0, 0, -10 };
	olc::GFX3D::vec3d vLookDir = { 0,0,1 };
	olc::GFX3D::PipeLine pipe;
	olc::GFX3D::mesh cubeMesh;

	olc::Sprite *spr_test;
	float fYaw;
	float fTheta = 0;
};

int main()
{
	Game game;
	if (game.Construct(768, 480, 1, 1, false, false))
	{
		game.Start();
	}

	return 0;
}