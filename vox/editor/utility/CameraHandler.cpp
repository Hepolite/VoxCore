
#include "vox/editor/utility/CameraHandler.h"

#include "hen/core/Core.h"
#include "hen/render/RenderCore.h"
#include "hen/ui/Display.h"
#include "hen/ui/Keyboard.h"
#include "hen/ui/Mouse.h"
#include "hen/util/MathLib.h"

void vox::editor::util::CameraHandler::process(float dt)
{
	processOrbital(dt);

	const auto& camera = hen::Core::getRenderCore().getMainCamera();
	m_position = camera.getPosition();
	m_direction = hen::math::toCartesian(camera.getRotation());
	m_vector = camera.getVector(hen::Core::getMouse().getPos());
}

void vox::editor::util::CameraHandler::processPersonal(float dt)
{
	auto& camera = hen::Core::getRenderCore().getMainCamera();
	const auto& keyboard = hen::Core::getKeyboard();
	const auto& mouse = hen::Core::getMouse();
	const auto& delta = mouse.getDeltaPos();

	const auto moveFactor = dt * m_sensitivity * std::sqrt(m_distance) * (keyboard.isHeld(ALLEGRO_KEY_R) ? 10.0f : 1.0f);
	const auto rotFactor = dt * m_sensitivity * glm::vec2{ 15.0f, 10.0f };
	
	auto pos = camera.getPosition();
	auto rot = camera.getRotation();

	if (keyboard.isHeld(ALLEGRO_KEY_W))
		pos += moveFactor * hen::math::toCartesian(rot);
	else if (keyboard.isHeld(ALLEGRO_KEY_S))
		pos -= moveFactor * hen::math::toCartesian(rot);
	if (keyboard.isHeld(ALLEGRO_KEY_A))
		pos += moveFactor * hen::math::toCartesian(rot.x + 90.0f, 0.0f);
	else if (keyboard.isHeld(ALLEGRO_KEY_D))
		pos -= moveFactor * hen::math::toCartesian(rot.x + 90.0f, 0.0f);
	if (keyboard.isHeld(ALLEGRO_KEY_SPACE))
		pos.z += moveFactor;
	else if (keyboard.isHeld(ALLEGRO_KEY_LCTRL))
		pos.z -= moveFactor;

	if (!keyboard.isHeld(ALLEGRO_KEY_TAB))
	{
		rot -= rotFactor * mouse.getDeltaPos();
		mouse.setPos(glm::ivec2{ hen::Core::getDisplay().getWidth() / 2, hen::Core::getDisplay().getHeight() / 2 });
	}

	camera.setPosition(pos);
	camera.setRotation(rot);
}
void vox::editor::util::CameraHandler::processOrbital(float dt)
{
	auto& camera = hen::Core::getRenderCore().getMainCamera();
	const auto& keyboard = hen::Core::getKeyboard();
	const auto& mouse = hen::Core::getMouse();
	const auto& delta = mouse.getDeltaPos();

	const auto moveFactor = dt * m_sensitivity * std::sqrt(m_distance);
	const auto rotFactor = dt * m_sensitivity * glm::vec2{ 30.0f, 20.0f };
	const auto scaleFactor = dt * m_sensitivity * (m_distance > 1.0f ? std::powf(m_distance, 0.3f) : 1.0f) * mouse.getDeltaScroll();

	auto pos = camera.getPosition();
	auto rot = camera.getRotation();

	if (scaleFactor != 0)
	{
		const auto oldDistance = m_distance;
		m_distance = hen::math::min(m_maxDistance, (1.0f - scaleFactor) * m_distance);
		pos -= (m_distance - oldDistance) * hen::math::toCartesian(rot);

	}

	if (mouse.isHeld(MOUSE_MIDDLE))
	{
		if (keyboard.isHeld(ALLEGRO_KEY_LSHIFT))
		{
			if (keyboard.isHeld(ALLEGRO_KEY_LCTRL))
				pos += moveFactor * delta.y * hen::math::toCartesian(rot);
			else
				pos += moveFactor * delta.y * hen::math::toCartesian(rot.x, rot.y + 90.0f);
			pos += moveFactor * delta.x * hen::math::toCartesian(rot.x + 90.0f, 0.0f);
		}
		else
		{
			const auto oldRot = rot;
			rot += rotFactor * delta * (keyboard.isHeld(ALLEGRO_KEY_LCTRL) ? 1.0f : -1.0f);
			rot.y = rot.y < -89.9f ? -89.9f : rot.y > 89.9f ? 89.9f : rot.y;

			if (!keyboard.isHeld(ALLEGRO_KEY_LCTRL))
				pos += m_distance * (hen::math::toCartesian(oldRot) - hen::math::toCartesian(rot));
		}
	}

	camera.setPosition(pos);
	camera.setRotation(rot);
}
