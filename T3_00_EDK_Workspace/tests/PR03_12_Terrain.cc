//
// PR03_00_Template.cc
// Ivan Sancho, Escuela Superior de Arte y Tecnologia
// Programacion Grafica
// ESAT 2022-2023
//

#include <stdio.h>
#include <math.h>
#include "ESAT/window.h"
#include "ESAT/input.h"
#include "ESAT/draw.h"
#include "ESAT/time.h"

#include "EDK3/geometry.h"
#include "EDK3/camera.h"
#include "EDK3/drawable.h"
#include "EDK3/matdiffusetexture.h"
#include "EDK3/texture.h"
#include "EDK3/dev/gpumanager.h"

#include "ESAT_extra/imgui.h"
#include "EDK3/dev/opengl.h"

#include "material_custom.h"
#include "geometry_custom_terrain.h"


//Unnamed struct and it's unique instance:
struct {
    EDK3::ref_ptr<EDK3::Camera> camera;
    EDK3::ref_ptr<EDK3::Node> root;
} GameState;

const int kWindowWidth = 1024;
const int kWindowHeight = 768;


void InitScene() {
    EDK3::ref_ptr<EDK3::TerrainCustom> cube_geo;
    //EDK3::CreateCube(&cube_geo, 1.0f, true, true);
    cube_geo.alloc();
    //cube_geo->init8v(5.0f);
    cube_geo->init(20,20);

    EDK3::ref_ptr<EDK3::Texture> texture;
    EDK3::Texture::Load("./test/T_Chopper.jpg", &texture);
    if (!texture) {
        printf("Error");
        exit(-2);
    }

    EDK3::ref_ptr<EDK3::MaterialCustom> material;
    material.alloc();
    material->init("./test/vertex_shader.vs", "./test/fragment_shader.fs");

    EDK3::ref_ptr<EDK3::MaterialCustom::MaterialCustomTexturedSettings> material_settings;
    material_settings.alloc();
    float color[] = { 1.0f, 0.0f, 0.0f, 1.0f };
    material_settings->set_color(color);
    material_settings->set_texture(texture.get(), 0);

    //Allocating root node:
    EDK3::Node* root = GameState.root.alloc();

    EDK3::ref_ptr<EDK3::Drawable> drawable;
    drawable.alloc();
    drawable->set_geometry(cube_geo.get());
    drawable->set_material(material.get());
    drawable->set_material_settings(material_settings.get());
    drawable->set_position(0.0f, 0.0f, 0.0f);
    drawable->set_HPR(0.0f, 0.0f, 0.0f);
    root->addChild(drawable.get());

    //Allocating and initializing the camera:
    GameState.camera.alloc();
    float pos[] = { 20.0f, 40.0f, 20.0f };
    float view[] = { -20.0f, -40.0f, -20.0f };
    GameState.camera->set_position(pos);
    GameState.camera->set_view_target(view);
    GameState.camera->setupPerspective(30.0f, 8.0f / 6.0f, 1.0f, 1500.0f);
    EDK3::dev::GPUManager::CheckGLError("Prepare END");
}

void UpdateFn() {
    //GameState.root->set_rotation_y(5 * ESAT::Time() / 100.0);
    GameState.camera->set_clear_color(0.94f, 1.0f, 0.94f, 1.0f);
}

void RenderFn() {
    //For every frame... determine what's visible:
    GameState.camera->doCull(GameState.root.get());

    //Rendering the scene:
    EDK3::dev::GPUManager::CheckGLError("begin Render-->");
    GameState.camera->doRender();
    EDK3::dev::GPUManager::CheckGLError("end Render-->");
}

void ImGuiFn(double dt) {
    ImGui::Begin("FPS Window");
    ImGui::Text("FPS: %0.1f", 1000.0 / dt);
    ImGui::Text("Delta time: %0.3f ms", dt);
    ImGui::End();
    ImGui::Render();
}

int ESAT::main(int argc, char** argv) {
    ESAT::WindowInit(kWindowWidth, kWindowHeight);
    InitScene();
    double dt = 0.0;
    double last_time = ESAT::Time();
    while (!ESAT::IsSpecialKeyDown(ESAT::kSpecialKey_Escape) &&
        ESAT::WindowIsOpened()) {
        UpdateFn();
        RenderFn();
        dt = ESAT::Time() - last_time;
        last_time = ESAT::Time();
        ImGuiFn(dt);
        ESAT::WindowFrame();
    }
    return 0;
}