//
// PR03_01_Demo.cc
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


//Unnamed struct and it's unique instance:
struct {
    EDK3::ref_ptr<EDK3::Camera> camera;
    EDK3::ref_ptr<EDK3::Node> root;
} GameState;

const int kWindowWidth = 1024;
const int kWindowHeight = 768;
float radius[8], speed[8], distance_offset[8];



void InitScene() {
    //Initializing the font:
    ESAT::DrawSetTextFont("./test/Font.ttf");
    ESAT::DrawSetTextSize(18);
    ESAT::DrawSetFillColor(255, 255, 255, 128);

    for (int i = 1; i < 9; i++) {
        radius[i - 1] = i * 5.0f;
        speed[i - 1] = (1.0f / i) / 1000.0f;
        distance_offset[i - 1] = i * 2.0f;
    }

    EDK3::dev::GPUManager::CheckGLError("Initializing the scene...");

    //Creating a cube:
    EDK3::ref_ptr<EDK3::Geometry> sphere;
    EDK3::CreateCube(&sphere, 1.0f, true, true);
    //sun
    //Loading texture:
    EDK3::ref_ptr<EDK3::Texture> texture_sun;
    EDK3::Texture::Load("./test/sun.jpg", &texture_sun);
    if (!texture_sun) {
        printf("Can't load texture sun.jpg\n");
        exit(-2);
    }
    //Initializing the material and its settings:
    EDK3::ref_ptr<EDK3::MatDiffuseTexture> mat_diff_text_sun;
    mat_diff_text_sun.alloc();
    EDK3::ref_ptr<EDK3::MatDiffuseTexture::Settings> mat_diff_text_settings_sun;
    mat_diff_text_settings_sun.alloc();
    float color_sun[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    mat_diff_text_settings_sun->set_color(color_sun);
    mat_diff_text_settings_sun->set_texture(texture_sun.get());

    //mercury
    //Loading texture:
    EDK3::ref_ptr<EDK3::Texture> texture_mercury;
    EDK3::Texture::Load("./test/mercury.png", &texture_mercury);
    if (!texture_mercury) {
        printf("Can't load texture mercury.png\n");
        exit(-2);
    }
    //Initializing the material and its settings:
    EDK3::ref_ptr<EDK3::MatDiffuseTexture> mat_diff_text_mercury;
    mat_diff_text_mercury.alloc();
    EDK3::ref_ptr<EDK3::MatDiffuseTexture::Settings> mat_diff_text_settings_mercury;
    mat_diff_text_settings_mercury.alloc();
    float color_mercury[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    mat_diff_text_settings_mercury->set_color(color_mercury);
    mat_diff_text_settings_mercury->set_texture(texture_mercury.get());


    //venus
    //Loading texture:
    EDK3::ref_ptr<EDK3::Texture> texture_venus;
    EDK3::Texture::Load("./test/venus.jpg", &texture_venus);
    if (!texture_venus) {
        printf("Can't load texture venus.png\n");
        exit(-2);
    }
    //Initializing the material and its settings:
    EDK3::ref_ptr<EDK3::MatDiffuseTexture> mat_diff_text_venus;
    mat_diff_text_venus.alloc();
    EDK3::ref_ptr<EDK3::MatDiffuseTexture::Settings> mat_diff_text_settings_venus;
    mat_diff_text_settings_venus.alloc();
    float color_venus[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    mat_diff_text_settings_venus->set_color(color_venus);
    mat_diff_text_settings_venus->set_texture(texture_venus.get());

    //earth
    //Loading texture:
    EDK3::ref_ptr<EDK3::Texture> texture_earth;
    EDK3::Texture::Load("./test/earth.jpg", &texture_earth);
    if (!texture_earth) {
        printf("Can't load texture earth.png\n");
        exit(-2);
    }
    //Initializing the material and its settings:
    EDK3::ref_ptr<EDK3::MatDiffuseTexture> mat_diff_text_earth;
    mat_diff_text_earth.alloc();
    EDK3::ref_ptr<EDK3::MatDiffuseTexture::Settings> mat_diff_text_settings_earth;
    mat_diff_text_settings_earth.alloc();
    float color_earth[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    mat_diff_text_settings_earth->set_color(color_earth);
    mat_diff_text_settings_earth->set_texture(texture_earth.get());

    //mars
    //Loading texture:
    EDK3::ref_ptr<EDK3::Texture> texture_mars;
    EDK3::Texture::Load("./test/mars.jpg", &texture_mars);
    if (!texture_mars) {
        printf("Can't load texture mars.png\n");
        exit(-2);
    }
    //Initializing the material and its settings:
    EDK3::ref_ptr<EDK3::MatDiffuseTexture> mat_diff_text_mars;
    mat_diff_text_mars.alloc();
    EDK3::ref_ptr<EDK3::MatDiffuseTexture::Settings> mat_diff_text_settings_mars;
    mat_diff_text_settings_mars.alloc();
    float color_mars[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    mat_diff_text_settings_mars->set_color(color_mars);
    mat_diff_text_settings_mars->set_texture(texture_mars.get());

    //jupiter
    //Loading texture:
    EDK3::ref_ptr<EDK3::Texture> texture_jupiter;
    EDK3::Texture::Load("./test/jupiter.jpg", &texture_jupiter);
    if (!texture_jupiter) {
        printf("Can't load texture jupiter.png\n");
        exit(-2);
    }
    //Initializing the material and its settings:
    EDK3::ref_ptr<EDK3::MatDiffuseTexture> mat_diff_text_jupiter;
    mat_diff_text_jupiter.alloc();
    EDK3::ref_ptr<EDK3::MatDiffuseTexture::Settings> mat_diff_text_settings_jupiter;
    mat_diff_text_settings_jupiter.alloc();
    float color_jupiter[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    mat_diff_text_settings_jupiter->set_color(color_jupiter);
    mat_diff_text_settings_jupiter->set_texture(texture_jupiter.get());

    //saturn
    //Loading texture:
    EDK3::ref_ptr<EDK3::Texture> texture_saturn;
    EDK3::Texture::Load("./test/saturn.jpg", &texture_saturn);
    if (!texture_saturn) {
        printf("Can't load texture saturn.png\n");
        exit(-2);
    }
    //Initializing the material and its settings:
    EDK3::ref_ptr<EDK3::MatDiffuseTexture> mat_diff_text_saturn;
    mat_diff_text_saturn.alloc();
    EDK3::ref_ptr<EDK3::MatDiffuseTexture::Settings> mat_diff_text_settings_saturn;
    mat_diff_text_settings_saturn.alloc();
    float color_saturn[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    mat_diff_text_settings_saturn->set_color(color_saturn);
    mat_diff_text_settings_saturn->set_texture(texture_saturn.get());

    //uranus
    //Loading texture:
    EDK3::ref_ptr<EDK3::Texture> texture_uranus;
    EDK3::Texture::Load("./test/uranus.jpg", &texture_uranus);
    if (!texture_uranus) {
        printf("Can't load texture uranus.png\n");
        exit(-2);
    }
    //Initializing the material and its settings:
    EDK3::ref_ptr<EDK3::MatDiffuseTexture> mat_diff_text_uranus;
    mat_diff_text_uranus.alloc();
    EDK3::ref_ptr<EDK3::MatDiffuseTexture::Settings> mat_diff_text_settings_uranus;
    mat_diff_text_settings_uranus.alloc();
    float color_uranus[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    mat_diff_text_settings_uranus->set_color(color_uranus);
    mat_diff_text_settings_uranus->set_texture(texture_uranus.get());

    //neptune
    //Loading texture:
    EDK3::ref_ptr<EDK3::Texture> texture_neptune;
    EDK3::Texture::Load("./test/neptune.jpg", &texture_neptune);
    if (!texture_neptune) {
        printf("Can't load texture neptune.png\n");
        exit(-2);
    }
    //Initializing the material and its settings:
    EDK3::ref_ptr<EDK3::MatDiffuseTexture> mat_diff_text_neptune;
    mat_diff_text_neptune.alloc();
    EDK3::ref_ptr<EDK3::MatDiffuseTexture::Settings> mat_diff_text_settings_neptune;
    mat_diff_text_settings_neptune.alloc();
    float color_neptune[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    mat_diff_text_settings_neptune->set_color(color_neptune);
    mat_diff_text_settings_neptune->set_texture(texture_neptune.get());

    //asteroids
    //Loading texture:
    EDK3::ref_ptr<EDK3::Texture> texture_asteroid;
    EDK3::Texture::Load("./test/asteroid.jpg", &texture_asteroid);
    if (!texture_asteroid) {
        printf("Can't load texture asteroid.png\n");
        exit(-2);
    }

    //Initializing the material and its settings asteroids:
    EDK3::ref_ptr<EDK3::MatDiffuseTexture> mat_diff_text_asteroids;
    mat_diff_text_asteroids.alloc();
    EDK3::ref_ptr<EDK3::MatDiffuseTexture::Settings> mat_diff_text_settings_asteroids;
    mat_diff_text_settings_asteroids.alloc();
    float color_asteroids[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    mat_diff_text_settings_asteroids->set_color(color_asteroids);
    mat_diff_text_settings_asteroids->set_texture(texture_asteroid.get());


    //Allocating root node:
    EDK3::Node* root = GameState.root.alloc();

    

    //Creates the drawables planets(Geometry + Material + Settings):
    EDK3::ref_ptr<EDK3::Drawable> drawable[9];
    for (int i = 0; i < 9; i++) {
        drawable[i].alloc();
        drawable[i]->set_geometry(sphere.get());
        drawable[i]->set_position(0.0f, 0.0f, 0.0f);
        drawable[i]->set_HPR(0.0f, 0.0f, 0.0f);
        root->addChild(drawable[i].get());
    }

    //Creates the drawables asteroids(Geometry + Material + Settings):
    EDK3::ref_ptr<EDK3::Drawable> drawable_asteroids[200];
    for (int i = 0; i < 200; i++) {
        drawable_asteroids[i].alloc();
        drawable_asteroids[i]->set_geometry(sphere.get());
        drawable_asteroids[i]->set_position(0.0, 0.0f, 0.0f);
        drawable_asteroids[i]->set_HPR(0.0f, 0.0f, 0.0f);
        drawable_asteroids[i]->set_scale(0.1f, 0.1f, 0.1f);
        drawable_asteroids[i]->set_material(mat_diff_text_asteroids.get());
        drawable_asteroids[i]->set_material_settings(mat_diff_text_settings_asteroids.get());
        root->addChild(drawable_asteroids[i].get());
    }

    EDK3::ref_ptr<EDK3::Drawable> drawable_comet;
    drawable_comet.alloc();
    drawable_comet->set_geometry(sphere.get());
    drawable_comet->set_position(0.0, 0.0f, 0.0f);
    drawable_comet->set_HPR(0.0f, 0.0f, 0.0f);
    drawable_comet->set_scale(0.3f, 0.3f, 0.3f);
    drawable_comet->set_material(mat_diff_text_asteroids.get());
    drawable_comet->set_material_settings(mat_diff_text_settings_asteroids.get());
    root->addChild(drawable_comet.get());


    EDK3::ref_ptr<EDK3::Drawable> drawable_asteroids_saturn[30];
    for (int i = 0; i < 30; i++) {
        drawable_asteroids_saturn[i].alloc();
        drawable_asteroids_saturn[i]->set_geometry(sphere.get());
        drawable_asteroids_saturn[i]->set_position(0.0, 0.0f, 0.0f);
        drawable_asteroids_saturn[i]->set_HPR(0.0f, 0.0f, 0.0f);
        drawable_asteroids_saturn[i]->set_scale(0.05f, 0.05f, 0.05f);
        drawable_asteroids_saturn[i]->set_material(mat_diff_text_asteroids.get());
        drawable_asteroids_saturn[i]->set_material_settings(mat_diff_text_settings_asteroids.get());
        root->child(6)->addChild(drawable_asteroids_saturn[i].get());
    }
    
    drawable[0]->set_scale(4.0f, 4.0f, 4.0f);
    drawable[1]->set_scale(0.5f, 0.5f, 0.5f);
    drawable[5]->set_scale(3.0f, 3.0f, 3.0f);
    drawable[6]->set_scale(2.0f, 2.0f, 2.0f);
    drawable[7]->set_scale(2.0f, 2.0f, 2.0f);
    drawable[8]->set_scale(1.5f, 1.5f, 1.5f);

    drawable[0]->set_material(mat_diff_text_sun.get());
    drawable[0]->set_material_settings(mat_diff_text_settings_sun.get());
    drawable[1]->set_material(mat_diff_text_mercury.get());
    drawable[1]->set_material_settings(mat_diff_text_settings_mercury.get());
    drawable[2]->set_material(mat_diff_text_venus.get());
    drawable[2]->set_material_settings(mat_diff_text_settings_venus.get());
    drawable[3]->set_material(mat_diff_text_earth.get());
    drawable[3]->set_material_settings(mat_diff_text_settings_earth.get());
    drawable[4]->set_material(mat_diff_text_mars.get());
    drawable[4]->set_material_settings(mat_diff_text_settings_mars.get());
    drawable[5]->set_material(mat_diff_text_jupiter.get());
    drawable[5]->set_material_settings(mat_diff_text_settings_jupiter.get());
    drawable[6]->set_material(mat_diff_text_saturn.get());
    drawable[6]->set_material_settings(mat_diff_text_settings_saturn.get());
    drawable[7]->set_material(mat_diff_text_uranus.get());
    drawable[7]->set_material_settings(mat_diff_text_settings_uranus.get());
    drawable[8]->set_material(mat_diff_text_neptune.get());
    drawable[8]->set_material_settings(mat_diff_text_settings_neptune.get());

    //Allocating and initializing the camera:
    GameState.camera.alloc();
    float pos[] = { 25.0f, 25.0f, 25.0f };
    float view[] = { -1.0f, -1.0f, -1.0f };
    GameState.camera->set_position(pos);
    GameState.camera->set_view_direction(view);
    GameState.camera->setupPerspective(70.0f, (float)kWindowWidth / (float)kWindowHeight, 1.0f, 1500.0f);
    EDK3::dev::GPUManager::CheckGLError("Prepare END");
}


void UpdateFn() {

    GameState.camera->set_clear_color(0.0f, 0.0f, 0.0f, 1.0f);
    for (int i = 1; i < 9; i++) {
        GameState.root->child(i)->set_rotation_y((float)(5 * ESAT::Time() / 100.0f));

        float new_pos[3] = { radius[i - 1] * cos(ESAT::Time() * speed[i - 1] + distance_offset[i - 1]),
                     0.0f, radius[i - 1] * sin(ESAT::Time() * speed[i - 1] + distance_offset[i - 1]) };

        GameState.root->child(i)->set_position(new_pos);
    }
   for (int i = 9; i < 209; i++) {

        float new_pos[3] = { 22.5f * cos(ESAT::Time() * (5.0f/10000.0f) + 0.5f * (float)i),
                     i%3, 20.5f * sin(ESAT::Time() * (5.0f / 10000.0f) + 0.5f * (float)i) };

        GameState.root->child(i)->set_position(new_pos);
    }
   for (int i = 0; i < 30; i++) {

       float new_pos[3] = { 1.5f * cos(ESAT::Time() * -(5.0f / 10000.0f) + 0.5f * (float)i),
                    0.0f, 1.5f * sin(ESAT::Time() * -(5.0f / 10000.0f) + 0.5f * (float)i) };

       GameState.root->child(6)->child(i)->set_position(new_pos);
   }
   float new_pos_comet[3] = { 11.5f * tan(ESAT::Time() * -(5.0f / 10000.0f)),
                    0.0f, 11.5f * cos(ESAT::Time() * (5.0f / 10000.0f)) };

    GameState.root->child(209)->set_position(new_pos_comet);
    GameState.root->child(0)->set_rotation_y((float)(5 * ESAT::Time() / 100.0f));

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
