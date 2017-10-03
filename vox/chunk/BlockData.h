
#pragma once

namespace vox
{
	namespace chunk
	{
		struct BlockData
		{
			BlockData() = default;
			BlockData(unsigned short id) : m_id(id) {}

			inline bool operator==(const BlockData& other) const { return m_id == other.m_id; }
			inline bool operator!=(const BlockData& other) const { return !(*this == other); }

			unsigned short m_id = 0;
		};
	}
}