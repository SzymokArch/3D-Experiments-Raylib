#include <raylib.h>
#include <rcamera.h>

Camera3D FPPCamera_init(Vector3 pos) {
  Camera3D fpcamera = {};
  fpcamera.position = pos;
  fpcamera.target = (Vector3){0, 0, 0};
  fpcamera.up = (Vector3){0, 1, 0};
  fpcamera.fovy = 45.0f;
  fpcamera.projection = CAMERA_PERSPECTIVE;
  return fpcamera;
}

void FPPCamera_update(Camera3D * fpcamera, float velocity) {
  double dt = GetFrameTime();
  Vector2 mousePositionDelta = GetMouseDelta();
  CameraYaw(fpcamera, -mousePositionDelta.x*0.003, false);
  CameraPitch(fpcamera, -mousePositionDelta.y*0.003, true, false, false);
        
  if (IsKeyDown(KEY_W)) CameraMoveForward(fpcamera, velocity * dt, true);
  if (IsKeyDown(KEY_A)) CameraMoveRight(fpcamera, -velocity * dt, true);
  if (IsKeyDown(KEY_S)) CameraMoveForward(fpcamera, -velocity * dt, true);
  if (IsKeyDown(KEY_D)) CameraMoveRight(fpcamera, velocity * dt, true);
}

int main() {

  InitWindow(1280, 720, "FPP Skeleton");
  DisableCursor();

  Camera3D fpcamera = FPPCamera_init((Vector3){10, 2, 10});
    
  Vector3 cubePosition = {0, 1, 0};

  SetTargetFPS(60);

  // Main game loop  
  while (!WindowShouldClose()) {

    FPPCamera_update(&fpcamera, 6);

    BeginDrawing(); {
      
      ClearBackground(RAYWHITE);

      BeginMode3D(fpcamera); {

        DrawCube(cubePosition, 2, 2, 2, RED);
        DrawCubeWires(cubePosition, 2, 2, 2, MAROON);
        DrawGrid(25, 1);

      } EndMode3D();

      DrawFPS(1200, 0);

    } EndDrawing();
  }
  return 0;
}
