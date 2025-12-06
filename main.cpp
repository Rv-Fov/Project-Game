#include <iostream>
#include <raylib.h>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <algorithm> 

using namespace std;


const bool USE_QUADTREE = true; //bruteforce

const int SCREEN_WIDTH = 1400;
const int SCREEN_HEIGHT = 800;
const int DISPLAY_INPUT_LIMIT = 5; 
const int BALL_LIMIT = 2000; 
int next_ball_id = 0;

struct Ball {
    float x;
    float y;
    float speed_x;
    float speed_y;
    int radius;
    Color color;
    int id; 
};

struct RectangleBounds {
    float x;
    float y;
    float width;
    float height;
};

// Konstanta quadtree
const int QT_CAPACITY = 4;      
const int QT_MAX_DEPTH = 8;     

struct QuadTreeNode {
    RectangleBounds bounds;
    int depth;
    vector<Ball*> objects;
    QuadTreeNode* children[4]; 
    bool divided;

    QuadTreeNode(RectangleBounds b, int d) : bounds(b), depth(d), divided(false) {
        for(int i=0; i<4; ++i) children[i] = nullptr;
    }

    ~QuadTreeNode() {
        for(int i=0; i<4; ++i) delete children[i];
    }
    
    void Clear() {
        if (divided) {
            for (int i = 0; i < 4; ++i) {
                if (children[i]) {
                    children[i]->Clear();
                    delete children[i];
                    children[i] = nullptr;
                }
            }
            divided = false;
        }
        objects.clear();
    }
    
    int GetIndex(const Ball* ball) const {
        int index = -1;
        float verticalMidpoint = bounds.x + bounds.width / 2;
        float horizontalMidpoint = bounds.y + bounds.height / 2;

        bool topQuadrant = (ball->y - ball->radius < horizontalMidpoint);
        bool bottomQuadrant = (ball->y + ball->radius > horizontalMidpoint);

        if (ball->x + ball->radius < verticalMidpoint) { 

            if (topQuadrant) index = 0; 

            else if (bottomQuadrant) index = 2; 
        } 
        else if (ball->x - ball->radius > verticalMidpoint) { 

            if (topQuadrant) index = 1; 

            else if (bottomQuadrant) index = 3; 
        }

        return index; 
    }

    void Subdivide() {
        float subWidth = bounds.width / 2;
        float subHeight = bounds.height / 2;
        float x = bounds.x;
        float y = bounds.y;
        int nextDepth = depth + 1;

        children[0] = new QuadTreeNode({x, y, subWidth, subHeight}, nextDepth);
        children[1] = new QuadTreeNode({x + subWidth, y, subWidth, subHeight}, nextDepth);
        children[2] = new QuadTreeNode({x, y + subHeight, subWidth, subHeight}, nextDepth);
        children[3] = new QuadTreeNode({x + subWidth, y + subHeight, subWidth, subHeight}, nextDepth);

        divided = true;

        for (Ball* ball : objects) {
            int index = GetIndex(ball);
            if (index != -1) {
                children[index]->Insert(ball);
            }
        }
        
        objects.erase(std::remove_if(objects.begin(), objects.end(), 
            [&](Ball* ball) { return GetIndex(ball) != -1; }), objects.end());
    }

    void Insert(Ball* ball) {
        if (divided) {
            int index = GetIndex(ball);

            if (index != -1) {
                children[index]->Insert(ball);
                return;
            }
        }
        
        objects.push_back(ball);

        if (objects.size() > QT_CAPACITY && depth < QT_MAX_DEPTH && !divided) {
            Subdivide();
        }
    }
    
    void Query(const Ball* ball, vector<Ball*>& returnObjects) {
        if (!CheckCollisionCircles(ball, bounds)) return; 

        returnObjects.insert(returnObjects.end(), objects.begin(), objects.end());

        if (divided) {
            for (int i = 0; i < 4; ++i) {
                children[i]->Query(ball, returnObjects);
            }
        }
    }

    bool CheckCollisionCircles(const Ball* ball, const RectangleBounds& rect) {
        float closestX = std::max(rect.x, std::min(ball->x, rect.x + rect.width));
        float closestY = std::max(rect.y, std::min(ball->y, rect.y + rect.height));

        float dx = ball->x - closestX;
        float dy = ball->y - closestY;

        return (dx * dx + dy * dy) < (ball->radius * ball->radius);
    }
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
    
    static bool seeded = false;
    if (!seeded) {
        srand(time(NULL));
        seeded = true;
    }
    next_ball_id = 0; 

    balls.clear(); 
    
    if (count > BALL_LIMIT) count = BALL_LIMIT; 
    if (count <= 0) return;

    for (int i = 0; i < count; ++i) {
        Ball newBall;
        newBall.radius = GetRandomValue(10, 25); 
        newBall.x = (float)GetRandomValue(newBall.radius * 2, SCREEN_WIDTH - newBall.radius * 2);
        newBall.y = (float)GetRandomValue(newBall.radius * 2, SCREEN_HEIGHT - newBall.radius * 2); 
        
        int minSpeed = 2;
        int maxSpeed = 8;
        newBall.speed_x = (float)((GetRandomValue(0, 1) == 0 ? -1 : 1) * GetRandomValue(minSpeed, maxSpeed));
        newBall.speed_y = (float)((GetRandomValue(0, 1) == 0 ? -1 : 1) * GetRandomValue(minSpeed, maxSpeed));
        newBall.color = getRandomColor(); 
        newBall.id = next_ball_id++; 
        
        balls.push_back(newBall);
    }
}

