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
#include "mgl_callback.h"
#include "simple_logger.h"
#include "graphics3d.h"
#include "shader.h"
#include "obj.h"
#include "vector.h"
#include "sprite.h"
#include "entity.h"

void set_camera(Vec3D position, Vec3D rotation);

extern int cSpeed;
extern float worldHeight;
extern float worldWidth;
extern float worldBack;
extern float worldFront;
extern Entity *player;
int cTime = 0;      //I did say this was a random generation-type game
                    //so, instead of using my former technique of having 


int main(int argc, char *argv[])
{
    int i;
    float r = 0;
    char bGameLoopRunning = 1;
    Vec3D cameraPosition = {0,-10,.3};
    Vec3D cameraRotation = {90,0,0};
    SDL_Event e;
    Entity *testEn;
	Obj *bgobj;
	Sprite *bgtext;

    init_logger("gametest3d.log");
    if (graphics3d_init(1024,768,1,"gametest3d",33) != 0)
    {
        return -1;
    }
    model_init();
    obj_init();
    entity_init(255);

	bgobj = obj_load("models/cube.obj");
    bgtext = LoadSprite("models/mountain_text.png",1024,1024);

	testEn = newWall(vec3d(0,20,0), "wall", bgobj, bgtext, W_STRAIGHT);

	mainInit();
    
	while (bGameLoopRunning)
    {
        entity_think_all();
        while ( SDL_PollEvent(&e) ) 
        {
            if (e.type == SDL_QUIT)
            {
                bGameLoopRunning = 0;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_ESCAPE)
                {
                    bGameLoopRunning = 0;
                }
                else if (e.key.keysym.sym == SDLK_SPACE)
                {
                    
                }
                else if (e.key.keysym.sym == SDLK_z)
                {
                  
                }
				else if (e.key.keysym.sym == SDLK_x){
				
				}
                else if (e.key.keysym.sym == SDLK_w)
                {
                    player->body.position.z += 0.2;
                }
                else if (e.key.keysym.sym == SDLK_s)
                {
                    player->body.position.z -= 0.2;
                }
                else if (e.key.keysym.sym == SDLK_d)
                {
                    player->body.position.x += 0.2;
                }
                else if (e.key.keysym.sym == SDLK_a)
                {
                   player->body.position.x -= 0.2;
				}
            }
        }

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