#include "LevelA.h"
#include "Utility.h"

#define LEVEL_WIDTH 20
#define LEVEL_HEIGHT 11

unsigned int LEVELA_DATA[] = {
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 3, 3, 3, 1, 3, 2, 3, 1, 3, 2, 3, 3, 3, 3, 3, 3, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3,
    3, 3, 3, 0, 0, 2, 2, 2, 0, 0, 1, 1, 1, 0, 0, 0, 0, 3, 3, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 1, 1, 1, 3, 3, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3
};

/** with slant tiles
3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
3, 0, 0, 3, 3, 3, 1, 3, 2, 3, 1, 3, 2, 3, 3, 3, 3, 3, 3, 3,
3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 0, 0, 0, 3,
3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3,
3, 3, 4, 0, 0, 5, 2, 2, 4, 0, 0, 5, 1, 1, 4, 0, 0, 3, 3, 3,
3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 3, 3, 3,
3, 3, 3, 3, 3, 3, 1, 1, 3, 3, 3, 3, 2, 2, 3, 3, 3, 3, 3, 3
**/

LevelA::~LevelA()
{
    delete [] m_state.enemies;
    delete    m_state.player;
    delete    m_state.map;
    Mix_FreeChunk(m_state.jump_sfx);
    Mix_FreeMusic(m_state.bgm);
}

void LevelA::initialise()
{
    m_state.next_scene_id = -1;  
    
    GLuint map_texture_id = Utility::load_texture("assets/images/element_tiles.png");
    m_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELA_DATA, map_texture_id, 1.0f, 6, 1);

    //background
    m_state.background = new Entity();
    m_state.background->set_entity_type(BACKGROUND);
    m_state.background->m_texture_id = Utility::load_texture("assets/images/background.png");
    m_state.background->set_position(glm::vec3(10.0, -5.0f, 0.0f));

    //door
    m_state.door = new Entity();
    m_state.door->set_position(glm::vec3(16.0f, -2.0f, 0.0f));
    m_state.door->m_texture_id = Utility::load_texture("assets/images/door.png");

    //platforms & enemies (collidable entities)
    m_state.enemies = new Entity[3];

    m_state.enemies[0].set_entity_type(PLATFORM);
    m_state.enemies[0].m_texture_id = Utility::load_texture("assets/images/platform.png");
    m_state.enemies[0].set_position(glm::vec3(1.0f, -3.0f, 0.0f));
    m_state.enemies[0].set_movement(glm::vec3(0.0f, 0.0f, 0.0f));
    m_state.enemies[0].set_speed(1.0f);

    m_state.enemies[1].set_entity_type(ENEMY);
    m_state.enemies[1].set_element_type(WATER);
    m_state.enemies[1].m_texture_id = Utility::load_texture("assets/images/waterSlime.png");
    m_state.enemies[1].set_position(glm::vec3(9.0f, -5.0f, 0.0f));
    m_state.enemies[1].set_movement(glm::vec3(1.0f, 0.0f, 0.0f));
    m_state.enemies[1].set_speed(1.0f);
    m_state.enemies[1].set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));

    m_state.enemies[2].set_entity_type(ENEMY);
    m_state.enemies[2].set_element_type(LAVA);
    m_state.enemies[2].m_texture_id = Utility::load_texture("assets/images/lavaSlime.png");
    m_state.enemies[2].set_position(glm::vec3(13.0f, -5.0f, 0.0f));
    m_state.enemies[2].set_movement(glm::vec3(1.0f, 0.0f, 0.0f));
    m_state.enemies[2].set_speed(1.0f);
    m_state.enemies[2].set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));

    //buttons
    m_state.buttons = new Entity[2];
    for (int i = 0; i < BUTTON_COUNT; i++) {
        m_state.buttons[i].m_texture_id = Utility::load_texture("assets/images/button.png");
    }

    m_state.buttons[0].set_position(glm::vec3(6.0f, -5.0f, 0.0f));
    m_state.buttons[1].set_position(glm::vec3(4.0f, -2.0f, 0.0f));
    
    // watergirl
    m_state.player = new Entity();
    m_state.player->set_entity_type(PLAYER);
    m_state.player->set_element_type(WATER);
    m_state.player->set_position(glm::vec3(1.0f, -9.0f, 0.0f));
    m_state.player->set_movement(glm::vec3(0.0f));
    m_state.player->set_speed(2.5f);
    m_state.player->set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    m_state.player->m_texture_id = Utility::load_texture("assets/images/watergirl.png");

    // fireboy
    m_state.player2 = new Entity();
    m_state.player2->set_entity_type(PLAYER);
    m_state.player2->set_element_type(LAVA);
    m_state.player2->set_position(glm::vec3(1.0f, -7.0f, 0.0f));
    m_state.player2->set_movement(glm::vec3(0.0f));
    m_state.player2->set_speed(2.5f);
    m_state.player2->set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    m_state.player2->m_texture_id = Utility::load_texture("assets/images/fireboy.png");
    
    // Walking
    m_state.player->m_walking[m_state.player->LEFT]  = new int[4] { 1, 5, 9,  13 };
    m_state.player->m_walking[m_state.player->RIGHT] = new int[4] { 3, 7, 11, 15 };
    m_state.player->m_walking[m_state.player->UP]    = new int[4] { 2, 6, 10, 14 };
    m_state.player->m_walking[m_state.player->DOWN]  = new int[4] { 0, 4, 8,  12 };

    m_state.player->m_animation_indices = m_state.player->m_walking[m_state.player->RIGHT];  
    m_state.player->m_animation_frames = 4;
    m_state.player->m_animation_index  = 0;
    m_state.player->m_animation_time   = 0.0f;
    m_state.player->m_animation_cols   = 4;
    m_state.player->m_animation_rows   = 4;
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

