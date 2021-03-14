#pragma once

#include <set>
#include <string>
#include <filesystem>

namespace MyEngine {
	class PluginLoader {
	public:
		static void loadStaticPlugins();
		static void loadDynamicPlugins(std::filesystem::path);
	private:
		static std::set<std::string> loadedPlugins;
	};
}
