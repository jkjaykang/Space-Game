#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_mixer.h>

#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#include "Map.h"
//enum  EntityType { PLAYER, PLATFORM, ENEMY, AOE_DAMAGE, LIFE, HAZARD };
enum  EntityType { PLAYER, PLATFORM, ENEMY, AOE_DAMAGE, LIFE, SWORD, HAZARD };
//enum  lastCollisionEntityType { PLAYER, PLATFORM, ENEMY, BULLET, LETTER};
enum AIState { IDLE, WALKING, RUNNING, SLOW_DOWN, AOE, ATTACK };
enum AIType { PAPARAZZI, JOOMBA, SPIKER, GUNNER, FLY, BOSS };
enum HazardState {DEPLOY, TICKING, EXPLODE};
enum HazardType {BOMB, SPIKE, LASER};


class Entity {
public:
    
    EntityType entityType;
    EntityType lastCollisionType;
    AIState aiState;
    AIType aiType;
    HazardType hzType;
    HazardState hzState;
    
    //Entity* hazards;
    //int hazards_num;
    GLuint textureID;
    
    Mix_Chunk* footstep;
	Mix_Chunk* slash;
	Mix_Chunk* jump;
    
    int cols;
    int rows;

    int* idleRight;
    int* idleLeft;
    int* walkRight;
    int* walkLeft;
    int* jumpUpRight;
    int* jumpUpLeft;
    int* jumpDownRight;
    int* jumpDownLeft;
    int* runRight;
    int* runLeft;
    int* currentAnim;
	int* attack;
	int* attackLeft;

	int* idle;
	int* ticking;
	int* exploding;

    int animFrames;
    int animIndex;
    float animTime;
    bool isStatic;
    bool isActive;
    bool isInvincible;
    
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    glm::vec3 initialPosition;
	glm::vec3 scale;
    
    float width;
    float height;
    
    float speed;
    float timer;
    
	bool attacked;
	bool attacking;

	bool isAlive;
	
	bool reflected;
    
    Entity();
    
	void Knockback(Entity& obj, Entity& reference);

    bool CheckCollision(Entity& other);


    
    void CheckCollisionsX(Map* map);
    void CheckCollisionsY(Map* map);
    
    bool CheckPartialOnLedge(Map* map);
    void CheckSlideOffLedge(Map* map);
    
    void CheckCollisionsX(Entity* objects, int objectCount);
    void CheckCollisionsY(Entity* objects, int objectCount);
    
    void Update(float deltaTime, Entity* objects, int objectCount, Entity* hazards, int hazard_count, Map* map, Entity* enemies, int enemyCount, Entity* sword, Entity* player);
    void Render(ShaderProgram* program);
    
    void Jump();
	void Attack(Entity* sword);
    
    void AI(Entity& player, Entity* hazards, int hazard_count, Map* map);
    void AISpiker(Entity player, Entity* hazards, int hazard_count, Map* map);
    void AIGunner(Entity player, Entity* hazards, int hazard_count,Map* map);
    
    void AIBoss(Entity player, Entity* hazards, int hazard_count,Map* map);
    
    void AIFly(Entity player, Entity* hazards, int hazard_count,Map* map);
    
    void HZ(Entity& player, float deltaTime, Map* map);
    void HZBomb(Entity player, float deltaTime, Map* map);
    void HZSpike(Entity player, float deltaTime, Map* map);
	void HZLaser(Entity player, float deltaTime, Map* map);

    void DrawSpriteFromTextureAtlas(ShaderProgram* program, int index);
    
    bool facingLeft();
    int stepping();
    
    void animate(float deltaTime, Entity* sword);
	void animateHz(float deltaTime);
	void animateSw(float deltaTime, Entity* player);
    
    bool collidedTop;
    bool collidedBottom;
    bool collidedLeft;
    bool collidedRight;
    
};

