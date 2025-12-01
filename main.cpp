#include <iostream>
#include <raylib.h>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

const int SCREEN_WIDTH = 1400;
const int SCREEN_HEIGHT = 800;
const int MAX_INPUT_CHARS = 100; // batas


struct Ball {
    float x;
    float y;
    float speed_x;
    float speed_y;
    int radius;
    Color color;
};

Color getRandomColor() {
    return { 
        (unsigned char)GetRandomValue(50, 255), 
        (unsigned char)GetRandomValue(50, 255), 
        (unsigned char)GetRandomValue(50, 255), 
        255 
    };
}

void SpawnBalls(vector<Ball>& balls, int count) {
    
    balls.clear(); 

    
    if (count > 100) count = 100; // limit
    if (count <= 0) return;

    for (int i = 0; i < count; ++i) {
        Ball newBall;
        
        newBall.radius = GetRandomValue(10, 25); // besar bola
        
        newBall.x = (float)GetRandomValue(newBall.radius * 2, SCREEN_WIDTH - newBall.radius * 2);
        newBall.y = (float)GetRandomValue(newBall.radius * 2, SCREEN_HEIGHT - newBall.radius * 2); // lokasi acak
        
        int minSpeed = 2;
        int maxSpeed = 8;
        newBall.speed_x = (float)((GetRandomValue(0, 1) == 0 ? -1 : 1) * GetRandomValue(minSpeed, maxSpeed));
        newBall.speed_y = (float)((GetRandomValue(0, 1) == 0 ? -1 : 1) * GetRandomValue(minSpeed, maxSpeed));
        // kecepatan acak
        
        newBall.color = getRandomColor(); // get colour
        
        balls.push_back(newBall);
    }
}


int main () {

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Project Balls");
    SetTargetFPS(60);
    
    vector<Ball> balls;
    string input_text = "";
    bool spawn_ready = false;
    
    
    SpawnBalls(balls, 5); // awal

    while (!WindowShouldClose()) {
        
        int key = GetCharPressed();

        while (key > 0) {
            
            if ((key >= 48) && (key <= 57) && (input_text.length() < MAX_INPUT_CHARS)) {
                input_text += (char)key;
            }
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (!input_text.empty()) {
                input_text.pop_back();
            }
        }

        if (IsKeyPressed(KEY_ENTER)) {
            if (!input_text.empty()) {
                try {

                    int num_to_spawn = stoi(input_text);
                    SpawnBalls(balls, num_to_spawn);
                    input_text = ""; 
                } catch (const std::exception& e) {
                    
                    input_text = "";
                }
            }
        }

        // Update posisi bola 
        
        for (auto& ball : balls) { 
            ball.x += ball.speed_x;
            ball.y += ball.speed_y;

            // collision
            if (ball.x + ball.radius >= SCREEN_WIDTH || ball.x - ball.radius <= 0) {
                ball.speed_x *= -1;
            }

            if (ball.y + ball.radius >= SCREEN_HEIGHT || ball.y - ball.radius <= 0) {
                ball.speed_y *= -1;
            }
        }
        
        // bentuk
        
        BeginDrawing();
            ClearBackground(BLACK);
            
            DrawText("Masukkan Jumlah Bola :", 10, 10, 20, RAYWHITE);
            
            DrawRectangle(320, 10, 60 + 10 * MAX_INPUT_CHARS, 25, LIGHTGRAY);
            DrawText(input_text.c_str(), 330, 13, 20, BLACK);

            for (const auto& ball : balls) {
                DrawCircle(ball.x, ball.y, ball.radius, ball.color);
            }

            string count_info = "Bola Aktif: " + to_string(balls.size());
            DrawText(count_info.c_str(), 10, 40, 20, YELLOW);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}