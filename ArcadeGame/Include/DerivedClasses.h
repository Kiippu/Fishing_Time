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

#ifndef DERIVED_CLASS_H
#define DERIVED_CLASS_H

#include "BaseClasses.h"
#include "WinCanvas.h"


class Block : public Sprite, public Dynamic, public Rect , public GameEntity
{
	private:
		// Implement inherited private implementation
		virtual void draw_impl(Canvas & c);
		virtual void update_impl(float const & dt);

	public:
		Block(unsigned int w, unsigned int h, LABEL_TYPE id , POINT2f const & p = { 0, 0 });
		~Block();

		int GetWidth();
		int GetHeight();

		void SetWidth(int w);
		void SetHeight(int h);

};

class TexturedSprite : public Sprite
{
	private:
		TEXTURE_PTR  m_pTexture;

		virtual void draw_impl(Canvas & c);

	protected:

	public:
		explicit TexturedSprite(TEXTURE_PTR pt = nullptr, POINT2f pos = { 0, 0 });

		virtual ~TexturedSprite(){}

		void SetTexture(TEXTURE_PTR pt);
};

class TexturedAnimatedSprite : public Sprite
{
private:
	
	int widthOfTexture;
	int heightOfTexture;
	POINT2f pos;
	TEXTURE_PTR  m_pTexture;
	int timer = 0;

	virtual void draw_impl(Canvas & c);

protected:

public:
	explicit TexturedAnimatedSprite(int width, int height, TEXTURE_PTR t, POINT2f pos);

	virtual ~TexturedAnimatedSprite() {};
};


class Player : public TexturedSprite, public Controllable, public GameEntity, public Dynamic
{
private:
	int money;
	float breath;
	float breathDecrease;
	float breathTotal;
	int fishCount = 0;
public:
	Player(std::shared_ptr<TexturedSprite>p, LABEL_TYPE id, std::shared_ptr<Block> coll);
	~Player();

	std::shared_ptr<Block> collisionRect;
	std::shared_ptr<TexturedSprite> pawn;

	void update_impl(float const & dt);
	void input_impl();
	void ChangeTexture();

	void SetBreath(float difference);
	float GetBreath() { return breath; };

	void SetBreathTotal(float difference);
	float GetBreathTotal() { return breathTotal; };

	int GetMoney() { return money; };
	void SetMoney(int difference);

	void SetFishCount(int FishInPocket);
	int GetFishCount() { return fishCount; };

	float PresureFourmulaAndResult(POINT2f playerPos);
};


class Collectables : public TexturedSprite, public GameEntity, public Dynamic
{
private:
	int RewardMoney;
	POINT2f spawnPoint;
	
public:
	Collectables(std::shared_ptr<TexturedAnimatedSprite>p, LABEL_TYPE id, int max, int min, VECTOR2f velocity, POINT2f startPoint, std::shared_ptr<Block>coll);
	~Collectables() {};

	void SetSpawnPoint(POINT2f startPoint);
	POINT2f GetSpawnPoint();
	virtual void update_impl(float const & dt);
	std::shared_ptr<TexturedAnimatedSprite> pawn;
	
	int moneyQuantity(int max, int min);
	int GetReward() { return RewardMoney; };

	std::shared_ptr<Block>(collisionArea); 
	void SetColisionAreaPosition(POINT2f b);
};

class gameScore : public TexturedSprite, public GameEntity, public Dynamic
{
private:
	int score = 0;
	int hScore = 0;
public:
	gameScore(LABEL_TYPE id, std::shared_ptr<TexturedSprite>units, std::shared_ptr<TexturedSprite>tens);
	~gameScore() {};

	
	virtual void update_impl(float const & dt);

	std::shared_ptr<TexturedSprite> unit;
	std::shared_ptr<TexturedSprite> tens;

	void changeScoreUnits(float breath);
	void changeScoreTens(float breath);

	void SetScore(float breath);

	void ChangeHighScore(int hScore);
};




#endif