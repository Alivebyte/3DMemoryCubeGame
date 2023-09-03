#include <math.h>
#define _USE_MATH_DEFINES
#define degToRad(angleInDegrees) ((angleInDegrees) * M_PI / 180.0)
#define radToDeg(angleInRadians) ((angleInRadians) * 180.0 / M_PI)

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
		sAppName = "Memory";
	}
private:
	void SetMouseView(float fElapsedTime)
	{
		POINT mousePos;
		int middleX = ScreenWidth() >> 1;
		int middleY = ScreenHeight() >> 1;

		float angleX = 0;
		float angleY = 0;

		GetCursorPos(&mousePos);

		if ((mousePos.x == middleX) && (mousePos.y == middleY)) return;

		if (IsFocused())
		{
			SetCursorPos(middleX, middleY);
			angleX = (float)((middleX - mousePos.x)) / 500.0f;
			angleY = (float)((middleY - mousePos.y)) / 500.0f;
			fYaw += angleX;
			fPitch += angleY;
			
		}
	}
public:
	bool OnUserCreate()
	{
		
		nGameState = GAME_RESET;
		nNextGameState = GAME_RESET;
		nEndState = END_NONE;

		fYaw = -5.0f;
		fPitch = 0.5f;
		if (cubeMesh.LoadOBJFile("content/meshes/cube.obj"))
		{
			std::cout << "Mesh loaded!";
		}
		else
		{
			std::cout << "Mesh load fail!";
		}

		// Load sprites
		sprites = new olc::Sprite*[9];

		for (int i = 0; i < 9; i++)
		{
			char buf[64];
			snprintf(buf, sizeof(buf), "content/textures/color_%d.png", i+1);
			sprites[i] = new olc::Sprite(buf);
		}	

		olc::GFX3D::ConfigureDisplay();
		// Projection stuff
		pipe.SetProjection(60.0f, (float)ScreenHeight()/(float)ScreenWidth(), 0.1f, 1000.0f, 0.0f, 0.0f, ScreenWidth(), ScreenHeight());
		return true;
	}
	
	void HandleGameState(float fElapsedTime)
	{
		int i = 0;
		srand(time(NULL));
		switch (nGameState)
		{
		case GAME_RESET:
			fWarmupTimer = 11.0f;
			fGameTimer = 60.0f;
			fGameEndTimer = 3.0f;
			vCamera = { 10.5, 0.5, -15 };
			bGameStarted = false;
			bUserControlEnabled = false;
			bRender = false;
			memorizeShapes = { 0, 0, 0, 0};
			gameShapes = { 0, 0, 0, 0};
			correctCount = 0;
			selectedShape = 0;
			selectedShapeColor = 0;
			nEndState = END_NONE;
			fPitch = -0.01f;
			fYaw = -6.32f;

			nNextGameState = GAME_PREPARE;
			break;
		case GAME_PREPARE:
			
			while( i < 4)
			{
				int rand_num = rand() % 7;
				bool isUnique = true;

				for (int j = 0; j < 4; j++)
				{
					if (memorizeShapes[j].color == rand_num)
					{
						isUnique = false;
						break;
					}
				}

				if (isUnique)
				{
					memorizeShapes[i] = { 0, rand_num };
					i++;
				}
			}

			// Set parameters to render and notify render to enable
			shapesToDraw = memorizeShapes;
			bRender = true;

			

			nNextGameState = GAME_WARMUP;
			break;
		case GAME_WARMUP:
			// Enable user input
			bUserControlEnabled = true;
			
			fWarmupTimer -= fElapsedTime;

			if (fWarmupTimer >= 0.0f)
			{
				//DrawString({ (ScreenWidth() / 3), (ScreenHeight() / 2) - 8 }, "Prepare for round!");
				//DrawString({ (ScreenWidth() / 2) - 8, (ScreenHeight() / 2) + 10 }, std::to_string((int)fWarmupTimer));
			}
			else
			{
				nNextGameState = GAME_WARMUP_END;
			}

			break;

		case GAME_WARMUP_END:
			
			for (int i = 0; i < 4; i++)
			{
				shapesToDraw[i].color = 0;
			}

			nNextGameState = GAME_START;
			break;
		case GAME_START:
			bGameStarted = true;

			fGameTimer -= fElapsedTime;

			if (selectedShape != 4)
			{
				gameShapes[selectedShape] = { 0, selectedShapeColor };
			}
			if (fGameTimer >= 0.0f)
			{
				if (selectedShape == 4)
				{
					fGameTimer = 60.0f;
					nNextGameState = GAME_END;
				}
			}
			else
			{
				nNextGameState = GAME_END;
			}

			shapesToDraw = gameShapes;
			break;
		case GAME_END:

			bGameStarted = false;
			bUserControlEnabled = false;

			for (int i = 0; i < 4; i++)
			{
				if (gameShapes[i].color == memorizeShapes[i].color)
				{
					correctCount++;
				}
			}

			if (correctCount == 4)
			{
				// Win
				nEndState = END_WIN;
				correctCount = 0;
			}
			else if(fGameTimer == 0.0f)
			{
				// Timeout
				nEndState = END_TIMEOUT;
				correctCount = 0;
			}
			else
			{
				// Wrong
				nEndState = END_WRONG;
				correctCount = 0;
			}

			fGameEndTimer -= fElapsedTime;

			if (fGameEndTimer <= 0.0f)
			{
				nNextGameState = GAME_RESET;
			}

			break;
		}
	}

	void HandleUserControl(float fElapsedTime)
	{

		olc::GFX3D::vec3d vForward = olc::GFX3D::Math::Vec_Mul(vLookDir, 5.0f * fElapsedTime);
		olc::GFX3D::vec3d vRight = olc::GFX3D::Math::Vec_Mul(olc::GFX3D::Math::Vec_CrossProduct(vLookDir, vWorldUp), 5.0f * fElapsedTime);
		olc::GFX3D::vec3d vUp = olc::GFX3D::Math::Vec_CrossProduct(vRight, vLookDir);

		if (bUserControlEnabled)
		{
			if (GetKey(olc::Key::W).bHeld) vCamera = olc::GFX3D::Math::Vec_Add(vCamera, vForward);
			if (GetKey(olc::Key::S).bHeld) vCamera = olc::GFX3D::Math::Vec_Sub(vCamera, vForward);
			if (GetKey(olc::Key::D).bHeld) vCamera = olc::GFX3D::Math::Vec_Sub(vCamera, vRight);
			if (GetKey(olc::Key::A).bHeld) vCamera = olc::GFX3D::Math::Vec_Add(vCamera, vRight);
			if (GetKey(olc::Key::ESCAPE).bReleased) exit(EXIT_SUCCESS);


			// Mouse input
			SetMouseView(fElapsedTime);

			fPitch = std::clamp(fPitch, -1.55f, 1.55f);

			if (GetKey(olc::Key::SPACE).bHeld) vCamera = olc::GFX3D::Math::Vec_Sub(vCamera, vUp);
			if (GetKey(olc::Key::C).bHeld) vCamera = olc::GFX3D::Math::Vec_Add(vCamera, vUp);

			//if (GetKey(olc::Key::CTRL).bPressed) std::cout << "\nCamera pos: X:" << vCamera.x << " Y: " << vCamera.y << " Z: " << vCamera.z << std::endl << "Camera pitch: " << fPitch << std::endl << "Camera yaw: " << fYaw << std::endl;
			if (bGameStarted)
			{
				if (GetKey(olc::Key::LEFT).bPressed)
				{
					selectedShapeColor -= 1;
					selectedShapeColor = std::clamp(selectedShapeColor, 0, 7);
					
				}
				if (GetKey(olc::Key::RIGHT).bPressed)
				{
					selectedShapeColor += 1;
					selectedShapeColor = std::clamp(selectedShapeColor, 0, 6);
					
					
				}
				if (GetKey(olc::Key::ENTER).bPressed)
				{
					selectedShape += 1;
					if (selectedShape == 3)
						bGameStarted = false;
				}
			}
		
		}
		olc::GFX3D::vec3d vLookTarget = { 0, 0, 1 };
		olc::GFX3D::mat4x4 matRotY = olc::GFX3D::Math::Mat_MakeRotationY(fYaw);
		olc::GFX3D::mat4x4 matRotX = olc::GFX3D::Math::Mat_MakeRotationX(fPitch);
		olc::GFX3D::mat4x4 matRotCam = olc::GFX3D::Math::Mat_MultiplyMatrix(matRotX, matRotY);
		vLookDir = olc::GFX3D::Math::Mat_MultiplyVector(matRotCam, vLookTarget);
		vLookTarget = olc::GFX3D::Math::Vec_Add(vCamera, vLookDir);

		
		pipe.SetCamera(vCamera, vLookTarget, vUp);
	}

	bool OnUserUpdate(float fElapsedTime)
	{
		
		Clear(olc::CYAN);
		olc::GFX3D::ClearDepth();
		

		HandleGameState(fElapsedTime);
		
		HandleUserControl(fElapsedTime);
		
		olc::GFX3D::vec3d lightdir = { -0.65f, -1.0f, -1.0f };
		pipe.SetLightSource(0, olc::GFX3D::LIGHT_AMBIENT, olc::Pixel(100, 100, 100), { 0,0,0 }, lightdir);
		pipe.SetLightSource(1, olc::GFX3D::LIGHT_DIRECTIONAL, olc::Pixel(255, 255, 22), { 0,0,0 }, lightdir);
		

		olc::GFX3D::mat4x4 matWorld = olc::GFX3D::Math::Mat_MakeIdentity();
		olc::GFX3D::mat4x4 matScale = olc::GFX3D::Math::Mat_MakeScale(2.5f, 2.5f, 2.5f);
				
		float fOffset = 0;
		if (bRender)
		{
			for (Shape shape : shapesToDraw)
			{

				matWorld = olc::GFX3D::Math::Mat_MakeTranslation(fOffset, 0, 0);
				matWorld = olc::GFX3D::Math::Mat_MultiplyMatrix(matWorld, matScale);

				pipe.SetTransform(matWorld);
				pipe.SetTexture(sprites[shape.color]);
				pipe.Render(cubeMesh.tris, olc::GFX3D::RENDER_CULL_CW | olc::GFX3D::RENDER_TEXTURED | olc::GFX3D::RENDER_DEPTH | olc::GFX3D::RENDER_LIGHTS);
				fOffset += 2.5f;
			}
		}

		switch (nGameState)
		{
		case GAME_WARMUP:
			DrawString({ (ScreenWidth() / 3), (ScreenHeight() / 2) - 232 }, "Remember each color");
			DrawString({ (ScreenWidth() / 2) - 8, (ScreenHeight() / 2) - 212 }, std::to_string((int)fWarmupTimer));
			break;
		case GAME_START:
			//DrawString(10, 10, std::to_string(selectedShape));
			//DrawString(10, 20, std::to_string(selectedShapeColor));

			
			matScale = olc::GFX3D::Math::Mat_MakeScale(0.0f, 5.0f, 0.0f);
			matWorld = olc::GFX3D::Math::Mat_MakeTranslation(0.0f, 5.0f, 0.0f);

			matWorld = olc::GFX3D::Math::Mat_MultiplyMatrix(matWorld, matScale);

			pipe.SetTransform(matWorld);

			pipe.SetTexture(sprites[7]);

			pipe.Render(cubeMesh.tris);


			DrawString({ 0, ScreenHeight() - 10 }, "Game ends in: ");
			DrawString({ ((int)sizeof("Game ends in: ") * 8), ScreenHeight() - 10 }, std::to_string((int)fGameTimer));
			//std::cout << "1: " << memorizeShapes[0].color + 1 << " 2: " << memorizeShapes[1].color + 1 << " 3: " << memorizeShapes[2].color + 1 << " 4: " << memorizeShapes[3].color + 1 << std::endl;
			//std::cout << "1: " << gameShapes[0].color + 1 << " 2: " << gameShapes[1].color + 1 << " 3: " << gameShapes[2].color + 1 << " 4: " << gameShapes[3].color + 1 << std::endl;
			break;
		case GAME_END:
			switch (nEndState)
			{
			case END_NONE:
				break;
			case END_WIN:
				DrawString({ (ScreenWidth() / 3), (ScreenHeight() / 2) - 232 }, "WIN!", olc::GREEN, 1.25f);
				break;
			case END_TIMEOUT:
				DrawString({ (ScreenWidth() / 3), (ScreenHeight() / 2) - 232 }, "TIMEOUT!", olc::YELLOW, 1.25f);
				break;
			case END_WRONG:
				DrawString({ (ScreenWidth() / 3), (ScreenHeight() / 2) - 232 }, "WRONG!", olc::RED, 1.25f);
				break;
			}
		}

		
		

		nGameState = nNextGameState;

		return true;
		
	}

	bool OnUserDestroy()
	{
		return false;
	}

