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
#include "body.h"
#include "shader.h"
#include "obj.h"
#include "vector.h"
#include "sprite.h"
#include "entity.h"
#include "level.h"
#include "text.h"
#include <math.h>
#include <stdlib.h>

void set_camera(Vec3D position, Vec3D rotation);

extern float cSpeed;
extern float worldHeight;
extern float worldWidth;
extern float worldBack;
extern float worldFront;
extern Entity *player1;
extern Entity *editUser;
extern int buttonDown;
extern int cBarrelRoll;
extern int cBarrelUse;
extern int powerLength;
extern int highScore;

int editX = 0;
int editY = 0;
int m = 0;
int gameState = 0;
int gamePause = 0;
int cTime = 0;      //I did say this was a random generation-type game
                    //so, instead of using my former technique of having 
int val = 200;
int cUse = 1000;
int score = 0;
Vec3D cameraPosition = {0, -12, 0};
Vec3D cameraRotation = {90,0,0};

int main(int argc, char *argv[])
{

    int i;
	int makePlayer = 0;		//This is so I can have the playable character come up on the main game state without over making itself
    float r = 0;
	Entity *testEn;
    char bGameLoopRunning = 1;
    SDL_Event e;
	Obj *enObj;
	char enText[] = "models/red_piece.png";
	char shipText[] = "models/yell_piece.png";
	char powText[] = "models/green_piece.png";
	char blText[] = "models/black_piece.png";
	int randomNum;

	//menu state variables
	int cursor = 0;

	//tutorial state variables
	int tutSection = 0;

    init_logger("gametest3d.log");
    if (graphics3d_init(768,768,1,"gametest3d",33, gameState) != 0)
    {
        return -1;
    }
    model_init();
    obj_init();
    entity_init(255);
	levelInit();
	getHighscore();
	val = 200;

	while (bGameLoopRunning)
    {
		if (gameState == 1){
			m = 0;
			if (makePlayer == 0){
				cameraPosition = vec3d(0, -12, 0 );
				mainInit();
				makePlayer += 1;
				cSpeed = .001;
			}
			if (gamePause == 0){
				cTime += 1;
				if (cSpeed < 1){
					cSpeed += .001;
				}
				if (val > 50){
					val--;
				}
				if (cUse < 1000){
					cUse += 5;
				}
				if (cBarrelRoll < 200 && cBarrelUse == 0){
					cBarrelRoll += 1;
				}
				if (powerLength > 0){
					powerLength -= 1;
				}
				else{
					player1->power = P_NONE;
				}
				switch (cTime % val){
					case 1:
						randomPick();
						break;
				}
				entity_think_all();
			}
		}
		else if (gameState == 0){
			if (makePlayer){
				entityClearList();
				makePlayer = 0;
			}
			if (m == 0){
				entityClearList();
				editUser = newMover(vec3d(worldWidth / 2 * (-2), cameraPosition.y + 10, worldHeight / 2 * (-2)), "edit", LoadSprite("models/orange_piece.png", 1024, 1024));
				editX = 0;
				editY = 0;
				createLvlEdit();
				m = 1;
			}
		}
		else if (gameState == 2){
			if (makePlayer || m == 1){
				entityClearList();
				makePlayer = 0;
				m = 0;
			}
			//Tutorial Game State
			//(Separated from Menu Game State due to reading files regarding the tutorial)
		}
		else if (gameState == 3){
			if (makePlayer){
				entityClearList();
				makePlayer = 0;
			}
			if (m == 0){
				entityClearList();
				editUser = newMover(vec3d(worldWidth / 2 * (-2), cameraPosition.y + 10, worldHeight / 2 * (-2)), "edit", LoadSprite("models/orange_piece.png", 1024, 1024));
				editX = 0;
				editY = 0;
				createLvlEdit();
				m = 1;
			}
		}
		bGameLoopRunning = mainInput();

		graphics3d_frame_begin(gameState);

		glPushMatrix();
		set_camera(
			cameraPosition,
			cameraRotation);
		entity_draw_all();
		glPushMatrix();
		//text_draw2("Score", vec2d(0.1f, 0.94f), vec2d(0.08f, -0.08f));
		glPopMatrix();
		//text_draw2("Score", vec2d(0.1f, 0.94f), vec2d(0.08f, -0.08f));
		glPopMatrix();
		/* drawing code above here! */
		graphics3d_next_frame(gameState);
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