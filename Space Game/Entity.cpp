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
    timer = 50.0f;
    footstep = Mix_LoadWAV("footstep.wav");
}

bool Entity::CheckCollision(Entity& other)
{
    if (isStatic == true) return false;
    if (isActive == false || other.isActive == false) return false;
    
    float xdist = fabs(position.x - other.position.x) - ((width + other.width) / 2.0f);
    float ydist = fabs(position.y - other.position.y) - ((height + other.height) / 2.0f);
    
    if (xdist < 0 && ydist < 0)
    {
        if (entityType == PLAYER && other.entityType == ENEMY)
        {
            isActive = false;
        }
        if (entityType == SWORD && other.entityType == ENEMY)
        {
            std :: cout << "DEATH" << std :: endl;
            other.isActive = false;
        }
        if (entityType == ENEMY && other.entityType == SWORD)
        {
            std :: cout << "DEATH" << std :: endl;
            isActive = false;
        }
        
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
                velocity.y = 0;
                collidedTop = true;
            }
            else if (velocity.y < 0) {
                position.y += penetrationY;
                velocity.y = 0;
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
                velocity.x = 0;
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
        collidedTop = true;
    }
    else if (map->IsSolid(top_left, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }
    else if (map->IsSolid(top_right, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }
    
    if (map->IsSolid(bottom, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
    }
    else if (map->IsSolid(bottom_left, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
    }
    else if (map->IsSolid(bottom_right, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
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
        velocity.y = 4.0f;
        /*if (collidedBottom)
         {
         acceleration.x = 5.2f;
         }*/
    }
}

//IDLE, WALKING, RUNNING, SLOW_DOWN

void Entity::AIPaparazzi(Entity player, Map* map) {
    switch (aiState) {
        case IDLE:
            velocity = glm::vec3(0, 0, 0);
            break;
        case WALKING:
            if (CheckPartialOnLedge(map)) {
                velocity.x *= -1;
                //velocity.x = 0;
            }
            if (glm::distance(position, player.position) < 2.5f) {
                aiState = RUNNING;
                velocity.x = 3.5;
                timer = 7.0f;
            }
            break;
        case RUNNING:
            std::cout << "RUNNING" << std::endl;
            if (CheckPartialOnLedge(map)) {
                Jump();
            }
            timer -= 1.0f;
            if (timer <= 0.0f) {
                timer = 25.0f;
                aiState = SLOW_DOWN;
                velocity.x = 1.25f;
            }
            break;
            //if in their slow down state they are too far off, they will go back to walking
        case SLOW_DOWN:
            std::cout << "SLOW DOWN" << std::endl;
            timer -= 1.0f;
            if (glm::distance(position, player.position) < 5.0f) {
                aiState = WALKING;
            }
            else if (timer <= 0.0f) {
                timer = 7.0f;
                aiState = RUNNING;
                velocity.x = 3.5f;
            }
            if (CheckPartialOnLedge(map)) {
                Jump();
            }
        case AOE:
            break;
    }
}
void Entity::AIJoomba(Entity player, Map* map) {
    switch (aiState) {
        case IDLE:
            velocity = glm::vec3(0, 0, 0);
            break;
        case WALKING:
            if (CheckPartialOnLedge(map)) {
                velocity.x *= -1;
                //velocity.x = 0;
            }
            timer -= 1.0f;
            if (timer <= 0.0f) {
                timer = 25.0f;
                aiState = AOE;
                velocity.x *= 0.001f;
            }
            //velocity.x *= 1;
            
            break;
        case RUNNING:
            break;
        case SLOW_DOWN:
            break;
        case AOE:
            //int temp_size = effects_num;
            int aoe_sprite_left = 2;
            bool left_sprite_done = false;
            bool right_sprite_done = false;
            /*
            for (int i = 0; i < hazards_num; ++i) {
                if (aoe_sprite_left > 0) {
                    if (hazards[i].isActive == false && left_sprite_done == false) {
                        hazards[i].isActive = true;
                        hazards[i].position = glm::vec3(position.x - 1.0f, position.y, 0);
                        left_sprite_done = true;
                    }
                    else if (hazards[i].isActive == false && left_sprite_done == true) {
                        hazards[i].isActive = true;
                        hazards[i].position = glm::vec3(position.x + 1.0f, position.y, 0);
                        right_sprite_done = true;
                    }
                    aoe_sprite_left -= 1;
                }
                else {
                    break;
                }
            }
            
            timer -= 1.0f;
            if (timer <= 0.0f) {
                timer = 100.0f + rand() % 200 + 1;
                aiState = WALKING;
                velocity.x *= 1000.0f;
                for (int i = 0; i < hazards_num; ++i) {
                    hazards[i].isActive = false;
                }
            }
            break;
             */
    }
}

void Entity::AIFly(Entity player, Entity* hazards, int hazard_count, Map* map) {
    switch (aiState) {
        case IDLE:
            if (glm::distance(position, player.position) < 3.0f && hazards[0].isActive == false) {
                aiState = WALKING;
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
                    hazards[0].position = position;
                    hazards[0].isActive = true;
                }
            }
            
            aiState = IDLE;
            break;
    }
}

void Entity::AISpiker(Entity player,Entity* hazards, int hazard_count, Map* map) {
    switch (aiState) {
        case IDLE:
            velocity = glm::vec3(0.0f, 0, 0);
            break;
        case WALKING:
            if (CheckPartialOnLedge(map)) {
                velocity.x *= -1;
            }
            if(abs(velocity.y) > 0.0f){
                velocity.x = 0.0f;
            }
            else if(position.x < player.position.x){
                velocity.x = 1.0f;
            }
            else{
                velocity.x = -1.0f;
            }
            if ((glm::distance(position, player.position) < 2.5f) && timer == 50.0f){
                aiState = ATTACK;
                timer -= 1.0f;
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
            
            for(int i = 0; i < hazard_count; ++i){
                if(hazards[i].hzType == SPIKE && hazards[i].isActive == false){
                    hazards[i].position = player.position;
                    hazards[i].isActive = true;
                    hazards[i].isStatic = true;
                    hazards[i].textureID = Util::LoadTexture("papa_evil.png");
                    hazards[i].hzState = TICKING;
                    hazards[i].timer = 50.0f;
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
    switch (aiType) {
        case PAPARAZZI:
            AIPaparazzi(player,  map);
            break;
        case JOOMBA:
            AIJoomba(player, map);
            break;
        case SPIKER:
            AISpiker(player, hazards,hazard_count, map);
            break;
        case GUNNER:
            AISpiker(player, hazards,hazard_count, map);
            break;
        case FLY:
            AIFly(player, hazards,hazard_count, map);
            break;
        case BOSS:
            AIBoss(player, hazards,hazard_count, map);
            break;
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
                hzState = EXPLODE;
                timer = 10.0f;
            }
            else
            {
                timer = timer - deltaTime;
            }
            
            break;
        case EXPLODE:
            isActive = false;
            break;
    }
}

void Entity::HZSpike(Entity player, float deltaTime, Map* map)
{
    switch (hzState) {
        case DEPLOY:
            break;
        case TICKING:
            if (timer <= 0)
            {
                hzState = EXPLODE;
                timer = 100.0f;
                textureID = Util::LoadTexture("papa_evil.png");
                isActive = true;
            }
            
            else if((int) timer % 10 == 0){
                std :: cout << "change" << std :: endl;
                textureID = Util::LoadTexture("me.png");
            }
            
            else{
                textureID = Util::LoadTexture("orange.png");
            }

            timer -= deltaTime;

            break;
        case EXPLODE:
            std :: cout << "BOOM" << std :: endl;
            if(timer <= 0.0f){
                isActive = false;
            }
            timer -= deltaTime;
            
            break;
    }
}

void Entity::HZ(Entity& player, float deltaTime, Map* map)
{
    switch (hzType) {
        case SPIKE:
            HZSpike(player, deltaTime, map);
            break;
        case LASER:
            //AIJoomba(player, map);
            break;
        case BOMB:
            HZBomb(player, deltaTime, map);
            break;
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
            
            position.x = objects -> position.x + 0.5f;
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
        
        if(timer > 0 && timer < 50.0f){
            timer-=1.0f;
        }
        else if(timer <= 0.0f){
            timer = 50.0f;
        }
    }
    
    if (entityType == ENEMY)
    {
        AI(*objects, hazards, hazard_count, map);
        //CheckCollisionsX(&player, 1);
        //CheckCollisionsY(&player, 1);
    }
    if (entityType == HAZARD)
    {
        HZ(*objects, deltaTime, map);
    }
    
    animate(deltaTime);
}



void Entity::Render(ShaderProgram *program) {
    if (isActive == true)
    {
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, position);
        program->SetModelMatrix(modelMatrix);
        
        
        
        if (entityType == PLAYER)
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

