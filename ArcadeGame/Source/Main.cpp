/* ********************************************************************************* *
 * *                  SIT153 - Introduction to Game Development                    * *
 * *                Assignment #2 - Develop a Classic Arcade Game                  * *
 * *                                                                               * *
 * *  This code is provided to students as a basic framework for the purposes of   * *
 * *  developing a game that meets the requirements of the assessment guidelines.  * *
 * *  This code base does not provide all features necessary to implement such a   * *
 * *  game. Students must modify and extend this framework to achieve the          * *
 * *  specific functionality required of their game.                               * *
 * *                                                                               * *
 * *  The code is provided as-is and while significant efforts have been made to   * *
 * *  ensure that it is bug-free, this is not a guarantee. It is each student's    * *
 * *  responsibility to ensure that their final submission compiles and executes   * *
 * *  without error.                                                               * *
 * *                                                                               * *
 * *  COPYRIGHT NOTICE                                                             * *
 * *  ----------------                                                             * *
 * *  (C) [2012] - [2016] Deakin University                                        * *
 * *  All rights reserved.                                                         * *
 * *  All information contained herein is, and remains the property of Deakin      * *
 * *  University and the authors (Tim Wilkin, Michael Hobbs and Shaun Bangay).     * *
 * *  Dissemination of this information or reproduction of this material is        * *
 * *  strictly forbidden unless prior written permission is obtained from Deakin   * *
 * *  University. The right to create derivative works from this material is       * *
 * *  hereby granted to students enrolled in SIT153, but only for the purposes of  * *
 * *  assessment while an enrolled student at Deakin University.                   * *
 * *                                                                               * *
 * *  Please complete the following information as acknowledgement of the          * *
 * *  above conditions regarding the use of this code. Failure to complete this    * *
 * *  acknowledgement will mean that your assignment submission will not be marked * *
 * *                                                                               * *
 * ********************************************************************************* *
 * *  STUDENT ID:                                                                  * *
 * *  STUDENT NAME:                                                                * *
 * *  DATE:                                                                        * *
 * ********************************************************************************* *
 */

/*****************************************************************************************
 * Demonstration of usage of the WinCanvas class for a Windows console application - to  *
 * support basic '2D graphics' through simplified API surrounding the Windows GDI system.*
 *                                                                                       *
 * This code is based on the code developed by Shaun Bangay (2012) & Tim Wilkin (2013).  *
 * Extended by Michael Hobbs and Tim Wilkin (2014) and re-written by Tim Wilkin (2015).  *
 *                                                                                       *
 *****************************************************************************************/

 /*****************************************- Personal notes -************************************************
 
 Game idea:
 - swimmer man!

 Game idea:
 - collects dangous fish and rubbish from the ocean
 - sells them for money to upgrade equipment allowing him to:
		- increase inventory size
		- breath holding time
		- swimmer speed
		- ...

What iI've done:
- movement
- BG
- Boundries
- collectables
- Basic collisions
- multiple collectables
- set breath as u dive
- set shows scores
- Underwater physics
- Set lose if no breath
- start/finish and pause menus
- Added sound


To do:
- boat - to drop off fish?
- shop - to buy items
- inventory




 ************************************************************************************************************/




#include <iostream>
#include <fstream>
#include <string>

#include <time.h>
#include <vector>

#include "WinCanvas.h"
#include "WinTexture.h"
#include "DerivedClasses.h"
#include "InputState.h"
#include "Scene.h"

#include <Mmsystem.h>
#pragma comment(lib, "Winmm.lib")

// Constant values used in the code - this is the whole window size including
// menu buttons and border - actual graphics canvas size is smaller!
const unsigned int	WINDOW_WIDTH = 880;
const unsigned int	WINDOW_HEIGHT= 775;

/*
--------------------------------------------------------------------------------------------

										BackGround music

---------------------------------------------------------------------------------------------
*/

