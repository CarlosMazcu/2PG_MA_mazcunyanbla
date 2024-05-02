//
// PR03_14_DemoRenderTarget.cc
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
#include "EDK3/matpostprocess.h"
#include "EDK3/texture.h"
#include "EDK3/rendertarget.h"
#include "EDK3/dev/gpumanager.h"

#include "ESAT_extra/imgui.h"


//Unnamed struct and it's unique instance:
struct {
  EDK3::ref_ptr<EDK3::Camera> camera;
  EDK3::ref_ptr<EDK3::Node> root;
  EDK3::ref_ptr<EDK3::RenderTarget> render_target;
  EDK3::ref_ptr<EDK3::MatDiffuseTexture> diffuse_material;
  EDK3::ref_ptr<EDK3::MatDiffuseTexture::Settings> none;
  EDK3::ref_ptr<EDK3::MatPostprocess> postprocess_material;
  EDK3::ref_ptr<EDK3::MatPostprocess::Blit> blit;
  EDK3::ref_ptr<EDK3::MatPostprocess::Fade> fade;
  EDK3::ref_ptr<EDK3::MatPostprocess::Blur> blur_v;
  EDK3::ref_ptr<EDK3::MatPostprocess::Blur> blur_h;
  int effect_index;
  float blit_speed;
  float blit_accum;
  float fade_speed;
  float fade_accum;
  bool draw_full_screen;
  float min_x;
  float min_y;
  float max_x;
  float max_y;
  bool should_move_mouse;
} GameState;

const int kWindowWidth = 1024;
const int kWindowHeight = 768;