void LevelA::update(float delta_time)
{
    //std::cout << "\n p1x: ";
    //std::cout << m_state.player->get_position().x;
    //std::cout << " p1y: ";
    //std::cout << m_state.player->get_position().y;
    //std::cout << "\n p2x: ";
    //std::cout << m_state.player->get_position().x;
    //std::cout << " p2y: ";
    //std::cout << m_state.player->get_position().y;

    m_state.background->update(delta_time, m_state.player, m_state.player, 1, m_state.map);
    m_state.door->update(delta_time, m_state.player, m_state.player, 1, m_state.map);
    for (int i = 0; i < ENEMY_COUNT; i++) { m_state.enemies[i].update(delta_time, m_state.door, m_state.door, 1, m_state.map); }
    for (int i = 0; i < BUTTON_COUNT; i++) { m_state.buttons[i].update(delta_time, m_state.player, m_state.player, 1, m_state.map); }
    m_state.player->update(delta_time, m_state.player, m_state.enemies, ENEMY_COUNT, m_state.map);
    m_state.player2->update(delta_time, m_state.player2, m_state.enemies, ENEMY_COUNT, m_state.map);

    // girl lava puddles
    if ((m_state.player->get_position().y == -7.1f && (m_state.player->get_position().x > 4.5f && m_state.player->get_position().x < 7.5f)) ||
        (m_state.player->get_position().y == -9.1f && (m_state.player->get_position().x > 9.5f && m_state.player->get_position().x < 12.5f)) ||
        (m_state.player->get_position().y == -2.1f && (m_state.player->get_position().x > 7.5f && m_state.player->get_position().x < 8.5f)) ||
        (m_state.player->get_position().y == -2.1f && (m_state.player->get_position().x > 11.5f && m_state.player->get_position().x < 12.5f))
    
    ){
        m_state.fell = true;
    }

    // fireboy water puddles
    if ((m_state.player2->get_position().y == -9.1f && (m_state.player2->get_position().x > 4.5f && m_state.player2->get_position().x < 7.5f)) ||
        (m_state.player2->get_position().y == -7.1f && (m_state.player2->get_position().x > 9.5f && m_state.player2->get_position().x < 12.5f)) ||
        (m_state.player2->get_position().y == -2.1f && (m_state.player2->get_position().x > 5.5f && m_state.player2->get_position().x < 6.5f)) ||
        (m_state.player2->get_position().y == -2.1f && (m_state.player2->get_position().x > 9.5f && m_state.player2->get_position().x < 10.5f))

    ){
       m_state.fell = true;
    }

    //button press
    if ((m_state.player->get_position().y == -5.1f && (m_state.player->get_position().x > 5.8f && m_state.player->get_position().x < 6.2f)) ||
        (m_state.player2->get_position().y == -5.1f && (m_state.player2->get_position().x > 5.8f && m_state.player2->get_position().x < 6.2f)) ||
        (m_state.player->get_position().y == -2.1f && (m_state.player->get_position().x > 3.8f && m_state.player->get_position().x < 4.2f)) ||
        (m_state.player2->get_position().y == -2.1f && (m_state.player2->get_position().x > 3.8f && m_state.player2->get_position().x < 4.2f))
    ){
        m_state.enemies[0].move_down();
    }
    else if (m_state.enemies[0].get_position().y < -3.0) {
        m_state.enemies[0].move_up();
    }

    // it case it glitches...
    if (m_state.enemies[0].get_position().y < -5.0) {
        m_state.enemies[0].set_position(glm::vec3(1.0f, -5.0f, 0.0f));
    }
    if (m_state.enemies[0].get_position().y > -3.0) {
        m_state.enemies[0].set_position(glm::vec3(1.0f, -3.0f, 0.0f));
    }


    // move patrolling enemies
    if (m_state.enemies[1].get_position().x <= 6.8) {
        m_state.enemies[1].move_right();
    }
    if (m_state.enemies[1].get_position().x >= 10.0) {
        m_state.enemies[1].move_left();
    }
    if (m_state.enemies[2].get_position().x <= 11.8) {
        m_state.enemies[2].move_right();
    }
    if (m_state.enemies[2].get_position().x >= 15.0) {
        m_state.enemies[2].move_left();
    }

    // player ran into enemy
    // if opposite elements, reset and lose life
    // if same element, enemy dies and gain absorbed status
    // if both players have absorbed status, gain a life
    if (m_state.player->m_collided_enemy) {
        if (m_state.player->m_collided_opposite) { m_state.fell = true; }

        if (m_state.player->m_collided_same) { 
            m_state.enemies[1].set_position(glm::vec3(30.0f)); 
            m_state.player->absorbed = true;
        }
    }
    if (m_state.player2->m_collided_enemy) {
        if (m_state.player2->m_collided_opposite) { m_state.fell = true; }

        if (m_state.player2->m_collided_same) { 
            m_state.enemies[2].set_position(glm::vec3(30.0f)); 
            m_state.player2->absorbed = true;
        }
    }

    std::cout << m_state.player->get_position().x;
    std::cout << "\n";
    std::cout << m_state.player->get_position().y;
    std::cout << "\n";

    // players both reached door
    if (m_state.player->get_position().y == -2.1f && (m_state.player->get_position().x > 15.5f && m_state.player->get_position().x < 16.1f) &&
        m_state.player2->get_position().y == -2.1f && (m_state.player2->get_position().x > 15.5f && m_state.player2->get_position().x < 16.1f)) m_state.finished = true;
}

