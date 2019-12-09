#include "Entity.h"
#include "Util.h"

Entity::Entity()
{
    entityType = PLATFORM;
    isStatic = true;
    isActive = true;
    position = glm::vec3(0);
    speed = 0;
    width = 1;
    height = 1;
    //timer = 50.0f;
    footstep = Mix_LoadWAV("footstep.wav");
    scale = glm::vec3(2.0f, 2.0f, 2.0f);
    timer = 2.0f;
    reflected = false;
}

Entity::Entity(std::string newEntityType, std::string newType, glm::vec3 newPosition)
{
    isActive = true;
    speed = 0;
    width = 1;
    height = 1;
    scale = glm::vec3(2.0f, 2.0f, 2.0f);
    timer = 2.0f;
    reflected = false;
    
    if(newEntityType == "ENEMY"){
        
        entityType = ENEMY;
        
        if(newType == "FLY"){
            //std :: cout << "created FLY" << std :: endl;
            textureID = Util::LoadTexture("fly.png");
            acceleration = glm::vec3(0, 0, 0);
            //isActive = true;
            isStatic = false;
            position = newPosition;
            aiState = IDLE;
            aiType = FLY;
            velocity = glm::vec3(0, -1, 0);
            initialPosition = newPosition;
    
        }
        else if(newType == "SPIKER"){
            textureID = Util::LoadTexture("fly.png");
            acceleration = glm::vec3(0, -9.8f, 0);
            isStatic = false;
            position = newPosition;
            aiState = WALKING;
            aiType = SPIKER;
            velocity = glm::vec3(5.0f, 0, 0);
            timer = 35.0f;
        }
        else if(newType == "GUNNER"){
            textureID = Util::LoadTexture("me.png");
            acceleration = glm::vec3(0, -9.8f, 0);
            isStatic = false;
            position = newPosition;
            aiState = WALKING;
            aiType = GUNNER;
            velocity = glm::vec3(5.0f, 0, 0);
            timer = 50.0f;
        }
    }
    else if(newEntityType == "HAZARD"){
        entityType = HAZARD;
        if(newType == "BOMB"){
            hzType = BOMB;
            hzState = DEPLOY;
            textureID = Util::LoadTexture("bomb.png");
            rows = 3;
            cols = 4;
            idle = new int[4]{1, 2, 3, 4};
            exploding = new int[4]{5, 6, 7, 8};
            ticking = new int[4]{ 9, 10, 11, 12 };
            acceleration = glm::vec3(0, -9.81f, 0);
            isStatic = false;
            isActive = false;
            position = glm::vec3(3, -2.25, 0);
            velocity = glm::vec3(0, -1, 0);
            initialPosition = glm::vec3(3, -2.25, 0);
            
            timer = 3.0f;
            
            currentAnim = idle;
            animFrames = 3;
            animIndex = 0;
        }
        else if(newType == "SPIKE"){
            hzType = SPIKE;
            hzState = TICKING;
            textureID = Util::LoadTexture("spike.png");
            acceleration = glm::vec3(0, -9.81f, 0);
            isStatic = false;
            isActive = false;
            position = glm::vec3(3, -2.25, 0);
            velocity = glm::vec3(0, -1, 0);
            //initialPosition = glm::vec3(3, -2.25, 0);
            timer = 5.0f;
            
            rows = 3;
            cols = 4;
            idle = new int[4]{ 9, 10, 11, 12 };
            exploding = new int[4]{ 1, 2, 3, 4 };
            ticking = new int[4]{ 5, 6, 7, 8 };
            
            currentAnim = idle;
            animFrames = 3;
            animIndex = 0;
        }
        else if(newType == "LASER"){
            entityType = HAZARD;
            hzType = LASER;
            hzState = EXPLODE;
            textureID = Util::LoadTexture("bullet.png");
            acceleration = glm::vec3(0, 0, 0);
            isStatic = false;
            isActive = false;
            position = glm::vec3(3, -2.25, 0);
            velocity = glm::vec3(0, 0, 0);
            //initialPosition = glm::vec3(3, -2.25, 0);
            
            timer = 10.0f;
            width = 0.4f;
            height = 0.4f;
            }
    }
    
    /*
    entityType = ENEMY;
    isStatic = true;
    isActive = true;
    position = glm::vec3(0);
    speed = 0;
    width = 1;
    height = 1;
    //timer = 50.0f;
    footstep = Mix_LoadWAV("footstep.wav");
    scale = glm::vec3(2.0f, 2.0f, 2.0f);
    timer = 2.0f;
    reflected = false;
     */
}



