#include "entity.h"
#include "gametest3d.h"
#include "simple_logger.h"
#include "mgl_callback.h"
#include "simple_logger.h"
#include "graphics3d.h"
#include "shader.h"
#include "obj.h"
#include "vector.h"
#include "sprite.h"
#include "entity.h"
#include "collisions.h"
#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"

static Entity *__entity_list = NULL;
static int __entity_max = 255;
static int __entity_initialized = 0;

static void entity_deinitialize();
static void shield_deinit();

extern Vec3D cameraPosition;
extern Vec3D cameraRotation;
extern char *fLoc;
extern int cUse; 
extern int score;
extern int gamePause;
extern int gameState;
extern int editX;
extern int editY;
extern int m;
extern int val;

int cBarrelRoll = 200;
int cBarrelUse = 0;
float worldHeight = 2;
float worldWidth = 2;
float worldBack = 10;
float worldFront = 50;
float cSpeed = 0.01;
int shields = 0;
int powerLength = 0;
int health = 3;
Entity *player1;
Entity *editUser;
Sprite *shieldText;
Sprite *temp;

void mainInit(){
	int i;
	Obj *playModel;
	Sprite *playSprite;
	Entity *gameSpeed, *liveCount;
	char liveSpr[] = "models/orange_piece.png";
	char speedSpr[] = "models/green_piece.png";
	entityClearList();
	score = 0;
	health = 3;

	temp = LoadSprite("models/mountain_text.png",50,50);
	playModel = obj_load("models/cube.obj");
	playSprite = LoadSprite("models/blue_piece.png",50,50);

	gameSpeed = newSpeeder(vec3d(0, 0, 4), "speedHUD", LoadSprite(speedSpr, 1024, 1024));
	for (i = 0; i < health; i++){
		liveCount = newLiveC(vec3d(i - 3, 0, -4), "life", LoadSprite(liveSpr, 1024, 1024), health - i);
	}
	player1 = newPlayer(vec3d(0,-5,0), "player", playModel,LoadSprite("models/mountain_text.png",1024,1024), playSprite, 3);
}

int mainInput(){
	Entity *shieldHero;
	SDL_Event eve;
	int keyn;
	Uint8 *keys;

	while(SDL_PollEvent(&eve)){
		if (gameState == 1){
			switch (eve.type){
			case SDL_QUIT:
				return 0;
				break;
			case SDL_KEYDOWN:
				switch (eve.key.keysym.sym){
				case SDLK_ESCAPE:
					return 0;
					break;
				case SDLK_w:
					if (player1->power == P_INVERT){
						player1->vVert = -0.05;
					}
					else{
						player1->vVert = 0.05;
					}
					break;
				case SDLK_s:
					if (player1->power == P_INVERT){
						player1->vVert = 0.05;
					}
					else{
						player1->vVert = -0.05;
					}
					break;
				case SDLK_a:
					if (player1->power == P_INVERT){
						player1->vHorz = 0.05;
					}
					else{
						player1->vHorz = -0.05;
					}
					break;
				case SDLK_d:
					if (player1->power == P_INVERT){
						player1->vHorz = -0.05;
					}
					else{
						player1->vHorz = 0.05;
					}
					break;
				default:
					break;
				}
				break;
			case SDL_KEYUP:
				switch (eve.key.keysym.sym){
				case SDLK_e:
					gameState = 0;
					break;
				case SDLK_w:
					player1->vVert = 0;
					break;
				case SDLK_s:
					player1->vVert = 0;
					break;
				case SDLK_a:
					player1->vHorz = 0;
					break;
				case SDLK_d:
					player1->vHorz = 0;
					break;
				case SDLK_p:
					if (gamePause == 0){
						gamePause = 1;
					}
					else{
						gamePause = 0;
					}
					break;
				default:
					break;
				}
				break;
			}
		}
		else if (gameState == 0){
			switch (eve.type){
				case SDL_KEYDOWN:
					switch (eve.key.keysym.sym){
						case SDLK_ESCAPE:
							return 0;
							break;
					}
					break;
				case SDL_KEYUP:
					switch (eve.key.keysym.sym){
						case SDLK_q:
							gameState = 1;
							break;
						case SDLK_1:
							fLoc = "files/string1.config";
							entityClearList();
							m = 0;
							break;
						case SDLK_2:
							fLoc = "files/string2.config";
							entityClearList();
							m = 0;
							break;
						case SDLK_3:
							fLoc = "files/string3.config";
							entityClearList();
							m = 0;
							break;
						case SDLK_4:
							fLoc = "files/string4.config";
							entityClearList();
							m = 0;
							break;
						case SDLK_5:
							fLoc = "files/string5.config";
							entityClearList();
							m = 0;
							break;
						case SDLK_w:
							if (editUser != NULL){
								if (editUser->body.position.z < 2){
									editUser->body.position.z += 1;
									editY += 1;
								}
							}
							break;
						case SDLK_a:
							if (editUser != NULL){
								if (editUser->body.position.x > -2){
									editUser->body.position.x -= 1;
									editX -= 1;
								}
							}
							break;
						case SDLK_s:
							if (editUser != NULL){
								if (editUser->body.position.z > -2){
									editUser->body.position.z -= 1;
									editY -= 1;
								}
							}
							break;
						case SDLK_d:
							if (editUser != NULL){
								if (editUser->body.position.x < 2){
									editUser->body.position.x += 1;
									editX += 1;
								}
							}
							break;
						case SDLK_z:
							changeLvlPiece(editY, editX);
							break;
					}
					break;
			}
		}
	}
	return 1;
	
}

