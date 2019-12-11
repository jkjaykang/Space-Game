#include "Level1.h"
#define LEVEL1_WIDTH 70
#define LEVEL1_HEIGHT 8

#define ENEMIES_COUNT 4
//#define JOOMBA_COUNT 1
#define HAZARD_COUNT 15


unsigned int level1_data[] =
{
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,2,0,0,0,0,0,0,0,0,0,0,0,0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,1,2,0,0,0,0,0,0,0,1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,2,0,0,0,0,0,0,0,0,0,0,0,0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,1,2,2,0,0,0,0,0,0,1,2,2,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,1,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,
    3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,2,2,2,1,1,1,1,1,1,2,2,2,2,2,1,1,1,1,1,0,1,1,1,0,1,1,1,1,1,2,2,2,1,1,1,1,1,1,1,1,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2
};

Entity level1_enemies[ENEMIES_COUNT];
Entity level1_hazards[HAZARD_COUNT];



void Level1::Initialize() {
    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 4, 1);
    state.player.entityType = PLAYER;
    state.player.isStatic = false;
    
    state.player.height = 1.0f;
    state.player.width = 0.5f;
    
    state.player.position = glm::vec3(1, -2, 0);
    state.player.acceleration = glm::vec3(0, -6.81f, 0);
    state.player.textureID = Util::LoadTexture("temp.png");
    state.player.cols = 16;
    state.player.rows = 8;
    state.player.idleRight = new int[8]{ 1,2,3,4,5,6,7,8 };
    state.player.walkRight = new int[8]{ 9,10,11,12,13,14,15,16 };
    state.player.idleLeft = new int[8]{ 17,18,19,20,21,22,23,24 };
    state.player.walkLeft = new int[8]{ 25,26,27,28,29,30,31,32 };
    state.player.jumpDownRight = new int[8]{ 33,34,35,36,37,38,39,40 };
    state.player.attackLeft = new int[5]{ 41,42,43,44,45 };
    state.player.jumpDownLeft = new int[8]{ 49,50,51,52,53,54,55,56 };
    state.player.attack = new int[5]{ 57,58,59,60,61 };
    state.player.jumpUpRight = new int[8]{ 65, 66, 67, 68, 69, 70, 71, 72 };
    state.player.jumpUpLeft = new int[8]{ 81, 82, 83, 84, 85, 86, 87, 88 };
    state.player.runRight = new int[6]{ 97,98,99,100,101,102 };
    state.player.runLeft = new int[6]{ 113,114,115,116,117,118 };
    
    state.player.currentAnim = state.player.idleRight;
    state.player.animFrames = 8;
    state.nextLevel = -1;
    
    state.sword.entityType = SWORD;
    state.sword.isActive = false;
    state.sword.isStatic = false;
    state.sword.position = state.player.position;
    state.sword.position.x += 0.5f;
    state.sword.timer = 25.0f;
    state.sword.textureID = Util::LoadTexture("slash.png");
    state.sword.cols = 6;
    state.sword.rows = 2;
    state.sword.attack = new int[6]{ 1,2,3,4,5,6 };
    state.sword.attackLeft = new int[6]{ 7,8,9,10,11,12 };
    state.sword.currentAnim = state.sword.attack;
    state.sword.animFrames = 5;
    
    
    
    state.enemies = level1_enemies;
    
    //state.enemies[0] = Entity("ENEMY", "FLY", glm::vec3(3, -0.35f, 0));
    //state.enemies[1] = Entity("ENEMY", "SPIKER", glm::vec3(13.0f, -3.25f, 0));
    //state.enemies[2] = Entity("ENEMY", "GUNNER", glm::vec3(15.0f, -3.25f, 0));
    
    state.enemies[0] = Entity("ENEMY", "BOSS", glm::vec3(8.0f, -3.25f, 0));
    /*
    state.enemies[1] = Entity("ENEMY", "GUNNER", glm::vec3(20.0f, -3.25f, 0));
    
    state.enemies[2] = Entity("ENEMY", "SPIKER", glm::vec3(34.0f, -3.25f, 0));
    
    state.enemies[3] = Entity("ENEMY", "FLY", glm::vec3(16.0f, -0.35f, 0));
     */
    //state.enemies[3] = Entity("ENEMY", "SPIKER", glm::vec3(2.0f, -3.25f, 0));
    
    state.hazards = level1_hazards;
    
    //state.hazards[0] = Entity("HAZARD", "BOMB", glm::vec3(3, -2.25, 0));
    //state.hazards[1] = Entity("HAZARD", "SPIKE", glm::vec3(3, -2.25, 0));
    //state.hazards[2] = Entity("HAZARD", "LASER", glm::vec3(3, -2.25, 0));
    
    
    state.hazards[0] = Entity("HAZARD", "LASER", glm::vec3(3, -2.25, 0));
    state.hazards[1] = Entity("HAZARD", "LASER", glm::vec3(3, -2.25, 0));
    state.hazards[2] = Entity("HAZARD", "LASER", glm::vec3(3, -2.25, 0));
    state.hazards[3] = Entity("HAZARD", "LASER", glm::vec3(3, -2.25, 0));
    state.hazards[4] = Entity("HAZARD", "LASER", glm::vec3(3, -2.25, 0));
    state.hazards[5] = Entity("HAZARD", "LASER", glm::vec3(3, -2.25, 0));
    state.hazards[6] = Entity("HAZARD", "SPIKE", glm::vec3(3, -2.25, 0));
    state.hazards[7] = Entity("HAZARD", "SPIKE", glm::vec3(3, -2.25, 0));
    state.hazards[8] = Entity("HAZARD", "SPIKE", glm::vec3(3, -2.25, 0));
    state.hazards[9] = Entity("HAZARD", "SPIKE", glm::vec3(3, -2.25, 0));
    state.hazards[10] = Entity("HAZARD", "BOMB", glm::vec3(3, -2.25, 0));
    
    state.hazards[11] = Entity("HAZARD", "CAN", glm::vec3(3, -2.25, 0));
    state.hazards[12] = Entity("HAZARD", "CAN", glm::vec3(3, -2.25, 0));
    state.hazards[13] = Entity("HAZARD", "CAN", glm::vec3(3, -2.25, 0));
    state.hazards[14] = Entity("HAZARD", "CAN", glm::vec3(3, -2.25, 0));
    
    
    //state.enemies[0].hazardList.push_back(&state.hazards[0]);
    //state.enemies[0].hazardList.push_back(&state.hazards[1]);
    //state.enemies[0].hazardList.push_back(&state.hazards[2]);
    
    
    
    state.enemies[0].hazardList.push_back(&state.hazards[11]);
    state.enemies[0].hazardList.push_back(&state.hazards[12]);
    state.enemies[0].hazardList.push_back(&state.hazards[13]);
    state.enemies[0].hazardList.push_back(&state.hazards[14]);
    
    
    /*
    //state.enemies[0].hazardList.push_back(&state.hazards[0]);
    //state.enemies[0].hazardList.push_back(&state.hazards[1]);
    //state.enemies[0].hazardList.push_back(&state.hazards[2]);
    /*
    state.enemies[1].hazardList.push_back(&state.hazards[3]);
    state.enemies[1].hazardList.push_back(&state.hazards[4]);
    state.enemies[1].hazardList.push_back(&state.hazards[5]);
    
    state.enemies[2].hazardList.push_back(&state.hazards[6]);
    state.enemies[2].hazardList.push_back(&state.hazards[7]);
    
    state.enemies[3].hazardList.push_back(&state.hazards[10]);
    //state.enemies[3].hazardList.push_back(&state.hazards[9]);
    */
    
}

void Level1::Update(float deltaTime) {
    state.player.Update(deltaTime, NULL, 0, state.hazards, HAZARD_COUNT, state.map, state.enemies, ENEMIES_COUNT, &state.sword, &state.player);
    state.sword.Update(deltaTime, &(state.player), 1, state.hazards, HAZARD_COUNT, state.map, state.enemies, ENEMIES_COUNT, &state.sword, &state.player);
    for (int i = 0; i < ENEMIES_COUNT; ++i) {
        state.enemies[i].Update(deltaTime, &state.player, 1, state.hazards, HAZARD_COUNT, state.map, state.enemies, ENEMIES_COUNT, &state.sword, &state.player);
    }
    for (int i = 0; i < HAZARD_COUNT; ++i) {
        state.hazards[i].Update(deltaTime, &state.player, 1, state.hazards, HAZARD_COUNT, state.map, state.enemies, ENEMIES_COUNT, &state.sword, &state.player);
    }
}
void Level1::Render(ShaderProgram* program) {
    state.map->Render(program);
    state.player.Render(program);
    state.sword.Render(program);
    
    
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
