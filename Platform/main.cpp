#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <vector>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_mixer.h>
#include <string>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#include "Util.h"

#include "Scene.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "StartScreen.h"

#include "Effects.h"
#include "Entity.h"
#include "Map.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

#define ENEMY_COUNT 1

GLuint fontTextureID;

Scene* currentScene;
Scene* sceneList[2];

Mix_Music* music;

Mix_Chunk* jump_1;
Mix_Chunk* jump_2;

Mix_Chunk* footstep;
Mix_Chunk* slash;

Mix_Chunk* fall_1;

Effects* effects;

int lives;


void SwitchToScene(Scene* scene) {
    currentScene = scene;
    currentScene->Initialize();
}


void Initialize() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("Jam de Space!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    
    music = Mix_LoadMUS("music.mp3");
    Mix_PlayMusic(music, -1);
	Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
    
    jump_1 = Mix_LoadWAV("jump_1.wav");
    jump_2 = Mix_LoadWAV("jump_2.wav");
    
    fall_1 = Mix_LoadWAV("fall_1.wav");
    
    footstep = Mix_LoadWAV("footstep.wav");
	//slash = Mix_LoadWAV("slash.wav");
    
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    fontTextureID = Util::LoadTexture("font.png");
    
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    program.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    
    glUseProgram(program.programID);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    
    glClearColor(0.3f, 0.2f, 0.2f, 0.6f);
    
    lives = 3;
    
    sceneList[0] = new StartScreen();
    sceneList[1] = new Level1();
    //sceneList[2] = new Level2();
    //sceneList[3] = new Level3();
    SwitchToScene(sceneList[0]);
    
    effects = new Effects(projectionMatrix, viewMatrix);
    //effects->Start(SHRINK, 5.0f);
}

void ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_SPACE:
                    {
						if (currentScene->state.player.isActive)
						{
							currentScene->state.player.Jump();
						}
                        
                    }
                        break;
                    
                    case SDLK_f:
						if (currentScene->state.player.isActive)
						{
							currentScene->state.player.Attack(&currentScene->state.sword);
						}
                        /*if(currentScene->state.player.timer == 50.0f){
							currentScene->state.player.attacking = true;
                            currentScene->state.player.timer -= 1.0f;
                            currentScene->state.sword.timer -= 1.0f;
                            currentScene->state.sword.isActive = true;
                            currentScene->state.sword.position = currentScene -> state.player.position;
							if (currentScene->state.player.facingLeft())
							{
								currentScene->state.sword.position.x -= 0.8f;
							}
							else
							{
								currentScene->state.sword.position.x += 0.8f;
							}
                        }*/
                        
                        break;
                        
                    case SDLK_RETURN:
                        if (currentScene == sceneList[0])
                        {
                            currentScene->state.nextLevel = 1;
                            SwitchToScene(sceneList[currentScene->state.nextLevel]);
                        }
                        break;
                        
                    case SDLK_k:
                        //currentScene->state.player.isActive = false;
                        effects->Start(SHAKE, 2.0f);
                        break;

					case SDLK_r:
						sceneList[1] = new Level1();
						currentScene->state.nextLevel = 1;
						SwitchToScene(sceneList[currentScene->state.nextLevel]);
						break;
                }
                break;
        }
    }
    
	if (currentScene->state.player.isActive)
	{
		currentScene->state.player.velocity.x = 0;
	}
	
    //currentScene->state.player.velocity.x -= currentScene->state.player.velocity.x/2;
    // Check for pressed/held keys below
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if (currentScene->state.player.isActive)
	{
		if (keys[SDL_SCANCODE_A] && keys[SDL_SCANCODE_LSHIFT])
		{
			currentScene->state.player.velocity.x = -5.0f;
		}
		else if (keys[SDL_SCANCODE_D] && keys[SDL_SCANCODE_LSHIFT])
		{
			currentScene->state.player.velocity.x = 5.0f;
		}
		else if (keys[SDL_SCANCODE_A])
		{
			currentScene->state.player.velocity.x = -3.0f;
		}
		else if (keys[SDL_SCANCODE_D])
		{
			currentScene->state.player.velocity.x = 3.0f;
		}
	}
    
}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    
    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }
    
    while (deltaTime >= FIXED_TIMESTEP) {
        
        currentScene->Update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;
    }
    
    accumulator = deltaTime;
    
    effects->Update(FIXED_TIMESTEP);
    
    //if (currentScene->state.player.currentAnim == currentScene->state.player.walkRight);
    //{
    //    Mix_PlayChannel(-1, footstep, 0);
    //}
    
    if (currentScene->state.player.isActive == false)
    {
        lives = lives - 1;
    }
    if (currentScene->state.player.isActive == false)
    {
        currentScene->state.player.isActive = true;
    }
    
    
    viewMatrix = glm::mat4(1.0f);
    if (currentScene->state.player.position.x > 5) {
        viewMatrix = glm::translate(viewMatrix,
                                    glm::vec3(-currentScene->state.player.position.x, 3.75, 0));
    }
    else {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 3.75, 0));
    }
    viewMatrix = glm::translate(viewMatrix, effects->viewTranslate);
    
}


void Render() {
    glUseProgram(program.programID);
    
    glClear(GL_COLOR_BUFFER_BIT);
    program.SetViewMatrix(viewMatrix);
    
    currentScene->Render(&program);
    
    if (currentScene == sceneList[0])
    {
        Util::DrawText(&program, fontTextureID, "Space Game", 0.4f, 0.1f, glm::vec3(0.8f, -2, 0));
        Util::DrawText(&program, fontTextureID, "press enter to start", 0.3f, 0.1f, glm::vec3(1.0f, -4, 0));
    }
    std::string s = std::to_string(lives);
    if (currentScene->state.player.position.x > 5) {
        Util::DrawText(&program, fontTextureID, s, 1.0f, 0.1f, glm::vec3(currentScene->state.player.position.x - 4.5f, -0.5, 0));
    }
    else {
        Util::DrawText(&program, fontTextureID, s, 1.0f, 0.1f, glm::vec3(0.5f, -0.5, 0));
    }
    //program.SetLightPosition(currentScene->state.player.position);
    program.SetLightPosition2(glm::vec3(8, 0, 0));
    program.SetLightPosition2(glm::vec3(8,-5,0));
    effects->Render();
    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
    Mix_FreeMusic(music);
    Mix_FreeChunk(jump_1);
    Mix_FreeChunk(jump_2);
    Mix_FreeChunk(fall_1);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();
    
    while (gameIsRunning) {
        if (currentScene->state.nextLevel >= 0) SwitchToScene(sceneList[currentScene->state.nextLevel]);
        
        ProcessInput();
        Update();
        Render();
    }
    
    Shutdown();
    return 0;
}