void entity_init(int maxEntity)
{
    if (__entity_initialized)
    {
        slog("already initialized");
		return;
    }
    __entity_list = (Entity *)malloc(sizeof(Entity)*maxEntity);
    memset(__entity_list,0,sizeof(Entity)*maxEntity);
    __entity_max = maxEntity;
    __entity_initialized = 1;
    slog("initialized %i entities",maxEntity);
    atexit(entity_deinitialize);
}

static void entity_deinitialize()
{
    int i;
    for (i = 0;i < __entity_max;i++)
    {
        if (__entity_list[i].inuse)
        {
            entity_free(&__entity_list[i]);
        }
    }
    free(__entity_list);
    __entity_max = 0;
    __entity_initialized = 0;
}

void entityClearList(){
	int i;
	for (i = 0; i < __entity_max; i++)
	{
		if (__entity_list[i].inuse)
		{
			entity_free(&__entity_list[i]);
		}
	}
}

void entity_free(Entity *ent)
{
    if (!ent)
    {
        slog("passed a null entity");
        return;
    }
    ent[0].inuse = 0;
    obj_free(ent->objModel);
    FreeSprite(ent->texture);
}

Entity *entity_new()
{
    int i;
    for (i = 0; i < __entity_max;i++)
    {
        if (!__entity_list[i].inuse)
        {
            memset(&__entity_list[i],0,sizeof(Entity));
            __entity_list[i].inuse = 1;
            vec3d_set(__entity_list[i].scale,1,1,1);
            vec4d_set(__entity_list[i].color,1,1,1,1);
            return &__entity_list[i];
        }
    }
    return NULL;
}
void entity_think_all()
{
    int i;
    for (i = 0; i < __entity_max; i++)
    {
		if ((__entity_list[i].inuse) && (gameState != 1)){
			slog("hit this");
			entity_free(&__entity_list[i]);
		}
        if ((__entity_list[i].inuse) &&
            (__entity_list[i].think != NULL))
        {
            __entity_list[i].think(&__entity_list[i]);
        }
    }
}

void entity_draw_all()
{
    int i;
    for (i = 0;i < __entity_max;i++)
    {
        if (__entity_list[i].inuse)
        {
            entity_draw(&__entity_list[i]);
        }
    }
}

void entity_draw(Entity *ent)
{
    if (!ent)
    {
        return;
    }
    obj_draw(
        ent->objModel,
        ent->body.position,
        ent->rotation,
        ent->scale,
        ent->color,
        ent->texture
    );
}

Entity *newPlayer(Vec3D position, const char *name, Obj *model, Sprite *subSprite, Sprite *sprite, int aHp){
    Entity *player;
    player = entity_new();
    if(player == NULL) return;
    player->objModel = model;
    player->texture = sprite;
	player->subTexture = subSprite;
    player->rotation = vec3d(90,90,90);
    vec3d_cpy(player->body.position, position);
    cube_set(player->body.bounds, position.x, position.y, position.z, 0.2, 0.2, 0.2);
	player->scale = vec3d(0.2,0.2,0.2);
    player->type = T_PLAYER;
    player->hp = aHp;
	player->vVert = 0;
	player->vHorz = 0;
	player->power = P_NONE;
    player->think = playerThink;
    
    return player;
}

