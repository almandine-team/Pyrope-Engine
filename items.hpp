#pragma once

#include "engine.hpp"
#include "string"
#include "Cstring"
#include "vector"
#include "list"
#include "map"
#include "algorithm"

//class Item {};
//class Object : public Item {};
//class Folder : public Item {};

void acquire() {}

class Item {
protected:
	std::list<Item> children;
	Item* parent;

	std::map<std::string, Item> properties;
public:
	std::string path;
	std::string name;

	size_t ID;

	Item()
		: name(std::string()), parent(nullptr)
	{
		time_t random = time(0);
		srand(static_cast<unsigned int>(random));

		ID = static_cast<size_t>(random);
	}

	bool has_children() { return (children.empty()); }
	virtual std::list<Item> get_children() { return children; }
	virtual bool child_exists(std::string childName) { return false; }
	virtual std::list<Item>::iterator* get_child(std::string childName) { return nullptr; }
	virtual void addChild(Item child) {}
	virtual void removeChild(Item child) {}
	virtual void removeChildByName(std::string name) {}
	virtual void removeChildByID(size_t childID) {}
	virtual void insertChildAt(Item child, int point) {}
	virtual void moveChildAt(Item child, int point) {}

	virtual bool has_parent() { return (parent != nullptr); }
	virtual Item* getParent() { return new Item(); }

	virtual void property_exists() {} // Properties (Collisions for example)

	bool checkSimilarName(std::string compared) {
		return name == compared;
	}

	bool checkSimilarID(size_t compared) {
		return ID == compared;
	}
};

class Object: public Item {
protected:
	std::list<Item> children;
	Item* parent;
public:
	std::string name;

	Vector2 position;
	Vector2 offset;

	bool locked;

	Object(Item* parent = nullptr)
		: name(std::string()), position(Vector2()), offset(Vector2()), locked(false), children(children), parent(parent)
	{
		time_t random = time(0);
		srand(static_cast<unsigned int>(random));

		ID = static_cast<size_t>(random);
	}

	~Object()
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

	std::list<Item>::iterator* get_child(std::string childName) {
		if (child_exists(childName)) {
			std::list<Item>::iterator item = children.begin();
			for (size_t index = 0; index < children.size(); index++) {
				if (item->checkSimilarName(childName)) {
					return &item;
				}
			}
		}
		else {
			return nullptr;
		}
	}

	void addChild(Item child) {
		children.push_back(child);
	}

	void removeChild(Item child) {
		std::list<Item>::iterator item = children.begin();
		for (size_t index = 0; index < children.size(); index++) {
			if (item->checkSimilarID(child.ID)) {
				// [ignore] children.remove(child); // Sets aside the child instead of deleting it from memory (might be useful with undoing and redoing)
				//children.erase(children.begin(), children.end(), index);

				children.erase(item);
			}
			std::advance(item, 1);
		}
	}

	void removeChildByName(std::string childName) {
		std::list<Item>::iterator item = children.begin();
		for (size_t index = 0; index < children.size(); index++) {
			if (item->checkSimilarName(childName)) {
				// [ignore] children.remove(child); // Sets aside the child instead of deleting it from memory (might be useful with undoing and redoing)
				//children.erase(children.begin(), children.end(), index);

				//auto point = std::find(children.begin(), children.end(), index);

				//if (point != children.end()) {
				//	children.erase(point);
				//}

				children.erase(item);
				break;
			}
			else { std::advance(item, 1); }
		}
	}

	void removeChildByID(size_t childID) {
		std::list<Item>::iterator item = children.begin();
		for (size_t index = 0; index < children.size(); index++) {
			if (item->checkSimilarID(childID)) {
				children.erase(item);
				break;
			}
			else { std::advance(item, 1); }
		}
	}

	void insertChildAt(Item child, size_t point) {
		children.insert(children.begin(), point, child);
	}

	void moveChildAt(Item child, size_t point) {
		children.insert(children.begin(), point, child);
	}

	bool has_parent() {
		return !(parent == nullptr);
	}

	Item* get_parent() {
		return parent;
	}
};

class Folder : public Item {
public:
	std::string name;

	bool locked;

	std::list<Item> children;
	Item* parent;

	Folder()
		: name(std::string()), locked(bool()), parent(nullptr) {}

	~Folder()
	{
		children.clear();

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

	std::list<Item>::iterator* get_child(std::string childName) {
		if (child_exists(childName)) {
			std::list<Item>::iterator item = children.begin();
			for (size_t index = 0; index < children.size(); index++) {
				if (item->checkSimilarName(childName)) {
					return &item;
				}
			}
		}
		else {
			return nullptr;
		}
	}

	void addChild(Item child) {
		children.push_back(child);
	}

	void removeChild(Item child) {
		std::list<Item>::iterator item = children.begin();
		for (size_t index = 0; index < children.size(); index++) {
			if (item->checkSimilarID(child.ID)) {
				// [ignore] children.remove(child); // Sets aside the child instead of deleting it from memory (might be useful with undoing and redoing)
				//children.erase(children.begin(), children.end(), index);

				children.erase(item);
			}
			std::advance(item, 1);
		}
	}

	void removeChildByName(std::string childName) {
		std::list<Item>::iterator item = children.begin();
		for (size_t index = 0; index < children.size(); index++) {
			if (item->checkSimilarName(childName)) {
				// [ignore] children.remove(child); // Sets aside the child instead of deleting it from memory (might be useful with undoing and redoing)
				//children.erase(children.begin(), children.end(), index);

				//auto point = std::find(children.begin(), children.end(), index);

				//if (point != children.end()) {
				//	children.erase(point);
				//}

				children.erase(item);
				break;
			}
			else { std::advance(item, 1); }
		}
	}

	void removeChildByID(size_t childID) {
		std::list<Item>::iterator item = children.begin();
		for (size_t index = 0; index < children.size(); index++) {
			if (item->checkSimilarID(childID)) {
				children.erase(item);
				break;
			}
			else { std::advance(item, 1); }
		}
	}

	void insertChildAt(Item child, size_t point) {
		children.insert(children.begin(), point, child);
	}

	void moveChildAt(Item child, size_t point) {
		children.insert(children.begin(), point, child);
	}

	Item* get_parent() {
		return parent;
	}
};

// Linked properties

/* to do:
* add initializeNode() function
* add createNode() function
*/

// Functions

// Initializers:
//
//auto CreateNode(std::string name, Vector2 position = Vector2(), Vector2 offset = Vector2(), std::vector<Item> children[], std::vector<Item> parent[]) {
//	Node test = new Node(name, position, offset);
//	return new Node(name, position, offset,);
//}