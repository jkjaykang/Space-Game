#include "Level1.h"
#define LEVEL1_WIDTH 49
#define LEVEL1_HEIGHT 8

#define ENEMIES_COUNT 3
//#define JOOMBA_COUNT 1
#define HAZARD_COUNT 3


unsigned int level1_data[] =
{
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2
};

Entity level1_enemies[ENEMIES_COUNT];
Entity level1_hazards[HAZARD_COUNT];

Entity sword;


void Level1::Initialize() {
    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 4, 1);
    state.player.entityType = PLAYER;
    state.player.isStatic = false;
    
    state.player.width = 0.8f;
    
    state.player.position = glm::vec3(6, 0, 0);
    state.player.acceleration = glm::vec3(0, -3.711f, 0);
    state.player.textureID = Util::LoadTexture("temp.png");
    state.player.cols = 16;
    state.player.rows = 8;
    state.player.idleRight = new int[8]{ 1,2,3,4,5,6,7,8 };
    state.player.walkRight = new int[8]{ 9,10,11,12,13,14,15,16};
    state.player.idleLeft = new int[8]{ 17,18,19,20,21,22,23,24 };
    state.player.walkLeft = new int[8]{ 25,26,27,28,29,30,31,32 };
    state.player.jumpDownRight = new int[8]{ 33,34,35,36,37,38,39,40 };
    //state.player.jumpDownRight = new int[8]{ 41,42,43,44,45,46,47,48};
    state.player.jumpDownLeft = new int[8]{ 49,50,51,52,53,54,55,56 };
    //state.player.jumpDownRight = new int[8]{ 57,58,59,60,61,62,63,64 };
    state.player.jumpUpRight = new int[8]{ 65, 66, 67, 68, 69, 70, 71, 72 };
    //    state.player.jumpDownRight = new int[8]{ 73,74,75,76,77,78,79,80 };
    state.player.jumpUpLeft = new int[8]{ 81, 82, 83, 84, 85, 86, 87, 88 };
    //state.player.jumpUpLeft = new int[8]{ 89, 90, 91, 92, 93, 94, 95, 96 };
    state.player.runRight = new int[6]{ 97,98,99,100,101,102 };
    //state.player.jumpUpLeft = new int[10]{ 103, 104, 105, 106, 107, 108, 109, 110, 111,112 };
    state.player.runLeft = new int[6]{ 113,114,115,116,117,118 };
    
    state.player.currentAnim = state.player.idleRight;
    state.player.animFrames = 8;
    state.nextLevel = -1;
    
    sword.entityType = SWORD;
    sword.isActive = false;
    sword.isStatic = false;
    sword.position = state.player.position;
    sword.position.x += 0.5f;
    sword.timer = 25.0f;
    sword.textureID = Util::LoadTexture("papa.png");
    

    
    state.enemies = level1_enemies;
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].textureID = Util::LoadTexture("fly.png");
    state.enemies[0].acceleration = glm::vec3(0, 0, 0);
    state.enemies[0].isStatic = false;
    state.enemies[0].position = glm::vec3(3, -0.35f, 0);
    state.enemies[0].aiState = IDLE;
    state.enemies[0].aiType = FLY;
    state.enemies[0].velocity = glm::vec3(0, -1, 0);
    state.enemies[0].initialPosition = glm::vec3(3, -0.35f, 0);
    
    state.enemies[1].entityType = ENEMY;
    state.enemies[1].textureID = Util::LoadTexture("fly.png");
    state.enemies[1].acceleration = glm::vec3(0, -9.8f, 0);
    state.enemies[1].isStatic = false;
    state.enemies[1].position = glm::vec3(13.0f, -2.25f, 0);
    state.enemies[1].aiState = WALKING;
    state.enemies[1].aiType = SPIKER;
    state.enemies[1].velocity = glm::vec3(5.0f, 0, 0);
    state.enemies[1].timer = 35.0f;
    //state.enemies[1].initialPosition = glm::vec3(3, -2.25, 0);
    
    state.enemies[2].entityType = ENEMY;
    state.enemies[2].textureID = Util::LoadTexture("me.png");
    state.enemies[2].acceleration = glm::vec3(0, -9.8f, 0);
    state.enemies[2].isStatic = false;
    state.enemies[2].position = glm::vec3(15.0f, -2.25f, 0);
    state.enemies[2].aiState = WALKING;
    state.enemies[2].aiType = GUNNER;
    state.enemies[2].velocity = glm::vec3(5.0f, 0, 0);
    state.enemies[2].timer = 50.0f;
    //state.enemies[1].initialPosition = glm::vec3(3, -2.25, 0);
    
    state.hazards = level1_hazards;
    state.hazards[0].entityType = HAZARD;
    state.hazards[0].hzType = BOMB;
    state.hazards[0].hzState = DEPLOY;
    state.hazards[0].textureID = Util::LoadTexture("bomb.png");
    state.hazards[0].rows = 3;
    state.hazards[0].cols = 4;
    state.hazards[0].idle = new int[4]{1, 2, 3, 4};
    state.hazards[0].exploding = new int[4]{5, 6, 7, 8};
    state.hazards[0].ticking = new int[4]{ 9, 10, 11, 12 };
    state.hazards[0].acceleration = glm::vec3(0, -9.81f, 0);
    state.hazards[0].isStatic = false;
    state.hazards[0].isActive = false;
    state.hazards[0].position = glm::vec3(3, -2.25, 0);
    state.hazards[0].velocity = glm::vec3(0, -1, 0);
    state.hazards[0].initialPosition = glm::vec3(3, -2.25, 0);
    
    state.hazards[0].timer = 3.0f;
    
    state.hazards[0].currentAnim = state.hazards[0].idle;
    state.hazards[0].animFrames = 3;
    state.hazards[0].animIndex = 0;
    
    state.hazards[1].entityType = HAZARD;
    state.hazards[1].hzType = SPIKE;
    state.hazards[1].hzState = TICKING;
    state.hazards[1].textureID = Util::LoadTexture("spike.png");
    state.hazards[1].acceleration = glm::vec3(0, -9.81f, 0);
    state.hazards[1].isStatic = false;
    state.hazards[1].isActive = false;
    state.hazards[1].position = glm::vec3(3, -2.25, 0);
    state.hazards[1].velocity = glm::vec3(0, -1, 0);
    //state.hazards[1].initialPosition = glm::vec3(3, -2.25, 0);
    state.hazards[1].timer = 5.0f;
    
    state.hazards[1].rows = 3;
    state.hazards[1].cols = 4;
    state.hazards[1].idle = new int[4]{ 9, 10, 11, 12 };
    state.hazards[1].exploding = new int[4]{ 1, 2, 3, 4 };
    state.hazards[1].ticking = new int[4]{ 5, 6, 7, 8 };
    
    state.hazards[1].currentAnim = state.hazards[1].idle;
    state.hazards[1].animFrames = 3;
    state.hazards[1].animIndex = 0;
    
    state.hazards[2].entityType = HAZARD;
    state.hazards[2].hzType = LASER;
    state.hazards[2].hzState = EXPLODE;
    state.hazards[2].textureID = Util::LoadTexture("bullet.png");
    state.hazards[2].acceleration = glm::vec3(0, 0, 0);
    state.hazards[2].isStatic = false;
    state.hazards[2].isActive = false;
    state.hazards[2].position = glm::vec3(3, -2.25, 0);
    state.hazards[2].velocity = glm::vec3(0, 0, 0);
    //state.hazards[2].initialPosition = glm::vec3(3, -2.25, 0);

    state.hazards[2].timer = 10.0f;
    state.hazards[2].width = 0.4f;
    state.hazards[2].height = 0.4f;
}

