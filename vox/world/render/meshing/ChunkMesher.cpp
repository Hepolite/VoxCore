
#include "vox/world/render/meshing/ChunkMesher.h"

#include "vox/world/render/meshing/MesherGreedy.h"

vox::world::render::ChunkMesher::ChunkMesher()
{
	m_thread = std::thread{ [this]() { doWork(); } };
}
vox::world::render::ChunkMesher::~ChunkMesher()
{
	m_running = false;
	clearWork();
	m_thread.join();
}

void vox::world::render::ChunkMesher::doWork()
{
	while (m_running)
	{
		ChunkMeshTask task;
		while (pollTask(task))
		{
			MesherGreedy mesher;
			mesher.mesh(task);
			pushResult(std::move(task));
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}
void vox::world::render::ChunkMesher::clearWork()
{
	std::lock_guard<std::mutex> guard{ m_mutex };
	m_tasks.clear();
	m_products.clear();
}

unsigned int vox::world::render::ChunkMesher::size()
{
	std::lock_guard<std::mutex> guard{ m_mutex };
	return m_tasks.size();
}
void vox::world::render::ChunkMesher::pushTask(ChunkMeshTask&& task)
{
	std::lock_guard<std::mutex> guard{ m_mutex };
	m_tasks.erase(task.getPos());
	m_tasks.emplace(task.getPos(), std::move(task));
}
bool vox::world::render::ChunkMesher::pollTask(ChunkMeshTask& task)
{
	std::lock_guard<std::mutex> guard{ m_mutex };
	if (m_tasks.empty())
		return false;
	const auto& begin = m_tasks.begin();
	std::swap(task, begin->second);
	m_tasks.erase(begin->first);
	return true;
}
void vox::world::render::ChunkMesher::pushResult(ChunkMeshTask&& task)
{
	std::lock_guard<std::mutex> guard{ m_mutex };
	m_products.erase(task.getPos());
	m_products.emplace(task.getPos(), std::move(task));
}
bool vox::world::render::ChunkMesher::pollResult(ChunkMeshTask& task)
{
	std::lock_guard<std::mutex> guard{ m_mutex };
	if (m_products.empty())
		return false;
	const auto& begin = m_products.begin();
	std::swap(task, begin->second);
	m_products.erase(begin->first);
	return true;
}

std::unique_ptr<vox::world::render::ChunkRenderer> vox::world::render::ChunkMesher::startTask(ChunkMeshTask&& task)
{
	pushTask(std::move(task));

	auto renderer = std::make_unique<ChunkRenderer>();
	renderer->m_mesh.addAttribute(hen::opengl::Attribute{ 0, hen::opengl::Format::FLOAT, 3, 0 });
	renderer->m_mesh.addAttribute(hen::opengl::Attribute{ 1, hen::opengl::Format::UNSIGNED_BYTE, 4, 12 });
	renderer->m_mesh.addAttribute(hen::opengl::Attribute{ 2, hen::opengl::Format::UNSIGNED_SHORT, 4, 16 });
	//renderer->m_mesh.addAttribute(hen::opengl::Attribute{ 3, hen::opengl::Format::UNSIGNED_SHORT, 2, 20 });
	renderer->m_mesh.addAttribute(hen::opengl::Attribute{ 4, hen::opengl::Format::UNSIGNED_BYTE, 4, 24 });
	return std::move(renderer);
}
