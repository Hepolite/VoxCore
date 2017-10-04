
#pragma once

#include <glm/vec4.hpp>

namespace vox
{
	namespace data
	{
		class BlockData
		{
		public:
			BlockData() = default;
			BlockData(unsigned int data) : m_data(data) {}
			BlockData(unsigned int id, const glm::ivec4& light) { setId(id); setLight(light); }

			inline bool operator==(const BlockData& other) const { return m_data == other.m_data; }
			inline bool operator!=(const BlockData& other) const { return !(*this == other); }

			inline unsigned int getData() const { return m_data; }
			inline unsigned int getId() const { return m_data >> 20; }
			inline glm::ivec4 getLight() const { return glm::ivec4{ m_data >> 15, m_data >> 10, m_data >> 5, m_data } & 0x1F; }

			inline void setData(unsigned int data) { m_data = data; }
			inline void setId(unsigned int id) { m_data = id << 20; };
			inline void setLight(const glm::ivec4& light) { const auto l = light & 0x1f; m_data = (m_data & 0xFFF00000) | (l.x << 15) | (l.y << 10) | (l.z << 5) | l.w; }

		private:
			unsigned int m_data = 0;
		};
	}
}