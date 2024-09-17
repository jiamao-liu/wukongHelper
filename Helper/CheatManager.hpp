#pragma once

#include <memory>
#include "GUI.hpp"
#include "Logger.hpp"
#include "Hook.hpp"
#include "Memory.hpp"

class CheatManager {
public:

	bool cheatState{ true };
	std::unique_ptr<GUI> gui = std::make_unique<GUI>();
	std::unique_ptr<WukongLogger> logger = std::make_unique<WukongLogger>();
	std::unique_ptr<Hooks> hooks = std::make_unique<Hooks>();
	std::unique_ptr<Memory> memory = std::make_unique<Memory>();
};

inline CheatManager cheatManager;