void LevelA::render(ShaderProgram *program)
{
    m_state.background->render(program);
    m_state.map->render(program);
    m_state.door->render(program);
    for (int i = 0; i < ENEMY_COUNT; i++) { m_state.enemies[i].render(program); }
    for (int i = 0; i < BUTTON_COUNT; i++) { m_state.buttons[i].render(program); }
    m_state.player->render(program);
    m_state.player2->render(program);

    // tutorial
    GLuint font_texture_id_1 = Utility::load_texture("assets/fonts/font1.png");
    if(m_state.player->get_position() == glm::vec3(1.0, -9.1, 0.0)) {
        Utility::draw_text(program, font_texture_id_1, "Move watergirl", 0.3f, -0.2f, glm::vec3(2.0, -8.7, 0.0));
        Utility::draw_text(program, font_texture_id_1, "with A, W, D", 0.3f, -0.2f, glm::vec3(2.0, -9.2, 0.0));
    }
    if (m_state.player2->get_position() == glm::vec3(1.0, -7.1, 0.0)) {
        Utility::draw_text(program, font_texture_id_1, "Move fireboy", 0.3f, -0.2f, glm::vec3(2.0, -6.7, 0.0));
        Utility::draw_text(program, font_texture_id_1, "with <, ^, >", 0.3f, -0.2f, glm::vec3(2.0, -7.2, 0.0));
    }
    if ((m_state.player->get_position().y == -9.1 && m_state.player->get_position().x > 3 && m_state.player->get_position().x < 4)) {
        Utility::draw_text(program, font_texture_id_1, "Do not mix", 0.3f, -0.2f, glm::vec3(8.0, -8.7, 0.0));
        Utility::draw_text(program, font_texture_id_1, "opposite elements!", 0.3f, -0.2f, glm::vec3(8.0, -9.2, 0.0));
    }
}
