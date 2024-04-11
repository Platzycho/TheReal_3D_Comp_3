#pragma once
#include "includes.h"

class Object;

class ObjectManager {
public:
	static ObjectManager& getInstance() {
		static ObjectManager instance;
		return instance;
	}

	void addObject(Object* obj) {
		objects.push_back(obj);

	}

	std::vector<Object*>& getObjects() {
		return objects;

	}

private:
	std::vector<Object*> objects;

	ObjectManager() {}
	ObjectManager(ObjectManager const&) = delete;
	void operator =(ObjectManager const&) = delete;


};