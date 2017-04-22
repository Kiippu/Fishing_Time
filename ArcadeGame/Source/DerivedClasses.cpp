/* ********************************************************************************* *
* *  COPYRIGHT NOTICE                                                             * *
* *  ----------------                                                             * *
* *  (C)[2012] - [2015] Deakin University                                         * *
* *  All rights reserved.                                                         * *
* *  All information contained herein is, and remains the property of Deakin      * *
* *  University and the authors(Tim Wilkin, Michael Hobbs and Shaun Bangay).      * *
* *  Dissemination of this information or reproduction of this material is        * *
* *  strictly forbidden unless prior written permission is obtained from Deakin   * *
* *  University.The right to create derivative works from this material is        * *
* *  hereby granted to students enrolled in SIT153, but only for the purposes of  * *
* *  assessment while an enrolled student at Deakin University.                   * *
* *                                                                               * *
* ********************************************************************************* */


#include "DerivedClasses.h"
#include "WinCanvas.h"
#include <fstream>

Block::Block(unsigned int w, unsigned int h, LABEL_TYPE id, POINT2f const & p)
: Sprite(p), Dynamic(), Rect(p, POINT2f(p.x+w,p.y+h)), GameEntity(id)
	{}

Block::~Block()
{}

/*
 *		Implementation of Block update, invoked
 *		through base class (Sprite) update method.
 */
void Block::update_impl(float const & dt)
{
	VECTOR2f d = br - ul;
	ul = m_position;
	br.x = m_position.x + d.x;
	br.y = m_position.y + d.y;
	m_position = m_position + m_velocity;
}

/*
*		The Block implementaiton method for draw, which is called
*		from the base object (Shape) draw method.
*/
void Block::draw_impl(Canvas & c)
{
	// Use the GraphicsCanvas API to draw
	c.DrawRect(Rect(ul, br));

}

int Block::GetWidth()
{

	return (int)(br.x - ul.x);
}
int Block::GetHeight()
{
	return (int)(br.y - ul.y);
}

void Block::SetWidth(int w)
{
	br.x = ul.x + w;
}
void Block::SetHeight(int h)
{
	br.y = ul.y + h;
}


TexturedSprite::TexturedSprite(TEXTURE_PTR pt, POINT2f pos)
: Sprite(pos),
  m_pTexture(pt)  
{}

void TexturedSprite::draw_impl(Canvas & c)
{
	c.DrawTexture(*m_pTexture, m_position);
}
void TexturedSprite::SetTexture(TEXTURE_PTR pt)
{
	m_pTexture = pt;
}

/*
****************************************************************************
*
*								Animated texture Class
*
int widthOfTexture, int heightOfTexture, int textureCol, int textureRow, Texture const & t, POINT2f pos

****************************************************************************
*/
TexturedAnimatedSprite::TexturedAnimatedSprite(int widthOfTexture_, int heightOfTexture_, TEXTURE_PTR t, POINT2f pos_)
	: Sprite(pos_),m_pTexture(t)
{
	widthOfTexture = widthOfTexture_;
	heightOfTexture = heightOfTexture_;

	pos = pos_;
	};

void TexturedAnimatedSprite::draw_impl(Canvas & c) // timmer to change fich animation
{
	timer++;
	if (timer >= 0 && timer < 40) {
		c.DrawAnimTexture(widthOfTexture, heightOfTexture, 0, 0, *m_pTexture, m_position);
	}
	else if (timer >= 40 && timer < 80 ) {
		c.DrawAnimTexture(widthOfTexture, heightOfTexture, 1, 0, *m_pTexture, m_position);
	}
	else if (timer >= 80 && timer < 120) {
		c.DrawAnimTexture(widthOfTexture, heightOfTexture, 2, 0, *m_pTexture, m_position);
	}
	else if (timer >= 120 && timer < 160) {
		c.DrawAnimTexture(widthOfTexture, heightOfTexture, 3, 0, *m_pTexture, m_position);
	}
	else if (timer >= 160 && timer < 200) {
		c.DrawAnimTexture(widthOfTexture, heightOfTexture, 4, 0, *m_pTexture, m_position);
	}
	else if (timer >= 200 && timer < 240) {
		c.DrawAnimTexture(widthOfTexture, heightOfTexture, 5, 0, *m_pTexture, m_position);
	}
	else if (timer >= 240 && timer < 280) {
		c.DrawAnimTexture(widthOfTexture, heightOfTexture, 6, 0, *m_pTexture, m_position);
	}
	else if (timer >= 280 && timer < 320) {
		c.DrawAnimTexture(widthOfTexture, heightOfTexture, 7, 0, *m_pTexture, m_position);
	}
	else if (timer >= 320 && timer < 360) {
		c.DrawAnimTexture(widthOfTexture, heightOfTexture, 8, 0, *m_pTexture, m_position);
	}
	else if (timer >= 360 && timer < 400) {
		c.DrawAnimTexture(widthOfTexture, heightOfTexture, 9, 0, *m_pTexture, m_position);
	}
	else
		timer = 0;
}



