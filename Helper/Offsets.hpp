#pragma once

#include <cstdint>

namespace offsets {
	namespace value{
		inline std::uint64_t will { 0 };// 0x1D9DFEF0;
		inline std::uint64_t will2{ 0 };//0x1D9E1938;
	};

	namespace GameObject {
		namespace VTable {
			enum {
				IsLaneMinion = 0xEB, // E8 ? ? ? ? 84 C0 0F 84 ? ? ? ? 39 1F
				IsEliteMinion = IsLaneMinion + 0x1,
				IsEpicMinion = IsEliteMinion + 0x1,
				IsMinion = IsEpicMinion + 0x4,
				IsJungle = IsMinion + 0x1
			};
		};
		enum {
			Team = 0x3C,
			Name = 0x60
		};
	};

	namespace global {
		inline std::uint64_t Player{ 0 };
		inline std::uint64_t ChampionManager{ 0 };
		inline std::uint64_t Riot__g_window{ 0 };
		inline std::uint64_t ManagerTemplate_AIMinionClient_{ 0 };
		inline std::uint64_t ManagerTemplate_AIHero_{ 0 };
		inline std::uint64_t ManagerTemplate_AITurret_{ 0 };
		inline std::uint64_t GameClient{ 0 };
	};

	namespace AIBaseCommon {
		inline std::uint64_t CharacterDataStack{ 0 };
		inline std::uint64_t SkinId{ 0 };
	};

	namespace MaterialRegistry {
		inline std::uint64_t SwapChain{ 0 };
	};

	namespace functions {
		inline std::uint64_t Renderer__MaterialRegistry__GetSingletonPtr{ 0x4FE20 };
		inline std::uint64_t translateString_UNSAFE_DONOTUSE{ 0 };
		inline std::uint64_t CharacterDataStack__Push{ 0 };
		inline std::uint64_t CharacterDataStack__Update{ 0 };
		inline std::uint64_t GetGoldRedirectTarget{ 0 };
	};
};
