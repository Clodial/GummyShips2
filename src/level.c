#include "level.h"
#include "graphics3d.h"
#include "body.h"
#include "shader.h"
#include "obj.h"
#include "vector.h"
#include "sprite.h"
#include "entity.h"
#include "simple_logger.h"
#include "gametest3d.h"
#include <stdio.h>
#include <stdlib.h>

extern int score;
extern float worldWidth;
extern float worldHeight;
extern Vec3D cameraPosition;

FILE *level1Edit = NULL;
FILE *level2Edit = NULL;
FILE *level3Edit = NULL;
FILE *level4Edit = NULL;
FILE *level5Edit = NULL;
FILE *hiScore = NULL;

int highScore;

int *lvlSect[5][5];		// this is the array that'll get updated periodically throughout the game

int *defLvl[5][5] = {
	{ 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0 }
};

void levelInit(){
	
	int i, j; // This is to write the default settings into the game

	//loading/creating the files
	if (level1Edit == NULL){
		level1Edit = fopen("files/string1.config", "r");
		if (level1Edit == NULL){
			level1Edit = fopen("files/string1.config", "w");
			if (level1Edit != NULL){
				for (i = 0; i < 5; i++){ //Y-axis
					for (j = 0; j < 5; j++){ //X-axis
						fprintf(level1Edit, "0\n");
					}
				}
				fprintf(level1Edit, "100");
				fclose(level1Edit);
				level1Edit = NULL;
			}
		}
	}
	if (level2Edit == NULL){
		level2Edit = fopen("files/string2.config", "r");
		if (level2Edit == NULL){
			level2Edit = fopen("files/string2.config", "w");
			if (level2Edit != NULL){
				for (i = 0; i < 5; i++){ //Y-axis
					for (j = 0; j < 5; j++){ //X-axis
						fprintf(level2Edit, "0\n");
					}
				}
				fprintf(level2Edit, "100");
				fclose(level2Edit);
				level2Edit = NULL;
			}
		}
	}
	if (level3Edit == NULL){
		level3Edit = fopen("files/string3.config", "r");
		if (level3Edit == NULL){
			level3Edit = fopen("files/string3.config", "w");
			if (level3Edit != NULL){
				for (i = 0; i < 5; i++){ //Y-axis
					for (j = 0; j < 5; j++){ //X-axis
						fprintf(level3Edit, "0\n");
					}
				}
				fprintf(level3Edit, "100");
				fclose(level3Edit);
				level2Edit = NULL;
			}
		}
	}
	if (level4Edit == NULL){
		level4Edit = fopen("files/string4.config", "r");
		if (level4Edit == NULL){
			level4Edit = fopen("files/string4.config", "w");
			if (level4Edit != NULL){
				for (i = 0; i < 5; i++){ //Y-axis
					for (j = 0; j < 5; j++){ //X-axis
						fprintf(level4Edit, "0\n");
					}
				}
				fprintf(level4Edit, "100");
				fclose(level4Edit);
				level2Edit = NULL;
			}
		}
	}
	if (level5Edit == NULL){
		level5Edit = fopen("files/string5.config", "r");
		if (level5Edit == NULL){
			level5Edit = fopen("files/string5.config", "w");
			if (level5Edit != NULL){
				for (i = 0; i < 5; i++){ //Y-axis
					for (j = 0; j < 5; j++){ //X-axis
						fprintf(level5Edit, "0\n");
					}
				}
				fprintf(level5Edit, "100");
				fclose(level5Edit);
				level2Edit = NULL;
			}
		}
	}
	
}

void closeLevels(){

	if (level1Edit != NULL){
		fclose(level1Edit);
		level1Edit = NULL;
	}
	if (level2Edit != NULL){
		fclose(level2Edit);
		level2Edit = NULL;
	}
	if (level3Edit != NULL){
		fclose(level3Edit);
		level3Edit = NULL;
	}
	if (level4Edit != NULL){
		fclose(level4Edit);
		level4Edit = NULL;
	}
	if (level5Edit != NULL){
		fclose(level5Edit);
		level5Edit = NULL;
	}
}

void randomPick(){

	int r = rand() % 5;
	switch (r){
		case 0:
			loadLevel("files/string1.config");
			break;
		case 1:
			loadLevel("files/string2.config");
			break;
		case 2:
			loadLevel("files/string3.config");
			break;
		case 3:
			loadLevel("files/string4.config");
			break;
		default:
			loadLevel("files/string5.config");
			break;
	}

}

