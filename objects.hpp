#pragma once

#include "items.hpp"

class Entity : public Object {
public:
	std::string name;
	std::string path;

	std::list<Item> Collisions;
};