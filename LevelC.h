#include "Scene.h"

class LevelC : public Scene {
public:
    int ENEMY_COUNT = 4;
    int PLATFORM_COUNT = 2;
    int BUTTON_COUNT = 2;
    int BOX_COUNT = 0;

    ~LevelC();

    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram* program) override;
};
