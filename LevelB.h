#include "Scene.h"

class LevelB : public Scene {
public:
    int ENEMY_COUNT = 13;
    int PLATFORM_COUNT = 10;
    int BUTTON_COUNT = 4;
    int BOX_COUNT = 1;
    
    ~LevelB();
    
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
};
