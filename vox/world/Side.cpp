
#include "vox/world/Side.h"

const vox::world::Side vox::world::Side::NORTH{ 0, SOUTH, glm::ivec3{ 1, 2, 0}, glm::ivec3{ 0, 1, 0 }, glm::ivec3{ 0, 0, 1 }, glm::ivec3{ 1, 0, 0 } };
const vox::world::Side vox::world::Side::SOUTH{ 1, NORTH, glm::ivec3{ 1, 2, 0 }, glm::ivec3{ 0, -1, 0 }, glm::ivec3{ 0, 0, 1 }, glm::ivec3{ -1, 0, 0 } };
const vox::world::Side vox::world::Side::WEST{ 2, EAST, glm::ivec3{ 0, 2, 1 }, glm::ivec3{ -1, 0, 0 }, glm::ivec3{ 0, 0, 1 }, glm::ivec3{ 0, 1, 0 } };
const vox::world::Side vox::world::Side::EAST{ 3, WEST, glm::ivec3{ 0, 2, 1 }, glm::ivec3{ 1, 0, 0 }, glm::ivec3{ 0, 0, 1 }, glm::ivec3{ 0, -1, 0 } };
const vox::world::Side vox::world::Side::TOP{ 4, BOTTOM, glm::ivec3{ 0, 1, 2 }, glm::ivec3{ 0, -1, 0 }, glm::ivec3{ 1, 0, 0 }, glm::ivec3{ 0, 0, 1 } };
const vox::world::Side vox::world::Side::BOTTOM{ 5, TOP, glm::ivec3{ 0, 1, 2 }, glm::ivec3{ 0, 1, 0 }, glm::ivec3{ 1, 0, 0 }, glm::ivec3{ 0, 0, -1 } };
const vox::world::Side vox::world::Side::UNKNOWN{ 6, UNKNOWN, glm::ivec3{}, glm::ivec3{}, glm::ivec3{}, glm::ivec3{} };

const vox::world::Side vox::world::Side::SIDES[vox::world::Side::COUNT] = { NORTH, SOUTH, WEST, EAST, TOP, BOTTOM, UNKNOWN };

vox::world::Side::Side(unsigned int id, const Side& opposite, const glm::ivec3& axis, const glm::ivec3& x, const glm::ivec3& y, const glm::ivec3& z)
	: id(id), opposite(opposite), axis(axis), x(x), y(y), z(z)
{}

const vox::world::Side& vox::world::Side::fromName(const std::string& name)
{
	if (name == "north")
		return Side::NORTH;
	else if (name == "east")
		return Side::EAST;
	else if (name == "south")
		return Side::SOUTH;
	else if (name == "west")
		return Side::WEST;
	else if (name == "top")
		return Side::TOP;
	else if (name == "bottom")
		return Side::BOTTOM;
	else
		return Side::UNKNOWN;
}

