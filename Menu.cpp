#include "Menu.h"
#include "Utility.h"

#define LEVEL_WIDTH 20
#define LEVEL_HEIGHT 11

unsigned int Menu_DATA[] =
{
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
};


Menu::~Menu()
{
    delete[]  m_state.enemies;
    delete    m_state.player;
    delete    m_state.player2;
    delete    m_state.map;
    Mix_FreeChunk(m_state.jump_sfx);
    Mix_FreeMusic(m_state.bgm);
}

void Menu::initialise()
{
    m_state.next_scene_id = -1;

    GLuint map_texture_id = Utility::load_texture("assets/images/element_tiles.png");
    m_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, Menu_DATA, map_texture_id, 1.0f, 6, 1);

    //background
    m_state.background = new Entity();
    m_state.background->set_entity_type(BACKGROUND);
    m_state.background->m_texture_id = Utility::load_texture("assets/images/background.png");
    m_state.background->set_position(glm::vec3(10.0, -5.0f, 0.0f));

    // door
    m_state.door = new Entity();
    m_state.door->set_position(glm::vec3(10.0f, -2.0f, 0.0f));
    m_state.door->m_texture_id = Utility::load_texture("assets/images/door.png");

    // Existing
    m_state.player = new Entity();
    m_state.player->set_entity_type(PLAYER);
    m_state.player->set_position(glm::vec3(5.0f, 0.0f, 0.0f));
    m_state.player->set_movement(glm::vec3(0.0f));
    m_state.player->set_speed(2.5f);
    m_state.player->set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    m_state.player->m_texture_id = Utility::load_texture("assets/images/watergirl.png");

    m_state.player2 = new Entity();
    m_state.player2->set_entity_type(PLAYER);
    m_state.player2->set_position(glm::vec3(5.0f, 0.0f, 0.0f));
    m_state.player2->set_movement(glm::vec3(0.0f));
    m_state.player2->set_speed(2.5f);
    m_state.player2->set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    m_state.player2->m_texture_id = Utility::load_texture("assets/images/fireboy.png");


    /* Enemies' stuff */
    GLuint enemy_texture_id = Utility::load_texture("assets/images/waterSlime.png");

    m_state.enemies = new Entity[ENEMY_COUNT];
    m_state.enemies[0].set_entity_type(ENEMY);
    m_state.enemies[0].set_ai_type(GUARD);
    m_state.enemies[0].set_ai_state(IDLE);
    m_state.enemies[0].m_texture_id = enemy_texture_id;
    m_state.enemies[0].set_position(glm::vec3(8.0f, 0.0f, 0.0f));
    m_state.enemies[0].set_movement(glm::vec3(0.0f));
    m_state.enemies[0].set_speed(1.0f);
    m_state.enemies[0].set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

    m_state.bgm = Mix_LoadMUS("assets/audio/dooblydoo.mp3");
    Mix_PlayMusic(m_state.bgm, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 16.0f);

    m_state.jump_sfx = Mix_LoadWAV("assets/audio/bounce.wav");
}

void Menu::update(float delta_time)
{
    m_state.player->update(delta_time, m_state.player, m_state.enemies, ENEMY_COUNT, m_state.map);
    m_state.player2->update(delta_time, m_state.player2, m_state.enemies, ENEMY_COUNT, m_state.map);
}

void Menu::render(ShaderProgram* program)
{
    m_state.map->render(program);
    GLuint font_texture_id_1 = Utility::load_texture("assets/fonts/font1.png");
    Utility::draw_text(program, font_texture_id_1, "Fireboy & Watergirl: Stone Temple", 0.5f, -0.25f, glm::vec3(1.2f, -4.5f, 0.0f));
    Utility::draw_text(program, font_texture_id_1, "Press S to start", 0.5f, -0.25f, glm::vec3(3.0f, -5.5f, 0.0f));
}
