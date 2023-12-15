/**
* Author: Amy Fouzia
* Assignment: Platformer
* Date due: 2023-12-02, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#define GL_SILENCE_DEPRECATION
#define GL_GLEXT_PROTOTYPES 1
#define FIXED_TIMESTEP 0.0166666f
#define LEVEL1_WIDTH 20
#define LEVEL1_HEIGHT 10
#define LEVEL1_LEFT_EDGE 0.0f
#define LEVEL1_RIGHT_EDGE 19.0f


#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "stb_image.h"
#include "cmath"
#include <ctime>
#include <vector>
#include "Entity.h"
#include "Map.h"
#include "Utility.h"
#include "Scene.h"
#include "LevelA.h"
#include "LevelB.h"
#include "LevelC.h"
#include "Menu.h"
#include "Effects.h"

const int WINDOW_WIDTH = 640,
WINDOW_HEIGHT = 640;

const float BG_RED = 0.1922f,
BG_BLUE = 0.549f,
BG_GREEN = 0.9059f,
BG_OPACITY = 1.0f;

const int VIEWPORT_X = 0,
VIEWPORT_Y = 0,
VIEWPORT_WIDTH = WINDOW_WIDTH,
VIEWPORT_HEIGHT = WINDOW_HEIGHT;

const char V_SHADER_PATH[] = "shaders/vertex_textured.glsl",
F_SHADER_PATH[] = "shaders/fragment_textured.glsl";

const float MILLISECONDS_IN_SECOND = 1000.0;


// ––––– GLOBAL VARIABLES ––––– //
Scene* g_current_scene;

Menu* g_levelM;
LevelA* g_levelA;
LevelB* g_levelB;
LevelC* g_levelC;

Effects* g_effects;
Scene* g_levels[4];

SDL_Window* g_display_window;
bool g_game_is_running = true;

ShaderProgram g_shader_program;
glm::mat4 g_view_matrix, g_projection_matrix;

float g_previous_ticks = 0.0f;
float g_accumulator = 0.0f;

bool g_is_colliding_bottom = false;

bool start = false;
int lives = 1;

// ––––– GENERAL FUNCTIONS ––––– //
void switch_to_scene(Scene* scene)
{
    g_current_scene = scene;
    g_current_scene->initialise(); // DON'T FORGET THIS STEP!
}

void initialise()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    g_display_window = SDL_CreateWindow("Platformer",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL);

    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);
    SDL_GL_MakeCurrent(g_display_window, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);

    g_shader_program.load(V_SHADER_PATH, F_SHADER_PATH);

    g_view_matrix = glm::mat4(1.0f);
    g_projection_matrix = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, -1.0f, 1.0f);

    g_shader_program.set_projection_matrix(g_projection_matrix);
    g_shader_program.set_view_matrix(g_view_matrix);

    glUseProgram(g_shader_program.get_program_id());

    glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);

    // enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    g_levelM = new Menu();
    g_levelA = new LevelA();
    g_levelB = new LevelB();
    g_levelC = new LevelC();
    
    g_levels[0] = g_levelM;
    g_levels[1] = g_levelA;
    g_levels[2] = g_levelB;
    g_levels[3] = g_levelC;

    // Start at Menu
    switch_to_scene(g_levels[0]);

    g_effects = new Effects(g_projection_matrix, g_view_matrix);
    g_effects->start(SHRINK, 2.0f);
}

void process_input()
{
    // VERY IMPORTANT: If nothing is pressed, we don't want to go anywhere
    g_current_scene->m_state.player->set_movement_x(0.0f);
    g_current_scene->m_state.player2->set_movement_x(0.0f);

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type) {
            // End game
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            g_game_is_running = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_q:
                // Quit the game with a keystroke
                g_game_is_running = false;
                break;

            case SDLK_s:
                // Start the game with a keystroke
                start = true;
                switch_to_scene(g_levels[3]); 
                break;

            case SDLK_w:
                // Jump
                if (g_current_scene->m_state.player->m_collided_bottom_map && start && lives > 0)
                {
                    g_current_scene->m_state.player->m_is_jumping = true;
                    Mix_PlayChannel(-1, g_current_scene->m_state.jump_sfx, 0);
                }
                break;

            case SDLK_UP:
                // Jump
                if (g_current_scene->m_state.player2->m_collided_bottom_map && start && lives > 0)
                {
                    g_current_scene->m_state.player2->m_is_jumping = true;
                    Mix_PlayChannel(-1, g_current_scene->m_state.jump_sfx, 0);
                }
                break;

            default:
                break;
            }

        default:
            break;
        }
    }

    const Uint8* key_state = SDL_GetKeyboardState(NULL);

    if (key_state[SDL_SCANCODE_A] && start && lives > 0)
    {
        g_current_scene->m_state.player->move_left();
        g_current_scene->m_state.player->m_animation_indices = g_current_scene->m_state.player->m_walking[g_current_scene->m_state.player->LEFT];
    }
    else if (key_state[SDL_SCANCODE_D] && start && lives > 0)
    {
        g_current_scene->m_state.player->move_right();
        g_current_scene->m_state.player->m_animation_indices = g_current_scene->m_state.player->m_walking[g_current_scene->m_state.player->RIGHT];
    }

    if (key_state[SDL_SCANCODE_LEFT] && start && lives > 0)
    {
        g_current_scene->m_state.player2->move_left();
        g_current_scene->m_state.player2->m_animation_indices = g_current_scene->m_state.player2->m_walking[g_current_scene->m_state.player2->LEFT];
    }
    else if (key_state[SDL_SCANCODE_RIGHT] && start && lives > 0)
    {
        g_current_scene->m_state.player2->move_right();
        g_current_scene->m_state.player2->m_animation_indices = g_current_scene->m_state.player2->m_walking[g_current_scene->m_state.player2->RIGHT];
    }

    if (glm::length(g_current_scene->m_state.player->get_movement()) > 1.0f)
    {
        g_current_scene->m_state.player->set_movement(glm::normalize(g_current_scene->m_state.player->get_movement()));
    }
    if (glm::length(g_current_scene->m_state.player2->get_movement()) > 1.0f)
    {
        g_current_scene->m_state.player2->set_movement(glm::normalize(g_current_scene->m_state.player2->get_movement()));
    }

}

void update()
{
    float ticks = (float)SDL_GetTicks() / MILLISECONDS_IN_SECOND;
    float delta_time = ticks - g_previous_ticks;
    g_previous_ticks = ticks;

    delta_time += g_accumulator;

    if (delta_time < FIXED_TIMESTEP)
    {
        g_accumulator = delta_time;
        return;
    }

    while (delta_time >= FIXED_TIMESTEP) {
        g_current_scene->update(FIXED_TIMESTEP);
        g_effects->update(FIXED_TIMESTEP);

        delta_time -= FIXED_TIMESTEP;
    }

    g_accumulator = delta_time;

    // midpoint btn players
    glm::vec3 midpoint = 0.5f * (g_current_scene->m_state.player->get_position() + g_current_scene->m_state.player2->get_position());

    // distance btn players
    float distance = glm::abs(g_current_scene->m_state.player->get_position().x - g_current_scene->m_state.player2->get_position().x);

    // camera width 
    float desired_camera_width = glm::max(10.0f, distance + 5.0f);
    float max_allowed_camera_width = LEVEL1_RIGHT_EDGE - LEVEL1_LEFT_EDGE;
    desired_camera_width = glm::min(desired_camera_width, max_allowed_camera_width);

    // dont go too left
    if (midpoint.x - desired_camera_width * 0.5f <= LEVEL1_LEFT_EDGE) {
        midpoint.x = LEVEL1_LEFT_EDGE + desired_camera_width * 0.5f;
    }
    // dont go too right
    else if (midpoint.x + desired_camera_width * 0.5f >= LEVEL1_RIGHT_EDGE) {
        midpoint.x = LEVEL1_RIGHT_EDGE - desired_camera_width * 0.5f;
    }

    // update the projection matrix
    g_projection_matrix = glm::ortho(-desired_camera_width * 0.5f, desired_camera_width * 0.5f, -5.0f, 5.0f, -1.0f, 1.0f);
    g_shader_program.set_projection_matrix(g_projection_matrix);

    // update the view matrix to midpoint
    g_view_matrix = glm::mat4(1.0f);
    g_view_matrix = glm::translate(g_view_matrix, glm::vec3(-midpoint.x, 5.0, 0));

    //switch levels
    if (g_current_scene == g_levelA && g_current_scene->m_state.finished) switch_to_scene(g_levelB);

    if (g_current_scene == g_levelB && g_current_scene->m_state.finished) switch_to_scene(g_levelC);

    // won game
    if (g_current_scene == g_levelC && g_current_scene->m_state.finished) {
        start = false;
    }

    // lose a life and reset position
    if (g_current_scene->m_state.fell) {
        g_current_scene->m_state.fell = false;
        if (g_current_scene == g_levelA) {
            g_current_scene->m_state.player->set_position(glm::vec3(1.0f, -9.0f, 0.0f));
            g_current_scene->m_state.player2->set_position(glm::vec3(1.0f, -7.0f, 0.0f));
        }
        if (g_current_scene == g_levelB) {
            g_current_scene->m_state.player->set_position(glm::vec3(9.0f, -9.0f, 0.0f));
            g_current_scene->m_state.player2->set_position(glm::vec3(17.0f, -9.0f, 0.0f));
        }
        if (g_current_scene == g_levelC) {
            g_current_scene->m_state.player->set_position(glm::vec3(18.0f, -9.0f, 0.0f));
            g_current_scene->m_state.player2->set_position(glm::vec3(1.0f, -9.0f, 0.0f));
        }
        lives -= 1;
    }

    // gain a life
    if (g_current_scene->m_state.player->absorbed && g_current_scene->m_state.player2->absorbed) {
        g_current_scene->m_state.player->absorbed = false;
        g_current_scene->m_state.player2->absorbed = false;
        lives += 1;
    }

    g_view_matrix = glm::translate(g_view_matrix, g_effects->m_view_offset);
}

void render()
{
    g_shader_program.set_view_matrix(g_view_matrix);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(g_shader_program.get_program_id());
    g_current_scene->render(&g_shader_program);
    g_effects->render();

    // lives
    GLuint font_texture_id_1 = Utility::load_texture("assets/fonts/font1.png");
    std::string livesStr = std::to_string(lives);
    Utility::draw_text(&g_shader_program, font_texture_id_1, livesStr, 0.3f, -0.25f, glm::vec3(g_current_scene->m_state.player->get_position().x, g_current_scene->m_state.player->get_position().y + 0.5, 0.0f));
    Utility::draw_text(&g_shader_program, font_texture_id_1, livesStr, 0.3f, -0.25f, glm::vec3(g_current_scene->m_state.player2->get_position().x, g_current_scene->m_state.player2->get_position().y + 0.5, 0.0f));

    // won game
    if (g_current_scene == g_levelC && g_current_scene->m_state.finished) {
        Utility::draw_text(&g_shader_program, font_texture_id_1, "you win!!!", 0.5f, -0.25f, glm::vec3(12.0f, -4.0f, 0.0f));
    }

    // lost game
    if (lives == 0) {
        Utility::draw_text(&g_shader_program, font_texture_id_1, "you lose :(", 0.5f, -0.25f, glm::vec3(g_current_scene->m_state.player->get_position().x, -4.0f, 0.0f));
    }

    SDL_GL_SwapWindow(g_display_window);
}

void shutdown()
{
    SDL_Quit();

    delete g_levelA;
    delete g_levelB;
    delete g_levelC;
    delete g_levelM;
    delete g_effects;
}

// ––––– DRIVER GAME LOOP ––––– //
int main(int argc, char* argv[])
{
    initialise();

    while (g_game_is_running)
    {
        process_input();
        update();

        if (g_current_scene->m_state.next_scene_id >= 0) switch_to_scene(g_levels[g_current_scene->m_state.next_scene_id]);

        render();
    }

    shutdown();
    return 0;
}