/*
****************************************************************************
*
*								Player Class
*
****************************************************************************
*/

Player::Player(std::shared_ptr<TexturedSprite>TextureForSprite, LABEL_TYPE id, std::shared_ptr<Block> coll)
	:GameEntity(id)
{
	collisionRect = std::shared_ptr<Block>(coll);
	money = 100;
	pawn = std::shared_ptr<TexturedSprite>(TextureForSprite);
	breath = 30;
	breathDecrease = 0.01;
	breathTotal = 30;
	pawn->SetPosition(POINT2f(400, 400));
	SetVelocity(VECTOR2f(1.1, 1.1));
}

Player::~Player() {};

void Player::SetBreath(float difference) { // sets breath value
	if (pawn->GetPosition().y > 160)
		breath = breath - breathDecrease;
	else if (breath < 30)
		breath = breath + 0.1;
}; 

void Player::SetBreathTotal(float difference) {}; // gets breath value

void Player::SetMoney(int difference) { // sets increments to money ---- want to add this as a feture but no time
	money = money + (difference);
};

void Player::input_impl() { // what the player does with input
	/*------------------------------------------------------------------------------------------------------------------

	Physics water pressure formula: Presure = r(density of fluid) * g(acceleration of gravity) * h(height aboveobject)

	---------------------------------------------------------------------------------------------------------------------*/

	if (m_pInput->isActive(LBUTTON)) {
		
				// sets mouse cords
				POINT2 mouseXY = m_pInput->GetMouseCoordinate();
				POINT2f playerXY = pawn->GetPosition();
				VECTOR2f velocity = GetVelocity();

				m_position = pawn->GetPosition();
				// to stop swimmer swimming above wayer level
				if (playerXY.y >= 144) { 
					// Moves player sprite to mouse position.
					if (playerXY.x < mouseXY.x) {
						playerXY.x = playerXY.x + (velocity.x - PresureFourmulaAndResult(pawn->GetPosition()));
						pawn->SetPosition(playerXY);
					}
					if (playerXY.x > mouseXY.x) {
						playerXY.x = playerXY.x - ( velocity.x - PresureFourmulaAndResult(pawn->GetPosition()));
						pawn->SetPosition(playerXY);
					}
					if (playerXY.y < mouseXY.y) {
						playerXY.y = playerXY.y + (velocity.y - PresureFourmulaAndResult(pawn->GetPosition()));
						pawn->SetPosition(playerXY);
					}
					if (playerXY.y > mouseXY.y) {
						playerXY.y = playerXY.y - (velocity.y - PresureFourmulaAndResult(pawn->GetPosition()));
						pawn->SetPosition(playerXY);
					}
				}
			}
	ChangeTexture();
};

