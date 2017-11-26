
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
			Block() = default;
			Block(const std::string& name, unsigned int id) : m_name(name), m_id(id) {}
			~Block() = default;

			inline std::string getName() const { return m_name; }
			inline unsigned int getId() const { return m_id; }

			inline void setLightEmittance(const glm::uvec4& emittance) { m_lightEmittance = emittance; }
			inline void setLightAbsorption(const glm::uvec4& absorption) { m_lightAbsorption = absorption; }
			inline void setLightFilter(const glm::uvec4& filter) { m_lightFilter = filter; }

			inline glm::uvec4 getLightEmittance() const { return m_lightEmittance; }
			inline glm::uvec4 getLightAbsorption() const { return m_lightAbsorption; }
			inline glm::uvec4 getLightFilter() const { return m_lightFilter; }

			inline bool doesEmitLight() const { return m_lightEmittance.x + m_lightEmittance.y + m_lightEmittance.z + m_lightEmittance.w != 0; }

		private:
			std::string m_name = "";
			unsigned int m_id = 0;

			glm::uvec4 m_lightEmittance;
			glm::uvec4 m_lightAbsorption;
			glm::uvec4 m_lightFilter;
		};
	}
}