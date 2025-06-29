#include "raylib.h"
#include <stdio.h>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <vector>
#include <thread>

#define NUM_FRAMES  4       // Number of frames (rectangles) for the button sprite texture

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

std::vector<std::string> split(std::string target, std::string delimiter)
{
    std::vector<std::string> v;
    size_t ln = target.length();
    while (ln != 0)
    {
        std::size_t pos = target.find(delimiter);
        if (pos == std::string::npos) {
            v.push_back(target);
            return v;
        }
        if (pos == 0) pos++;
        std::string tmp = target.substr(0, pos);
        if (tmp.length() > 1) v.push_back(target.substr(0, pos));
        target = target.substr(pos, ln - pos);
        ln = target.length();
    }
    return v;
}

bool parse_ping(std::string cmd_out) {
    bool result = false;


    return result;
}

void button_push_execution(void) {
    std::cout << "------a\n";
    std::string result = exec("ping 192.168.1.131");
    std::cout << result;
    std::vector <std::string> r = split(split(result, "\n")[1], " ");
    std::cout << r[r.size() - 1] << std::endl;
    std::cout << "------b\n";
    return;
}

#ifdef DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 300;
    const int screenHeight = 150;

    

    InitWindow(screenWidth, screenHeight, "button panel test");
    InitAudioDevice();
    Image img = LoadImage("Resosrces/button.png");
    Sound button_sound = LoadSound("Resosrces/button_click_sound.wav");
    

    SetWindowIcon(img);

    Texture2D button_toggle = LoadTexture("Resosrces/buttons_toggle_sprite_sheet.png"); // Load button texture
    Texture2D button_push = LoadTexture("Resosrces/buttons_push_sprite_sheet.png"); // Load button texture

    // Define frame rectangle for drawing
    float frameWidth_toggle = (float)button_toggle.width / NUM_FRAMES;
    Rectangle sourceRec_toggle = { 0, 0, (float)frameWidth_toggle, button_toggle.height };

    //**
    float frameWidth_push = (float)button_push.width / NUM_FRAMES;
    Rectangle sourceRec_push = { 0, 0, (float)frameWidth_push, button_push.height };

    // Define button bounds on screen
    Rectangle btnBounds_toggle = {
        screenWidth/2.0f - button_toggle.width/NUM_FRAMES/2.0f,
        (2*screenHeight/4.0f) - 1.5 * (button_push.height / 2.0f),
        (float)frameWidth_toggle,
        button_toggle.height
    };

    //**
    Rectangle btnBounds_push = {
        screenWidth / 2.0f - button_push.width / NUM_FRAMES / 2.0f,
        (3 * screenHeight / 4.0f) - (button_push.height / 2.0f),
        (float)frameWidth_push,
        button_push.height
    };

    int btnState_toggle = 0;               // Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
    bool btn_on_off_toggle = true;

    int btnState_push = 0;               // Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
    bool btn_on_off_push = true;

    Vector2 mousePoint = { 0.0f, 0.0f };

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------
    std::thread first;
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        mousePoint = GetMousePosition();
        btnState_toggle = false;

        // Check button state
        if (btn_on_off_toggle) {
            if (CheckCollisionPointRec(mousePoint, btnBounds_toggle))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) btnState_toggle = 2;
                else btnState_toggle = 1;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                {
                    btn_on_off_toggle = false;
                    PlaySound(button_sound);
                }
            }
            else btnState_toggle = 0;
        }
        else
        {
            if (CheckCollisionPointRec(mousePoint, btnBounds_toggle))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) btnState_toggle = 2;
                else btnState_toggle = 3;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                {
                    btn_on_off_toggle = true;
                    PlaySound(button_sound);
                }
            }
            else btnState_toggle = 2;
        }

        //**
        if (btn_on_off_push) {
            if (CheckCollisionPointRec(mousePoint, btnBounds_push))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) btnState_push = 2;
                else btnState_push = 1;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                {
                    btn_on_off_push = false;
                    PlaySound(button_sound);
                    if (first.joinable())
                        first.join();
                    first = std::thread(button_push_execution);
                    //first.join();
                }
            }
            else btnState_push = 0;
        }
        else
        {
            if (CheckCollisionPointRec(mousePoint, btnBounds_push))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) btnState_push = 2;
                else btnState_push = 3;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                {
                    btn_on_off_push = true;
                    PlaySound(button_sound);
                }
            }
            else btnState_push = 2;
        }

        // Calculate button frame rectangle to draw depending on button state
        sourceRec_toggle.x = btnState_toggle * frameWidth_toggle;
        sourceRec_push.x = btnState_push * frameWidth_push;

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(GRAY);
        const char* button_col1_lable = TextFormat("RadarPC0%d:", 1);
        const char* button_row1_lable = TextFormat("ON/OFF:", 1);
        const char* button_row2_lable = TextFormat("REBOOT:", 1);
        DrawText(button_col1_lable, (screenWidth/3.0f) - 3*(TextLength(button_col1_lable)/4.0f), 10, 25, BLACK);
        DrawText(button_row1_lable, screenWidth / 30.0f - TextLength(button_row1_lable), 50, 20, BLACK);
        DrawText(button_row2_lable, screenWidth / 30.0f - TextLength(button_row1_lable), 100, 20, BLACK);
        Vector2 v_togle = {
            btnBounds_toggle.x, btnBounds_toggle.y
        };
        DrawTextureRec(button_toggle, sourceRec_toggle, v_togle, WHITE); // Draw button frame

        Vector2 v_push = {
            btnBounds_push.x, btnBounds_push.y
        };
        DrawTextureRec(button_push, sourceRec_push, v_push, WHITE); // Draw button frame

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    if (first.joinable())
        first.join();

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(button_toggle);  // Unload button texture
    UnloadTexture(button_push);  // Unload button texture
    UnloadSound(button_sound);

    CloseAudioDevice();     // Close audio device

    CloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

