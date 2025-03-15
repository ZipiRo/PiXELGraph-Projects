#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>
#include <string>
#include <vector>
#include <list>
#include <conio.h>
#include <thread>
#include <chrono>
#include <random>
#include "PixelGraph.h"

////
//  By ZipiRo
////

// Constants
const int RES = 10;              // Grid resolution
const int WIDTH = RES + 1;       // Canvas width
const int HEIGHT = RES + 5;      // Canvas height

// Initialize screen with specified dimensions
Screen screen(WIDTH, HEIGHT);

int main() {
    // Initialize colors
    Color::Color cursorColor = Color::Red;
    Color::Color indexOneColor = Color::Transparent;
    Color::Color indexTwoColor = Color::Transparent;

    // Variables for points and indices
    Vector2 cursorPosition;
    std::vector<Vector2> points;
    std::vector<int> indices;
    char character[10];

    // State variables
    bool DRAWPOINTS = true;
    bool DRAWOUTLINE = false;
    bool DRAWCROSSHAIR = false;
    bool INDICESSTATE = false;
    bool SAVE = false;
    bool INDICESDONE = false;

    int indexCount = 1;
    Vector2 indexOne;
    Vector2 indexTwo;

    int curentKey;
    bool running = true;

    // Main loop
    while (running) {
        // Check for keyboard input
        if (_kbhit())
            curentKey = _getch();

        // Global controls
        if (curentKey == Key::SPACE) CLEAR_CONSOLE;
        if (curentKey == Key::ESCAPE) running = false;

        // Cursor movement
        Vector2 cursorDirection;
        if (curentKey == Key::ARROW_DOWN) cursorDirection = DOWN;
        if (curentKey == Key::ARROW_UP) cursorDirection = UP;
        if (curentKey == Key::ARROW_RIGHT) cursorDirection = RIGHT;
        if (curentKey == Key::ARROW_LEFT) cursorDirection = LEFT;

        // Toggle outline drawing
        if (INDICESDONE && curentKey == 'd') DRAWOUTLINE = !DRAWOUTLINE;

        // Toggle crosshair
        if (curentKey == 'c') DRAWCROSSHAIR = !DRAWCROSSHAIR;

        // Switch between modes
        if (curentKey == 'q') INDICESSTATE = !INDICESSTATE;

        // **Point Mode**
        if (!INDICESSTATE) {
            if (!INDICESDONE) {
                if (curentKey == 'p') DRAWPOINTS = true;

                if (curentKey == Key::ENTER) points.emplace_back(cursorPosition);

                if (curentKey == '-') {
                    if (!points.empty()) points.pop_back();
                }
            } else if (curentKey == 'S') {
                running = false;
                SAVE = true;
            }
        }

        // **Indices Mode**
        else {
            // Add index
            if (curentKey == Key::ENTER) {
                int i = 0;
                for (const Vector2 &point : points) {
                    if (cursorPosition == point) {
                        indices.emplace_back(i);

                        // Reset colors
                        if (indexCount > 2) {
                            indexOneColor = Color::Transparent;
                            indexTwoColor = Color::Transparent;
                            indexCount = 1;
                        }

                        if (indexCount == 1) {
                            indexOne = point;
                            indexOneColor = Color::IntenseGreen;
                        } else if (indexCount == 2) {
                            indexTwo = point;
                            indexTwoColor = Color::IntenseBlue;
                        }
                        indexCount++;
                    }
                    i++;
                }
            }

            // Reset indices
            if (curentKey == 'R') {
                indices.clear();
                INDICESDONE = false;
                DRAWOUTLINE = false;
                DRAWPOINTS = true;
                indexOneColor = Color::Transparent;
                indexTwoColor = Color::Transparent;
                indexCount = 1;
            }

            // Save indices
            if (curentKey == 'S' && indices.size() > 1) {
                INDICESDONE = true;
                INDICESSTATE = false;
                DRAWOUTLINE = true;
                DRAWPOINTS = false;
            }
        }

        curentKey = 0;

        // Update cursor color based on mode
        cursorColor = INDICESSTATE ? Color::IntensePurple : Color::Red;

        // Move cursor
        cursorPosition += cursorDirection;

        // Clear screen
        screen.Clear();

        // Draw horizontal divider line
        DrawLine(screen, 0, HEIGHT - 4, WIDTH, HEIGHT - 4, Color::IntenseWhite);

        // Draw crosshair
        if (DRAWCROSSHAIR) {
            DrawLine(screen, 0, cursorPosition.y, WIDTH, cursorPosition.y, Color::IntenseWhite);
            DrawLine(screen, cursorPosition.x, 0, cursorPosition.x, HEIGHT, Color::IntenseWhite);
        }

        // Draw outline
        if (DRAWOUTLINE && INDICESDONE) {
            for (int i = 0; i + 1 < indices.size(); i += 2) {
                int a = indices[i];
                int b = indices[i + 1];
                DrawLine(screen, points[a].x, points[a].y, points[b].x, points[b].y, Color::Black);
            }
        }

        // Draw points
        if (DRAWPOINTS) {
            for (const Vector2 &point : points) {
                screen.PutPixel(point.x, point.y, Color::Blue);
            }
        }

        // Draw cursor
        screen.PutPixel(cursorPosition.x, cursorPosition.y, cursorColor);

        // Draw indices
        if (INDICESSTATE) {
            screen.PutPixel(indexOne.x, indexOne.y, indexOneColor);
            screen.PutPixel(indexTwo.x, indexTwo.y, indexTwoColor);
        }

        // Render the screen
        screen.Display();

        // Frame delay
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // Cleanup
    RESET_CURSOR_POSITION;
    CLEAR_CONSOLE;

    if (!SAVE) return 0;

    std::ofstream out;
    // **Parsing and Saving Data**
    std::cin >> character;
    out.open(strcat(character, ".txt"));

    out << "\"" << character[0] << "\": {\n";
    out << "     \"vertices\": [";
    for (size_t i = 0; i < points.size(); ++i) {
        out << "[" << float(points[i].x) / RES << ", " << float(points[i].y) / RES << "]";
        if (i != points.size() - 1) out << ", ";
    }
    out << "],\n";

    out << "     \"indices\": [";
    for (size_t i = 0; i < indices.size(); ++i) {
        out << indices[i];
        if (i != indices.size() - 1) out << ", ";
    }
    out << "],\n";

    out << "     \"advance\": 1.0\n";
    out << "},\n";

    out.close();
    return 0;
}
