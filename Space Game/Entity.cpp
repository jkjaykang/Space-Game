#include "Entity.h"
#include "Util.h"
#include "Effects.h"

Entity::Entity()
{
    entityType = PLATFORM;
    isStatic = false;
    isActive = true;
    position = glm::vec3(0);
    scale = glm::vec3(4.0f, 4.0f, 2.0f);
    speed = 0;
    width = .8f;
    height = .8f;
    timer = 2.0f;
    slash = Mix_LoadWAV("slash.wav");
    jump = Mix_LoadWAV("jump.wav");
    hit = Mix_LoadWAV("hit.wav");
    explosion = Mix_LoadWAV("explosion.wav");
    laser = Mix_LoadWAV("laser.wav");
    lives =3;
    footstep = Mix_LoadWAV("footstep.wav");
    
    
    isAlive = true;
    attacking = false;
    
    reflected = false;
}

Entity::Entity(std::string newEntityType, std::string newType, glm::vec3 newPosition)
{
    explosion = Mix_LoadWAV("explosion.wav");
    laser = Mix_LoadWAV("laser.wav");
    isActive = true;
    isStatic = false;
    speed = 0;
    width = 1;
    height = 1;
    scale = glm::vec3(2.0f, 2.0f, 2.0f);
    timer = 1.0f;
    reflected = false;
    lives = 3;
    if (newEntityType == "ENEMY") {
        
        entityType = ENEMY;
        
        if (newType == "FLY") {
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
        else if (newType == "SPIKER") {
            textureID = Util::LoadTexture("spiker.png");
            acceleration = glm::vec3(0, -9.8f, 0);
            isStatic = false;
            position = newPosition;
            aiState = WALKING;
            aiType = SPIKER;
            velocity = glm::vec3(5.0f, 0, 0);
            timer = 2.0f;
        }
        else if (newType == "GUNNER") {
            textureID = Util::LoadTexture("gunner.png");
            acceleration = glm::vec3(0, -9.8f, 0);
            isStatic = false;
            position = newPosition;
            aiState = WALKING;
            aiType = GUNNER;
            velocity = glm::vec3(5.0f, 0, 0);
            timer = 1.0f;
            
        }
        else if (newType == "BOSS") {
            textureID = Util::LoadTexture("gunner.png");
            acceleration = glm::vec3(0, -9.8f, 0);
            isStatic = false;
            position = newPosition;
            aiState = WALKING;
            aiType = BOSS;
            velocity = glm::vec3(3.0f, 0, 0);
            timer = 1.0f;
            lives = 3;
            
        }
    }
    else if (newEntityType == "HAZARD") {
        entityType = HAZARD;
        animTime = 0;
        if (newType == "BOMB") {
            hzType = BOMB;
            hzState = DEPLOY;
            textureID = Util::LoadTexture("bomb.png");
            rows = 3;
            cols = 4;
            idle = new int[4]{ 1, 2, 3, 4 };
            exploding = new int[4]{ 5, 6, 7, 8 };
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
        else if (newType == "SPIKE") {
            hzType = SPIKE;
            hzState = TICKING;
            textureID = Util::LoadTexture("spike.png");
            acceleration = glm::vec3(0, 0, 0);
            isStatic = false;
            isActive = false;
            position = glm::vec3(3, -2.0f, 0);
            velocity = glm::vec3(0, 0, 0);
            //initialPosition = glm::vec3(3, -2.25, 0);
            timer = 2.0f;
            
            rows = 3;
            cols = 4;
            idle = new int[4]{ 9, 10, 11, 12 };
            exploding = new int[4]{ 1, 2, 3, 4 };
            ticking = new int[4]{ 5, 6, 7, 8 };
            
            currentAnim = idle;
            animFrames = 3;
            animIndex = 0;
        }
        else if (newType == "LASER") {
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
            width = 1.0f;
            height = 1.0f;
        }
        else if (newType == "CAN") {
            entityType = HAZARD;
            hzType = CAN;
            hzState = EXPLODE;
            textureID = Util::LoadTexture("me.png");
            acceleration = glm::vec3(0, -9.81f, 0);
            isStatic = false;
            isActive = false;
            position = glm::vec3(3, -2.25, 0);
            velocity = glm::vec3(0, 0, 0);
            //initialPosition = glm::vec3(3, -2.25, 0);
            
            timer = 10.0f;
            width = .2f;
            height = .2f;
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


void Entity::Knockback(Entity& obj, Entity& reference) {
    if (obj.position.x < reference.position.x) {
        obj.velocity = glm::vec3(-3, 4, 0);
    }
    else {
        obj.velocity = glm::vec3(3, 4, 0);
    }
}

bool Entity::CheckCollision(Entity& other)
{
    
    //Entity temp = other;
    
    if (isStatic == true) return false;
    if (isActive == false || other.isActive == false) return false;
    
    //float leeway = 0.5f;
    //float old_position_x = float(other.position.x);
    //float old_position_y = float(other.position.y);
    
    /*
    if(entityType == PLAYER){
        if(abs(velocity.x) > 0){
            std :: cout << "changed x pos" << std :: endl;
            old_position_x -= velocity.x;
        }
        if(abs(velocity.y) > 0){
            std :: cout << "changed y pos" << std :: endl;
            old_position_y -= velocity.y;
        }
    }
    
    
    float xdist = (fabs(old_position_x - other.position.x) - ((width + other.width) / 2.0f));
    float ydist = (fabs(old_position_y - other.position.y) - ((height + other.height) / 2.0f));
    */
    
    float xdist = (fabs(position.x - other.position.x) - ((width + other.width) / 2.0f));
    float ydist = (fabs(position.y - other.position.y) - ((height + other.height) / 2.0f));
    
    /*
    float leeway = 0.5f;
    
    float xdist = (fabs(position.x - other.position.x + leeway) - ((width + other.width) / 2.0f));
    float ydist = (fabs(position.y - other.position.y + leeway) - ((height + other.height) / 2.0f));
    */
    
    /*
    if(other.entityType == HAZARD && entityType == PLAYER){
        std :: cout << xdist << " " << ydist << std :: endl;
    }
    */
    
    /*
    if((other.hzType == LASER || other.hzType == CAN) && xdist < 0.1f && ydist < 0 && abs(velocity.x) >= 0){
        return true;
    }
    */
    
    
    if ((xdist < 0 && ydist < 0))
    {
        /*
        if(other.entityType == HAZARD && entityType == PLAYER){
            std :: cout << "CHECKING COLLISION" << std :: endl;
        }
         */
        if (entityType == PLAYER)
        {
            std :: cout << "CURR: PLAYER" << std :: endl;
            if (other.entityType == ENEMY)
            {
                Knockback(*this, other);
                isActive = false;
                //isAlive = false;
                
                //velocity.x = -10.0f;
                //velocity.y = 3.0f;
            }
            if (other.entityType == HAZARD && other.hzType == SPIKE && other.hzState == EXPLODE) {
                
                Knockback(*this, other);
                isActive = false;
                
                //isActive = false;
            }
            if (other.entityType == HAZARD && other.hzType == BOMB && other.hzState == EXPLODE) {
                Knockback(*this, other);
                isActive = false;
                //isActive = false;
            }
            if (other.entityType == HAZARD && other.hzType == SPIKE && other.hzState == TICKING) {
                return false;
            }
            if (other.entityType == HAZARD && other.hzType == LASER && other.hzState == EXPLODE) {
                Knockback(*this, other);
                isActive = false;
                //isActive = false;
            }
            if (other.entityType == HAZARD && other.hzType == CAN && other.hzState == EXPLODE) {
                std :: cout << "HZ-CAN-EXPLODE -> PLAYER = FALSE" << std :: endl;
                Knockback(*this, other);
                isActive = false;
                //isActive = false;
            }
        }

        if (entityType == SWORD && isActive)
        {
            if (other.entityType != PLAYER )
            {
                Mix_PlayChannel(-1, hit, 0);
                
            }
            if (other.entityType == ENEMY && other.aiType != BOSS) {
                std::cout << "DEATH" << std::endl;
                other.isActive = false;
            }
            if (other.entityType == HAZARD) {
                switch (other.hzType) {
                    case BOMB:
                        if (currentAnim == attack) {
                            other.velocity = glm::vec3(3, 4, 0);
                        }
                        else
                        {
                            other.velocity = glm::vec3(-3, 4, 0);
                        }
                        other.reflected = true;
                        break;
                    case SPIKE:
                        std::cout << "DESTROYED" << std::endl;
                        other.isActive = false;
                        break;
                    case LASER:
                        //std::cout << "REFLECT" << std::endl;
                        
                        //std::cout << "VELOCITY" << other.velocity.x << std::endl;
                        if (other.reflected) {
                            return false;
                        }
                        else {
                            // std::cout << "MULTIPLY BY -1" << std::endl;
                            
                            other.reflected = true;
                            other.velocity.x *= -1;
                            
                        }
                        break;
                    case CAN:
                        if (currentAnim == attack) {
                            other.velocity = glm::vec3(6.0f, 0.0, 0);;
                            
                        }
                        else
                        {
                            other.velocity = glm::vec3(-6.0f, 0.0, 0);;
                        }
                        other.reflected = true;
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
        if (entityType == HAZARD && isActive) {
            if (other.entityType == ENEMY && reflected && hzType == LASER) {
                other.isActive = false;
                isActive = false;
            }
            if (other.entityType == ENEMY && other.aiType != BOSS && reflected && hzType == BOMB && hzState == EXPLODE) {
                other.isActive = false;
            }
            if (other.entityType == ENEMY && other.aiType == BOSS && reflected && hzType == CAN && hzState == EXPLODE) {
                
                //std :: cout << lives << std :: endl;
                if(other.lives <= 1){
                    other.isActive = false;
                    other.aiState = DEAD;
                    
                }
                else{
                    std :: cout << "DEPLOY" << std :: endl;
                    other.lives -= 1;
                    hzState = DEPLOY;
                    reflected = false;
                }
                isActive = false;
                std :: cout << other.lives << std :: endl;
            }
            
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
                
                if(entityType == PLAYER && object.entityType == HAZARD && (object.hzType == LASER || object.hzType == CAN)){
                    isActive = false;
                    object.isActive = false;
                }
                
                
                velocity.y = 0;
                collidedTop = true;
            }
            else if (velocity.y < 0) {
                position.y += penetrationY;
                
                if(entityType == PLAYER && object.entityType == HAZARD && (object.hzType == LASER || object.hzType == CAN)){
                    isActive = false;
                    object.isActive = false;
                }
                
                velocity.y = 0;
                collidedBottom = true;
            }
            
            else if(velocity.y == 0){
                if(entityType == PLAYER && object.entityType == HAZARD && (object.hzType == LASER || object.hzType == CAN)){
                    isActive = false;
                    object.isActive = false;
                }
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
                
                if(entityType == PLAYER && object.entityType == HAZARD && (object.hzType == LASER || object.hzType == CAN)){
                    isActive = false;
                    object.isActive = false;
                }
                
                velocity.x = 0;
                collidedRight = true;
                
                if (entityType == ENEMY && object.entityType == SWORD)
                {
                    std :: cout << "DEATH" << std :: endl;
                    isActive = false;
                }
            }
            else if (velocity.x < 0) {
                position.x += penetrationX;
                
                if(entityType == PLAYER && object.entityType == HAZARD && (object.hzType == LASER || object.hzType == CAN)){
                    isActive = false;
                    object.isActive = false;
                }
                
                
                velocity.x = 0;
                collidedLeft = true;
                
                if (entityType == ENEMY && object.entityType == SWORD)
                {
                    std :: cout << "DEATH" << std :: endl;
                    isActive = false;
                }
            }
            else if(velocity.x == 0){
                if(entityType == PLAYER && object.entityType == HAZARD && (object.hzType == LASER || object.hzType == CAN)){
                    isActive = false;
                    object.isActive = false;
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
        
        if (hzType == LASER)
        {
            isActive = false;
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
        
        if (hzType == LASER)
        {
            isActive = false;
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
        if (entityType == HAZARD && hzType == BOMB) {
            velocity = glm::vec3(0, 0, 0);
        }
        if (entityType == PLAYER && isActive == false) {
            velocity = glm::vec3(0, 0, 0);
        }
        /*
        if(entityType == ENEMY && aiType == BOSS){
            /
        }
        */
        collidedTop = true;
    }
    else if (map->IsSolid(top_left, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        if (entityType == HAZARD && hzType == BOMB) {
            velocity = glm::vec3(0, 0, 0);
        }
        if (entityType == PLAYER && isActive == false) {
            velocity = glm::vec3(0, 0, 0);
        }
        collidedTop = true;
    }
    else if (map->IsSolid(top_right, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        if (entityType == HAZARD && hzType == BOMB) {
            velocity = glm::vec3(0, 0, 0);
        }
        if (entityType == PLAYER && isActive == false) {
            velocity = glm::vec3(0, 0, 0);
        }
        collidedTop = true;
    }
    
    if (map->IsSolid(bottom, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        if (entityType == HAZARD && hzType == BOMB) {
            velocity = glm::vec3(0, 0, 0);
        }
        if (entityType == PLAYER && isActive == false) {
            velocity = glm::vec3(0, 0, 0);
        }
        collidedBottom = true;
    }
    else if (map->IsSolid(bottom_left, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        if (entityType == HAZARD && hzType == BOMB) {
            velocity = glm::vec3(0, 0, 0);
        }
        if (entityType == PLAYER && isActive == false) {
            velocity = glm::vec3(0, 0, 0);
        }
        collidedBottom = true;
    }
    else if (map->IsSolid(bottom_right, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        if (entityType == HAZARD && hzType == BOMB) {
            velocity = glm::vec3(0, 0, 0);
        }
        if (entityType == PLAYER && isActive == false) {
            velocity = glm::vec3(0, 0, 0);
        }
        collidedBottom = true;
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
        if (entityType == PLAYER)
        {
            Mix_PlayChannel(-1, jump, 0);
            
        }
        velocity.y = 6.0f;
        /*if (collidedBottom)
         {
         acceleration.x = 5.2f;
         }*/
    }
}

void Entity::Attack(Entity* sword)
{
    if (timer == 50.0f) {
        Mix_PlayChannel(-1, slash, 0);
        attacking = true;
        timer -= 1.0f;
        sword->timer -= 1.0f;
        sword->isActive = true;
        sword->position =position;
        if (facingLeft())
        {
            sword->position.x -= 0.8f;
        }
        else
        {
            sword->position.x += 0.8f;
        }
    }
}

void Entity::AIFly(Entity player, Entity* hazards, int hazard_count, float deltaTime, Map* map) {
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
            for(int i = 0; i < hazardList.size(); ++i){
                if(hazardList[i]->isActive == false){
                    hazardList[i]->position.y = position.y - 0.2f;
                    hazardList[i]->position.x = position.x;
                    
                    hazardList[i]->isActive = true;
                    aiState = IDLE;
                    break;
                }
            }
            break;
    }
}

void Entity::AISpiker(Entity player, Entity* hazards, int hazard_count, float deltaTime, Map* map) {
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
                if ((glm::distance(position, player.position) < 2.5f) && timer >= 1.0f) {
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
            
            if (timer < 1.0f) {
                timer += deltaTime;
            }
            break;
        case RUNNING:
            break;
        case SLOW_DOWN:
            break;
        case AOE:
            break;
        case ATTACK:
            
            for (int i = 0; i < hazardList.size(); ++i) {
                if (hazardList[i]->isActive == false) {
                    //hazards[i].acceleration.y = -9.81;
                    hazardList[i]->position.x = player.position.x;
                    hazardList[i]->position.y = -5.5f;
                    hazardList[i]->isActive = true;
                    //hazards[i].hzState = TICKING;
                    //hazards[i].timer = 4.0f;
                    break;
                }
            }
            
            aiState = WALKING;
            
            break;
    }
}

void Entity::AIGunner(Entity player, Entity* hazards, int hazard_count, float deltaTime, Map* map) {
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
                if ((glm::distance(position, player.position) < 5.5f) && timer >= 1.0f) {
                    aiState = ATTACK;
                    timer = 0.0f;
                }
                if (position.x < player.position.x) {
                    velocity.x = 1.25f;
                }
                else {
                    velocity.x = -1.25f;
                }
            }
            else {
                aiState = IDLE;
                velocity = glm::vec3(0, 0, 0);
            }
            
            if (timer < 1.0f) {
                timer += deltaTime;
            }
            break;
        case RUNNING:
            break;
        case SLOW_DOWN:
            break;
        case AOE:
            break;
        case ATTACK:
            
            std::cout << "ATTACK" << std::endl;
            
            for (int i = 0; i < hazardList.size(); ++i) {
                //if (hazards[i].hzType == LASER && hazards[i].isActive == false)
                if (hazardList[i]->isActive == false)
                {
                    hazardList[i]->position = position;
                    
                    if (player.position.x > position.x) {
                        hazardList[i]->position.x += 0.25f;
                        hazardList[i]->velocity = glm::vec3(9.0f, 0, 0);
                    }
                    else {
                        hazardList[i]->position.x -= 0.25f;
                        hazardList[i]->velocity = glm::vec3(-9.0f, 0, 0);
                    }
                    Mix_PlayChannel(-1, laser, 0);
                    hazardList[i]->isActive = true;
                    hazardList[i]->isStatic = false;
                    hazardList[i]->textureID = Util::LoadTexture("bullet.png");
                    hazardList[i]->hzState = EXPLODE;
                    hazardList[i]->timer = 1.5f;
                    break;
                    
                }
            }
            
            aiState = WALKING;
            
            break;
    }
}

void Entity :: AIBoss(Entity& player, Entity* hazards, int hazard_count, float deltaTime, Map* map){
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
            if (abs(velocity.y) > 0.0f) {
                velocity.x = 0.0f;
            }
            
            if ((glm::distance(position, player.position) <= 7.5f)) {
                if ((glm::distance(position, player.position) < 5.5f) && timer >= 1.0f) {
                    aiState = ATTACK;
                    timer = 0.0f;
                }
                if (position.x < player.position.x) {
                    velocity.x = 1.25f;
                }
                else {
                    velocity.x = -1.25f;
                }
            }
            else {
                aiState = IDLE;
                velocity = glm::vec3(0, 0, 0);
            }
            
            if (timer < 1.0f) {
                timer += deltaTime;
            }
            break;
        case RUNNING:
            break;
        case SLOW_DOWN:
            break;
        case AOE:
            break;
        case ATTACK:{
            int randomAttack = rand() % 2;
            if(randomAttack == 0){
                /*
                Jump();
                if(abs(velocity.y) == 0){
                    if(abs(player.velocity.y) == 0){
                        player.isActive = false;
                    }
                }
                 */
                
            }
            else if(randomAttack == 1){
                for (int i = 0; i < hazardList.size(); ++i) {
                    //if (hazards[i].hzType == LASER && hazards[i].isActive == false)
                    if (hazardList[i]->isActive == false)
                    {
                        hazardList[i]->position = position;
                        
                        if (player.position.x > position.x) {
                            hazardList[i]->position.x += 0.25f;
                            //hazardList[i]->velocity = glm::vec3(6.0f, 4.0, 0);
                            hazardList[i]->velocity = glm::vec3(6.0f, 0, 0);
                            hazardList[i]->acceleration = glm::vec3(0.0f, 0, 0);
                            //hazardList[i]->acceleration = glm::vec3(0.0f, -9.8, 0);
                        }
                        else {
                            hazardList[i]->position.x -= 0.25f;
                            //hazardList[i]->velocity = glm::vec3(-6.0f, 4, 0);
                            hazardList[i]->velocity = glm::vec3(-6.0f, 0, 0);
                            hazardList[i]->acceleration = glm::vec3(0.0f, 0, 0);
                            //hazardList[i]->acceleration = glm::vec3(0.0f, -9.8, 0);
                        }
                        Mix_PlayChannel(-1, laser, 0);
                        hazardList[i]->isActive = true;
                        hazardList[i]->isStatic = false;
                        hazardList[i]->textureID = Util::LoadTexture("bullet.png");
                        hazardList[i]->hzState = EXPLODE;
                        hazardList[i]->timer = 1.5f;
                        break;
                        
                    }
                }
                
                aiState = WALKING;
            }
            break;
        }
        case DEAD:
            break;
            
            
    }
}

void Entity::AI(Entity& player, Entity* hazards, int hazard_count, float deltaTime, Map* map) {//, Entity* effects_sprites, int ef_s_size){
    if (isActive && player.isActive) {
        switch (aiType) {
            case SPIKER:
                AISpiker(player, hazards, hazard_count, deltaTime, map);
                break;
            case GUNNER:
                AIGunner(player, hazards, hazard_count, deltaTime, map);
                break;
            case FLY:
                AIFly(player, hazards, hazard_count, deltaTime, map);
                break;
            case BOSS:
                AIBoss(player, hazards, hazard_count, deltaTime, map);
                break;
        }
    }
    if (!player.isActive) {
        velocity = glm::vec3(0, 0, 0);
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
                Mix_PlayChannel(-1, explosion, 0);
                hzState = EXPLODE;
                width = 3;
                height = 2;
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
                timer = 0.10f;
                isActive = true;
            }
            else {
                timer -= deltaTime;
            }
            break;
        case EXPLODE:
            if(timer <= 0.0f){
                timer = 2.0f;
                isActive = false;
                hzState = DEPLOY;
                reflected = false;
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
            if (abs(velocity.x) > 0)
            {
                hzState = EXPLODE;
            }
            break;
        case TICKING:
            break;
        case EXPLODE:
            // std::cout << timer << std::endl;
            //velocity.x += .05f;
            if (timer <= 0.0f) {
                //std :: cout << "FALSE" << std :: endl;
                isActive = false;
                hzState = DEPLOY;
                reflected = false;
            }
            timer -= deltaTime;
            
            break;
    }
}
void Entity::HZCan(Entity player, float deltaTime, Map* map){
    switch (hzState) {
        case DEPLOY:
            if (abs(velocity.x) > 0)
            {
                hzState = EXPLODE;
            }
            break;
        case TICKING:
            break;
        case EXPLODE:
            //std::cout << timer << std::endl;
            //velocity.x += .05f;
            timer -= deltaTime;
            if (timer <= 0.0f) {
                //std :: cout << "FALSE" << std :: endl;
                timer = 1.5f;
                isActive = false;
                hzState = DEPLOY;
                reflected = false;
            }
            
            
            break;
    }
}


void Entity::HZ(Entity& player, float deltaTime, Map* map)
{
    if (isActive && player.isActive) {
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
            case CAN:
                HZCan(player, deltaTime, map);
                break;
        }
    }
    
    if (!player.isActive) {
        velocity = glm::vec3(0, 0, 0);
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
    if (currentAnim == idleLeft || currentAnim == walkLeft || currentAnim == jumpUpLeft || currentAnim == jumpDownLeft || currentAnim == attackLeft || currentAnim == runLeft)
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

void Entity::animate(float deltaTime, Entity* sword)
{
    if (sword->isActive)
    {
        animFrames = 4;
        if (facingLeft())
        {
            currentAnim = attackLeft;
        }
        else
        {
            currentAnim = attack;
        }
    }
    else if (velocity.y > 0 && velocity.x >= 0)
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
        currentAnim = idleLeft;
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

void Entity::animateSw(float deltaTime, Entity* player)
{
    if (isActive == true)
    {
        if (position.x < player->position.x)
        {
            currentAnim = attackLeft;
        }
        else
        {
            currentAnim = attack;
        }
        animTime += deltaTime;
        if (animTime >= 0.075f)
        {
            animTime = 0;
            animIndex++;
            if (animIndex >= animFrames)
            {
                animIndex = 0;
                isActive = false;
            }
        }
    }
}

void Entity::Update(float deltaTime, Entity* objects, int objectCount, Entity* hazards, int hazard_count, Map* map, Entity* enemies, int enemyCount, Entity* sword, Entity* player)
{
    collidedTop = false;
    collidedBottom = false;
    collidedLeft = false;
    collidedRight = false;
    
    velocity += acceleration * deltaTime;
    
    
    
    position.y += velocity.y * deltaTime; // Move on Y
    if (hzType != SPIKE)
    {
        CheckCollisionsY(map);
        CheckCollisionsY(objects, objectCount); // Fix if needed
    }
    
    position.x += velocity.x * deltaTime; // Move on X
    if (hzType != SPIKE)
    {
        CheckCollisionsX(map);
        CheckCollisionsX(objects, objectCount); // Fix if needed
    }
    
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
            //isActive = false;
        }
        animateSw(deltaTime, player);
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
        animate(deltaTime, sword);
        if (isActive == false)
        {
            textureID = Util::LoadTexture("dead.png");
            velocity.x = -3.0f;
        }
        
    }
    
    if (entityType == ENEMY)
    {
        AI(*objects, hazards, hazard_count, deltaTime, map);
        //CheckCollisionsX(&player, 1);
        //CheckCollisionsY(&player, 1);
    }
    if (entityType == HAZARD)
    {
        HZ(*objects, deltaTime, map);
        if (reflected)
        {
            CheckCollisionsX(map);
            CheckCollisionsY(map);
            
            CheckCollisionsX(enemies, enemyCount);
            CheckCollisionsY(enemies, enemyCount);
        }
        
        else{
            CheckCollisionsX(player, 1);
            CheckCollisionsY(player, 1);
        }
        
        
        if (hzType == BOMB || hzType == SPIKE)
        {
            animateHz(deltaTime);
        }
    }
    
}



void Entity::Render(ShaderProgram *program) {
    if (entityType == PLAYER || isActive == true)
    {
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, position);
        if (entityType == HAZARD && hzType == BOMB && hzState == EXPLODE)
        {
            modelMatrix = glm::scale(modelMatrix, scale);
        }
        
        program->SetModelMatrix(modelMatrix);
        
        
        
        if ((entityType == PLAYER && isActive) || entityType == SWORD)
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

