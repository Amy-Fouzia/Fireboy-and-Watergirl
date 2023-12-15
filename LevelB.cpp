#include "LevelB.h"
#include "Utility.h"

#define LEVEL_WIDTH 20
#define LEVEL_HEIGHT 11

unsigned int LEVELB_DATA[] = {
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 3,
    3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 3, 3, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 0, 0, 0, 0, 0, 3, 3, 3,
    3, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3,
    3, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 1, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 3, 3, 3, 3
};

LevelB::~LevelB()
{
    delete[] m_state.enemies;
    delete    m_state.player;
    delete    m_state.map;
    Mix_FreeChunk(m_state.jump_sfx);
    Mix_FreeMusic(m_state.bgm);
}

void LevelB::initialise()
{
    m_state.next_scene_id = -1;

    GLuint map_texture_id = Utility::load_texture("assets/images/element_tiles.png");
    m_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELB_DATA, map_texture_id, 1.0f, 6, 1);

    //background
    m_state.background = new Entity();
    m_state.background->set_entity_type(BACKGROUND);
    m_state.background->m_texture_id = Utility::load_texture("assets/images/background.png");
    m_state.background->set_position(glm::vec3(10.0, -5.0f, 0.0f));

    //door
    m_state.door = new Entity();
    m_state.door->set_position(glm::vec3(2.0f, -2.0f, 0.0f));
    m_state.door->m_texture_id = Utility::load_texture("assets/images/door.png");

    //platforms & enemies & boxes (collidable entities)
    m_state.enemies = new Entity[ENEMY_COUNT];

    for (int i = 0; i < PLATFORM_COUNT; i++) {
        m_state.enemies[i].set_entity_type(PLATFORM);
        m_state.enemies[i].m_texture_id = Utility::load_texture("assets/images/platform.png");
        m_state.enemies[i].set_movement(glm::vec3(0.0f, 0.0f, 0.0f));
        m_state.enemies[i].set_speed(1.0f);
    }

    m_state.enemies[0].set_position(glm::vec3(11.0f, -7.0f, 0.0f));
    m_state.enemies[1].set_position(glm::vec3(12.0f, -7.0f, 0.0f));
    m_state.enemies[2].set_position(glm::vec3(13.0f, -7.0f, 0.0f));
    m_state.enemies[3].set_position(glm::vec3(14.0f, -7.0f, 0.0f));
    m_state.enemies[4].set_position(glm::vec3(15.0f, -7.0f, 0.0f));
    m_state.enemies[5].set_position(glm::vec3(5.0f, -5.0f, 0.0f));
    m_state.enemies[6].set_position(glm::vec3(5.9f, -5.0f, 0.0f));
    m_state.enemies[7].set_position(glm::vec3(2.0f, -3.0f, 0.0f));
    m_state.enemies[8].set_position(glm::vec3(3.0f, -3.0f, 0.0f));
    m_state.enemies[9].set_position(glm::vec3(4.0f, -3.0f, 0.0f));

    for (int i = PLATFORM_COUNT; i < ENEMY_COUNT - 1; i++) {
        m_state.enemies[i].set_entity_type(ENEMY);
        m_state.enemies[i].set_movement(glm::vec3(1.0f, 0.0f, 0.0f));
        m_state.enemies[i].set_speed(1.0f);
        m_state.enemies[i].set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    }

    m_state.enemies[10].set_position(glm::vec3(13.0f, -5.0f, 0.0f));
    m_state.enemies[10].set_element_type(WATER);
    m_state.enemies[10].m_texture_id = Utility::load_texture("assets/images/waterSlime.png");
    m_state.enemies[11].set_position(glm::vec3(2.0f, -9.0f, 0.0f));
    m_state.enemies[11].set_element_type(LAVA);
    m_state.enemies[11].m_texture_id = Utility::load_texture("assets/images/lavaSlime.png");

    //box
    m_state.enemies[12].m_texture_id = Utility::load_texture("assets/images/box.jpg");
    m_state.enemies[12].set_position(glm::vec3(30.0f, -3.0f, 0.0f));

    //buttons
    m_state.buttons = new Entity[BUTTON_COUNT];

    for (int i = 0; i < BUTTON_COUNT; i++) {
        m_state.buttons[i].m_texture_id = Utility::load_texture("assets/images/button.png");
    }

    m_state.buttons[0].set_position(glm::vec3(4.0f, -2.0f, 0.0f));
    m_state.buttons[1].set_position(glm::vec3(9.0f, -1.0f, 0.0f));
    m_state.buttons[2].set_position(glm::vec3(7.0f, -4.0f, 0.0f));
    m_state.buttons[3].set_position(glm::vec3(1.0f, -6.0f, 0.0f));

    // watergirl
    m_state.player = new Entity();
    m_state.player->set_entity_type(PLAYER);
    m_state.player->set_element_type(WATER);
    m_state.player->set_position(glm::vec3(9.0f, -9.0f, 0.0f));
    m_state.player->set_movement(glm::vec3(0.0f));
    m_state.player->set_speed(2.5f);
    m_state.player->set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    m_state.player->m_texture_id = Utility::load_texture("assets/images/watergirl.png");

    // fireboy
    m_state.player2 = new Entity();
    m_state.player2->set_entity_type(PLAYER);
    m_state.player2->set_element_type(LAVA);
    m_state.player2->set_position(glm::vec3(17.0f, -9.0f, 0.0f));
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
    m_state.player->m_jumping_power = 6.0f;
    m_state.player2->m_jumping_power = 6.0f;

    /**
     BGM and SFX
     */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

    m_state.bgm = Mix_LoadMUS("assets/audio/dooblydoo.mp3");
    Mix_PlayMusic(m_state.bgm, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 16.0f);

    m_state.jump_sfx = Mix_LoadWAV("assets/audio/bounce.wav");
}

