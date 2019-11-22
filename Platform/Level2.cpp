#include "Level2.h"
#define LEVEL2_WIDTH 16
#define LEVEL2_HEIGHT 8

#define ENEMIES_COUNT 1
#define JOOMBA_COUNT 1
unsigned int level2_data[] =
{
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2,
 3, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2,
 3, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
};

Entity level2_enemies[ENEMIES_COUNT];
Entity AOE_dmg_sprites_2[JOOMBA_COUNT * 2];

void Level2::Initialize() {
	GLuint mapTextureID = Util::LoadTexture("tileset.png");
	state.map = new Map(LEVEL2_WIDTH, LEVEL2_HEIGHT, level2_data, mapTextureID, 1.0f, 4, 1);
	state.player.entityType = PLAYER;
	state.player.isStatic = false;
	state.player.width = 1.0f;
	state.player.position = glm::vec3(5, 0, 0);
	state.player.acceleration = glm::vec3(0, -9.81f, 0);
	state.player.textureID = Util::LoadTexture("jackson1.png");
	state.player.cols = 19;
	state.player.rows = 1;
	state.player.walkRight = new int[7]{ 1, 2, 3, 4, 5, 6, 7 };
	state.player.walkLeft = new int[7]{ 13, 14, 15, 16, 17, 18, 19 };
	state.player.jumpUp = new int[1]{ 10 };
	state.player.currentAnim = state.player.walkRight;
	state.player.animFrames = 6;

	state.enemies = level2_enemies;
	state.enemies[0].entityType = ENEMY;
	state.enemies[0].textureID = Util::LoadTexture("evil.png");
	state.enemies[0].acceleration = glm::vec3(0, -9.81f, 0);
	state.enemies[0].isStatic = false;
	state.enemies[0].position = glm::vec3(1, -2.25, 0);
	state.enemies[0].aiState = WALKING;
	state.enemies[0].aiType = JOOMBA;
	//state.enemies[0].aiType = PAPARAZZI;
	state.enemies[0].velocity = glm::vec3(1.0f, 0, 0);
	state.enemies[0].timer = 100.0f + rand() % 200 + 1;
	state.enemies[0].effects = AOE_dmg_sprites_2;
	state.enemies[0].effects_num = JOOMBA_COUNT * 2;

	state.nextLevel = -1;
}
void Level2::Update(float deltaTime) {
	state.player.Update(deltaTime, NULL, 0, state.map, state.enemies, ENEMIES_COUNT);
	for (int i = 0; i < ENEMIES_COUNT; ++i) {
		state.enemies[i].Update(deltaTime, &state.player, 1, state.map, state.enemies, ENEMIES_COUNT);
	}

	if (state.player.position.x >= 15)
	{
		state.nextLevel = 3;
	}
}
void Level2::Render(ShaderProgram* program) {
	state.map->Render(program);
	state.player.Render(program);
}