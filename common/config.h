#pragma once
#include <string>
#include "./common/Color.h"

using nlohmann::json;
struct config {
	std::string Nick;
	Color color;
	config(json j){
		color = color.fromJson(j["color"]);
		Nick = j["nick"];
	}
};