void Entity::Knockback(Entity& obj, Entity& reference){
    if(obj.position.x < reference.position.x){
        obj.velocity = glm::vec3(-3,4,0);
    }
    else{
        obj.velocity = glm::vec3(3,4,0);
    }
}




bool Entity::CheckCollision(Entity& other)
{
    if (isStatic == true) return false;
    if (isActive == false || other.isActive == false) return false;
    
    float xdist = fabs(position.x - other.position.x) - ((width + other.width) / 2.0f);
    float ydist = fabs(position.y - other.position.y) - ((height + other.height) / 2.0f);
    
    if (xdist < 0 && ydist < 0)
    {
        if(entityType == PLAYER){
            if(other.entityType == ENEMY){
                Knockback(*this, other);
                isActive = false;
            }
            
            if(other.entityType == HAZARD && other.hzType == BOMB && other.hzState == TICKING){
                return false;
            }
            
            if(other.entityType == HAZARD && other.hzType == SPIKE && other.hzState == EXPLODE){
                
                if(other.entityType == ENEMY){
                    Knockback(*this, other);
                    isActive = false;
                }
                
                
                //isActive = false;
            }
            if(other.entityType == HAZARD && other.hzType == SPIKE && other.hzState == TICKING){
                return false;
            }
            if(other.entityType == HAZARD && other.hzType == LASER && other.hzState == EXPLODE){
                
                if(other.entityType == ENEMY){
                    Knockback(*this, other);
                    isActive = false;
                }
                
                //isActive = false;
            }
        }
        if (entityType == SWORD)
        {
            if(other.entityType == ENEMY && other.aiType != BOSS){
                std :: cout << "DEATH" << std :: endl;
                other.isActive = false;
            }
            if(other.entityType == HAZARD){
                switch (other.hzType) {
                    case BOMB:
                        if(position.x < other.position.x){
                            other.velocity = glm::vec3(3,4,0);
                        }
                        else{
                            other.velocity = glm::vec3(-3,4,0);
                        }
                        other.reflected = true;
                        break;
                    case SPIKE:
                        std :: cout << "DESTROYED" << std :: endl;
                        other.isActive = false;
                        break;
                    case LASER:
                        std :: cout << "REFLECT" << std :: endl;
                        
                        std :: cout << "VELOCITY" << other.velocity.x << std :: endl;
                        if(other.reflected){
                            return false;
                        }
                        else{
                            std :: cout << "MULTIPLY BY -1" << std :: endl;
                            
                            other.reflected = true;
                            other.velocity.x *= -1;

                        }
                        break;
                }
            }
            
            /*
            if(other.entityType == HAZARD && (other.hzType == LASER || other.hzType == SPIKE)){
                std :: cout << "DESTROYED" << std :: endl;
                other.isActive = false;
            }
             */
            
        }
        if(entityType == HAZARD){
            if(other.entityType == ENEMY && reflected && hzType == LASER){
                other.isActive = false;
            }
            if(other.entityType == ENEMY && reflected && hzType == BOMB && hzState == EXPLODE){
                other.isActive = false;
            }
            //if(other.entityType == PLAYER && )
        }
        
        
        /*
        if (entityType == ENEMY && other.entityType == SWORD)
        {
            std :: cout << "DEATH" << std :: endl;
            isActive = false;
        }
        */
        return true;
    }
    
    return false;
}