void loadLevel(char *fName){	

	//int iSect[5][5];
	int i = 0;
	int j = 0;
	int c = 25;
	int scItem;
	int ch;
	char line[256];
	FILE *fFile;

	fFile = fopen(fName, "r");
	slog("%s", fName);
	if (fFile != NULL){
		while (fgets(line, 256, fFile)){
			sscanf(line, "%i", &ch);
			if ((i < 5 || j < 4) && c > 0){
				c--;
				switch (ch){
					case 0:
						lvlSect[j][i] = 0;
						break;
					case 1:
						lvlSect[j][i] = 1;
						break;
					case 2:
						lvlSect[j][i] = 2;
						break;
					case 3:
						lvlSect[j][i] = 3;
						break;
				}
				slog("%i", lvlSect[j][i]);
				if (i < 4){
					i++;
				}
				else if(j < 4){
					j++;
					i = 0;
				}
			}
			else{
				sscanf(line, "%i", &scItem);
			}
		}
		createLevel(scItem);
		fclose(fFile);
	}
	//memcpy(*lvlSect, *iSect, sizeof(iSect));

}

void createLevel(int sc){

	int i, j, c;
	//int iSect[5][5];
	int randomNum;
	Entity *testEn;
	char enText[] = "models/red_piece.png";
	char shipText[] = "models/yell_piece.png";
	char powText[] = "models/green_piece.png";
	char blText[] = "models/orange_piece.png";
	char enObj[] = "models/cube.obj";
	
	//memcpy(iSect, *lvlSect, sizeof(lvlSect));

	testEn = newScorer(vec3d(100, cameraPosition.y + 60, 100),sc);
	for (i = 0; i < 5; i++){ //Y-Axis
		for (j = 0; j < 5; j++){ //X-Axis
			c = lvlSect[i][j];
			switch (c){
				case 0:
					//nothing
					//testEn = newship(vec3d((worldWidth / 2) * ((float)j - 2), cameraPosition.y + 60, (worldHeight / 2) * ((float)i - 2)), "ship", obj_load("models/cube.obj"), LoadSprite(shipText, 1024, 1024));
					break;
				case 1:
					//ships
					testEn = newship(vec3d(worldWidth/2 * (j - 2.5), cameraPosition.y + 60, worldHeight/2 * (i - 2.5)), "ship", obj_load(enObj), LoadSprite(shipText, 1024, 1024));
					break;
				case 2:
					//powerups
					switch (rand() % 3){
						case 1:
							testEn = newPower(vec3d(worldWidth / 2 * (j - 2.5), cameraPosition.y + 60, worldHeight / 2 * (i - 2.5)), "power", obj_load(enObj), LoadSprite(powText, 1024, 1024), P_MINI);
							break;
						case 2:
							testEn = newPower(vec3d(worldWidth / 2 * (j - 2.5), cameraPosition.y + 60, worldHeight / 2 * (i - 2.5)), "power", obj_load(enObj), LoadSprite(powText, 1024, 1024), P_BOMB);
							break;
						default:
							testEn = newPower(vec3d(worldWidth / 2 * (j - 2.5), cameraPosition.y + 60, worldHeight / 2 * (i - 2.5)), "power", obj_load(enObj), LoadSprite(powText, 1024, 1024), P_INVERT);
							break;
					}
					break;
				case 3:
					//walls
					switch (rand() % 4){
						case 0:
							testEn = newWall(vec3d(worldWidth / 2 * (j - 2.5), cameraPosition.y + 60, worldHeight / 2 * (i - 2.5)), "wall", obj_load(enObj), LoadSprite(enText, 1024, 1024), W_STRAIGHT, 0);
							break;
						case 1:
							testEn = newWall(vec3d(worldWidth / 2 * (j - 2.5), cameraPosition.y + 60, worldHeight / 2 * (i - 2.5)), "wall", obj_load(enObj), LoadSprite(enText, 1024, 1024), W_HORZ, rand() % 2);
							break;
						case 2:
							testEn = newWall(vec3d(worldWidth / 2 * (j - 2.5), cameraPosition.y + 60, worldHeight / 2 * (i - 2.5)), "wall", obj_load(enObj), LoadSprite(enText, 1024, 1024), W_ZIG, rand() % 4);
							break;
						default:
							testEn = newWall(vec3d(worldWidth / 2 * (j - 2.5), cameraPosition.y + 60, worldHeight / 2 * (i - 2.5)), "wall", obj_load(enObj), LoadSprite(enText, 1024, 1024), W_VERT, rand() % 2);
							break;
					}
					break;
			}
		}
	}

}

void getHighscore(){
	
	char line[256];

	hiScore = fopen("files/score.config", "r");
	if (hiScore != NULL){
		while (fgets(line, 256, hiScore)){
			sscanf(line, "%i", &highScore);
			slog("%i", highScore);
		}
		fclose(hiScore);
	}
	else{
		return;
	}

}

void writeHighScore(int sc){

	if (sc > highScore){
		hiScore = fopen("files/score.config", "w");
		fprintf(hiScore, "%i", sc);
		fclose(hiScore);
	}

}