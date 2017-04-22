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

#include "Scene.h"
#include "DerivedClasses.h"
#include "WinCanvas.h"


Scene::Scene(POINT2 const & s, POINT2 const & e)
	: m_ul(s),
	  m_br(e),
	  m_game_entities(),
	  m_drawable_objects(),
	  m_dynamic_objects(),
	  m_controlled_objects()
{}

Scene::~Scene()
{
	Clear();
}

void Scene::OnInput()
{
	for (auto c : m_controlled_objects)
		c->OnInput();
}

void Scene::OnUpdate(float const & dt)
{
	// Update any sprite object that requires it
	for (auto d : m_dynamic_objects)
		d->OnUpdate(dt);

	// resolve collisions
	RunCollisionTests();

	// do any other update actions (such as remove dead sprites)
}

void Scene::OnDraw(Canvas & c)
{
	for (auto s : m_drawable_objects)
		s->OnDraw(c);
}

bool Scene::Load(std::string const & filename)
{
	/*
	 * Add functionality here to load a set of sprites from the given file
	 */





	return false; // if failed to load
}
void Scene::AddEntity(ENTITY_PTR pEntity)
{
	m_game_entities.insert( std::make_pair(pEntity->GetUID(), pEntity) );
}

void Scene::AddSprite(SPRITE_PTR pSprite)
{
	m_drawable_objects.push_back(pSprite);
}

void Scene::AddDynamic(DYNAMIC_PTR pDynamic)
{
	m_dynamic_objects.push_back(pDynamic);
}


ENTITY_PTR Scene::RemoveEntity(UID_TYPE const & uid)
{
	ENTITY_PTR p = nullptr;
	auto iter = m_game_entities.find(uid);
	if (iter != m_game_entities.end())
	{
		
		iter = m_game_entities.erase(iter);
		p = Find(uid);
	}
	return p;
}

void Scene::RemoveSprite(SPRITE_PTR pSprite)
{
	m_drawable_objects.remove(pSprite);
}
void Scene::RemoveDynamic(DYNAMIC_PTR pDynamic)
{
	m_dynamic_objects.remove(pDynamic);
}


ENTITY_PTR Scene::Find(UID_TYPE const & uid)
{
	auto iter = m_game_entities.find(uid);
	if (iter != m_game_entities.end())
		return iter->second;
	else
		return nullptr;

}



void Scene::Clear()
{
	m_game_entities.clear();
	m_drawable_objects.clear();
	m_dynamic_objects.clear();
	m_controlled_objects.clear();
}

void Scene::RunCollisionTests()
{

	for (auto e_i : m_game_entities)
	{
		/*
		 * Test for a collision between this Sprite and boundary
		 */
		if (e_i.second->GetType() == LABEL_TYPE::ID_FISH0) {
			std::shared_ptr<Collectables>pFish = std::dynamic_pointer_cast<Collectables>(e_i.second);
			if (pFish->pawn->GetPosition().x > m_br.x) {
				RemoveDynamic(pFish);
				RemoveSprite(pFish->pawn);
				RemoveEntity(e_i.first);

				RemoveSprite(pFish->collisionArea);
				RemoveDynamic(pFish->collisionArea);
				RemoveEntity(pFish->collisionArea->GetUID());
				break;
			}
		}
		if (e_i.second->GetType() == ID_PLAYER) {
			std::shared_ptr<Player>pPlayer = std::dynamic_pointer_cast<Player>(e_i.second);
			if (pPlayer->GetPosition().x > (m_br.x - 112)) {
				pPlayer->pawn->SetPosition(POINT2f((m_br.x - 112),pPlayer->GetPosition().y));
			}
			else if (pPlayer->GetPosition().x < m_ul.x) {
				pPlayer->pawn->SetPosition(POINT2f((m_ul.x), pPlayer->GetPosition().y));
			}
		}


		/*
		 * Test for a collision between this Sprite and each other Sprite
		 */

		for (auto e_j : m_game_entities)
		{

			if (e_i.first == e_j.first) // don't test a Sprite against itself
				continue;

			if (e_i.second->GetType() == LABEL_TYPE::ID_PLAYER) {
				if (e_j.second->GetType() == LABEL_TYPE::ID_FISH0) {
					std::shared_ptr<Collectables>pFish = std::dynamic_pointer_cast<Collectables>(e_j.second);
					std::shared_ptr<Player>pPlayer = std::dynamic_pointer_cast<Player>(e_i.second);

					if (((pFish->GetPosition().x >  pPlayer->collisionRect->start().x) && (pFish->GetPosition().x < pPlayer->collisionRect->end().x)) || ((pPlayer->collisionRect->start().x > pFish->collisionArea->start().x) && (pPlayer->collisionRect->start().x < pFish->collisionArea->end().x))) {
						if (((pFish->GetPosition().y > pPlayer->collisionRect->start().y) && (pFish->GetPosition().y < pPlayer->collisionRect->end().y)) || ((pPlayer->collisionRect->start().y > pFish->collisionArea->start().y) && (pPlayer->collisionRect->start().y < pFish->collisionArea->end().y))) {

							pPlayer->SetFishCount(pPlayer->GetFishCount() + 1);

							RemoveDynamic(pFish);
							RemoveSprite(pFish->pawn);
							RemoveEntity(pFish->GetUID());

							RemoveSprite(pFish->collisionArea);
							RemoveDynamic(pFish->collisionArea);
							RemoveEntity(pFish->collisionArea->GetUID());


							

							break;
						}
					}

				}
			}
		}
	}
}