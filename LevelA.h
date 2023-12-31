#include "Scene.h"

class LevelA : public Scene {
public:
    int ENEMY_COUNT = 3;
    int PLATFORM_COUNT = 1;
    int BUTTON_COUNT = 2;
    int BOX_COUNT = 0;
    
    ~LevelA();
    
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
};
