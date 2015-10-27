/**
 * gametest3d
 * @license The MIT License (MIT)
 *   @copyright Copyright (c) 2015 EngineerOfLies
 *    Permission is hereby granted, free of charge, to any person obtaining a copy
 *    of this software and associated documentation files (the "Software"), to deal
 *    in the Software without restriction, including without limitation the rights
 *    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the Software is
 *    furnished to do so, subject to the following conditions:
 *    The above copyright notice and this permission notice shall be included in all
 *    copies or substantial portions of the Software.
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *    SOFTWARE.
 */
#include "simple_logger.h"
#include "graphics3d.h"
#include "shader.h"
#include "obj.h"
#include "vector.h"
#include "sprite.h"
#include "entity.h"
#include <math.h>
#include <stdlib.h>

void set_camera(Vec3D position, Vec3D rotation);

extern float cSpeed;
extern float worldHeight;
extern float worldWidth;
extern float worldBack;
extern float worldFront;
extern Entity *player1;
extern int buttonDown;
extern int cBarrelRoll;
extern int cBarrelUse;
extern int powerLength;
int cTime = 0;      //I did say this was a random generation-type game
                    //so, instead of using my former technique of having 
int cUse = 1000;
Vec3D cameraPosition = {0, -12, 0};
Vec3D cameraRotation = {90,0,0};

