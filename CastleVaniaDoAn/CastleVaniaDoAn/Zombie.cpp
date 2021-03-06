#include "Zombie.h"


CZombie::CZombie(float x, float y, int nx):CEnemy(x, y, nx)
{
	this->AddAnimation(6001);
	this->AddAnimation(6000);
	this->AddAnimation(502);
	damageDeal = 2;
	if (nx > 0) {
		this->vx = ZOMBIE_MOVE_SPEED;
		canUpdate = false;
	}
	else {
		this->vx = -ZOMBIE_MOVE_SPEED;
		canUpdate = true;
	}
	this->height = 32;
	this->width = 16;
	this->vy = BASE_GRAVITY;
	this->enemyType = ZOMBIE_ID;

}

void CZombie::Update(DWORD dt, vector<LPGAMEOBJECT>* CreepColiObjects, float SimonPosx, bool stopEnemy)
{
	if (state != STATE_REMOVED) {
		this->stopEnemy = stopEnemy;

		if (state == STATE_BEING_REMOVE) {
			if (GetTickCount() - destroy_start > DESTROY_TIME) {
				state = STATE_REMOVED;
				_x = x;
				return;
			}
		}
		else {
			vy += BASE_GRAVITY * dt;
			if (!stopEnemy) {
				CGameObject::Update(dt);
			}
			else {
				dx = 0;
				dy = 0;
			}
			vector<LPCOLLISIONEVENT> coEvents;
			vector<LPCOLLISIONEVENT> coEventsResult;

			coEvents.clear();
			CalcPotentialCollisions(CreepColiObjects, coEvents);
			if (coEvents.size() == 0)
			{
				x += dx;
				y += dy;
			}
			else
			{
				float min_tx, min_ty, nx = 0, ny;
				FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);
				x += min_tx * dx + nx * 0.1f;		
				y += min_ty * dy + ny * 0.1f;
				if (ny != 0) vy = 0;
			}
			for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
		}

	}

}

void CZombie::Render(float & cameraPosX, float & cameraPosY)
{
	if (state == STATE_BEING_REMOVE || state == STATE_REMOVED) {
		animations[2]->Render(x - cameraPosX, y - cameraPosY);
	}
	else {
		if (nx > 0) {
			if (!stopEnemy) {
				animations[0]->Render(x - cameraPosX, y - cameraPosY);
			}
			else {
				animations[0]->RenderCurrentFrame(x - cameraPosX, y - cameraPosY);
			}
		}
		else {
			if (!stopEnemy) {
				animations[1]->Render(x - cameraPosX, y - cameraPosY);
			}
			else {
				animations[1]->RenderCurrentFrame(x - cameraPosX, y - cameraPosY);
			}
		}
	}
}

CZombie::~CZombie()
{
}
