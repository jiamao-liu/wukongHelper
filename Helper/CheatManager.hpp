#pragma once

#include <memory>
#include "GUI.hpp"
#include "Logger.hpp"
#include "Hook.hpp"
#include "Memory.hpp"

class CheatManager {
public:
	void start() noexcept
	{
		this->hooks = std::make_unique<Hooks>();
		this->gui = std::make_unique<GUI>();
		this->logger = std::make_unique<WukongLogger>();
		this->memory = std::make_unique<Memory>();
	}
	bool cheatState{ true };
	std::unique_ptr<GUI> gui;
	std::unique_ptr<WukongLogger> logger;
	std::unique_ptr<Hooks> hooks;
	std::unique_ptr<Memory> memory;
};

inline CheatManager cheatManager;
