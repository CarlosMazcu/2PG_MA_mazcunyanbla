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
#include "geometry_custom_quad.h"


//Unnamed struct and it's unique instance:
struct {
  EDK3::ref_ptr<EDK3::Camera> camera;
  EDK3::ref_ptr<EDK3::Node> root;
} GameState;

const int kWindowWidth = 1024;
const int kWindowHeight = 768;


void InitScene() {
  //Initializing the font:
  ESAT::DrawSetTextFont("./test/Font.ttf");
  ESAT::DrawSetTextSize(18);
  ESAT::DrawSetFillColor(255, 255, 255, 128);

  EDK3::dev::GPUManager::CheckGLError("Initializing the scene...");
  
  //Creating a cube:
  EDK3::ref_ptr<EDK3::QuadCustom> quad_geo;
  quad_geo.alloc();
  //EDK3::CreateCube(&quad_geo, 1.0f, true, true);
  quad_geo->init(3.0f);


  if (!quad_geo) {
      printf("Error geometry quad\n");

      exit(-1);
  }
  //Loading texture:
  EDK3::ref_ptr<EDK3::Texture> texture;
  EDK3::Texture::Load("./test/mitext.jpg", &texture);
  if (!texture) {
    printf("Can't load texture.png\n");
    exit(-2);
  }

  //Initializing the material and its settings:
  EDK3::ref_ptr<EDK3::MatDiffuseTexture> mat_diff_text;
  mat_diff_text.alloc();
  EDK3::ref_ptr<EDK3::MatDiffuseTexture::Settings> mat_diff_text_settings;
  mat_diff_text_settings.alloc();
  float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
  mat_diff_text_settings->set_color(color);
  mat_diff_text_settings->set_texture(texture.get());

  //Allocating root node:
  EDK3::Node* root = GameState.root.alloc();

  //Creates the drawables (Geometry + Material + Settings):
  EDK3::ref_ptr<EDK3::Drawable> drawable;
  drawable.alloc();
  drawable->set_geometry(quad_geo.get());
  drawable->set_material(mat_diff_text.get());
  drawable->set_material_settings(mat_diff_text_settings.get());
  drawable->set_position(0.0f, 0.0f, 0.0f);
  drawable->set_HPR(0.0f, 0.0f, 0.0f);
  root->addChild(drawable.get());


  //Allocating and initializing the camera:
  GameState.camera.alloc();
  float pos[] = {5.0f, 5.0f, 5.0f };
  float view[] = { -1.0f, -1.0f, -1.0f };
  GameState.camera->set_position(pos);
  GameState.camera->set_view_direction(view);
  GameState.camera->setupPerspective(70.0f, (float)kWindowWidth / (float)kWindowHeight, 1.0f, 1500.0f);
  EDK3::dev::GPUManager::CheckGLError("Prepare END");
}

void UpdateFn() {
  GameState.camera->set_clear_color(0.94f, 1.0f, 0.94f, 1.0f);
  //GameState.root->set_rotation_y(5 * ESAT::Time() / 100.0);

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