void Entity::CheckCollisionsY(Entity *objects, int objectCount)
{
    for (int i = 0; i < objectCount; i++)
    {
        Entity& object = objects[i];
        
        if (CheckCollision(object))
        {
            float ydist = fabs(position.y - object.position.y);
            float penetrationY = fabs(ydist - (height / 2) - (object.height / 2));
            if (velocity.y > 0) {
                position.y -= penetrationY;
                
                if(object.entityType == HAZARD && ((object.hzType == BOMB && object.hzState == TICKING) ||
                                                   (object.hzType == SPIKE && object.hzState == TICKING))){
                    //do nothing
                }
                else{
                    velocity.y = 0;
                }
                
                if(object.entityType == HAZARD && object.hzType == BOMB){
                    std :: cout << "CCY G" << std :: endl;
                    velocity.x = 0;
                }
                
                
                collidedTop = true;
            }
            else if (velocity.y < 0) {
                position.y += penetrationY;
                velocity.y = 0;
                
                if(object.entityType == HAZARD && ((object.hzType == BOMB && object.hzState == TICKING) ||
                                                   (object.hzType == SPIKE && object.hzState == TICKING))){
                    //do nothing
                }
                else{
                    velocity.y = 0;
                }
                
                if(object.entityType == HAZARD && object.hzType == BOMB){
                    std :: cout << "CCY G" << std :: endl;
                    velocity.x = 0;
                }
                
                collidedBottom = true;
            }
        }
    }
}

void Entity::CheckCollisionsX(Entity *objects, int objectCount)
{
    for (int i = 0; i < objectCount; i++)
    {
        Entity& object = objects[i];
        
        if (CheckCollision(object))
        {
            float xdist = fabs(position.x - object.position.x);
            float penetrationX = fabs(xdist - (width / 2) - (object.width / 2));
            if (velocity.x > 0) {
                position.x -= penetrationX;
                
                //std :: cout << "CCX Greater" << std :: endl;
                
                if(entityType == HAZARD && hzType == LASER && reflected){
                    //do nothing
                }
                else{
                    velocity.x = 0;
                }
                
                
                collidedRight = true;
                
                if (entityType == ENEMY && object.entityType == SWORD)
                {
                    std :: cout << "DEATH" << std :: endl;
                    isActive = false;
                }
            }
            else if (velocity.x < 0) {
                position.x += penetrationX;
                
                //std :: cout << "CCX LESS" << std :: endl;
                if(entityType == HAZARD && hzType == LASER && reflected){
                    //do nothing
                }
                else{
                    velocity.x = 0;
                }
                
                collidedLeft = true;
                
                if (entityType == ENEMY && object.entityType == SWORD)
                {
                    std :: cout << "DEATH" << std :: endl;
                    isActive = false;
                }
            }
        }
    }
}

void Entity::CheckCollisionsX(Map* map)
{
    // Probes for tiles
    glm::vec3 left = glm::vec3(position.x - (width / 2), position.y, position.z);
    glm::vec3 right = glm::vec3(position.x + (width / 2), position.y, position.z);
    
    float penetration_x = 0;
    float penetration_y = 0;
    if (map->IsSolid(left, &penetration_x, &penetration_y) && velocity.x < 0) {
        position.x += penetration_x;
        
        if(entityType == ENEMY){
            if (aiType == PAPARAZZI && aiState == RUNNING){
                Jump();
            }
            else{
                velocity *= -1;
            }
            
        }
        
        else{
            velocity.x = 0;
        }
        
        collidedLeft = true;
    }
    
    if (map->IsSolid(right, &penetration_x, &penetration_y) && velocity.x > 0) {
        position.x -= penetration_x;
        
        if(entityType == ENEMY){
            if (aiType == PAPARAZZI && aiState == RUNNING){
                Jump();
            }
            else{
                velocity *= -1;
            }
            
        }
        
        else{
            velocity.x = 0;
        }
        
        collidedRight = true;
    }
}

