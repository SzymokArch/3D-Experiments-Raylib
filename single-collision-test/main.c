#include <raylib.h>
#include <rcamera.h>
#include <raymath.h>

void draw_xyz(Camera3D * fpcamera){
    Vector3 playerPos = fpcamera->position;
    playerPos.y = playerPos.y - 0.5;
    Vector3 forward = GetCameraForward(fpcamera);
    forward.y = 0;
    forward = Vector3Normalize(forward);
    //forward = Vector3Scale(forward, 1);
    forward = Vector3Add(playerPos, forward);
                
    Vector3 up = GetCameraUp(fpcamera);
    up = Vector3Normalize(up);
    //up = Vector3Scale(up, 1);
    up = Vector3Add(playerPos, up);

    Vector3 right = GetCameraRight(fpcamera);
    right.y = 0;
    right = Vector3Normalize(right);
    //right = Vector3Scale(right, 1);
    right = Vector3Add(playerPos, right);

                
    DrawLine3D(playerPos, forward, RED);
    DrawLine3D(playerPos, up, GREEN);
    DrawLine3D(playerPos, right, BLUE);
}

void draw_collision_vector(Camera3D * fpcamera, Vector3 cubePosition){
    Vector3 playerPos = fpcamera->position;
    playerPos.y = playerPos.y - 0.5;
    Vector3 collision_vector = Vector3Subtract(cubePosition, playerPos);
    collision_vector = Vector3Normalize(collision_vector);
    collision_vector = Vector3Scale(collision_vector, -1);
    collision_vector = Vector3Add(playerPos, collision_vector);
    
    DrawLine3D(playerPos, collision_vector, PURPLE);
}

void draw_closest_to_col_vector(Camera3D * fpcamera, Vector3 cubePosition){
    Vector3 playerPos = fpcamera->position;
    playerPos.y = playerPos.y - 0.5;

    enum direction_enum {UP, DOWN, FORWARD, BACKWARD, RIGHT, LEFT};

    Vector3 directions[6];

    Vector3 forward = GetCameraForward(fpcamera);
    forward.y = 0;
    forward = Vector3Normalize(forward);
                
    Vector3 up = GetCameraUp(fpcamera);
    up = Vector3Normalize(up);

    Vector3 right = GetCameraRight(fpcamera);
    right.y = 0;
    right = Vector3Normalize(right);

    directions[UP] = up;
    directions[DOWN] = Vector3Scale(up, -1);
    directions[FORWARD] =  forward;
    directions[BACKWARD] = Vector3Scale(forward, -1);
    directions[RIGHT] = right;
    directions[LEFT] = Vector3Scale(right, -1);

    Vector3 collision_vector = Vector3Subtract(cubePosition, playerPos);
    collision_vector = Vector3Normalize(collision_vector);
    collision_vector = Vector3Scale(collision_vector, -1);

    double current_min = Vector3Distance(collision_vector, directions[UP]);
    int min_index = 0;
    for (int i = 1; i < 6; i ++){
        double current_dist =  Vector3Distance(collision_vector, directions[i]);
        if (current_dist < current_min) {
            current_min = current_min;
            min_index = i;
        }
    }
    DrawLine3D(playerPos, Vector3Add(playerPos, directions[min_index]), BROWN);
}

void collision_response(Camera3D * fpcamera, Vector3 cubePosition, float velocity){
    Vector3 playerPos = fpcamera->position;
    playerPos.y = playerPos.y - 0.5;

    enum direction_enum {UP, DOWN, FORWARD, BACKWARD, RIGHT, LEFT};

    Vector3 directions[6];

    Vector3 forward = GetCameraForward(fpcamera);
    forward.y = 0;
    forward = Vector3Normalize(forward);
                
    Vector3 up = GetCameraUp(fpcamera);
    up = Vector3Normalize(up);

    Vector3 right = GetCameraRight(fpcamera);
    right.y = 0;
    right = Vector3Normalize(right);

    directions[UP] = up;
    directions[DOWN] = Vector3Scale(up, -1);
    directions[FORWARD] =  forward;
    directions[BACKWARD] = Vector3Scale(forward, -1);
    directions[RIGHT] = right;
    directions[LEFT] = Vector3Scale(right, -1);

    Vector3 collision_vector = Vector3Subtract(cubePosition, playerPos);
    collision_vector = Vector3Normalize(collision_vector);
    collision_vector = Vector3Scale(collision_vector, -1);

    double current_min = Vector3Distance(collision_vector, directions[UP]);
    int min_index = 0;
    for (int i = 1; i < 6; i ++){
        double current_dist =  Vector3Distance(collision_vector, directions[i]);
        if (current_dist < current_min) {
            current_min = current_min;
            min_index = i;
        }
    }
    
    if (min_index == UP) CameraMoveUp(fpcamera, velocity);;
    if (min_index == DOWN) CameraMoveUp(fpcamera, -velocity);
    if (min_index == FORWARD) CameraMoveForward(fpcamera, velocity, true);
    if (min_index == BACKWARD) CameraMoveForward(fpcamera, -velocity, true);
    if (min_index == RIGHT) CameraMoveRight(fpcamera, velocity, true);
    if (min_index == LEFT) CameraMoveRight(fpcamera, -velocity, true); 
}

