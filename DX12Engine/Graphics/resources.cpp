#include "resources.h"

namespace engine::gfx
{
	/////// TEXTURE /////////////////////

	Texture& Texture::operator=(const Texture& o)
	{
		assert(&o != this);
		m_res = o.m_res;
		o.m_res->AddRef();
		m_srv = o.m_srv;
		o.m_srv->AddRef();
		return *this;
	}

	Texture::Texture(Texture&& o)
	{
		assert(&o != this);
		m_res = MOVE(m_res);
		m_srv = MOVE(m_srv);
	}

	Texture& Texture::operator=(Texture&& o)
	{
		assert(&o != this);
		m_res = MOVE(m_res);
		m_srv = MOVE(m_srv);
		return *this;
	}

	void Texture::Release()
	{
		SRVHeap.Free(m_srv);
		RELEASE(m_res);
	}
}