void Player::ChangeTexture() {
	//-----------------------------------------------------------------------
	//						Change player textures
	//
	//			All textures are not mine, (Fixride.com, 2016)
	//
	//----------------------------------------------------------------------
	POINT2 mouse = m_pInput->GetMouseCoordinate();
	POINT2f playerSprite = pawn->GetPosition();
	if ((mouse.x > playerSprite.x) && (mouse.x < playerSprite.x + 1) && (mouse.y > playerSprite.y)) {
		TEXTURE_PTR swimDown = std::make_shared<WinTexture>("resource/player//swimDown.bmp", RGB(255, 255, 255));
		pawn->SetTexture(swimDown);
	}
	else if ((mouse.x > playerSprite.x) && ((mouse.y > playerSprite.y) && (mouse.y < playerSprite.y + 1))) {
		TEXTURE_PTR swimRight = std::make_shared<WinTexture>("resource/player//swimRight.bmp", RGB(255, 255, 255));
		pawn->SetTexture(swimRight);
	}
	else if ((mouse.x > playerSprite.x) && (mouse.y > playerSprite.y)) {
		TEXTURE_PTR swimDownRight = std::make_shared<WinTexture>("resource/player//swimDownRight.bmp", RGB(255, 255, 255));
		pawn->SetTexture(swimDownRight);
	}

	// Selection statement - swim down, swim left, swim down and left
	if ((mouse.x > playerSprite.x) && (mouse.x < playerSprite.x + 1) && (mouse.y > playerSprite.y)) {
		TEXTURE_PTR swimDown = std::make_shared<WinTexture>("resource/player//swimDown.bmp", RGB(255, 255, 255));
		pawn->SetTexture(swimDown);
	}
	else if ((mouse.x < playerSprite.x) && ((mouse.y > playerSprite.y) && (mouse.y < playerSprite.y + 1))) {
		TEXTURE_PTR swimLeft = std::make_shared<WinTexture>("resource/player//swimLeft.bmp", RGB(255, 255, 255));
		pawn->SetTexture(swimLeft);
	}
	else if ((mouse.x < playerSprite.x) && (mouse.y > playerSprite.y)) {
		TEXTURE_PTR swimDownLeft = std::make_shared<WinTexture>("resource/player//swimDownLeft.bmp", RGB(255, 255, 255));
		pawn->SetTexture(swimDownLeft);
	}

	// Selection statement - swim up, swim left, swim up and left
	if ((mouse.x > playerSprite.x) && (mouse.x < playerSprite.x + 1) && (mouse.y < playerSprite.y)) {
		TEXTURE_PTR swimUp = std::make_shared<WinTexture>("resource/player//swimUp.bmp", RGB(255, 255, 255));
		pawn->SetTexture(swimUp);
	}
	else if ((mouse.x < playerSprite.x) && ((mouse.y > playerSprite.y) && (mouse.y < playerSprite.y + 1))) {
		TEXTURE_PTR swimLeft = std::make_shared<WinTexture>("resource/player//swimLeft.bmp", RGB(255, 255, 255));
		pawn->SetTexture(swimLeft);
	}
	else if ((mouse.x < playerSprite.x) && (mouse.y < playerSprite.y)) {
		TEXTURE_PTR swimUpLeft = std::make_shared<WinTexture>("resource/player//swimUpLeft.bmp", RGB(255, 255, 255));
		pawn->SetTexture(swimUpLeft);
	}

	// Selection statement - swim up, swim right, swim up and right
	if ((mouse.x > playerSprite.x) && (mouse.x < playerSprite.x + 1) && (mouse.y < playerSprite.y)) {
		TEXTURE_PTR swimUp = std::make_shared<WinTexture>("resource/player//swimUp.bmp", RGB(255, 255, 255));
		pawn->SetTexture(swimUp);
	}
	else if ((mouse.x > playerSprite.x) && ((mouse.y > playerSprite.y) && (mouse.y < playerSprite.y + 1))) {
		TEXTURE_PTR swimRight = std::make_shared<WinTexture>("resource/player//swimRight.bmp", RGB(255, 255, 255));
		pawn->SetTexture(swimRight);
	}
	else if ((mouse.x > playerSprite.x) && (mouse.y < playerSprite.y)) {
		TEXTURE_PTR swimUpRight = std::make_shared<WinTexture>("resource/player//swimUpRight.bmp", RGB(255, 255, 255));
		pawn->SetTexture(swimUpRight);
	}
}

