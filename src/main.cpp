#include <iostream>
#include <windows.h>
#include <chrono>
#include <thread>
#include <cmath>
#include <algorithm>

using std::cout;
using std::cin;
using std::endl;
using namespace std::chrono;

int main()
{
    // vars
    float speed = 0.1;
    float intensity = 100;
    int res_x = 1280;
    int res_y = 720;
    HWND cWindow = GetConsoleWindow();
    SetWindowTextA(cWindow, "gd groover");

    char use_default;

    cout << "gd groover 1.1\n=================\n";

    while (true)
    {
        cout << "would you like to use default settings instead of putting your own? (Y/N)";
        cin >> use_default;

        if ((char) tolower(use_default) == 'n')
        {
            cout << "enter the speed of spinning (between 0.05 and 0.3 is good): ";
            cin >> speed;
            cout << "enter radius/intensity of spinning (in pixels, around 50-300 is good): ";
            cin >> intensity;
            cout << "enter window width in pixels (example: if you want 1600x900, put in 1600): ";
            cin >> res_x;
            cout << "enter window height in pixels (example: if you want 1600x900, put in 900): ";
            cin >> res_y;
        }

        // inital setup

        HWND window = FindWindowA(NULL, "Geometry Dash");
        if (!window)
        {
            cout << "groover is ready! please open geometry dash to start.\n";
            while (!window)
            {
                Sleep(1000);
                window = FindWindowA(NULL, "Geometry Dash");
                if (window)
                {
                    cout << "Window found, ID: " << window << "\n";
                    break;
                }
            }
        }

        // precalc
        float xOffset = 1900 - res_x - intensity * 1.5;
        float yOffset = 950.0 - res_y - intensity;

        // Sine lookup table by "Eric" https://stackoverflow.com/a/13042151/ , modified for calculating each position.
        float *xPositions = (float*)malloc(3140 * sizeof(float));
        float *yPositions = (float*)malloc(3140 * sizeof(float));

        float t = 1.0;
        for (int i=1; i < 3140; i++, t += speed)
        {
            xPositions[i] = sin(t - 1.57) * intensity + xOffset;
            yPositions[i] = sin(t) * intensity + yOffset;
        }

        // initialize loop variables
        float frame_d;
        int frames;
        int c_status = 0;

        // main loop (infinite until closed)
        for (int i = 0; i < 3141; i++)
        {
            auto start = high_resolution_clock::now();
            c_status = MoveWindow(window, xPositions[i], yPositions[i], res_x, res_y, true);
            if (!c_status)
            {
                break;
            }
            auto stop = high_resolution_clock::now();
            // calculate how many frames to skip
            frame_d = duration_cast<milliseconds>(stop - start).count();
            if (frame_d > 18.0) // skip frames
            {
                frames = frame_d / 13 - 1;
                i += frames;
            }
            if (i > 3138) { // Reset
                i = 0;
            }
            Sleep(1);

        }
        // exit or change settings?
        char change_settings;

        cout << "Geometry Dash has been closed, would you like to change settings? (Y/N)" << endl;
        cin >> change_settings;

        if ((char) tolower(change_settings) == 'n')
        {
            std::string ex;
            cout << "Exiting, press enter to close this window." << endl;
            cin >> ex;
            break; // close program
        }

    }
    return 0;
}
