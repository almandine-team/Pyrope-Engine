#pragma once

#include "objects.hpp"

//class Scene : public Item {};

class Scene : public Item {
protected:
	std::list<Item> children;
	Item* parent = nullptr;
public:
	std::string name;

	Vector2 position;
	Vector2 offset;

	bool locked;

	Scene(Item* parent = nullptr)
		: name(std::string()), position(Vector2()), offset(Vector2()), locked(false), children(children)
	{
		time_t random = time(0);
		srand(static_cast<unsigned int>(random));

		ID = static_cast<size_t>(random);
	}

	~Scene()
	{
		// Child list is being safely eliminated

		//for (size_t index; index < children.size(); index++) {
		//	children.pop_back();
		//}

		children.clear();

		// Parent removes child through accessing the ID.

		if (not (parent == nullptr)) { (*parent).removeChildByID(ID); }
	}

	bool has_children() {
		return children.empty();
	}

	bool child_exists(std::string childName) {
		std::list<Item>::iterator item = children.begin();
		for (size_t index = 0; index < children.size(); index++) {
			if (item->checkSimilarName(childName)) {
				return true;
			}
		}
		return false;
	}

	int get_child(std::string childName) {
		if (child_exists(childName)) {
			std::list<Item>::iterator item = children.begin();
			for (size_t index = 0; index < children.size(); index++) {
				if (item->checkSimilarName(childName)) {
					return index;
				}
			}
		}
	}

	void addChild(Item child) {
		children.push_back(child);
	}

	void removeChild(size_t childID) {
		std::list<Item>::iterator item = children.begin();
		for (size_t index = 0; index < children.size(); index++) {
			if (item->checkSimilarID(childID)) {
				children.erase(item);
			}
			std::advance(item, 1);
		}
	}

	void insertChildAt(Item child, size_t point) {
		children.insert(children.begin(), point, child);
	}

	void moveChildAt(Item child, size_t point) {
		children.insert(children.begin(), point, child);
	}
};

Scene* scene = nullptr;

std::list<Item> acquire(std::list<Item>::iterator iterSec) {
	std::list<Item> acquired;
	acquired.push_back(*iterSec);
	
	if (iterSec->has_children()) {
		std::list<Item>::iterator iterSec2 = iterSec->get_children().begin();
		for (size_t point = 0; point < iterSec->get_children().size(); point++) { // Goes through main parents children
			acquired.push_back(*iterSec2);
			if (iterSec2->has_children()) {
				std::list<Item> acquiredacquired = acquire(iterSec2);
				std::list<Item>::iterator iterSec3 = acquiredacquired.begin();
				for (size_t point2 = 0; point2 < acquiredacquired.size(); point2++) {
					acquired.push_back(*iterSec3);
					std::advance(iterSec3, 1); // Ending generation of that child (grand and so on).
				}
			}
			std::advance(iterSec2, 1); // Off to next child to get their ending generation
		}
	}

	return acquired;
}

std::list<Item> get_tree() {
	std::list<Item> tree; // The family tree of objects in the scene.
	if (scene != nullptr) {
		tree.push_back(*scene); // Adds scene object to tree
		std::list<Item> sector = scene->get_children(); // (Which section of children are being reviewed right now)
		while (true) { // (this should work fine, a bit dangerous in my opinion though..)
			std::list<Item>::iterator iterSec = sector.begin(); // Iterator for sector (object or selected item (both pyrope and c++) of the sector list.. index think)
			for (size_t point = 0; point < sector.size(); point++) { // (this too, unless you have A LOT of children).
				std::advance(iterSec, 1); // Maybe rename to childIteration for user friendly reading.
				tree.push_back(*iterSec);

				std::list<Item>::iterator iterSec2 = acquire(iterSec).begin();
				for (size_t point2 = 0; point2 < acquire(iterSec).size(); point2++) { tree.push_back(*iterSec2); std::advance(iterSec2, 1); }
				// Acquire() gets every generation that comes from this child
			}
		}
	}

	return tree;
}

Item& access_item(std::string itemName) {
	if (!get_tree().empty()) {
		std::list<Item>::iterator item = get_tree().begin();
		for (size_t index = 0; index < get_tree().size(); index++) {
			if (item->checkSimilarName(itemName)) {
				break;
			}
			std::advance(item, 1);
		}

		return *item;
	}

	Item* defItem = new Item();
	return *defItem; // References to a deconstructed value, make sure this won't cause issues to future referencing to items.
}

int count_dupe(const Item& child) {
	size_t copies = 0;
	std::list<Item>::iterator item = get_tree().begin();
	for (size_t currentItem = 0; currentItem < get_tree().size(); currentItem++) {
		if (copies != 0) {
			if (item->checkSimilarName((child.name + std::to_string(copies)))) {
				copies++;
			}
		}
		else {
			if (item->checkSimilarName((child.name + std::to_string(copies)))) {
				copies++;
			}
		}

		std::advance(item, 1);
	}

	return copies;
}

template<typename PotItem> // potential item

std::list<PotItem> duplicate(Item child) {
	Item new_child = child;

	time_t random = time(0);
	srand(static_cast<unsigned int>(random));

	new_child.ID = static_cast<size_t>(random);

	new_child.parent = child.getParent();

	new_child.name = child.name + std::to_string(count_dupe(child));
}