private:
	olc::GFX3D::vec3d vWorldUp = { 0, 1, 0 };
	//olc::GFX3D::vec3d vUp = { 0, 1, 0 };
	//olc::GFX3D::vec3d vRight = { 1, 0, 0 };
	olc::GFX3D::vec3d vCamera = { 10.5, 0.5, -15 };
	olc::GFX3D::vec3d vLookDir = { 0,0,1 };
	olc::GFX3D::PipeLine pipe;
	olc::GFX3D::mesh cubeMesh;

	olc::Sprite **sprites;

	float fYaw;
	float fPitch;

	float fWarmupTimer;
	float fGameTimer;
	float fGameEndTimer;
	bool bUserControlEnabled;
	bool bGameStarted;
	bool bRender;
	
	int selectedShape = 0;
	int selectedShapeColor = 0;
	int correctCount = 0;

	enum GAME_STATE
	{
		GAME_RESET, // Reset all variables
		GAME_PREPARE, // Create random set of shape types and colors
		GAME_WARMUP, // Setup warmup stage where player has time to memerize shapes and colors 
		GAME_WARMUP_END, // Warmup stage end
		GAME_START, // Game started, init choosing shapes and colors
		GAME_END // Game end, sets when all shapes are right, wrong or timeout
	} nGameState, nNextGameState;

	enum END_STATE
	{
		END_NONE,
		END_WIN,
		END_TIMEOUT,
		END_WRONG
	} nEndState;

	struct Shape
	{
		int type;
		int color;
	};

	std::array<Shape, 4> memorizeShapes;
	std::array<Shape, 4> gameShapes;
	std::array<Shape, 4> shapesToDraw;
};

int main()
{
	Game game;
	if (game.Construct(768, 480, 1, 1, false, false))
	{
		SetCursor(NULL);
		ShowCursor(FALSE);
		game.Start();
		
	}

	return 0;
}