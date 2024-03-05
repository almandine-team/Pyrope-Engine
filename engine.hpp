#pragma once

#include "iostream"
#include "raylib.h"

void Log(std::string output, std::string relevance = "Note") {
	Color logCol = RAYWHITE; // By default in case an issue occurs
	if (relevance == "Note") { logCol = DARKPURPLE; }
	DrawText(output.c_str(), 50, 50, 30, logCol);
}