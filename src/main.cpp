#include "Application/Application.hpp"

class Voxely : public Application{
public:
    Voxely() : Application("Voxely", 1280, 720){}
private:
    void OnStart() override {
        LoadTexture("../assets/textures/Black.png");
        CreateMesh(MeshType::Cube, Transform{ .position = {(float)0, 0, 0} });
    }

    void OnUpdate(float deltaTime) override {
    }
};


int main(void){
    Voxely voxely;
    voxely.Run();
    return 0;
}