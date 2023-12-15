#include "LevelC.h"
#include "Utility.h"

#define LEVEL_WIDTH 20
#define LEVEL_HEIGHT 11

unsigned int LEVELC_DATA[] = {
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 1, 1, 1, 1, 3, 3, 0, 0, 0, 3, 2, 2, 2, 2, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 3,
    3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 0, 0, 0, 3, 3, 3, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 3, 3, 2, 2, 3, 3, 3, 0, 0, 0, 3, 3, 3, 3, 1, 1, 3, 3, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
};

LevelC::~LevelC()
{
    delete[] m_state.enemies;
    delete    m_state.player;
    delete    m_state.map;
    Mix_FreeChunk(m_state.jump_sfx);
    Mix_FreeMusic(m_state.bgm);
}

void LevelC::initialise()
{
    m_state.next_scene_id = -1;

    GLuint map_texture_id = Utility::load_texture("assets/images/element_tiles.png");
    m_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELC_DATA, map_texture_id, 1.0f, 6, 1);

    //background
    m_state.background = new Entity();
    m_state.background->set_entity_type(BACKGROUND);
    m_state.background->m_texture_id = Utility::load_texture("assets/images/background.png");
    m_state.background->set_position(glm::vec3(10.0, -5.0f, 0.0f));

    //door
    m_state.door = new Entity();
    m_state.door->set_position(glm::vec3(10.0f, -3.0f, 0.0f));
    m_state.door->m_texture_id = Utility::load_texture("assets/images/door.png");

    //platforms & enemies & boxes (collidable entities)
    m_state.enemies = new Entity[ENEMY_COUNT];

    for (int i = 0; i < PLATFORM_COUNT; i++) {
        m_state.enemies[i].set_entity_type(PLATFORM);
        m_state.enemies[i].m_texture_id = Utility::load_texture("assets/images/platform.png");
        m_state.enemies[i].set_movement(glm::vec3(0.0f, 0.0f, 0.0f));
        m_state.enemies[i].set_speed(1.0f);
    }

    for (int i = PLATFORM_COUNT; i < ENEMY_COUNT; i++) {
        m_state.enemies[i].set_entity_type(ENEMY);
        m_state.enemies[i].set_movement(glm::vec3(1.0f, 0.0f, 0.0f));
        m_state.enemies[i].set_speed(1.0f);
        m_state.enemies[i].set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    }

    m_state.enemies[2].set_position(glm::vec3(5.0f, -9.0f, 0.0f));
    m_state.enemies[2].set_element_type(WATER);
    m_state.enemies[2].m_texture_id = Utility::load_texture("assets/images/waterSlime.png");

    m_state.enemies[3].set_position(glm::vec3(13.0f, -9.0f, 0.0f));
    m_state.enemies[3].set_element_type(LAVA);
    m_state.enemies[3].m_texture_id = Utility::load_texture("assets/images/lavaSlime.png");

    //buttons
    m_state.buttons = new Entity[BUTTON_COUNT];

    for (int i = 0; i < BUTTON_COUNT; i++) {
        m_state.buttons[i].m_texture_id = Utility::load_texture("assets/images/button.png");
    }

    m_state.buttons[0].set_position(glm::vec3(2.0f, -5.0f, 0.0f));
    m_state.buttons[1].set_position(glm::vec3(12.0f, -1.0f, 0.0f));

    // watergirl
    m_state.player = new Entity();
    m_state.player->set_entity_type(PLAYER);
    m_state.player->set_element_type(WATER);
    m_state.player->set_position(glm::vec3(18.0f, -9.0f, 0.0f));
    m_state.player->set_movement(glm::vec3(0.0f));
    m_state.player->set_speed(2.5f);
    m_state.player->set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    m_state.player->m_texture_id = Utility::load_texture("assets/images/watergirl.png");

    // fireboy
    m_state.player2 = new Entity();
    m_state.player2->set_entity_type(PLAYER);
    m_state.player2->set_element_type(LAVA);
    m_state.player2->set_position(glm::vec3(1.0f, -9.0f, 0.0f));
    m_state.player2->set_movement(glm::vec3(0.0f));
    m_state.player2->set_speed(2.5f);
    m_state.player2->set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    m_state.player2->m_texture_id = Utility::load_texture("assets/images/fireboy.png");

    // Walking
    m_state.player->m_walking[m_state.player->LEFT] = new int[4] { 1, 5, 9, 13 };
    m_state.player->m_walking[m_state.player->RIGHT] = new int[4] { 3, 7, 11, 15 };
    m_state.player->m_walking[m_state.player->UP] = new int[4] { 2, 6, 10, 14 };
    m_state.player->m_walking[m_state.player->DOWN] = new int[4] { 0, 4, 8, 12 };

    m_state.player->m_animation_indices = m_state.player->m_walking[m_state.player->RIGHT];
    m_state.player->m_animation_frames = 4;
    m_state.player->m_animation_index = 0;
    m_state.player->m_animation_time = 0.0f;
    m_state.player->m_animation_cols = 4;
    m_state.player->m_animation_rows = 4;
    m_state.player->set_height(0.8f);
    m_state.player->set_width(0.8f);

    m_state.player2->m_walking[m_state.player2->LEFT] = new int[4] { 1, 5, 9, 13 };
    m_state.player2->m_walking[m_state.player2->RIGHT] = new int[4] { 3, 7, 11, 15 };
    m_state.player2->m_walking[m_state.player2->UP] = new int[4] { 2, 6, 10, 14 };
    m_state.player2->m_walking[m_state.player2->DOWN] = new int[4] { 0, 4, 8, 12 };

    m_state.player2->m_animation_indices = m_state.player2->m_walking[m_state.player2->RIGHT];
    m_state.player2->m_animation_frames = 4;
    m_state.player2->m_animation_index = 0;
    m_state.player2->m_animation_time = 0.0f;
    m_state.player2->m_animation_cols = 4;
    m_state.player2->m_animation_rows = 4;
    m_state.player2->set_height(0.8f);
    m_state.player2->set_width(0.8f);

    // Jumping
    m_state.player->m_jumping_power = 7.0f;
    m_state.player2->m_jumping_power = 7.0f;

    /**
     BGM and SFX
     */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

    m_state.bgm = Mix_LoadMUS("assets/audio/dooblydoo.mp3");
    Mix_PlayMusic(m_state.bgm, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 16.0f);

    m_state.jump_sfx = Mix_LoadWAV("assets/audio/bounce.wav");
}

