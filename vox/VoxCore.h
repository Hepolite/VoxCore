
#pragma once

namespace vox
{
	namespace world { class Universe; }
	namespace world { namespace io { class UniverseDataFile; } }
	namespace world { namespace render { class UniverseRenderer; } }

	class VoxCore
	{
	public:
		VoxCore();
		VoxCore(const VoxCore&) = delete;
		VoxCore(VoxCore&&) = delete;
		~VoxCore();

		VoxCore& operator=(const VoxCore&) = delete;
		VoxCore& operator=(VoxCore&&) = delete;

		static world::Universe& getUniverse();
		static world::io::UniverseDataFile& getUniverseDataFile();
		static world::render::UniverseRenderer& getUniverseRenderer();

	private:
		class Internal;
		static Internal* m_internal;
	};
}