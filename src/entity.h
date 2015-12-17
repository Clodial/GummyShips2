#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "obj.h"
#include "vector.h"
#include "sprite.h"
#include "collisions.h"
#include "body.h"

enum Type{
	T_PLAYER, T_SHIP, T_WALL, T_BULLET, T_POWER, T_SHIELDW, T_SHIELDB, T_BLOCK, T_SCORE, T_STATE
};

enum WType{
	W_ZIG, W_STRAIGHT, W_HORZ, W_VERT
};
enum PType{
	P_MINI, P_BOMB, P_INVERT, P_NONE
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

	int wMove;

	int blockState;

    Obj *objModel;
    Obj *objAnimation[24];

	int scAdd;
	int changeAble;

    int state;
    float frame;
    Sprite *texture; 
	Sprite *subTexture;
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
/**
* @brief activates the associated think function of all active entities (if applicable)
*/
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

/**
 * @brief clear list without breaking the game
 */
void entityClearList();
/**
* @brief given data, check if it is specifically an entity
* @param data : 
* @return int : either 1 or 0 
*/
int entity_is_entity(void *data);

/**
* @brief Creates a new player object
* @param position: position to be instantiated
* @param name: 
* @param model:
* @param texture: 
* @param aHp: 
*/
Entity *newPlayer(Vec3D position, const char *name, Obj *model, Sprite *subSprite, Sprite *texture, int aHp);
/**
* @brief Creates a new player object
* @param position: position to be instantiated
* @param name: 
* @param model:
* @param texture: 
* @param aHp: 
*/
Entity *newPower(Vec3D position, const char *name, Obj *model, Sprite *texture, int type);
/**
* @brief Creates a new player object
* @param position: position to be instantiated
* @param name: 
* @param model:
* @param texture: 
* @param aHp: 
*/
Entity *newWall(Vec3D position, const char *name, Obj *model, Sprite *texture, int type, int move);
/**
* @brief Creates a new player object
* @param position: position to be instantiated
* @param name: 
* @param model:
* @param texture: 
* @param aHp: 
*/
Entity *newBullet(Vec3D position, const char *name, Obj *model, Sprite *texture);
/**
* @brief Creates a new player object
* @param position: position to be instantiated
* @param name: 
* @param model:
* @param texture: 
* @param aHp: 
*/
Entity *newShip(Vec3D position, const char *name, Obj *model, Sprite *texture);
/**
* @brief Creates a new Blockade Wall Object
* @param position: position to be instantiated
* @param name: Object's name
* @param model: Object's first model
* @param subModel: Object's second model
* @param texture: Object's first texture
* @param subTexture: Object's second texture 
*/
Entity *newBlockAde(Vec3D position, const char *name, Obj *model, Sprite *sprite);
/**
* @brief Creates a new player object
* @param position: position to be instantiated
* @param name: 
* @param model:
* @param texture: 
* @param aHp: 
*/
Entity *newShield(Vec3D position, const char *name, Obj *model, Sprite *texture, int type);

/**
* @brief Player think method
* @param self: the player entity
*/
void playerThink(Entity *self);
/**
* @brief Player think method
* @param self: the player entity
*/
void powerThink(Entity *self);
/**
* @brief Player think method
* @param self: the player entity
*/
void wallThink(Entity *self);
/**
* @brief Player think method
* @param self: the player entity
*/
void bulletThink(Entity *self);
/**
* @brief Player think method
* @param self: the player entity
*/
void shipThink(Entity *self);
/**
* @brief Player think method
* @param self: the player entity
*/
void shieldThink(Entity *self);
/**
* @brief Blockade Wall Object think
* @param self: the blockade wall's think object
*/
void blockThink(Entity *self);


// This is the object created to determine the score of each new wave coming in
Entity *newScorer(Vec3D position, int scorer);
void scoreThink(Entity *self);

Entity *newState(Vec3D position, const char *name, Sprite *spr, int st);
void stateThink(Entity *self);

Entity *newMover(Vec3D position, const char *name, Sprite *spr);
void moveThink(Entity *self);

Entity *newSpHUD(Vec3D position, const char *name, Sprite *spr);
void hudThink(Entity *self);
void drawHUD();

#endif
