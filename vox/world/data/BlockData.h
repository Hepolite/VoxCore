
#pragma once

#include <glm/vec4.hpp>

#include <vector>

namespace vox
{
	namespace data
	{
		class BlockData
		{
		public:
			static const unsigned int BITMASK_ALL = 0xFFFFFFFFu;
			static const unsigned int BITMASK_ID = 0xFFF00000u;
			static const unsigned int BITMASK_LIGHT = 0x000FFFFFu;
			static const unsigned int BITMASK_COLOR = 0x000FFFE0u;
			static const unsigned int BITMASK_ALPHA = 0x0000001Fu;

			BlockData() noexcept = default;
			BlockData(unsigned int data) noexcept : m_data(data) {}
			BlockData(unsigned int id, const glm::uvec4& light) noexcept { setId(id); setLight(light); }
			BlockData(const BlockData&) noexcept = default;
			BlockData(BlockData&&) noexcept = default;

			inline BlockData& operator=(const BlockData&) noexcept = default;
			inline BlockData& operator=(BlockData&&) noexcept = default;
			inline bool operator==(const BlockData& other) const { return m_data == other.m_data; }
			inline bool operator!=(const BlockData& other) const { return !(*this == other); }
			inline BlockData operator|(const unsigned int mask) const { return m_data | mask; }
			inline void operator|=(const unsigned int mask) { m_data |= mask; }
			inline BlockData operator&(const unsigned int mask) const { return m_data & mask; }
			inline void operator&=(const unsigned int mask) { m_data &= mask; }

			inline unsigned int getData() const { return m_data; }
			inline unsigned int getId() const { return m_data >> 20u; }
			inline glm::uvec4 getLight() const { return glm::uvec4{ m_data >> 15u, m_data >> 10u, m_data >> 5u, m_data } & 0x1Fu; }

			inline void setData(unsigned int data) { m_data = data; }
			inline void setId(unsigned int id) { m_data = (m_data & BITMASK_LIGHT) | (id << 20u); };
			inline void setLight(const glm::uvec4& light) { const auto l = light & 0x1Fu; m_data = (m_data & BITMASK_ID) | (l.x << 15u) | (l.y << 10u) | (l.z << 5u) | l.w; }

		private:
			unsigned int m_data = 0;
		};

		using BlockDataList = std::vector<BlockData>;
	}
}