Entity *newPower(Vec3D position, const char *name, Obj *model, Sprite *sprite, int type){
    Entity *power;
    power = entity_new();
    if(power == NULL) return;
    power->objModel = model;
    power->texture = sprite;
    power->rotation = vec3d(90,90,90);
	power->scale = vec3d(1,0.2,1);
    vec3d_cpy(power->body.position, position);
    cube_set(power->body.bounds, position.x, position.y, position.z, 1, 0.2, 1);
    power->type = T_POWER;
    power->power = type;
    power->think = powerThink;
    
    return power;
}


Entity *newWall(Vec3D position, const char *name, Obj *model, Sprite *sprite, int type){
    Entity *wall;
	float wSize,hSize,dSize;
	wSize = rand()%10/10;
	hSize = rand()%10/10;
	dSize = rand()%10/10;
    wall = entity_new();
    if(wall == NULL) return;
    wall->objModel = model;
    wall->texture = sprite;
    wall->rotation = vec3d(90,90,90);
	//wall->scale = vec3d(wSize,hSize,dSize);
	wall->scale = vec3d(.5,.5,.5);
    vec3d_cpy(wall->body.position, position);
    cube_set(wall->body.bounds, position.x, position.y, position.z, .5, .5, .5);
    wall->type = T_WALL;
    wall->wall = type;
    wall->think = wallThink;
    
    return wall;
}

Entity *newBullet(Vec3D position, const char *name, Obj *model, Sprite *sprite){
    Entity *bull;
    bull = entity_new();
    if(bull == NULL) return;
    bull->objModel = model;
    bull->texture = sprite;
    bull->rotation = vec3d(90,90,90);
	bull->scale = vec3d(0.05,0.2,0.05);
    vec3d_cpy(bull->body.position, position);
    cube_set(bull->body.bounds, position.x, position.y, position.z, 0.05, 0.2, 0.05);
    bull->type = T_BULLET;
    bull->think = bulletThink;
    
    return bull;
}

Entity *newship(Vec3D position, const char *name, Obj *model, Sprite *sprite){
    Entity *ship;
    ship = entity_new();
    if(ship == NULL) return;
    ship->objModel = model;
    ship->texture = sprite;
    ship->rotation = vec3d(90,90,90);
	ship->scale = vec3d(0.2,0.2,0.2);
    vec3d_cpy(ship->body.position, position);
    cube_set(ship->body.bounds, position.x, position.y, position.z, 0.2, 0.2, 0.2);
    ship->type = T_SHIP;
	ship->time = 0;
    ship->think = shipThink;

    return ship;
}

Entity *newBlockAde(Vec3D position, const char *name, Obj *model, Sprite *sprite){
	Entity *block;
	slog("Created Block");
	block = entity_new();
	if(block == NULL)return;
	block->objModel = model;
	block->texture = sprite;
	block->rotation = vec3d(90,90,90);
	block->scale = vec3d(1,0.01,1);
	block->time = 240;
	block->blockState = 0;
	vec3d_cpy(block->body.position, position);
	cube_set(block->body.bounds, position.x, position.y, position.z, 1, 0.01, 1);
	block->type = T_BLOCK;
	block->think = blockThink;

	return block;
}

void playerThink(Entity *self){
	cameraPosition.y += cSpeed;
	if((self->vHorz < 0 && self->body.position.x > -worldWidth) || (self->vHorz > 0 && self->body.position.x < worldWidth))
	{
		self->body.position.x += self->vHorz;
	}
	if((self->vVert < 0 && self->body.position.z > -worldHeight) || (self->vVert > 0 && self->body.position.z < worldHeight))
	{
		self->body.position.z += self->vVert;
	}
	self->body.position.y += cSpeed;
	cube_set(self->body.bounds, self->body.position.x, self->body.position.y, self->body.position.z, 0.2, 0.2, 0.2);
}

void powerThink(Entity *self){
	if(cube_cube_intersection(self->body.bounds, player1->body.bounds)){
		player1->power = self->power;
		switch(self->power){
			case P_MINI:
				powerLength = 300;
				break;
			case P_BOMB:
				powerLength = 30;
				break;
			case P_INVERT:
				powerLength = 150;
				break;
		}
		entity_free(self);
	}
	if(self->body.position.y < cameraPosition.y-worldBack){
		entity_free(self);
	}
	cube_set(self->body.bounds, self->body.position.x, self->body.position.y, self->body.position.z, 1, 0.2, 1);
}

