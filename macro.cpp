#include <Windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>

// Function to simulate a key press
void PressKey(WORD key) {
    INPUT ip = { 0 };
    ip.type = INPUT_KEYBOARD;
    ip.ki.wVk = key; // Virtual-key code for the key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // Key release
    SendInput(1, &ip, sizeof(INPUT));
}

int main() {
    // Open the timing file
    std::ifstream infile("timing.txt");
    if (!infile.is_open()) {
        std::cerr << "Unable to open timing.txt" << std::endl;
        return 1;
    }

    // Read timings from the file
    std::string line;
    std::vector<int> timings;
    while (std::getline(infile, line)) {
        try {
            timings.push_back(std::stoi(line));
        }
        catch (const std::invalid_argument& e) {
            std::cerr << "Invalid timing value: " << line << std::endl;
            return 1;
        }
        catch (const std::out_of_range& e) {
            std::cerr << "Timing value out of range: " << line << std::endl;
            return 1;
        }
    }
    infile.close();

    if (timings.empty()) {
        std::cerr << "No timings found in timing.txt" << std::endl;
        return 1;
    }

    // Wait for 5 seconds before starting
    std::cout << "Starting in 5 seconds..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // Press keys based on the timings read from the file
    for (size_t i = 0; i < timings.size(); ++i) {
        std::cout << "Pressing key " << i + 1 << " with delay " << timings[i] << " ms" << std::endl;
        PressKey(A);
        std::this_thread::sleep_for(std::chrono::milliseconds(timings[i]));
    }

    std::cout << "Macro finished." << std::endl;
    return 0;
}