void Player::update_impl(float const & dt) {

	
	POINT2f floatUp = pawn->GetPosition();
	if (floatUp.y > 145) { // float play up slowly if not swimming
		floatUp.y = floatUp.y - 0.05;
		pawn->SetPosition(floatUp);
	} // resets  y-axis if it goes below value. it is possible as the position is a float.
	else if (floatUp.y < 145) {
		floatUp.y = 145;
		pawn->SetPosition(floatUp);
	}
	collisionRect->SetPosition(pawn->GetPosition()); // sets collision rect on testure
	
	SetBreath(breathDecrease);
};

float Player::PresureFourmulaAndResult(POINT2f playerPos) {
	// (Fluid pressure and depth, 2016)
	//Normal density of salt water is 1.3x10^3 kg/m^3 . So assuming that every 30 points on the y-axis is 1m.
	float r = ((1.3) * ((10) * 3));

	//Normal atmospheric pressure is 1.01x10^5 Newtons/m^2 . So assuming that the game world is "normal".
	float g = ((1.01) * ((10) * 5));

	//as we are assuming that each 30 points in the window is 1m and minus the area above the water.
	float h = ((playerPos.y - 145));

	// so the final foumula is:
	float presure = (r * g * h);
	return (presure/800000);

};

void Player::SetFishCount(int fish) { // sets fish count
	fishCount = fish;
};


/*
****************************************************************************
*
*			Collectables Class - base for other collectables
*
****************************************************************************
*/

Collectables::Collectables(std::shared_ptr<TexturedAnimatedSprite>TextureForSprite, LABEL_TYPE id, int numAboveMin, int min, VECTOR2f velocity, POINT2f startPoint, std::shared_ptr<Block>coll)
	:GameEntity(id)
{
	collisionArea = std::shared_ptr<Block>(coll);
	RewardMoney = moneyQuantity(numAboveMin, min);
	pawn = TextureForSprite;
	SetSpawnPoint(startPoint);
	SetVelocity(velocity);
};

// Random money generator, min and max depends on ID of collectable.
int Collectables::moneyQuantity(int numAboveMin, int min) {
	return (rand() % numAboveMin + min);
};
 // updates position of collectable depending on where its spawnpoint is.
void Collectables::update_impl(float const & dt) {
		pawn->SetPosition(POINT2f(pawn->GetPosition().x + m_velocity.x, pawn->GetPosition().y + m_velocity.y));
		collisionArea->SetPosition(POINT2f(pawn->GetPosition().x + m_velocity.x, pawn->GetPosition().y + m_velocity.y));
		m_position =  pawn->GetPosition();
		collisionArea->SetPosition(POINT2f(m_position.x + 10, m_position.y + 10));
};

void Collectables::SetSpawnPoint(POINT2f startPoint) {
	// Creating variation when spawned, -50 to 50, a range Y. Range X can and should be only -50 or window width +50 so sync with update_impl.
	int startPointVariation = rand() % 500; 
	pawn->SetPosition(POINT2f( startPoint.x,(startPoint.y + startPointVariation) ));
	spawnPoint = POINT2f(startPoint.x, (startPoint.y + startPointVariation));
};


// returns spawnPoint
POINT2f Collectables::GetSpawnPoint() {
	return spawnPoint;
};


void Collectables::SetColisionAreaPosition(POINT2f b) { // sets collision rect to point of b
	collisionArea->SetPosition(b);
};

/*
****************************************************************************
*
*			Score Class - holds scores
*
****************************************************************************
*/

gameScore::gameScore(LABEL_TYPE id, std::shared_ptr<TexturedSprite>units, std::shared_ptr<TexturedSprite>tenss)
	: GameEntity(id)
{
	unit = units;
	tens = tenss;
};

