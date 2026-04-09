#pragma once

#include <string>


class Category {
public:
	static const Category COMBAT;
	static const Category PLAYER;
	static const Category MOVEMENT;
	static const Category WORLD;
	static const Category CAMERA;
	static const Category RENDER;
	static const Category NETWORK;
	static const Category INVENTORY;
	static const Category CLIENT;
	static const Category MISC;
	static const Category ALL;
	
	const std::string getName() const {
		return name;
	}

private:
	Category(const std::string& name) : name(name) {}

	std::string name;
};

inline const Category Category::COMBAT = Category("combate");
inline const Category Category::PLAYER = Category("jugador");
inline const Category Category::MOVEMENT = Category("movimiento");
inline const Category Category::WORLD = Category("mundo");
inline const Category Category::CAMERA = Category("cámara");
inline const Category Category::RENDER = Category("renderizado");
inline const Category Category::NETWORK = Category("red");
inline const Category Category::INVENTORY = Category("inventario");
inline const Category Category::CLIENT = Category("cliente");
inline const Category Category::MISC = Category("misc.");
inline const Category Category::ALL = Category("todos");