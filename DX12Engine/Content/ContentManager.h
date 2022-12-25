#pragma once
#include <boost/serialization/serialization.hpp>
#include <memory>

namespace engine::content
{
	bool LoadEngineShaders(std::unique_ptr<uint8_t[]>& shaders, uint64_t& size);
}