void Level1::Update(float deltaTime) {
    state.player.Update(deltaTime, NULL, 0, state.hazards, HAZARD_COUNT, state.map, state.enemies, ENEMIES_COUNT);
    sword.Update(deltaTime, &(state.player), 1, state.hazards, HAZARD_COUNT, state.map, state.enemies, ENEMIES_COUNT);
    for (int i = 0; i < ENEMIES_COUNT; ++i) {
        //state.enemies[i].Update(deltaTime, &state.player, 1, state.map, state.enemies, ENEMIES_COUNT);
        state.enemies[i].Update(deltaTime, &state.player, 1, state.hazards, HAZARD_COUNT, state.map, state.enemies, ENEMIES_COUNT);
    }
    for (int i = 0; i < HAZARD_COUNT; ++i) {
        //AOE_dmg_sprites[i].Update(deltaTime, &state.player, 1, state.map, state.enemies, ENEMIES_COUNT);
        state.hazards[i].Update(deltaTime, &state.player, 1, state.hazards, HAZARD_COUNT, state.map, state.enemies, ENEMIES_COUNT);
    }
    /*if (state.player.position.x > 23) {
     state.nextLevel = 2;
     }*/
}
void Level1::Render(ShaderProgram* program) {
    state.map->Render(program);
    state.player.Render(program);
    sword.Render(program);
    
    
    for (int i = 0; i < ENEMIES_COUNT; ++i) {
        if (state.enemies[i].isActive) {
            state.enemies[i].Render(program);
        }
        
    }
    
    for (int i = 0; i < HAZARD_COUNT; ++i) {
        if (state.hazards[i].isActive) {
            state.hazards[i].Render(program);
        }
        
    }
    
    
}
