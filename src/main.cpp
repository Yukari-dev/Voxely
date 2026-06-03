#include "Application/Application.hpp"

class Voxely : public Application{
public:
    Voxely() : Application("Voxely", 1280, 720){}
private:
    void OnStart() override {
        for(int i = 0; i < 10; i++)
            CreateMesh(MeshType::Cube, Transform{ .position = {(float)i, 0, 0} });
    }

    void OnUpdate(float deltaTime) override {
    }
};


int main(void){
    Voxely voxely;
    voxely.Run();
    return 0;
}