void wallThink(Entity *self){
	int i;
	switch(self->wall){
		case W_ZIG:
			switch(self->wMove){
				case 1:
					if(self->body.position.x < worldWidth && self->body.position.z < worldHeight){
						self->body.position.x += 0.02;
						self->body.position.z += 0.02;
					}
					else{
						self->wMove = 3;
					}
					break;
				case 2:
					if(self->body.position.x > -worldWidth && self->body.position.z < worldHeight){
						self->body.position.x -= 0.02;
						self->body.position.z += 0.02;
					}
					else{
						self->wMove = 1;
					}
					break;
				case 3:
					if(self->body.position.x < worldWidth && self->body.position.z > -worldHeight){
						self->body.position.x += 0.02;
						self->body.position.z -= 0.02;
					}
					else{
						self->wMove = 0;
					}
					break;
				default:
					if(self->body.position.x > worldWidth && self->body.position.z > worldHeight){
						self->body.position.x -= 0.02;
						self->body.position.z -= 0.02;
					}
					else{
						self->wMove = 2;
					}
					break;
			}
			break;
		case W_HORZ:
			switch(self->wMove){
				case 1:
					if(self->body.position.x > -worldWidth){
						self->body.position.x -= 0.05;
					}
					else{
						self->wMove = 0;
					}
					break;
				default:
					if(self->body.position.x < worldWidth){
						self->body.position.x += 0.05;
					}
					else{
						self->wMove = 1;
					}
					break;
			}
			break;
		case W_VERT:
			switch(self->wMove){
				case 1:
					if(self->body.position.z > -worldHeight){
						self->body.position.z -= 0.05;
					}
					else{
						self->wMove = 0;
					}
					break;
				default:
					if(self->body.position.z < worldHeight){
						self->body.position.z += 0.05;
					}
					else{
						self->wMove = 1;
					}
					break;
			}
			break;
		default:
			break;
	}
	if(player1->power == P_BOMB){
		entity_free(self);
	}
	self->body.position.y -= cSpeed;
	if(cube_cube_intersection(self->body.bounds, player1->body.bounds) && cBarrelUse == 0){
		cSpeed = 0.01;
		cUse -= 200;
		entity_free(self);
		val = 200;
		health--;
		if (health < 0){
			writeHighScore(score);
			gameState = 0;
		}
	}
	if(self->body.position.y < cameraPosition.y-worldBack){
		entity_free(self);
	}
	if(player1->power == P_MINI){
		self->scale = vec3d(0.1,0.1,0.1);
		cube_set(self->body.bounds, self->body.position.x, self->body.position.y, self->body.position.z, 0.1, 0.1, 0.1);
	}
	else{
		self->scale = vec3d(0.5,0.5,0.5);
		cube_set(self->body.bounds, self->body.position.x, self->body.position.y, self->body.position.z, 0.5, 0.5, 0.5);
	}
}

void bulletThink(Entity *self){
	if(player1->power == P_BOMB){
		entity_free(self);
	}
	if(player1->power == P_MINI){
		entity_free(self);
	}
    if(self->body.position.y < cameraPosition.y-worldBack){
		entity_free(self);
	}
	if(cube_cube_intersection(self->body.bounds, player1->body.bounds) && cBarrelUse == 0){
		entity_free(self);
		score -= 50;
	}
	cUse -= 50;
	self->body.position.y -= 0.1;
	cube_set(self->body.bounds, self->body.position.x, self->body.position.y, self->body.position.z, 0.2, 0.2, 0.2);
}

void shipThink(Entity *self){
	Entity *bullet;
	self->time += 1;
	if(player1->power == P_BOMB){
		entity_free(self);
	}
	if(self->time >= 300){
		bullet = newBullet(self->body.position, "bullet", obj_load("models/cube.obj"), LoadSprite("models/yell_piece.png",1024,1024));
		self->time = 0;
	}
	self->body.position.y += 0.01;
	if(self->body.position.y < cameraPosition.y-worldBack){
		entity_free(self);
	}
	if(cube_cube_intersection(self->body.bounds, player1->body.bounds) && cBarrelUse == 0){
		cSpeed = 0.01;
		entity_free(self);
		val = 200;
		health--;
		if (health < 0){
			writeHighScore(score);
			gameState = 0;
		}
	}
	if(player1->power == P_MINI){
		self->scale = vec3d(0.02, 0.02, 0.02);
		cube_set(self->body.bounds, self->body.position.x, self->body.position.y, self->body.position.z, 0.02, 0.02, 0.02);
	}else{
		self->scale = vec3d(0.2, 0.2, 0.2);
		cube_set(self->body.bounds, self->body.position.x, self->body.position.y, self->body.position.z, 0.2, 0.2, 0.2);
	}
}
void blockThink(Entity *self){
	//Sprite *sprite1, *sprite2;
	//sprite1 = self->texture;
	//sprite2 = self->subTexture;

	if(self->time > 0){
		self->time -= 1;
	}
	if(self->blockState == 0 && self->time <= 0){
		self->blockState = 1;
		self->time = 200;
		//FreeSprite(self->texture);
		slog("switched state");
	}
	if(self->blockState == 1){
		if(self->time > 0){
			self->time -= 1;
		}else{
			entity_free(self);
		}
	}
	self->body.position.y += cSpeed;
	cube_set(self->body.bounds, self->body.position.x, self->body.position.y, self->body.position.z, 0.5, 0.1, 0.5);
}

