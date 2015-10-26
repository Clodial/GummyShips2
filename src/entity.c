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
static int __entity_max = 0;
static int __entity_initialized = 0;

static Entity *__shield_list = NULL;
static int __shield_max = 0;
static int __shield_init = 0;


static void entity_deinitialize();
static void shield_deinit();

extern Vec3D cameraPosition;
extern Vec3D cameraRotation;
extern int cUse; 

float worldHeight = 2;
float worldWidth = 2;
float worldBack = 10;
float worldFront = 50;
float cSpeed = 0.01;
int shields = 0;
Entity *player1;
Sprite *shieldText;
Obj *shieldObj;

void mainInit(){
	Obj *playModel;
	Sprite *playSprite;

	shieldText = LoadSprite("models/cube_text.png",50,50);
	shieldObj = obj_load("models/cube.obj");
	playModel = obj_load("models/cube.obj");
	playSprite = LoadSprite("models/cube_text.png",50,50);

	player1 = newPlayer(vec3d(0,-5,0), "player", playModel, playSprite, 3);
}

int mainInput(){
	Entity *shieldHero;
	SDL_Event eve;
	int keyn;
	Uint8 *keys;

	while(SDL_PollEvent(&eve)){
		switch(eve.type){
			case SDL_QUIT:
				return 0;
				break;
			case SDL_KEYDOWN:
				switch(eve.key.keysym.sym){
					case SDLK_ESCAPE:
						return 0;
						break;
					case SDLK_w:
						player1->vVert = 0.05;
						break;
					case SDLK_s:
						player1->vVert = -0.05;
						break;
					case SDLK_a:
						player1->vHorz = -0.05;
						break;
					case SDLK_d:
						player1->vHorz = 0.05;
						break;
					case SDLK_c:
						if(cUse > 200){
							cSpeed = 2;
							cUse -= 20;
						}else{
							cSpeed = 0.01;
						}
					default:
						break;
				}
				break;
			case SDL_KEYUP:
				switch(eve.key.keysym.sym){
					case SDLK_z:
						slog("button pressed");
						shieldHero = newShield(vec3d(player1->body.position.x, player1->body.position.y + 1, player1->body.position.z), "shield", shieldObj, shieldText, T_SHIELDW); 
						break;
					case SDLK_x:
						shieldHero = newShield(vec3d(player1->body.position.x, player1->body.position.y, player1->body.position.z), "shield", shieldObj, shieldText, T_SHIELDB); 
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
					case SDLK_c:
						cSpeed = 0.01;
						cUse = 0;
					default:
						break;
				}
				break;
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


/*
* For the shields
*/
void shield_init(int maxShields)
{
    if (__shield_init)
    {
        slog("already initialized shields");
        return;
    }
    __shield_list = (Entity *)malloc(sizeof(Entity)*maxShields);
    memset(__shield_list,0,sizeof(Entity)*maxShields);
    __shield_max = maxShields;
    __shield_init = 1;
    slog("initialized %i shields",maxShields);
    atexit(shield_deinit);
}

static void shield_deinit()
{
    int i;
    for (i = 0;i < __shield_max;i++)
    {
        if (__shield_list[i].inuse)
        {
            entity_free(&__shield_list[i]);
        }
    }
    free(__shield_list);
    __shield_max = 0;
    __shield_init = 0;
}

Entity *shield_new()
{
    int i;
	
	slog("Hi");
	for (i = 0; i < __shield_max;i++)
    {
        if (!__shield_list[i].inuse)
        {
            memset(&__shield_list[i],0,sizeof(Entity));
            __shield_list[i].inuse = 1;
            vec3d_set(__shield_list[i].scale,1,1,1);
            vec4d_set(__shield_list[i].color,1,1,1,1);
            slog("Bye");
			return &__shield_list[i];

        }
    }
    return NULL;
}

void shield_think_all()
{
    int i;
    for (i = 0; i < __shield_max; i++)
    {
        if ((__shield_list[i].inuse) &&
            (__shield_list[i].think != NULL))
        {
            __shield_list[i].think(&__shield_list[i]);
        }
    }
}

void shield_draw_all()
{
    int i;
    for (i = 0;i < __shield_max;i++)
    {
        if (__shield_list[i].inuse)
        {
            shield_draw(&__shield_list[i]);
        }
    }
}

void shield_draw(Entity *ent)
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

int shield_is_shield(void *data)
{
    if (!data)return 0;
    if (!__shield_init)return 0;
    if ((Entity *)data < __shield_list)return 0;
    if ((Entity *)data >= (__shield_list + __shield_max))return 0;
    return 1;
}

Entity *newPlayer(Vec3D position, const char *name, Obj *model, Sprite *sprite, int aHp){
    Entity *player;
    player = entity_new();
    if(player == NULL) return;
    player->objModel = model;
    player->texture = sprite;
    player->rotation = vec3d(90,90,90);
    vec3d_cpy(player->body.position, position);
    cube_set(player->body.bounds, position.x, position.y, position.z, 0.2, 0.2, 0.2);
	player->scale = vec3d(0.2,0.2,0.2);
    player->type = T_PLAYER;
    player->hp = aHp;
	player->vVert = 0;
	player->vHorz = 0;
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
	power->scale = vec3d(0.2,0.2,0.2);
    vec3d_cpy(power->body.position, position);
    cube_set(power->body.bounds, position.x, position.y, position.z, 0.2, 0.2, 0.2);
    power->type = T_POWER;
    power->power = type;
    power->think = powerThink;
    
    return power;
}


Entity *newWall(Vec3D position, const char *name, Obj *model, Sprite *sprite, int type){
    Entity *wall;
    wall = entity_new();
    if(wall == NULL) return;
    wall->objModel = model;
    wall->texture = sprite;
    wall->rotation = vec3d(90,90,90);
	wall->scale = vec3d(0.2,0.2,0.2);
    vec3d_cpy(wall->body.position, position);
    cube_set(wall->body.bounds, position.x, position.y, position.z, 0.2, 0.2, 0.2);
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

Entity *newShield(Vec3D position, const char *name, Obj *model, Sprite *sprite, int type){
	Entity *shield;
	shield = shield_new();
	if(shield == NULL)return;
	vec3d_cpy(shield->body.position, position);
	shield->body.position.y += .5;
	shield->rotation = vec3d(90,90,90);
	//shield->scale = vec3d(0.3,0.05,0.3);
	shield->scale = vec3d(6,0.05,6);
	shield->objModel = model;
	shield->texture = sprite;
	cube_set(shield->body.bounds, position.x, position.y, position.z, 10, 5, 10);
    shield->type = type;
	shield->time = 3000;
	shield->think = shieldThink;

	return shield;
}

void playerThink(Entity *self){
	cube_set(self->body.bounds, self->body.position.x, self->body.position.y, self->body.position.z, 0.2, 0.2, 0.2);
	self->body.position.y += cSpeed;
	cameraPosition.y += cSpeed;
	if((self->vHorz < 0 && self->body.position.x > -worldWidth) || (self->vHorz > 0 && self->body.position.x < worldWidth))
	{
		self->body.position.x += self->vHorz;
	}
	if((self->vVert < 0 && self->body.position.z > -worldHeight) || (self->vVert > 0 && self->body.position.z < worldHeight))
	{
		self->body.position.z += self->vVert;
	}
}

void powerThink(Entity *self){
	cube_set(self->body.bounds, self->body.position.x, self->body.position.y, self->body.position.z, 0.2, 0.2, 0.2);
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
	cube_set(self->body.bounds, self->body.position.x, self->body.position.y, self->body.position.z, 0.2, 0.2, 0.2);
	self->body.position.y -= cSpeed;
	if(cube_cube_intersection(self->body.bounds, player1->body.bounds)){
		cSpeed = 0.01;
		slog("Collision Detected");
		cUse -= 200;
		self->body.position.y = cameraPosition.y + 100;
	}
	for(i = 0; i < __shield_max; i+= 1){
		if(__shield_list[i].inuse && __shield_list[i].type == T_SHIELDW){
			slog("stuff %i", __shield_list[i].body.bounds.x);
			if(cube_cube_intersection(self->body.bounds, __shield_list[i].body.bounds)){
				slog("collided with shield");
				entity_free(&__shield_list[i]);
				entity_free(self);
			}
		}
	}
	if(self->body.position.y < cameraPosition.y-worldBack){
		entity_free(self);
	}
}

void bulletThink(Entity *self){
	cube_set(self->body.bounds, self->body.position.x, self->body.position.y, self->body.position.z, 0.2, 0.2, 0.2);
	self->body.position.y -= 0.1;
    if(self->body.position.y < cameraPosition.y-worldBack){
		entity_free(self);
	}
	if(cube_cube_intersection(self->body.bounds, player1->body.bounds)){
		cSpeed -= 0.01;
		entity_free(self);
	}
	cUse -= 50;
}

void shipThink(Entity *self){
	Entity *bullet;

	cube_set(self->body.bounds, self->body.position.x, self->body.position.y, self->body.position.z, 0.2, 0.2, 0.2);
	self->time += 1;
	if(self->time >= 300){
		bullet = newBullet(self->body.position, "bullet", obj_load("models/cube.obj"), LoadSprite("models/mountain_text.png",1024,1024));
		self->time = 0;
	}
	self->body.position.y += 0.01;
	if(self->body.position.y < cameraPosition.y-worldBack){
		entity_free(self);
	}
	if(cube_cube_intersection(self->body.bounds, player1->body.bounds)){
		cSpeed = 0.01;
		cUse = 0;
		entity_free(self);
	}
}

void shieldThink(Entity *self){

	self->body.position.y += cSpeed;
	if(self->time > 0){
		self->time -= 1;
	}else{
		entity_free(self);		
	}

}


/*eol@eof*/