void LevelC::update(float delta_time)
{
    m_state.background->update(delta_time, m_state.player, m_state.player, 1, m_state.map);
    m_state.door->update(delta_time, m_state.player, m_state.player, 1, m_state.map);
    for (int i = 0; i < ENEMY_COUNT; i++) { m_state.enemies[i].update(delta_time, m_state.door, m_state.door, 1, m_state.map); }
    for (int i = 0; i < BUTTON_COUNT; i++) { m_state.buttons[i].update(delta_time, m_state.player, m_state.player, 1, m_state.map); }
    m_state.player->update(delta_time, m_state.player, m_state.enemies, ENEMY_COUNT, m_state.map);
    m_state.player2->update(delta_time, m_state.player2, m_state.enemies, ENEMY_COUNT, m_state.map);

    // girl lava puddles
    if ((m_state.player->get_position().y == -1.1f && (m_state.player->get_position().x > 12.5f && m_state.player->get_position().x < 16.5f)) ||
        (m_state.player->get_position().y == -5.1f && (m_state.player->get_position().x > 2.5f && m_state.player->get_position().x < 4.5f))

        ) {
        m_state.fell = true;
    }

    // fireboy water puddles
    if ((m_state.player2->get_position().y == -1.1f && (m_state.player2->get_position().x > 2.5f && m_state.player2->get_position().x < 6.5f)) ||
        (m_state.player2->get_position().y == -5.1f && (m_state.player2->get_position().x > 14.5f && m_state.player2->get_position().x < 16.5f))

        ) {
        m_state.fell = true;
    }

    //button press
    if ((m_state.player->get_position().y == -5.1f && (m_state.player->get_position().x > 1.8f && m_state.player->get_position().x < 2.2f)) ||
        (m_state.player2->get_position().y == -5.1f && (m_state.player2->get_position().x > 1.8f && m_state.player2->get_position().x < 2.2f))
        ) {
        m_state.enemies[1].move_up();
        //std::cout << "\n up";

    }
    else if (m_state.enemies[1].get_position().y > -2.0) {
        m_state.enemies[1].move_down();
        //std::cout << "\n down";
    }
    else {
        m_state.enemies[1].set_position(glm::vec3(18.0, -4.0, 0.0));
        //std::cout << "\n reset";
    }

    if ((m_state.player->get_position().y == -1.1f && (m_state.player->get_position().x > 11.8f && m_state.player->get_position().x < 12.2f)) ||
        (m_state.player2->get_position().y == -1.1f && (m_state.player2->get_position().x > 11.8f && m_state.player2->get_position().x < 12.2f))
        ) {
        m_state.enemies[0].move_up();
        std::cout << "\n up";
    }
    else if (m_state.enemies[0].get_position().y > -2.0) {
        m_state.enemies[0].move_down();
        std::cout << "\n down";
    }
    else {
        m_state.enemies[0].set_position(glm::vec3(1.0, -4.0, 0.0));
        std::cout << "\n reset";
    }

    // move patrolling enemies
    if (m_state.enemies[2].get_position().x <= 2.0) {
        m_state.enemies[2].move_right();
    }
    if (m_state.enemies[2].get_position().x >= 7.0) {
        m_state.enemies[2].move_left();
    }
    if (m_state.enemies[3].get_position().x <= 11.0) {
        m_state.enemies[3].move_right();
    }
    if (m_state.enemies[3].get_position().x >= 17.0) {
        m_state.enemies[3].move_left();
    }
    if (m_state.enemies[4].get_position().x <= 15.8) {
        m_state.enemies[4].move_right();
    }
    if (m_state.enemies[4].get_position().x >= 17.0) {
        m_state.enemies[4].move_left();
    }
    if (m_state.enemies[5].get_position().x <= 2.8) {
        m_state.enemies[5].move_right();
    }
    if (m_state.enemies[5].get_position().x >= 4.0) {
        m_state.enemies[5].move_left();
    }

    // player ran into enemy
    // if opposite elements, reset and lose life
    // if same element, enemy dies and gain absorbed status
    // if both players have absorbed status, gain a life
    if (m_state.player->m_collided_enemy) {
        if (m_state.player->m_collided_opposite) { m_state.fell = true; }

        if (m_state.player->m_collided_same) {
            m_state.enemies[2].set_position(glm::vec3(30.0f));
            m_state.player->absorbed = true;
        }
    }
    if (m_state.player2->m_collided_enemy) {
        if (m_state.player2->m_collided_opposite) { m_state.fell = true; }

        if (m_state.player2->m_collided_same) {
            m_state.enemies[3].set_position(glm::vec3(30.0f));
            m_state.player2->absorbed = true;
        }
    }

    // players both reached door
    if (m_state.player->get_position().y == -3.1f && (m_state.player->get_position().x > 9.5f && m_state.player->get_position().x < 10.1f) &&
        m_state.player2->get_position().y == -3.1f && (m_state.player2->get_position().x > 9.5f && m_state.player2->get_position().x < 10.1f)) m_state.finished = true;
}

void LevelC::render(ShaderProgram* program)
{
    m_state.background->render(program);
    m_state.map->render(program);
    m_state.door->render(program);
    for (int i = 0; i < ENEMY_COUNT; i++) { m_state.enemies[i].render(program); }
    //for (int i = 0; i < BUTTON_COUNT; i++) { m_state.buttons[i].render(program); }
    m_state.player->render(program);
    m_state.player2->render(program);
}