void LevelB::update(float delta_time)
{
    m_state.background->update(delta_time, m_state.player, m_state.player, 1, m_state.map);
    m_state.door->update(delta_time, m_state.player, m_state.player, 1, m_state.map);
    for (int i = 0; i < ENEMY_COUNT; i++) { m_state.enemies[i].update(delta_time, m_state.door, m_state.door, 1, m_state.map); }
    for (int i = 0; i < BUTTON_COUNT; i++) { m_state.buttons[i].update(delta_time, m_state.player, m_state.player, 1, m_state.map); }
    m_state.player->update(delta_time, m_state.player, m_state.enemies, ENEMY_COUNT, m_state.map);
    m_state.player2->update(delta_time, m_state.player2, m_state.enemies, ENEMY_COUNT, m_state.map);

    // fireboy water puddles
    if ((m_state.player2->get_position().y == -9.1f && (m_state.player2->get_position().x > 10.5f && m_state.player2->get_position().x < 15.5f)) ||
        (m_state.player2->get_position().y == -6.1f && (m_state.player2->get_position().x > 1.5f && m_state.player2->get_position().x < 2.5f)) ||
        (m_state.player2->get_position().y == -7.1f && (m_state.player2->get_position().x > 2.5f && m_state.player2->get_position().x < 3.5f))
        ) {
        m_state.fell = true;
    }

    // squashed  by ceiling
    if (m_state.player->get_position().y == -0.1 ||
        (m_state.player->get_position().y == -6.1 && m_state.player->get_position().x > 10.5 && m_state.player->get_position().x < 15.5)) {
        m_state.fell = true;
    }

    if (m_state.player2->get_position().y == -0.1 ||
        (m_state.player2->get_position().y == -6.1 && m_state.player2->get_position().x > 10.5 && m_state.player2->get_position().x < 15.5)) {
        m_state.fell = true;
    }

    //button press
    if ((m_state.player->get_position().y == -6.1f && (m_state.player->get_position().x > 0.8f && m_state.player->get_position().x < 1.2f)) ||
        (m_state.player2->get_position().y == -6.1f && (m_state.player2->get_position().x > 0.8f && m_state.player2->get_position().x < 1.2f)) ||
        (m_state.player->get_position().y == -4.1f && (m_state.player->get_position().x > 6.8f && m_state.player->get_position().x < 7.2f)) ||
        (m_state.player2->get_position().y == -4.1f && (m_state.player2->get_position().x > 6.8f && m_state.player2->get_position().x < 7.2f))
        ) {
        m_state.enemies[0].move_down();
        m_state.enemies[1].move_down();
        m_state.enemies[2].move_down();
        m_state.enemies[3].move_down();
        m_state.enemies[4].move_down();
        m_state.enemies[5].move_down();
        m_state.enemies[6].move_down();
    }
    else if (m_state.enemies[5].get_position().y < -5.0) {
        m_state.enemies[0].move_up();
        m_state.enemies[1].move_up();
        m_state.enemies[2].move_up();
        m_state.enemies[3].move_up();
        m_state.enemies[4].move_up();
        m_state.enemies[5].move_up();
        m_state.enemies[6].move_up();
    }

    else {
        m_state.enemies[5].set_position(glm::vec3(5.0, -5.0, 0.0));
        m_state.enemies[6].set_position(glm::vec3(5.9, -5.0, 0.0));
    }

    if ((m_state.player->get_position().y == -2.1f && (m_state.player->get_position().x > 3.8f && m_state.player->get_position().x < 4.2f)) ||
        (m_state.player2->get_position().y == -2.1f && (m_state.player2->get_position().x > 3.8f && m_state.player2->get_position().x < 4.2f)) ||
        (m_state.player->get_position().y == -1.1f && (m_state.player->get_position().x > 8.8f && m_state.player->get_position().x < 9.2f)) ||
        (m_state.player2->get_position().y == -1.1f && (m_state.player2->get_position().x > 8.8f && m_state.player2->get_position().x < 9.2f))
        ) {
        m_state.enemies[7].set_position(glm::vec3(5.0f, -3.0f, 0.0f));
        m_state.enemies[8].set_position(glm::vec3(6.0f, -3.0f, 0.0f));
        m_state.enemies[9].set_position(glm::vec3(7.0f, -3.0f, 0.0f));
    }
    else if (m_state.enemies[9].get_position().x > 4.0) {
        m_state.enemies[7].set_position(glm::vec3(2.0f, -3.0f, 0.0f));
        m_state.enemies[8].set_position(glm::vec3(3.0f, -3.0f, 0.0f));
        m_state.enemies[9].set_position(glm::vec3(4.0f, -3.0f, 0.0f));
    }

    // move patrolling enemies
    if (m_state.enemies[10].get_position().x <= 12.0) {
        m_state.enemies[10].move_right();
    }
    if (m_state.enemies[10].get_position().x >= 15.0) {
        m_state.enemies[10].move_left();
    }
    if (m_state.enemies[11].get_position().x <= 1.0) {
        m_state.enemies[11].move_right();
    }
    if (m_state.enemies[11].get_position().x >= 6.0) {
        m_state.enemies[11].move_left();
    }

    // allow push box
    // only if both players are pushing
    if (m_state.player->m_collided_box && m_state.player2->m_collided_box && m_state.player->get_movement().x > 0 && m_state.player2->get_movement().x > 0) {
        m_state.enemies[12].move_left();
    }

    // player ran into enemy
    // if opposite elements, reset and lose life
    // if same element, enemy dies and gain absorbed status
    // if both players have absorbed status, gain a life
    if (m_state.player->m_collided_enemy) {
        if (m_state.player->m_collided_opposite) { m_state.fell = true; }

        if (m_state.player->m_collided_same) {
            m_state.enemies[10].set_position(glm::vec3(30.0f));
            m_state.player->absorbed = true;
        }
    }
    if (m_state.player2->m_collided_enemy) {
        if (m_state.player2->m_collided_opposite) { m_state.fell = true; }

        if (m_state.player2->m_collided_same) {
            m_state.enemies[11].set_position(glm::vec3(30.0f));
            m_state.player2->absorbed = true;
        }
    }

    // players both reached door
    if (m_state.player->get_position().y == -2.1f && (m_state.player->get_position().x > 1.5f && m_state.player->get_position().x < 2.1f) &&
        m_state.player2->get_position().y == -2.1f && (m_state.player2->get_position().x > 1.5f && m_state.player2->get_position().x < 2.1f)) m_state.finished = true;
}

void LevelB::render(ShaderProgram* program)
{
    m_state.background->render(program);
    m_state.map->render(program);
    m_state.door->render(program);
    for (int i = 0; i < ENEMY_COUNT; i++) { m_state.enemies[i].render(program); }
    for (int i = 0; i < BUTTON_COUNT; i++) { m_state.buttons[i].render(program); }
    m_state.player->render(program);
    m_state.player2->render(program);
}
