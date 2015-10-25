#include "entity.h"
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

static Entity *__entity_list = NULL;
static int __entity_max = 0;
static int __entity_initialized = 0;

static void entity_deinitialize();

float worldHeight = 30;
float worldWidth = 30;
float worldBack = 10;
float worldFront = 50;
int cSpeed = 0;
Entity *player;

void mainInit(){
	Obj *playModel;
	Sprite *playSprite;

	playModel = obj_load("models/cube.obj");
	playSprite = LoadSprite("models/cube_text.png",50,50);

	player = newPlayer(vec3d(0,0,0), "player", playModel, playSprite, 3);
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

int entity_is_entity(void *data)
{
    if (!data)return 0;
    if (!__entity_initialized)return 0;
    if ((Entity *)data < __entity_list)return 0;
    if ((Entity *)data >= (__entity_list + __entity_max))return 0;
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
    player->think = playerThink;
    mgl_callback_set(&player->body.touch, touch_callback_player, player);
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
    mgl_callback_set(&power->body.touch, touch_callback_dest, power);
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
    mgl_callback_set(&wall->body.touch, touch_callback_dest, wall);
    return wall;
}
Entity *newBullet(Vec3D position, const char *name, Obj *model, Sprite *sprite){
    Entity *bull;
    bull = entity_new();
    if(bull == NULL) return;
    bull->objModel = model;
    bull->texture = sprite;
    bull->rotation = vec3d(90,90,90);
	bull->scale = vec3d(0.2,0.2,0.2);
    vec3d_cpy(bull->body.position, position);
    cube_set(bull->body.bounds, position.x, position.y, position.z, 0.2, 0.2, 0.2);
    bull->type = T_BULLET;
    bull->think = bulletThink;
    mgl_callback_set(&bull->body.touch, touch_callback_dest, bull);
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
    ship->think = shipThink;
    mgl_callback_set(&ship->body.touch, touch_callback_dest, ship);
    return ship;
}

void playerThink(Entity *self){
	cube_set(self->body.bounds, self->body.position.x, self->body.position.y, self->body.position.z, 0.2, 0.2, 0.2);
}
void powerThink(Entity *self){
    
}

void wallThink(Entity *self){
	cube_set(self->body.bounds, self->body.position.x, self->body.position.y, self->body.position.z, 0.2, 0.2, 0.2);
    self->body.position.y += -0.1;
	if(cube_cube_intersection(self->body.bounds, player->body.bounds)){
		slog("Collision Detected");
		entity_free(self);
	}
}
void bulletThink(Entity *self){
    
}
void shipThink(Entity *self){
  
}

void touch_callback_dest(void *data, void *context){
    Entity *self, *other;
    Body *obody;
    if((!data)||(!context))return;
    self = (Entity *)data;
    obody = (Body *)context;
    if(entity_is_entity(obody->touch.data)){
        other = (Entity *)obody->touch.data;
        if(other->type == T_PLAYER || other->type == T_POWER){
            entity_free(self);
        }
    }
}

void touch_callback_player(void *data, void *context){
    Entity *self, *other;
    Body *obody;
    if((!data)||(!context))return;
    self = (Entity *)data;
    obody = (Body *)context;
    if(entity_is_entity(obody->touch.data)){
        other = (Entity *)obody->touch.data;
        switch (other->type){
            case T_BULLET:
                if(cSpeed > 0){
                    cSpeed--;
                }
                break;
            case T_POWER:
                switch (other->power){
                    case P_SHIELD:
                        self->power = P_SHIELD;
                        break;
                    case P_BOMB:
                        self->power = P_BOMB;
                        break;
                    case P_BLAST:
                        self->power = P_BLAST;
                        break;
                }
                break;
            default:
                if(self->hp > 0){
					self->hp--;
					slog("self->hp");
				}
                break;
        }
    }
}


/*eol@eof*/