//backgound music to play waves. (Beach Waves Sound,2016)
void play_music() {
	PlaySound(TEXT("beachWaves.wav"), GetModuleHandle(NULL), SND_FILENAME | SND_ASYNC | SND_LOOP);
}


// The main function of the program
int main()
{
	// Declare which object from <iostream> and <string> the program will use
	using std::cout;
	using std::cin;
	using std::endl;
	using std::string;

	/********************  DO NOT EDIT THIS CODE *****************************
	 *
	 *	Declare a WinCanvas object. We wouldn't normally create an object this way as
	 *  it is non-portable, but this is a very simple 'graphics canvas' interface to
	 *  the Windows Graphical Device Interface (GDI) API.
	 */

	WinCanvas wc(WINDOW_WIDTH, WINDOW_HEIGHT, L"Class Arcade Game");

	/*
	 *  An InputState object provides an interface to the Keyboard and Mouse devices.
	 */

	InputState	  ui;
	OpenUIHandler(ui);	// Initialise the input interface

	/*********************END RESTRICTED CODE *********************************
	 **************************************************************************/

	 /*
	  *  Console Splash Screen
	  */

	cout << "  SIT153 Project - 2D Game Framework" << endl;
	cout << "=======================================" << endl << endl;
restart:
	/*
	 *   Demonstration of game initialisation - You can and should
	 *   modify this code in line with your software design, to
	 *   provide the necessary runtime functionality.
	 */

	 // Create a scene object - a container and manager for our sprites
	Scene demo(
		POINT2(0, 0),							// Upper left corner coordinate
		POINT2(WINDOW_WIDTH, WINDOW_HEIGHT)		// lower right corner coordinate
	);

	//initialising handle and mouse position variable
	POINT pawnPos = { 200, 200 };
	HWND hWnd = FindWindow(NULL, (LPCWSTR)L"Class Arcade Game");

	/*
	--------------------------------------------------------------------------------------------

				Loading all of the textures and creating texturedsprites 

	---------------------------------------------------------------------------------------------
	*/

	//Background image and GUI
	TEXTURE_PTR backgroundImg= std::make_shared<WinTexture>("resource/enviroment//background.bmp", RGB(255, 0, 255));
	std::shared_ptr<TexturedSprite> background = std::make_shared<TexturedSprite>(backgroundImg); // (Colourbox, 2016)
	
	//top banner
	TEXTURE_PTR topGUI_Img = std::make_shared<WinTexture>("resource/enviroment//topGUI.bmp", RGB(255, 255, 255));
	std::shared_ptr<TexturedSprite> topGUI = std::make_shared<TexturedSprite>(topGUI_Img);
	topGUI->SetPosition(POINT2f(130,0));

	//boat left
	TEXTURE_PTR boatImg = std::make_shared<WinTexture>("resource/enviroment//boat.bmp", RGB(255, 255, 255));
	std::shared_ptr<TexturedSprite> boat = std::make_shared<TexturedSprite>(boatImg);
	boat->SetPosition(POINT2f(50, 100)); // (123RF, 2016)

	//boat right
	TEXTURE_PTR shopImg = std::make_shared<WinTexture>("resource/enviroment//shop.bmp", RGB(255, 255, 255));
	std::shared_ptr<TexturedSprite> shop = std::make_shared<TexturedSprite>(shopImg);
	shop->SetPosition(POINT2f(700, 0)); // (123RF, 2016)

	// main menu
	TEXTURE_PTR menuImg = std::make_shared<WinTexture>("resource/enviroment//menu.bmp", RGB(255, 255, 255));
	std::shared_ptr<TexturedSprite> menu = std::make_shared<TexturedSprite>(menuImg);
	menu->SetPosition(POINT2f(200, 200));

	// die menu
	TEXTURE_PTR diedImg = std::make_shared<WinTexture>("resource/enviroment//die.bmp", RGB(255, 255, 255));
	std::shared_ptr<TexturedSprite> died = std::make_shared<TexturedSprite>(diedImg);
	died->SetPosition(POINT2f(200, 200));

	// start menu
	TEXTURE_PTR startScreenImg = std::make_shared<WinTexture>("resource/enviroment//start.bmp", RGB(255, 255, 255));
	std::shared_ptr<TexturedSprite> startScreen = std::make_shared<TexturedSprite>(startScreenImg);
	startScreen->SetPosition(POINT2f(200, 200));

	// player texture start(Fixride.com, 2016)
	TEXTURE_PTR swimDownLeft = std::make_shared<WinTexture>("resource/player//swimUp.bmp", RGB(255, 255, 255));
	std::shared_ptr<TexturedSprite> playerTexture = std::make_shared<TexturedSprite>(swimDownLeft);

	//Creating a Block object  and adding to scene
	std::shared_ptr<Block> pBlock = std::make_shared<Block>(50, 50, ID_PLAYER_COLLISION);

	// numbers for scoring (Gabrielwoj,2016)
	TEXTURE_PTR b_num0 = std::make_shared<WinTexture>("resource/numbers//num0.bmp", RGB(255, 255, 255));
	std::shared_ptr<TexturedSprite> b_num00 = std::make_shared<TexturedSprite>(b_num0);
	TEXTURE_PTR b_num1 = std::make_shared<WinTexture>("resource/numbers//num0.bmp", RGB(255, 255, 255));
	std::shared_ptr<TexturedSprite> b_num11 = std::make_shared<TexturedSprite>(b_num1);
	std::shared_ptr<gameScore> breathScoreSystem = std::make_shared<gameScore>(ID_NUMBERS, b_num00, b_num11);

	// numbers for scoring (Gabrielwoj,2016)
	TEXTURE_PTR f_num0 = std::make_shared<WinTexture>("resource/numbers//num0.bmp", RGB(255, 255, 255));
	std::shared_ptr<TexturedSprite> f_num00 = std::make_shared<TexturedSprite>(f_num0);
	TEXTURE_PTR f_num1 = std::make_shared<WinTexture>("resource/numbers//num0.bmp", RGB(255, 255, 255));
	std::shared_ptr<TexturedSprite> f_num11 = std::make_shared<TexturedSprite>(f_num1);
	std::shared_ptr<gameScore> fishScoreSystem = std::make_shared<gameScore>(ID_NUMBERS, f_num00, f_num11);

	// numbers for scoring (Gabrielwoj,2016)
	TEXTURE_PTR h_num0 = std::make_shared<WinTexture>("resource/numbers//num0.bmp", RGB(255, 255, 255));
	std::shared_ptr<TexturedSprite> h_num00 = std::make_shared<TexturedSprite>(h_num0);
	TEXTURE_PTR h_num1 = std::make_shared<WinTexture>("resource/numbers//num0.bmp", RGB(255, 255, 255));
	std::shared_ptr<TexturedSprite> h_num11 = std::make_shared<TexturedSprite>(h_num1);
	std::shared_ptr<gameScore> highScoreSystem = std::make_shared<gameScore>(ID_NUMBERS, h_num00, h_num11);


	// creating player

	std::shared_ptr<Player> swimmer = std::make_shared<Player>(playerTexture, ID_PLAYER, pBlock);



	/*
	--------------------------------------------------------------------------------------------

	Loading all of the textures and creating texturedsprites

	---------------------------------------------------------------------------------------------
	*/
	
	// adding all textures into list
	demo.AddSprite(background);
	demo.AddSprite(boat);
	demo.AddSprite(shop);
	demo.AddSprite(topGUI);
	demo.AddSprite(playerTexture);
	demo.AddEntity(swimmer);
	demo.AddDynamic(swimmer);

	demo.AddSprite(b_num11);
	demo.AddEntity(breathScoreSystem);
	demo.AddSprite(b_num00);
	demo.AddDynamic(breathScoreSystem);

	demo.AddSprite(f_num11);
	demo.AddEntity(fishScoreSystem);
	demo.AddSprite(f_num00);
	demo.AddDynamic(fishScoreSystem);

	demo.AddSprite(h_num11);
	demo.AddEntity(highScoreSystem);
	demo.AddSprite(h_num00);
	demo.AddDynamic(highScoreSystem);

	// adding player collision rect into list
	demo.AddDynamic(pBlock);
	demo.AddEntity(pBlock);

	/*
	--------------------------------------------------------------------------------------------

							setting scoring sprite locations of screen

	---------------------------------------------------------------------------------------------
	*/
	b_num00->SetPosition(POINT2f(-50,-50));
	b_num11->SetPosition(POINT2f(-50, -50));

	f_num00->SetPosition(POINT2f(-50, -50));
	f_num11->SetPosition(POINT2f(-50, -50));

	h_num00->SetPosition(POINT2f(-50, -50));
	h_num11->SetPosition(POINT2f(-50, -50));

	play_music(); // to play wave music, (Beach Waves Sound,2016)

	/*
	--------------------------------------------------------------------------------------------

								Creating game timer for fish

	---------------------------------------------------------------------------------------------
	*/
	int spawnTimer = 1;

	/*------------------------------------------------------------------------------------------
	  Execute the game loop
	 --------------------------------------------------------------------------------------------*/
	// bool to show startup menu information
	bool startOFGame = true;
	// The flag that the game loop uses to see if the game has finished
	bool gamefinished = false;
	// The main game loop

	/*------------------------------------------------------------------------------------------
										Start up menu loop
	--------------------------------------------------------------------------------------------*/
	while (!gamefinished)
	{
		if (startOFGame == true) {
			do {
				demo.AddSprite(startScreen);
					if (ui.isActive(VK_RETURN)) { // Enter key
						demo.RemoveSprite(startScreen);
						ui.ClearKeyState(VK_RETURN);
						startOFGame = false;
					}

					demo.OnDraw(wc);
					wc.Display();
			} while (startOFGame == true);
		}
		// ****************************************************************		
		// *** STAGE 1: Check InputState										***
		// ****************************************************************		


		/*------------------------------------------------------------------------------------------
												Checking inputs
		--------------------------------------------------------------------------------------------*/

		if (ui.isActive(VK_ESCAPE))		// opens ingame menu
		{
			bool exitMenu = false;
			ui.ClearKeyState(VK_ESCAPE); // clears key so it doesnt stick to buffer
			demo.AddSprite(menu); // adding menu to window

			//----------------------makes the menu stay up---------------------------
			demo.OnUpdate(0.01f);
			wc.Clear(255, 255, 255);		
			demo.OnDraw(wc);				
			wc.Display();
			//-----------------------------------------------------------------------

			do {
				
				if (ui.isActive(VK_ESCAPE)) { // return back to game
					exitMenu = true;
					demo.RemoveSprite(menu);
					ui.ClearKeyState(VK_ESCAPE);
				}
				else if (ui.isActive(0x52)) { // R key to restart
					demo.RemoveSprite(menu);
					goto restart; // gots back to keywork "restart"
				}
				else if (ui.isActive(0x51)) {// activales the endgame bool. exits game
					gamefinished = true;	
					wc.Terminate();			 
											
					exitMenu = true;
					continue;
				}
			} while (exitMenu == false); // loop to keep menu open and waits for user input
		}

		/*------------------------------------------------------------------------------------------
										Main input to direct swimmer
		--------------------------------------------------------------------------------------------*/
		else if (ui.isActive(LBUTTON)) {
			// bool to say cursor is on the screen assigning its position
			if (GetCursorPos(&pawnPos))
			{
				// bool to say it in the window and assigning its poition
				if (ScreenToClient(hWnd, &pawnPos))
				{
					ui.SetMouseCoordinate(pawnPos.x, pawnPos.y); // sets mouse position
					swimmer->Attach(&ui);
					swimmer->input_impl();
				}
			}
		}
		else {
			TEXTURE_PTR swimUp = std::make_shared<WinTexture>("resource/player//swimUp.bmp", RGB(255, 255, 255));
			swimmer->pawn->SetTexture(swimUp); // sets default texture  to swimming up
		}




		demo.OnInput();

		// ******************************************************************************		
		// *** STAGE 2: Update the state of the scene objects based on the user input ***
		// ******************************************************************************

		/*------------------------------------------------------------------------------------------
										sets scores for GUI
		--------------------------------------------------------------------------------------------*/

		breathScoreSystem->unit->SetPosition(POINT2f(290, 1));
		breathScoreSystem->tens->SetPosition(POINT2f(260, 1));
		breathScoreSystem->SetScore(int(swimmer->GetBreath()));

		fishScoreSystem->unit->SetPosition(POINT2f(655, 1));
		fishScoreSystem->tens->SetPosition(POINT2f(625, 1));
		fishScoreSystem->SetScore(int(swimmer->GetFishCount()));

		highScoreSystem->unit->SetPosition(POINT2f(429, 85));
		highScoreSystem->tens->SetPosition(POINT2f(399, 85));
		highScoreSystem->ChangeHighScore(swimmer->GetFishCount());


		/*------------------------------------------------------------------------------------------
										Spawner for fish
		--------------------------------------------------------------------------------------------*/
		spawnTimer++;
		if ((spawnTimer == 350)) {
			spawnTimer = 0;
			
					std::shared_ptr<Block> fishCollision = std::make_shared<Block>(40, 35, ID_FISH_COLLISION);

					TEXTURE_PTR fishAnimImg = std::make_shared<WinTexture>("resource/collectables//fish.bmp", RGB(255, 255, 255)); // (Beetle Muncher,2016)
					std::shared_ptr<TexturedAnimatedSprite> fishAnim = std::make_shared<TexturedAnimatedSprite>(80.6, 100, fishAnimImg, POINT2f(200, 200));

					std::shared_ptr<Collectables> fish0 = std::make_shared<Collectables>(fishAnim, ID_FISH0, 1, 200, VECTOR2f(0.5, 0), POINT2f(-20, 200), fishCollision);
					
					demo.AddEntity(fish0);
					demo.AddDynamic(fish0);
					demo.AddSprite(fishAnim);

					
					fish0->collisionArea->SetPosition(fish0->pawn->GetPosition());
					demo.AddEntity(fishCollision);
					demo.AddDynamic(fishCollision);
				
		}
		/*------------------------------------------------------------------------------------------
								Checks to see if game over by no breath only

											Menu for no breath
		--------------------------------------------------------------------------------------------*/

		if (gamefinished == false && (swimmer->GetBreath() < 0)) {
			bool choice = false; //bool check to see if user has entered input
			
			// adds menu to scene 
			demo.AddSprite(died); 
			demo.OnUpdate(0.01f);
			demo.OnDraw(wc);
			wc.Display();

			// loop to check for key press
			do {
				if (ui.isActive(0x59)) { // Y key to restart game
					demo.RemoveSprite(died);
					goto restart;
					ui.ClearKeyState(0x59);
			}
				else if(ui.isActive(0x4E)){ // N key to quit
					choice = true;
					gamefinished = true;
				}

			} while (choice == false);
		}
		
		demo.OnUpdate(0.01f);

		// ******************************************************************		
		// *** STAGE 3: Draw the shapes & text to the screen			  ***
		// ******************************************************************		
	
		



		wc.Clear(255, 255, 255);		// Need to clear the old screen before drawing new updated screen!
		demo.OnDraw(wc);				// Call the scene object's draw handler
		wc.Display();					// Last step is to actually 'display' the elements to the screen
		
	}

	// Finished the game. 
	cout << "Game finished. Now exiting...." << endl;
	return 0;
}