void Entity::CheckCollisionsY(Map* map)
{
    // Probes for tiles
    glm::vec3 top = glm::vec3(position.x, position.y + (height / 2), position.z);
    glm::vec3 top_left = glm::vec3(position.x - (width / 2), position.y + (height / 2), position.z);
    glm::vec3 top_right = glm::vec3(position.x + (width / 2), position.y + (height / 2), position.z);
    
    glm::vec3 bottom = glm::vec3(position.x, position.y - (height / 2), position.z);
    glm::vec3 bottom_left = glm::vec3(position.x - (width / 2), position.y - (height / 2), position.z);
    glm::vec3 bottom_right = glm::vec3(position.x + (width / 2), position.y - (height / 2), position.z);
    
    float penetration_x = 0;
    float penetration_y = 0;
    if (map->IsSolid(top, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        
        if(entityType == HAZARD && hzType == BOMB){
            velocity = glm::vec3(0,0,0);
        }
        if(entityType == PLAYER && isActive == false){
            velocity = glm::vec3(0,0,0);
        }
        
        
        
        collidedTop = true;
    }
    else if (map->IsSolid(top_left, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
        
        if(entityType == HAZARD && hzType == BOMB){
            velocity = glm::vec3(0,0,0);
        }
        if(entityType == PLAYER && isActive == false){
            velocity = glm::vec3(0,0,0);
        }
    }
    else if (map->IsSolid(top_right, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
        
        if(entityType == HAZARD && hzType == BOMB){
            velocity = glm::vec3(0,0,0);
        }
        if(entityType == PLAYER && isActive == false){
            velocity = glm::vec3(0,0,0);
        }
    }
    
    if (map->IsSolid(bottom, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
        
        if(entityType == HAZARD && hzType == BOMB){
            velocity = glm::vec3(0,0,0);
        }
        if(entityType == PLAYER && isActive == false){
            velocity = glm::vec3(0,0,0);
        }
    }
    else if (map->IsSolid(bottom_left, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
        
        if(entityType == HAZARD && hzType == BOMB){
            velocity = glm::vec3(0,0,0);
        }
        if(entityType == PLAYER && isActive == false){
            velocity = glm::vec3(0,0,0);
        }
    }
    else if (map->IsSolid(bottom_right, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
        
        if(entityType == HAZARD && hzType == BOMB){
            velocity = glm::vec3(0,0,0);
        }
        if(entityType == PLAYER && isActive == false){
            velocity = glm::vec3(0,0,0);
        }
    }
}

bool Entity::CheckPartialOnLedge(Map* map)
{
    glm::vec3 bottom = glm::vec3(position.x, position.y - (height / 2), position.z);
    glm::vec3 bottom_left = glm::vec3(position.x - (width / 2), position.y - (height / 2), position.z);
    glm::vec3 bottom_right = glm::vec3(position.x + (width / 2), position.y - (height / 2), position.z);
    float penetration_x = 0;
    float penetration_y = 0;
    
    if ((map->IsSolid(bottom, &penetration_x, &penetration_y)) && !(map->IsSolid(bottom_right, &penetration_x, &penetration_y))) {
        position.y += penetration_y;
        //velocity.y = 0;
        
        return true;
    }
    if ((map->IsSolid(bottom, &penetration_x, &penetration_y)) && !(map->IsSolid(bottom_left, &penetration_x, &penetration_y))) {
        position.y += penetration_y;
        //velocity.y = 0;
        
        return true;
    }
    /*
     if ((map->IsSolid(bottom_left, &penetration_x, &penetration_y))&& !(map->IsSolid(bottom_right, &penetration_x, &penetration_y))){
     position.y += penetration_y;
     //velocity.y = 0;
     collidedBottom = true;
     }
     if (collidedBottom){
     return false;
     }
     */
    return false;
}


void Entity::Jump()
{
    if (collidedBottom)
    {
        velocity.y = 4.0f;
        /*if (collidedBottom)
         {
         acceleration.x = 5.2f;
         }*/
    }
}

void Entity::Attack(){
    //std :: cout << "done" << std::endl;
}

void Entity::AIFly(Entity player, Entity* hazards, int hazard_count, Map* map) {
    switch (aiState) {
        case IDLE:
            
            
            
            if (glm::distance(position, player.position) < 5.0f) {
                if (position.y >= initialPosition.y + 0.25)
                {
                    velocity.y = -2;
                }
                else if (position.y <= initialPosition.y - 0.25)
                {
                    velocity.y = 2;
                }
                for (int i = 0; i < hazard_count; ++i) {
                    if (hazards[i].hzType == BOMB && hazards[i].isActive == false) {
                        aiState = WALKING;
                    }
                }
                
            }
            else if (position.y >= initialPosition.y + 0.25)
            {
                velocity.y = -2;
            }
            else if (position.y <= initialPosition.y - 0.25)
            {
                velocity.y = 2;
            }
            
            break;
        case WALKING:
            
            
            
            for(int i = 0; i < hazard_count; ++i){
                if(hazards[i].hzType == BOMB && hazards[i].isActive == false){
                    hazards[i].position.x = position.x;
                    hazards[i].position.y = position.y - 0.2f;
                    hazards[i].isActive = true;
                    aiState = IDLE;
                    break;
                }
            }
            
            
            
            
            
            break;
    }
}

void Entity::AISpiker(Entity player, Entity* hazards, int hazard_count, Map* map) {
    switch (aiState) {
        case IDLE:
            velocity = glm::vec3(0.0f, 0, 0);
            if ((glm::distance(position, player.position) <= 7.5f)) {
                aiState = WALKING;
            }
            break;
        case WALKING:
            if (CheckPartialOnLedge(map)) {
                velocity.x *= -1;
            }
            if (abs(velocity.y) > 0.0f) {
                velocity.x = 0.0f;
            }
            
            if ((glm::distance(position, player.position) <= 7.5f)) {
                if ((glm::distance(position, player.position) < 2.5f) && timer >= 35.0f) {
                    aiState = ATTACK;
                    timer = 0.0f;
                }
                if (position.x < player.position.x) {
                    velocity.x = 1.0f;
                }
                else {
                    velocity.x = -1.0f;
                }
            }
            else {
                aiState = IDLE;
                velocity = glm::vec3(0, 0, 0);
            }
            
            if (timer < 35.0f) {
                timer += 1.0f;
            }
            break;
        case RUNNING:
            break;
        case SLOW_DOWN:
            break;
        case AOE:
            break;
        case ATTACK:
            
            for (int i = 0; i < hazard_count; ++i) {
                if (hazards[i].hzType == SPIKE && hazards[i].isActive == false) {
                    //hazards[i].acceleration.y = -9.81;
                    hazards[i].position.x = player.position.x;
                    hazards[i].position.y = player.position.y - 0.25f;
                    hazards[i].isActive = true;
                    //hazards[i].hzState = TICKING;
                    //hazards[i].timer = 4.0f;
                }
            }
            
            aiState = WALKING;
            
            break;
    }
    
    
    
    
}

void Entity::AIGunner(Entity player, Entity* hazards, int hazard_count, Map* map) {
    switch (aiState) {
        case IDLE:
            velocity = glm::vec3(0, 0, 0);
            if ((glm::distance(position, player.position) <= 7.5f)) {
                aiState = WALKING;
            }
            break;
        case WALKING:
            if (CheckPartialOnLedge(map)) {
                velocity.x *= -1;
            }
            if(abs(velocity.y) > 0.0f){
                velocity.x = 0.0f;
            }
            
            if((glm::distance(position, player.position) <= 7.5f)){
                if ((glm::distance(position, player.position) < 5.5f) && timer >= 50.0f){
                    aiState = ATTACK;
                    timer = 0.0f;
                }
                if(position.x < player.position.x){
                    velocity.x = 1.25f;
                }
                else{
                    velocity.x = -1.25f;
                }
            }
            else{
                aiState = IDLE;
                velocity = glm::vec3(0,0,0);
            }
            
            if(timer < 50.0f){
                timer += 1.0f;
            }
            break;
        case RUNNING:
            break;
        case SLOW_DOWN:
            break;
        case AOE:
            break;
        case ATTACK:
            
            //std :: cout << "ATTACK" << std :: endl;
            
            for(int i = 0; i < hazard_count; ++i){
                if(hazards[i].hzType == LASER && hazards[i].isActive == false){
                    hazards[i].position = position;
                    
                    if(player.position.x > position.x){
                        hazards[i].position.x += 0.5f;
                        hazards[i].velocity = glm::vec3(3.0f,0,0);
                    }
                    else{
                        position.x -= 0.5f;
                        hazards[i].velocity = glm::vec3(-3.0f,0,0);
                    }
                    hazards[i].isActive = true;
                    hazards[i].isStatic = false;
                    hazards[i].textureID = Util::LoadTexture("bullet_temp.png");
                    hazards[i].hzState = EXPLODE;
                    hazards[i].timer = 3.5f;
                    
                    hazards[i].width = 0.4f;
                    hazards[i].height = 0.4f;
                    
                }
            }
            
            aiState = WALKING;
            
            break;
    }
}


void Entity::AIBoss(Entity player, Entity* hazards, int hazard_count, Map* map) {
    switch (aiState) {
        case IDLE:
            velocity = glm::vec3(0, 0, 0);
            break;
        case WALKING:
            break;
        case RUNNING:
            break;
        case SLOW_DOWN:
            break;
        case AOE:
            break;
        case ATTACK:
            break;
    }
}






void Entity::AI(Entity& player, Entity* hazards, int hazard_count, Map* map) {//, Entity* effects_sprites, int ef_s_size){
    if(isActive && player.isActive){
        switch (aiType) {
            case SPIKER:
                AISpiker(player, hazards,hazard_count, map);
                break;
            case GUNNER:
                AIGunner(player, hazards,hazard_count, map);
                break;
            case FLY:
                AIFly(player, hazards,hazard_count, map);
                break;
            case BOSS:
                AIBoss(player, hazards,hazard_count, map);
                break;
        }
    }
    if(!player.isActive){
        velocity = glm::vec3(0,0,0);
    }
    
    
}
void Entity::HZBomb(Entity player, float deltaTime, Map* map)
{
    switch (hzState) {
        case DEPLOY:
            if (velocity.y == 0)
            {
                
                hzState = TICKING;
            }
            break;
        case TICKING:
            if (timer <= 0)
            {
                timer = 0.1f;
                hzState = EXPLODE;
                width = 3;
                height = 2;
                //timer = 10.0f;
            }
            else
            {
                timer = timer - deltaTime;
            }
            
            break;
        case EXPLODE:
            if (timer <= 0)
            {
                timer = 3.0f;
                isActive = false;
                velocity = glm::vec3(0,0,0);
                
                
                hzState = DEPLOY;
                width = 1;
                height = 1;
                reflected = false;
            }
            else
            {
                timer = timer - deltaTime;
            }
            break;
    }
}

void Entity::HZSpike(Entity player, float deltaTime, Map* map)
{
    switch (hzState) {
        case DEPLOY:
            if (isActive)
            {
                hzState = TICKING;
            }
            break;
        case TICKING:
            if (timer <= 0)
            {
                hzState = EXPLODE;
                timer = 5.0f;
                isActive = true;
                
                
                
                
            }
            else {
                timer -= deltaTime;
            }
            break;
        case EXPLODE:
            
            
            if(timer <= 0.0f){
                
                
                isActive = false;
                reflected = false;
                timer = 5.0f;
                hzState = DEPLOY;
                
                
            }
            else
            {
                timer -= deltaTime;
            }
            
            break;
    }
}


void Entity::HZLaser(Entity player, float deltaTime, Map* map)
{
    switch (hzState) {
        case DEPLOY:
            break;
        case TICKING:
            break;
        case EXPLODE:
            // std :: cout << timer << std :: endl;
            //velocity.x += .05f;
            if(timer <= 0.0f){
                //std :: cout << "TIMER GONE" << std :: endl;
                isActive = false;
                reflected = false;
            }
            timer -= deltaTime;
            
            break;
    }
}



void Entity::HZ(Entity& player, float deltaTime, Map* map)
{
    if(isActive && player.isActive){
        switch (hzType) {
            case SPIKE:
                HZSpike(player, deltaTime, map);
                break;
            case LASER:
                HZLaser(player, deltaTime, map);
                break;
            case BOMB:
                HZBomb(player, deltaTime, map);
                break;
        }
    }
    
    if(!player.isActive){
        velocity = glm::vec3(0,0,0);
    }
    
}
void Entity::DrawSpriteFromTextureAtlas(ShaderProgram* program, int index)
{
    float u = (float)(index % cols) / (float)cols;
    float v = (float)(index / cols) / (float)rows;
    
    float width = 1.0f / (float)cols;
    float height = 1.0f / (float)rows;
    
    float texCoords[] = { u, v + height, u + width, v + height, u + width, v,
        u, v + height, u + width, v, u, v };
    
    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

bool Entity::facingLeft()
{
    if (currentAnim == idleLeft || currentAnim == walkLeft || currentAnim == jumpUpLeft || currentAnim == jumpDownLeft)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int Entity::stepping()
{
    if (currentAnim == walkRight || currentAnim == walkLeft)
    {
        return 1;
    }
    else if (currentAnim == runRight || currentAnim == runLeft)
    {
        return 2;
    }
    return 0;
}

void Entity::animate(float deltaTime)
{
    
    if (velocity.y > 0 && velocity.x >= 0)
    {
        animFrames = 7;
        if (facingLeft() && velocity.x == 0)
        {
            currentAnim = jumpUpLeft;
        }
        else
        {
            currentAnim = jumpUpRight;
        }
    }
    else if (velocity.y < 0 && velocity.x >= 0)
    {
        animFrames = 7;
        if (facingLeft() && velocity.x == 0)
        {
            currentAnim = jumpDownLeft;
        }
        else
        {
            currentAnim = jumpDownRight;
        }
    }
    else if (velocity.y > 0 && velocity.x < 0)
    {
        animFrames = 7;
        currentAnim = jumpUpLeft;
    }
    else if (velocity.y < 0 && velocity.x < 0)
    {
        animFrames = 7;
        currentAnim = jumpDownLeft;
    }
    else if (velocity.x > 4)
    {
        animFrames = 5;
        currentAnim = runRight;
    }
    else if (velocity.x < -4)
    {
        animFrames = 5;
        currentAnim = runLeft;
    }
    else if (velocity.x > 0)
    {
        animFrames = 7;
        currentAnim = walkRight;
    }
    else if (velocity.x < 0)
    {
        animFrames = 7;
        currentAnim = walkLeft;
    }
    else if (velocity.x == 0 && velocity.y == 0)
    {
        animFrames = 7;
        if (facingLeft())
        {
            currentAnim = idleLeft;
        }
        else
        {
            currentAnim = idleRight;
        }
    }
    else
    {
        animFrames = 7;
        currentAnim = idleRight;
    }
    
    animTime += deltaTime;
    if (animTime >= 0.075f)
    {
        animTime = 0;
        animIndex++;
        if (stepping() == 1)
        {
            if (animIndex == 0 || animIndex == 4)
            {
                Mix_PlayChannel(-1, footstep, 0);
            }
        }
        else if (stepping() == 2)
        {
            if (animIndex % 2 == 0)
            {
                Mix_PlayChannel(-1, footstep, 0);
            }
        }
        if (currentAnim == jumpUpLeft || currentAnim == jumpUpRight)
        {
            animIndex = 2;
        }
        else if (animIndex >= animFrames)
        {
            animIndex = 0;
        }
    }
    
}
    
    



void Entity::animateHz(float deltaTime)
{
    if (hzType == BOMB || hzType == SPIKE)
    {
        if (hzState == DEPLOY)
        {
            currentAnim = idle;
        }
        else if (hzState == TICKING)
        {
            currentAnim = ticking;
        }
        else if (hzState == EXPLODE)
        {
            currentAnim = exploding;
        }
        animTime += deltaTime;
        if (animTime >= 0.075f)
        {
            animTime = 0;
            animIndex++;
            if (animIndex >= animFrames)
            {
                animIndex = 0;
            }
        }
    }
    
    
}

void Entity::Update(float deltaTime, Entity* objects, int objectCount, Entity* hazards, int hazard_count, Map* map, Entity* enemies, int enemyCount)
{
    collidedTop = false;
    collidedBottom = false;
    collidedLeft = false;
    collidedRight = false;
    
    velocity += acceleration * deltaTime;
    
    position.y += velocity.y * deltaTime; // Move on Y
    CheckCollisionsY(map);
    CheckCollisionsY(objects, objectCount); // Fix if needed
    
    position.x += velocity.x * deltaTime; // Move on X
    CheckCollisionsX(map);
    CheckCollisionsX(objects, objectCount); // Fix if needed
    
    if(entityType == SWORD){
        //std :: cout << timer << std :: endl;
        if(isActive){
            CheckCollisionsX(enemies, enemyCount);
            CheckCollisionsY(enemies, enemyCount);
            
            CheckCollisionsX(hazards, hazard_count);
            CheckCollisionsY(hazards, hazard_count);
            
            
            
            //position.x = objects -> position.x + 0.5f;
        }
        
        
        if(timer > 0 && timer < 25.0f){
            timer-=1.0f;
        }
        else if(timer <= 0.0f){
            timer = 25.0f;
            isActive = false;
        }
    }
    
    if (entityType == PLAYER)
    {
        CheckCollisionsX(enemies, enemyCount);
        CheckCollisionsY(enemies, enemyCount);
        
        CheckCollisionsX(hazards, hazard_count);
        CheckCollisionsY(hazards, hazard_count);
        
        if(timer > 0 && timer < 50.0f){
            timer-=1.0f;
        }
        else if(timer <= 0.0f){
            timer = 50.0f;
        }
        animate(deltaTime);
    }
    
    if (entityType == ENEMY)
    {
        AI(*objects, hazards, hazard_count, map);
        //CheckCollisionsX(&player, 1);
        //CheckCollisionsY(&player, 1);
    }
    if (entityType == HAZARD)
    {
        if(reflected){
            CheckCollisionsX(enemies, enemyCount);
            CheckCollisionsY(enemies, enemyCount);
        }
        
        HZ(*objects, deltaTime, map);
        if (hzType == BOMB || hzType == SPIKE)
        {
            animateHz(deltaTime);
        }
        
        ;
    }
    
    //animate(deltaTime);
}



void Entity::Render(ShaderProgram *program) {
    if(entityType == PLAYER && !isActive){
        
        if (abs(velocity.y) >= 0)
        {
            std :: cout << "AIR BUD " << velocity.y << std :: endl;
            DrawSpriteFromTextureAtlas(program, currentAnim[animIndex]);
        }
        
        std :: cout << "COLLIDED" << std :: endl;
    }
    
    
    else if (isActive == true)
    {
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, position);
        
        if (entityType == HAZARD && hzType == BOMB && hzState == EXPLODE)
        {
            modelMatrix = glm::scale(modelMatrix, scale);
        }
        
        program->SetModelMatrix(modelMatrix);
        
        
        
        if (entityType == PLAYER)
        {
            DrawSpriteFromTextureAtlas(program, currentAnim[animIndex]);
        }
        else if (entityType == HAZARD && (hzType == BOMB || hzType == SPIKE))
        {
            DrawSpriteFromTextureAtlas(program, currentAnim[animIndex]);
        }
        else
        {
            float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
            float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
            
            glBindTexture(GL_TEXTURE_2D, textureID);
            
            glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
            glEnableVertexAttribArray(program->positionAttribute);
            
            glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
            glEnableVertexAttribArray(program->texCoordAttribute);
            
            glDrawArrays(GL_TRIANGLES, 0, 6);
            
            glDisableVertexAttribArray(program->positionAttribute);
            glDisableVertexAttribArray(program->texCoordAttribute);
        }
    }
}

