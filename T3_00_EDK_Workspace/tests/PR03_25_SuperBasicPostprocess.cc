//
// PR03_25_SuperBasicPostprocess.cc
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
#include "EDK3/texture.h"
#include "EDK3/matdiffusetexture.h"
#include "EDK3/rendertarget.h"
#include "EDK3/dev/gpumanager.h"
#include "postprocess_basic.h"

#include "ESAT_extra/imgui.h"
#include "EDK3/dev/opengl.h"


//Unnamed struct and it's unique instance:
struct {
  EDK3::ref_ptr<EDK3::Camera> camera;
  EDK3::ref_ptr<EDK3::Node> root;
  EDK3::ref_ptr<EDK3::RenderTarget> render_target;
  EDK3::ref_ptr<EDK3::PostprocessBasic> mat_postprocess;
  EDK3::ref_ptr<EDK3::PostprocessBasic::PostprocessBasicSettings> mat_postprocess_settings;
} GameState;

const int kWindowWidth = 1024;
const int kWindowHeight = 768;


void InitScene() {
  EDK3::dev::GPUManager::CheckGLError("Initializing the scene...");

  //Creating a cube:
  EDK3::ref_ptr<EDK3::Geometry> cube_geo;
  EDK3::CreateCube(&cube_geo, 1.0f, true, true);

  //Initializing the texture:
  EDK3::ref_ptr<EDK3::Texture> texture;
  EDK3::Texture::Load("./test/T_EDK_Logo.png", &texture);

  //Initializing the diffuse textured material and its settings:
  EDK3::ref_ptr<EDK3::MatDiffuseTexture> mat_basic;
  mat_basic.alloc();
  float white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
  EDK3::ref_ptr<EDK3::MatDiffuseTexture::Settings> mat_basic_settings;
  mat_basic_settings.alloc();
  mat_basic_settings->set_color(white);
  mat_basic_settings->set_texture(texture.get());

  //Allocating root node:
  EDK3::Node* root = GameState.root.alloc();

  //Allocating and initializing the cube drawables:
  EDK3::ref_ptr<EDK3::Drawable> cube_drawable_1;
  cube_drawable_1.alloc();
  cube_drawable_1->set_position(-1.0f, 0.0f, 0.0f);
  cube_drawable_1->set_geometry(cube_geo.get());
  cube_drawable_1->set_material(mat_basic.get());
  cube_drawable_1->set_material_settings(mat_basic_settings.get());
  root->addChild(cube_drawable_1.get());

  EDK3::ref_ptr<EDK3::Drawable> cube_drawable_2;
  cube_drawable_2.alloc();
  cube_drawable_2->set_position(1.0f, 0.0f, 0.0f);
  cube_drawable_2->set_geometry(cube_geo.get());
  cube_drawable_2->set_material(mat_basic.get());
  cube_drawable_2->set_material_settings(mat_basic_settings.get());
  root->addChild(cube_drawable_2.get());

  //Allocating and initializing the postprocess elements:
  //TODO something with the postprocess render target, material and
  //material settings.
  GameState.render_target.alloc();
  GameState.render_target->init(kWindowWidth, kWindowHeight, 1);

  GameState.mat_postprocess.alloc();
  GameState.mat_postprocess->init();

  GameState.mat_postprocess_settings.alloc();
  GameState.mat_postprocess_settings->set_texture(GameState.render_target->color_texture(0));
  float green[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
  GameState.mat_postprocess_settings->set_color(green);
  //Allocating and initializing the camera:
  GameState.camera.alloc();
  float pos[] = { 0.0f, 2.0f, 3.0f };
  float view[] = { 0.0f, 0.0f, 0.0f };
  GameState.camera->set_position(pos);
  GameState.camera->set_view_target(view);
  GameState.camera->setupPerspective(70.0f, 8.0f / 6.0f, 1.0f, 1500.0f);
  EDK3::dev::GPUManager::CheckGLError("Prepare END");
}

void UpdateFn() {
  GameState.camera->set_clear_color(0.94f, 1.0f, 0.94f, 1.0f);
  GameState.root->child(0)->set_rotation_y(ESAT::Time() * 0.01f);
  GameState.root->child(1)->set_rotation_y(ESAT::Time() * 0.01f);
}

void RenderFn() {
  //For every frame... determine what's visible:
  GameState.camera->doCull(GameState.root.get());

  //Rendering the scene:
  EDK3::dev::GPUManager::CheckGLError("begin Render-->");
  GameState.render_target->begin();
  GameState.camera->doRender();
  GameState.render_target->end();

  EDK3::dev::GPUManager::CheckGLError("end Render-->");
  GameState.mat_postprocess->drawFullScreenQuad(GameState.mat_postprocess_settings.get());
  //TODO something to render the postprocess.
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
