#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "obj.h"
#include "vector.h"
#include "sprite.h"
#include "collisions.h"
#include "body.h"

enum Type{
	T_PLAYER, T_SHIP, T_WALL, T_BULLET, T_POWER, T_SHIELDW, T_SHIELDB
};

enum WType{
	W_ZIG, W_STRAIGHT, W_HORZ, W_VERT
};
enum PType{
	P_SHIELD, P_BOMB, P_BLAST
};

typedef struct Entity_S
{
    int inuse;
    int uid;    /**<unique id of this entity*/
    char name[128];

    Vec3D acceleration;
    Vec3D rotation;
    Vec3D scale;
    Vec4D color;

    int type;
    int power;
    int wall;
    int hp;

	float vVert;
	float vHorz;

	int time;

    Obj *objModel;
    Obj *objAnimation[24];
    int state;
    float frame;
    Sprite *texture;    /**<object texture*/
    Body body;
    void (*think)(struct Entity_S *self);
}Entity;

/**
* @brief initializes what has to be loaded at the start of the game
*/
void mainInit();

/**
* @brief control input for the game
* @return the status of the game loop, if it has ended or not
*/
int mainInput();

/**
 * @brief initialize the entity sub system
 * @param maxEntity the maximum number of simultaneously supported entities.
 */
void entity_init(int maxEntity);

/**
 * @brief get a pointer to a new entity
 * @return NULL on no more entities or error,  a valid entity pointer otherwise
 */
Entity *entity_new();

/**
 * @brief draws all active entities
 */
void entity_draw_all();
void entity_think_all();

/**
 * @brief draws an entity
 * @param ent the entity to draw
 */
void entity_draw(Entity *ent);

/**
 * @brief frees an entity
 */
void entity_free(Entity *ent);

int entity_is_entity(void *data);

Entity *newPlayer(Vec3D position, const char *name, Obj *model, Sprite *texture, int aHp);
Entity *newPower(Vec3D position, const char *name, Obj *model, Sprite *texture, int type);

Entity *newWall(Vec3D position, const char *name, Obj *model, Sprite *texture, int type);
Entity *newBullet(Vec3D position, const char *name, Obj *model, Sprite *texture);
Entity *newShip(Vec3D position, const char *name, Obj *model, Sprite *texture);

Entity *newShield(Vec3D position, const char *name, Obj *model, Sprite *texture, int type);

void playerThink(Entity *self);
void powerThink(Entity *self);

void wallThink(Entity *self);
void bulletThink(Entity *self);
void shipThink(Entity *self);

void shieldThink(Entity *self);

#endif