int main(int argc, char *argv[])
{
    int i;
    float r = 0;
    char bGameLoopRunning = 1;
    SDL_Event e;
    Entity *testEn;
	Obj *enObj;
	Sprite *enText;
	int randomNum;

    init_logger("gametest3d.log");
    if (graphics3d_init(1024,768,1,"gametest3d",33) != 0)
    {
        return -1;
    }
    model_init();
    obj_init();
    entity_init(255);

	enObj = obj_load("models/cube.obj");
    enText = LoadSprite("models/mountain_text.png",1024,1024);

	mainInit();
    
	while (bGameLoopRunning)
    {
		cTime += 1;
		if(cSpeed < .1){
			cSpeed += .001;
		}
		if(cUse < 1000){
			cUse += 5;
		}
		if(cBarrelRoll < 200 && cBarrelUse == 0){
			cBarrelRoll += 1;
		}
		if(powerLength > 0){
			powerLength -= 1;
		}else{
			player1->power = P_NONE;
		}
		if(cSpeed < 1){
			switch(cTime % 200){
				case 50:
					randomNum = rand()%4;
					switch(randomNum){
						case W_ZIG:
							testEn = newWall(vec3d((rand() % (int)(worldWidth*2))-worldWidth,cameraPosition.y+60,(rand() % (int)(worldHeight*2))-worldHeight), "wall", obj_load("models/cube.obj"), LoadSprite("models/mountain_text.png",1024,1024), W_ZIG, rand()%4);
							break;
						case W_STRAIGHT:
							testEn = newWall(vec3d((rand() % (int)(worldWidth*2))-worldWidth,cameraPosition.y+60,(rand() % (int)(worldHeight*2))-worldHeight), "wall", obj_load("models/cube.obj"), LoadSprite("models/mountain_text.png",1024,1024), W_STRAIGHT, 0);
							break;
						case W_HORZ:
							testEn = newWall(vec3d((rand() % (int)(worldWidth*2))-worldWidth,cameraPosition.y+60,(rand() % (int)(worldHeight*2))-worldHeight), "wall", obj_load("models/cube.obj"), LoadSprite("models/mountain_text.png",1024,1024), W_HORZ, rand()%2);
							break;
						case W_VERT:
							testEn = newWall(vec3d((rand() % (int)(worldWidth*2))-worldWidth,cameraPosition.y+60,(rand() % (int)(worldHeight*2))-worldHeight), "wall", obj_load("models/cube.obj"), LoadSprite("models/mountain_text.png",1024,1024), W_VERT, rand()%2);
							break;
					}
					break;
				case 100:
					testEn = newship(vec3d((rand() % (int)(worldWidth*2))-worldWidth,cameraPosition.y+60,(rand() % (int)(worldHeight*2))-worldHeight), "ship", obj_load("models/cube.obj"), LoadSprite("models/cube_text.png",50,50));
					break;
				case 150:
					randomNum = rand()%4;
					switch(randomNum){
						case W_ZIG:
							testEn = newWall(vec3d((rand() % (int)(worldWidth*2))-worldWidth,cameraPosition.y+60,(rand() % (int)(worldHeight*2))-worldHeight), "wall", obj_load("models/cube.obj"), LoadSprite("models/mountain_text.png",1024,1024), W_ZIG, rand()%4);
							break;
						case W_STRAIGHT:
							testEn = newWall(vec3d((rand() % (int)(worldWidth*2))-worldWidth,cameraPosition.y+60,(rand() % (int)(worldHeight*2))-worldHeight), "wall", obj_load("models/cube.obj"), LoadSprite("models/mountain_text.png",1024,1024), W_STRAIGHT, 0);
							break;
						case W_HORZ:
							testEn = newWall(vec3d((rand() % (int)(worldWidth*2))-worldWidth,cameraPosition.y+60,(rand() % (int)(worldHeight*2))-worldHeight), "wall", obj_load("models/cube.obj"), LoadSprite("models/mountain_text.png",1024,1024), W_HORZ, rand()%2);
							break;
						case W_VERT:
							testEn = newWall(vec3d((rand() % (int)(worldWidth*2))-worldWidth,cameraPosition.y+60,(rand() % (int)(worldHeight*2))-worldHeight), "wall", obj_load("models/cube.obj"), LoadSprite("models/mountain_text.png",1024,1024), W_VERT, rand()%2);
							break;
					}
					break;
				case 175:
					switch(rand()%3){
						case 1:
							testEn = newPower(vec3d((rand() % (int)(worldWidth*2))-worldWidth,cameraPosition.y+160,(rand() % (int)(worldHeight*2))-worldHeight), "wall", obj_load("models/cube.obj"), LoadSprite("models/mountain_text.png",1024,1024), P_MINI);
							break;
						case 2:
							testEn = newPower(vec3d((rand() % (int)(worldWidth*2))-worldWidth,cameraPosition.y+160,(rand() % (int)(worldHeight*2))-worldHeight), "wall", obj_load("models/cube.obj"), LoadSprite("models/mountain_text.png",1024,1024), P_BOMB);
							break;
						default:
							testEn = newPower(vec3d((rand() % (int)(worldWidth*2))-worldWidth,cameraPosition.y+160,(rand() % (int)(worldHeight*2))-worldHeight), "wall", obj_load("models/cube.obj"), LoadSprite("models/mountain_text.png",1024,1024), P_INVERT);
							break;
					}
					break;
				default:
					break;
			}
		}else{
			switch(cTime % 60){
				case 15:
					randomNum = rand()%4;
					switch(randomNum){
						case W_ZIG:
							testEn = newWall(vec3d((rand() % (int)(worldWidth*2))-worldWidth,cameraPosition.y+160,(rand() % (int)(worldHeight*2))-worldHeight), "wall", obj_load("models/cube.obj"), LoadSprite("models/mountain_text.png",1024,1024), W_ZIG, rand()%4);
							break;
						case W_STRAIGHT:
							testEn = newWall(vec3d((rand() % (int)(worldWidth*2))-worldWidth,cameraPosition.y+160,(rand() % (int)(worldHeight*2))-worldHeight), "wall", obj_load("models/cube.obj"), LoadSprite("models/mountain_text.png",1024,1024), W_STRAIGHT, 0);
							break;
						case W_HORZ:
							testEn = newWall(vec3d((rand() % (int)(worldWidth*2))-worldWidth,cameraPosition.y+160,(rand() % (int)(worldHeight*2))-worldHeight), "wall", obj_load("models/cube.obj"), LoadSprite("models/mountain_text.png",1024,1024), W_HORZ, rand()%2);
							break;
						case W_VERT:
							testEn = newWall(vec3d((rand() % (int)(worldWidth*2))-worldWidth,cameraPosition.y+60,(rand() % (int)(worldHeight*2))-worldHeight), "wall", obj_load("models/cube.obj"), LoadSprite("models/mountain_text.png",1024,1024), W_VERT, rand()%2);
							break;
					}
					break;
				case 30:
					testEn = newship(vec3d((rand() % (int)(worldWidth*2))-worldWidth,cameraPosition.y+160,(rand() % (int)(worldHeight*2))-worldHeight), "ship", obj_load("models/cube.obj"), LoadSprite("models/cube_text.png",50,50));
					break;
				case 45:
					randomNum = rand()%4;
					switch(randomNum){
						case W_ZIG:
							testEn = newWall(vec3d((rand() % (int)(worldWidth*2))-worldWidth,cameraPosition.y+160,(rand() % (int)(worldHeight*2))-worldHeight), "wall", obj_load("models/cube.obj"), LoadSprite("models/mountain_text.png",1024,1024), W_ZIG, rand()%4);
							break;
						case W_STRAIGHT:
							testEn = newWall(vec3d((rand() % (int)(worldWidth*2))-worldWidth,cameraPosition.y+160,(rand() % (int)(worldHeight*2))-worldHeight), "wall", obj_load("models/cube.obj"), LoadSprite("models/mountain_text.png",1024,1024), W_STRAIGHT, 0);
							break;
						case W_HORZ:
							testEn = newWall(vec3d((rand() % (int)(worldWidth*2))-worldWidth,cameraPosition.y+160,(rand() % (int)(worldHeight*2))-worldHeight), "wall", obj_load("models/cube.obj"), LoadSprite("models/mountain_text.png",1024,1024), W_HORZ, rand()%2);
							break;
						case W_VERT:
							testEn = newWall(vec3d((rand() % (int)(worldWidth*2))-worldWidth,cameraPosition.y+160,(rand() % (int)(worldHeight*2))-worldHeight), "wall", obj_load("models/cube.obj"), LoadSprite("models/mountain_text.png",1024,1024), W_VERT, rand()%2);
							break;
					}
					break;
				case 59:
					switch(rand()%3){
						case 1:
							testEn = newPower(vec3d((rand() % (int)(worldWidth*2))-worldWidth,cameraPosition.y+160,(rand() % (int)(worldHeight*2))-worldHeight), "wall", obj_load("models/cube.obj"), LoadSprite("models/mountain_text.png",1024,1024), P_MINI);
							break;
						case 2:
							testEn = newPower(vec3d((rand() % (int)(worldWidth*2))-worldWidth,cameraPosition.y+160,(rand() % (int)(worldHeight*2))-worldHeight), "wall", obj_load("models/cube.obj"), LoadSprite("models/mountain_text.png",1024,1024), P_BOMB);
							break;
						default:
							testEn = newPower(vec3d((rand() % (int)(worldWidth*2))-worldWidth,cameraPosition.y+160,(rand() % (int)(worldHeight*2))-worldHeight), "wall", obj_load("models/cube.obj"), LoadSprite("models/mountain_text.png",1024,1024), P_INVERT);
							break;
					}
				default:
					break;
			}
		}
        entity_think_all();

        bGameLoopRunning = mainInput();

        graphics3d_frame_begin();
        
        glPushMatrix();
        set_camera(
            cameraPosition,
            cameraRotation);
        
        entity_draw_all();
        //glTranslatef(-5,0,0);
        glPushMatrix();
		glPopMatrix();
        glPopMatrix();
        /* drawing code above here! */
        graphics3d_next_frame();
    } 
    return 0;
}

void set_camera(Vec3D position, Vec3D rotation)
{
    glRotatef(-rotation.x, 1.0f, 0.0f, 0.0f);
    glRotatef(-rotation.y, 0.0f, 1.0f, 0.0f);
    glRotatef(-rotation.z, 0.0f, 0.0f, 1.0f);
    glTranslatef(-position.x,
                 -position.y,
                 -position.z);
}

/*eol@eof*/