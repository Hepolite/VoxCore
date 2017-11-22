
#pragma once

#include <glm/glm.hpp>

#include <string>

namespace vox
{
	namespace world
	{
		class Block
		{
		public:
			Block() = delete;
			Block(const std::string& name, unsigned int id) : m_name(name), m_id(id) {}
			~Block() = default;

			inline std::string getName() const { return m_name; }
			inline unsigned int getId() const { return m_id; }

			inline void setColorEmittance(const glm::ivec4& emittance) { m_colorEmittance = emittance; }
			inline void setColorAbsorption(const glm::ivec4& absorption) { m_colorAbsorption = absorption; }
			inline void setColorFilter(const glm::ivec4& filter) { m_colorFilter = filter; }

			inline glm::ivec4 getColorEmittance() const { return m_colorEmittance; }
			inline glm::ivec4 getColorAbsorption() const { return m_colorAbsorption; }
			inline glm::ivec4 getColorFilter() const { return m_colorFilter; }

		private:
			std::string m_name;
			unsigned int m_id;

			glm::ivec4 m_colorEmittance;
			glm::ivec4 m_colorAbsorption;
			glm::ivec4 m_colorFilter;
		};
	}
}