void gameScore::changeScoreUnits(float breath) { // changes tens texture depending on score
	
	if (int(breath) == 30|| int(breath) == 20|| int(breath) == 10) {
		TEXTURE_PTR num0 = std::make_shared<WinTexture>("resource/numbers//num0.bmp", RGB(255, 255, 255));
		unit->SetTexture(num0);
	}
	else if (int(breath) == 29|| int(breath) == 19|| int(breath) == 9) {
		TEXTURE_PTR num9 = std::make_shared<WinTexture>("resource/numbers//num9.bmp", RGB(255, 255, 255));
		unit->SetTexture(num9);
	}
	else if (int(breath) == 28|| int(breath) == 18|| int(breath) == 8) {
		TEXTURE_PTR num8 = std::make_shared<WinTexture>("resource/numbers//num8.bmp", RGB(255, 255, 255));
		unit->SetTexture(num8);
	}
	else if (int(breath) == 27|| int(breath) == 17|| int(breath) == 7) {
		TEXTURE_PTR num7 = std::make_shared<WinTexture>("resource/numbers//num7.bmp", RGB(255, 255, 255));
		unit->SetTexture(num7);
	}
	else if (int(breath) == 26|| int(breath) == 16|| int(breath) == 6) {
		TEXTURE_PTR num6 = std::make_shared<WinTexture>("resource/numbers//num6.bmp", RGB(255, 255, 255));
		unit->SetTexture(num6);
	}
	else if (int(breath) == 25|| int(breath) == 15|| int(breath) == 5) {
		TEXTURE_PTR num5 = std::make_shared<WinTexture>("resource/numbers//num5.bmp", RGB(255, 255, 255));
		unit->SetTexture(num5);
	}
	else if (int(breath) == 24|| int(breath) == 14|| int(breath) == 4) {
		TEXTURE_PTR num4 = std::make_shared<WinTexture>("resource/numbers//num4.bmp", RGB(255, 255, 255));
		unit->SetTexture(num4);
	}
	else if (int(breath) == 23|| int(breath) == 13|| int(breath) == 3) {
		TEXTURE_PTR num3 = std::make_shared<WinTexture>("resource/numbers//num3.bmp", RGB(255, 255, 255));
		unit->SetTexture(num3);
	}
	else if (int(breath) == 22|| int(breath) == 12|| int(breath) == 2) {
		TEXTURE_PTR num2 = std::make_shared<WinTexture>("resource/numbers//num2.bmp", RGB(255, 255, 255));
		unit->SetTexture(num2);
	}
	else if (int(breath) == 21|| int(breath) == 11|| int(breath) == 1) {
		TEXTURE_PTR num1 = std::make_shared<WinTexture>("resource/numbers//num1.bmp", RGB(255, 255, 255));
		unit->SetTexture(num1);
	}
	
};

void gameScore::changeScoreTens(float breath) { // changes tens  depending on score
	if (int(breath) < 10) {
		TEXTURE_PTR num0 = std::make_shared<WinTexture>("resource/numbers//num0.bmp", RGB(255, 255, 255));
		tens->SetTexture(num0);
	}
	else if (int(breath) == 30) {
		TEXTURE_PTR num3 = std::make_shared<WinTexture>("resource/numbers//num3.bmp", RGB(255, 255, 255));
		tens->SetTexture(num3);
	}
	else if (int(breath) > 19 && int(breath) < 30) {
		TEXTURE_PTR num2 = std::make_shared<WinTexture>("resource/numbers//num2.bmp", RGB(255, 255, 255));
		tens->SetTexture(num2);
	}
	else if (int(breath) > 9 && int(breath) < 20) {
		TEXTURE_PTR num1 = std::make_shared<WinTexture>("resource/numbers//num1.bmp", RGB(255, 255, 255));
		tens->SetTexture(num1);
	}
};

void gameScore::update_impl(float const & dt) { // changes new scores
	changeScoreTens(score);
	changeScoreUnits(score);
};

void gameScore::SetScore(float aScoreFromPlayer) { //sets score
	score = aScoreFromPlayer;
};

void gameScore::ChangeHighScore(int Score) { // file saving for high score
	std::ifstream	f_read;	
	std::ofstream	f_write;
	int	data;	

	f_read.open("high_score.txt"); 
	f_read >> data;
	hScore = data;
	score = hScore;
	if (hScore < Score) { // high score to new score
		f_read.close();
		f_write.open("high_score.txt");
		if (f_write.is_open()) { // opens  file to write new high score to.
			f_write << Score;
			hScore = data;
			f_write.close();
		}
		f_write.close(); // close file
	}
	else
		f_read.close(); // closes file.

};