int main(){

    const int screenWidth = 1280;
    const int screenHeight = 720;
    bool is_fpp = true;

    InitWindow(screenWidth, screenHeight, "FPP Test");

    Camera3D fpcamera = { 0 };
    fpcamera.position = (Vector3){ 10.0f, 2.0f, 10.0f };
    fpcamera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    fpcamera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    fpcamera.fovy = 45.0f;
    fpcamera.projection = CAMERA_PERSPECTIVE;

    Camera3D upcamera = { 0 };
    upcamera.position = (Vector3){ 10.0f, 10.0f, 10.0f };
    upcamera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    upcamera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    upcamera.fovy = 45.0f;
    upcamera.projection = CAMERA_PERSPECTIVE;

    float velocity = 0;
    double dt;
    
    Vector3 cubePosition = {0.0f, 1.0f, 0.0f};

    DisableCursor();

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose()){

        dt = GetFrameTime();

        Vector2 mousePositionDelta = GetMouseDelta();
        CameraYaw(&fpcamera, -mousePositionDelta.x*0.003f, false);
        CameraPitch(&fpcamera, -mousePositionDelta.y*0.003f, true, false, false);
        Vector3 playerPos = fpcamera.position;
        playerPos.y = playerPos.y - 0.5;
        velocity = 6;

        BoundingBox playerBBox;
        playerBBox.min = (Vector3){playerPos.x - 0.5, playerPos.y - 1.5, playerPos.z - 0.5};
        playerBBox.max = (Vector3){playerPos.x + 0.5, playerPos.y + 1.5, playerPos.z + 0.5};
        BoundingBox cubeBBox;
        cubeBBox.min = (Vector3){cubePosition.x - 1, cubePosition.y - 1, cubePosition.z - 1};
        cubeBBox.max = (Vector3){cubePosition.x + 1, cubePosition.y + 1, cubePosition.z + 1};

        if (IsKeyDown(KEY_W)) CameraMoveForward(&fpcamera, velocity * dt, true);
        if (IsKeyDown(KEY_A)) CameraMoveRight(&fpcamera, -velocity * dt, true);
        if (IsKeyDown(KEY_S)) CameraMoveForward(&fpcamera, -velocity * dt, true);
        if (IsKeyDown(KEY_D)) CameraMoveRight(&fpcamera, velocity * dt, true);
        if (IsKeyPressed(KEY_C)) is_fpp = !is_fpp;
        if (CheckCollisionBoxes(playerBBox, cubeBBox)) collision_response(&fpcamera, cubePosition,  velocity * dt);

        BeginDrawing();

            ClearBackground(RAYWHITE);

            if (is_fpp) {
                BeginMode3D(fpcamera);
                DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, RED);
            }
            else {
                BeginMode3D(upcamera);
                DrawCubeWires(playerPos, 1.0f, 3.0f, 1.0f, (Color){0, 0, 0, 100});
                draw_xyz(&fpcamera);
                draw_closest_to_col_vector(&fpcamera, cubePosition);
            }

            
            DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, MAROON);
            DrawGrid(25, 1.0f);
            draw_collision_vector(&fpcamera, cubePosition);

            EndMode3D();
        
        DrawFPS(1200, 0);
        if (CheckCollisionBoxes(playerBBox, cubeBBox)) DrawText("Colision", 0, 0, 25, PURPLE);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