Entity *newScorer(Vec3D position, int scorer){
	Entity *scoreP;
	scoreP = entity_new();
	if (scoreP == NULL) return;
	scoreP->objModel = obj_load("models/cube.obj");
	scoreP->rotation = vec3d(90, 90, 90);
	vec3d_cpy(scoreP->body.position, position);
	cube_set(scoreP->body.bounds, position.x, position.y, position.z, .001, .001, .001);
	scoreP->scAdd = scorer;
	scoreP->type = T_SCORE;
	scoreP->think = scoreThink;

	return scoreP;
}
void scoreThink(Entity *self){
	if (self->body.position.y < cameraPosition.y - worldBack){
		slog("%i", score);
		score += self->scAdd;
		entity_free(self);
	}
}

Entity *newState(Vec3D position, const char *name, Sprite *spr, int st){
	Entity *stateItem;
	stateItem = entity_new();
	if (stateItem == NULL) return;
	stateItem->objModel = obj_load("models/cube.obj");
	stateItem->texture = spr;
	stateItem->scale = vec3d(.2, .2, .2);
	stateItem->rotation = vec3d(90, 90, 90);
	vec3d_cpy(stateItem->body.position, position);
	cube_set(stateItem->body.bounds, position.x, position.y, position.z, .25, .25, .25);
	stateItem->state = st;
	stateItem->type = T_STATE;
	stateItem->think = stateThink;
	stateItem->changeAble = 0;

	return stateItem;
}

void stateThink(Entity *self){
	if (editUser->body.position.x == self->body.position.x && editUser->body.position.z == self->body.position.z){
		
	}
}

Entity *newMover(Vec3D position, const char *name, Sprite *spr){
	Entity *editor;
	
	editor = entity_new();
	if (editor == NULL) return;
	editor->objModel = obj_load("models/cube.obj");
	editor->texture = spr;
	editor->scale = vec3d(.5, .5, .5);
	editor->rotation = vec3d(90, 90, 90);
	vec3d_cpy(editor->body.position, position);
	cube_set(editor->body.bounds, position.x, position.y, position.z, .25, .25, .25);
	editor->state = 0;
	editor->type = T_STATE;
	editor->think = moveThink;
	
	return editor;
}

void moveThink(Entity *self){

}

Entity *newSpeeder(Vec3D position, const char *name, Sprite *spr){
	Entity *gHud;

	gHud = entity_new();
	if (gHud == NULL) return;
	gHud->objModel = obj_load("models/cube.obj");
	gHud->texture = spr;
	gHud->scale = vec3d(cSpeed, .1, .25);
	gHud->rotation = vec3d(90, 90, 90);
	vec3d_cpy(gHud->body.position, position);
	cube_set(gHud->body.bounds, position.x, position.y, position.z, cSpeed, .1, .1);
	gHud->think = hudThink;

	return gHud;
}
void hudThink(Entity *self){

	self->body.position.y += cSpeed;
	self->scale = vec3d(.2, .01, cSpeed);

}

Entity *newLiveC(Vec3D position, const char *name, Sprite *spr, int state){
	Entity *live;
	live = entity_new();
	if (live == NULL) return;
	live->objModel = obj_load("models/cube.obj");
	live->texture = spr;
	live->scale = vec3d(.1, .1, .1);
	live->rotation = vec3d(90, 90, 90);
	vec3d_cpy(live->body.position, position);
	cube_set(live->body.bounds, position.x, position.y, position.z, .1, .1,.1);
	live->state = state;
	live->think = liveThink;

	return live;
}
void liveThink(Entity *self){

	self->body.position.y += cSpeed;
	if (health < self->state){
		entity_free(self); //kill the counter if the # of lives < which one this was
	}

}


/*eol@eof*/
