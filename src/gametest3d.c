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
int cTime = 0;      //I did say this was a random generation-type game
                    //so, instead of using my former technique of having 
Vec3D cameraPosition = {0, -15, 0};
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
		if(cSpeed < 0.1){
			cSpeed += .001;
		}
		switch(cTime % 100){
			/*case 0:
				randomNum = rand()%4;
				switch(randomNum){
					case W_ZIG:
						testEn = newWall(vec3d((rand() % (int)(worldWidth*2))-worldWidth,50,(rand() % (int)(worldHeight*2))-worldHeight), "wall", obj_load("models/cube.obj"), LoadSprite("models/mountain_text.png",1024,1024), W_ZIG);
						break;
					case W_STRAIGHT:
						testEn = newWall(vec3d((rand() % (int)(worldWidth*2))-worldWidth,50,(rand() % (int)(worldHeight*2))-worldHeight), "wall", obj_load("models/cube.obj"), LoadSprite("models/mountain_text.png",1024,1024), W_STRAIGHT);
						break;
					case W_HORZ:
						testEn = newWall(vec3d((rand() % (int)(worldWidth*2))-worldWidth,50,(rand() % (int)(worldHeight*2))-worldHeight), "wall", obj_load("models/cube.obj"), LoadSprite("models/mountain_text.png",1024,1024), W_HORZ);
						break;
					case W_VERT:
						testEn = newWall(vec3d((rand() % (int)(worldWidth*2))-worldWidth,50,(rand() % (int)(worldHeight*2))-worldHeight), "wall", obj_load("models/cube.obj"), LoadSprite("models/mountain_text.png",1024,1024), W_VERT);
						break;
				}
				break;*/
			case 50:
				break;
			/*case 75:
				randomNum = rand()%4;
				switch(randomNum){
					case W_ZIG:
						testEn = newWall(vec3d((rand() % (int)(worldWidth*2))-worldWidth,50,(rand() % (int)(worldHeight*2))-worldHeight), "wall", obj_load("models/cube.obj"), LoadSprite("models/mountain_text.png",1024,1024), W_ZIG);
						break;
					case W_STRAIGHT:
						testEn = newWall(vec3d((rand() % (int)(worldWidth*2))-worldWidth,50,(rand() % (int)(worldHeight*2))-worldHeight), "wall", obj_load("models/cube.obj"), LoadSprite("models/mountain_text.png",1024,1024), W_STRAIGHT);
						break;
					case W_HORZ:
						testEn = newWall(vec3d((rand() % (int)(worldWidth*2))-worldWidth,50,(rand() % (int)(worldHeight*2))-worldHeight), "wall", obj_load("models/cube.obj"), LoadSprite("models/mountain_text.png",1024,1024), W_HORZ);
						break;
					case W_VERT:
						testEn = newWall(vec3d((rand() % (int)(worldWidth*2))-worldWidth,50,(rand() % (int)(worldHeight*2))-worldHeight), "wall", obj_load("models/cube.obj"), LoadSprite("models/mountain_text.png",1024,1024), W_VERT);
						break;
				}
				break;*/
			default:
				break;
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