bool CheckCollisionCircles(const Ball& b1, const Ball& b2) {
    float dx = b2.x - b1.x;
    float dy = b2.y - b1.y;
    float distance = sqrt(dx * dx + dy * dy);
    return distance <= (b1.radius + b2.radius);
}

void ResolveCollision(Ball& b1, Ball& b2) {
    
    // saat tabrakan reverse kecepatan
    float temp_speed_x = b1.speed_x;
    float temp_speed_y = b1.speed_y; 

    b1.speed_x = b2.speed_x;
    b1.speed_y = b2.speed_y;

    b2.speed_x = temp_speed_x;
    b2.speed_y = temp_speed_y;
    
    // ganti saat tabrakan
    b1.color = getRandomColor(); 
    b2.color = getRandomColor(); 

    float dx = b2.x - b1.x;
    float dy = b2.y - b1.y;
    float distance = sqrt(dx * dx + dy * dy);
    float overlap = (b1.radius + b2.radius) - distance; 
    
    if (distance != 0) {
        float normalX = dx / distance;
        float normalY = dy / distance;
        
        b1.x -= normalX * (overlap / 2.0f);
        b1.y -= normalY * (overlap / 2.0f);
        b2.x += normalX * (overlap / 2.0f);
        b2.y += normalY * (overlap / 2.0f); 
    }
}

int main () {

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Project Balls - Collision Detection");
    SetTargetFPS(60);
    
    vector<Ball> balls;
    string input_text = "";
    
    SpawnBalls(balls, 10); 

    RectangleBounds worldBounds = {0, 0, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT};
    QuadTreeNode* quadtreeRoot = nullptr;

    while (!WindowShouldClose()) {
        
        int key = GetCharPressed();

        while (key > 0) {
            
            if ((key >= 48) && (key <= 57) && (input_text.length() < DISPLAY_INPUT_LIMIT)) { 
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

        // Update and fix position
        for (auto& ball : balls) { 
            ball.x += ball.speed_x;
            ball.y += ball.speed_y;

            // collision sumbu X
            if (ball.x + ball.radius >= SCREEN_WIDTH) {
                ball.speed_x *= -1;
                ball.x = SCREEN_WIDTH - ball.radius; 
            } else if (ball.x - ball.radius <= 0) {
                ball.speed_x *= -1;
                ball.x = ball.radius; 
            }

            // collision sumbu Y
            if (ball.y + ball.radius >= SCREEN_HEIGHT) {
                ball.speed_y *= -1;
                ball.y = SCREEN_HEIGHT - ball.radius; 
            } else if (ball.y - ball.radius <= 0) {
                ball.speed_y *= -1;
                ball.y = ball.radius; 
            }
        }
        
    
        // Deteksi tabrakan bola
        if (USE_QUADTREE) {
            
            if (quadtreeRoot) delete quadtreeRoot;
            quadtreeRoot = new QuadTreeNode(worldBounds, 0);

            for (auto& ball : balls) quadtreeRoot->Insert(&ball);

            for (size_t i = 0; i < balls.size(); ++i) {
                vector<Ball*> candidates;
                quadtreeRoot->Query(&balls[i], candidates); 

                for (Ball* candidate : candidates) {
                    if (balls[i].id < candidate->id && CheckCollisionCircles(balls[i], *candidate)) {
                        ResolveCollision(balls[i], *candidate);
                    }
                }
            }
            
        } else {
            // Bruteforce
            for (size_t i = 0; i < balls.size(); ++i) {
                for (size_t j = i + 1; j < balls.size(); ++j) {
                    
                    if (CheckCollisionCircles(balls[i], balls[j])) {
                        ResolveCollision(balls[i], balls[j]);
                    }
                }
            }
        }
        
        // Bentuk
        BeginDrawing();
            ClearBackground(BLACK);

            DrawFPS(SCREEN_WIDTH - 100, 10);
            
            string method = USE_QUADTREE ? "Quadtree" : "Brute Force";
            DrawText(("Metode: " + method).c_str(), 10, 40, 20, GREEN);

            DrawText("Masukkan Jumlah Bola :", 10, 10, 20, RAYWHITE);
            
            DrawRectangle(320, 10, 60 + 10 * DISPLAY_INPUT_LIMIT, 25, LIGHTGRAY);
            DrawText(input_text.c_str(), 330, 13, 20, BLACK);

            for (const auto& ball : balls) {
                DrawCircle(ball.x, ball.y, ball.radius, ball.color);
            }

            string count_info = "Bola Aktif: " + to_string(balls.size());
            DrawText(count_info.c_str(), 10, 70, 20, YELLOW);

        EndDrawing();
    }
    
    if (quadtreeRoot) delete quadtreeRoot;

    CloseWindow();
    return 0;
}