void InitScene() {
  EDK3::dev::GPUManager::EnableCheckGLError(false);
  
  //Initializing data:
  GameState.effect_index = 0;
  GameState.blit_speed = 1.0f;
  GameState.blit_accum = 0.0f;
  GameState.fade_speed = 1.0f;
  GameState.fade_accum = 0.0f;
  GameState.draw_full_screen = true;
  GameState.min_x = 0.0f;
  GameState.min_y = 0.0f;
  GameState.max_x = ((float)kWindowWidth) * 0.5f;
  GameState.max_y = ((float)kWindowHeight) * 0.5f;
  GameState.should_move_mouse = true;

  //Initializing the font:
  ESAT::DrawSetTextFont("./test/Font.ttf");
  ESAT::DrawSetTextSize(18);
  ESAT::DrawSetFillColor(255, 255, 255, 128);

  //Allocating root node:
  EDK3::Node* root = GameState.root.alloc();

  //Creating a quad:
  EDK3::ref_ptr<EDK3::Geometry> quad_geo;
  EDK3::CreateQuad(&quad_geo, 40.0f, 20.0f, true, true);

  //Initializing the material and its settings:
  EDK3::ref_ptr<EDK3::MatDiffuseTexture> mat;
  mat.alloc();
  EDK3::ref_ptr<EDK3::MatDiffuseTexture::Settings> mat_settings;

  //Allocating and initializing the texture:
  EDK3::ref_ptr<EDK3::Texture> texture;
  EDK3::Texture::Load("./test/T_EDK_Logo.png", &texture);
  if (!texture) {
    printf("Can't load T_EDK_Logo.png\n");
    exit(-2);
  }

  //Creates the drawables (Geometry + Material + Settings):
  EDK3::ref_ptr<EDK3::Drawable> drawable;
  for(int i = -10; i < 10; i++) {
    for(int j = -10; j < 10; j++) {
      mat_settings.alloc();
      float color[] = { rand() / (float)RAND_MAX, 
                        rand() / (float)RAND_MAX, 
                        rand() / (float)RAND_MAX, 
                        1.0f };
      mat_settings->set_color(color);
      mat_settings->set_texture(texture.get());
      drawable.alloc();
      drawable->set_geometry(quad_geo.get());
      drawable->set_material(mat.get());
      drawable->set_material_settings(mat_settings.get());
      drawable->set_position(i * 70.0f, 0.0f, j * 70.0f);
      drawable->set_HPR(360.0f * rand() / RAND_MAX,
                        360.0f * rand() / RAND_MAX, 
                        360.0f * rand() / RAND_MAX);
      root->addChild(drawable.get());
    }
  }

  //Allocating and initializing the camera:
  GameState.camera.alloc();
  float pos[] = { 120.0f, 140.0f, 120.0f };
  float view[] = { -120.0f, -140.0f, -120.0f };
  GameState.camera->set_position(pos);
  GameState.camera->set_view_direction(view);
  float aspect_ratio = (float) kWindowWidth / (float) kWindowHeight;
  GameState.camera->setupPerspective(70.0f, aspect_ratio, 1.0f, 1500.0f);

  //Allocating and initializing the render target and its material:
  GameState.render_target.alloc()->init((float) kWindowWidth,
                                        (float) kWindowHeight, 1);
  GameState.postprocess_material.alloc();


  //Allocating and initializing the diffuse material:
  GameState.diffuse_material.alloc();
  
  //Allocating and initializing the normal material settings:
  GameState.none.alloc();
  float white[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
  GameState.none->set_color(white);
  GameState.none->set_texture(GameState.render_target->color_texture(0));

  //Allocating and initializing the vertical blur postprocess:
  GameState.blur_v.alloc();
  GameState.blur_v->set_texture(GameState.render_target->color_texture(0));
  GameState.blur_v->set_direction(EDK3::MatPostprocess::Blur::kVertical);

  //Allocating and initializing the horizontal blur postprocess:
  GameState.blur_h.alloc();
  GameState.blur_h->set_texture(GameState.render_target->color_texture(0));
  GameState.blur_h->set_direction(EDK3::MatPostprocess::Blur::kHorizontal);

  //Allocating and initializing the blit postprocess:
  GameState.blit.alloc();
  GameState.blit->set_texture(GameState.render_target->color_texture(0));

  //Allocating and initializing the fade postprocess:
  GameState.fade.alloc();
  GameState.fade->set_textures(GameState.render_target->color_texture(0), texture.get());
}

void UpdateFn() {
  if (ESAT::IsSpecialKeyDown(ESAT::SpecialKey::kSpecialKey_Control)) {
    GameState.should_move_mouse = !GameState.should_move_mouse;
  }
  if (GameState.should_move_mouse) {
    //Orbital camera:
    double mx = ESAT::MousePositionX();
    double my = ESAT::MousePositionY();
    double p = sin(-my / 200.0f) * 220.0f;
    float pos[] = { (float)(p * cos(mx * 0.01f)),
                    (float)cos(-my / 200) * 220.0f,
                    (float)(p * sin(mx * 0.01f)) };
    float view[] = { -pos[0], -pos[1], -pos[2] };
    GameState.camera->set_position(pos);
    GameState.camera->set_view_direction(view);
  }
  GameState.camera->set_clear_color(0.94f, 1.0f, 0.94f, 1.0f);
}

void RenderFn(double dt) {
  //Clears the window:
  ESAT::DrawClear(255, 255, 255, 255);

  //For every frame... determine what's visible:
  GameState.camera->doCull(GameState.root.get());
  //GameState.camera->doCull(GameState.root->child(2));

  //Render the scene to a render target:
  GameState.render_target->begin();
  GameState.camera->doRender();
  GameState.render_target->end();
  
  float multiply_color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
  float additive_color[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
  float alpha = 0.0f;

  switch (GameState.effect_index) {
  case 0: //None
    if (GameState.draw_full_screen) {
      GameState.camera->doRender();
    }
    else {
      GameState.diffuse_material->drawScreenQuad(GameState.min_x, GameState.min_y, 
                                                 GameState.max_x, GameState.max_y, 
                                                 GameState.none.get());
    }
    break;
  case 1: //Vertical Blur
    if (GameState.draw_full_screen) {
      GameState.postprocess_material->drawFullScreenQuad(GameState.blur_v.get());
    }
    else {
      GameState.postprocess_material->drawScreenQuad(GameState.min_x, GameState.min_y, 
                                                     GameState.max_x, GameState.max_y, 
                                                     GameState.blur_v.get());
    }
    break;
  case 2: //Horizontal Blur
    if (GameState.draw_full_screen) {
      GameState.postprocess_material->drawFullScreenQuad(GameState.blur_h.get());
    }
    else {
      GameState.postprocess_material->drawScreenQuad(GameState.min_x, GameState.min_y,
                                                     GameState.max_x, GameState.max_y,
                                                     GameState.blur_h.get());
    }
    break;
  case 3: //Combined Blur
    GameState.render_target->begin();
    GameState.postprocess_material->drawFullScreenQuad(GameState.blur_v.get());
    GameState.render_target->end();
    if (GameState.draw_full_screen) {
      GameState.postprocess_material->drawFullScreenQuad(GameState.blur_h.get());
    }
    else {
      GameState.postprocess_material->drawScreenQuad(GameState.min_x, GameState.min_y, 
                                                     GameState.max_x, GameState.max_y, 
                                                     GameState.blur_h.get());
    }
    break;
  case 4: //Blit
    GameState.camera->doRender();
    GameState.blit_accum += dt * 0.001f * GameState.blit_speed;
    alpha = sinf(GameState.blit_accum) * 0.5f + 0.5f;
    multiply_color[0] = alpha;
    multiply_color[1] = alpha;
    multiply_color[2] = alpha;
    GameState.blit->set_multiply_color(multiply_color);
    GameState.blit->set_additive_color(additive_color);
    if (GameState.draw_full_screen) {
      GameState.postprocess_material->drawFullScreenQuad(GameState.blit.get());
    }
    else {
      GameState.postprocess_material->drawScreenQuad(GameState.min_x, GameState.min_y, GameState.max_x, GameState.max_y, GameState.blit.get());
    }
    break;
  case 5: //Fade
    GameState.fade_accum += dt * 0.001f * GameState.fade_speed;
    alpha = sinf(GameState.fade_accum) * 0.5f + 0.5f;
    GameState.fade->set_progress(alpha);
    if (GameState.draw_full_screen) {
      GameState.postprocess_material->drawFullScreenQuad(GameState.fade.get());
    }
    else {
      GameState.postprocess_material->drawScreenQuad(GameState.min_x, GameState.min_y, GameState.max_x, GameState.max_y, GameState.fade.get());
    }
  }
}

void ImGuiFn(double dt) {
  ImGui::Begin("FPS Window");
  ImGui::Text("FPS: %0.1f", 1000.0 / dt);
  ImGui::Text("Delta time: %0.3f ms", dt);
  ImGui::Separator();
  ImGui::Combo("Effect: ", &GameState.effect_index, "None\0VBlur\0HBlur\0CombinedBlur\0Blit\0Fade\0");
  if (4 == GameState.effect_index) { //Blit.
    ImGui::DragFloat("Blit Speed: ", &GameState.blit_speed, 
                     1.0f, 0.0f, 10.0f, "%.1f");
  }
  else if (5 == GameState.effect_index) { //Fade.
    ImGui::DragFloat("Fade Speed: ", &GameState.fade_speed, 
                     1.0f, 0.0f, 10.0f, "%.1f");
  }
  ImGui::Separator();
  ImGui::Checkbox("Draw Full Screen", &GameState.draw_full_screen);
  if (!GameState.draw_full_screen) {
    ImGui::SliderFloat("Min X: ", &GameState.min_x, 0.0f, 
                       GameState.max_x - 1.0f, "%.1f");
    ImGui::SliderFloat("Min Y: ", &GameState.min_y, 0.0f,
                       GameState.max_y - 1.0f, "%.1f");
    ImGui::SliderFloat("Max X: ", &GameState.max_x, GameState.min_x + 1.0f,
                       (float) kWindowWidth, "%.1f");
    ImGui::SliderFloat("Max Y: ", &GameState.max_y, GameState.min_y + 1.0f,
                       (float) kWindowHeight, "%.1f");
  }
  ImGui::Separator();
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
    RenderFn(dt);
    dt = ESAT::Time() - last_time;
    last_time = ESAT::Time();
    ImGuiFn(dt);
    ESAT::WindowFrame();
  }
  
  return 0;
}
