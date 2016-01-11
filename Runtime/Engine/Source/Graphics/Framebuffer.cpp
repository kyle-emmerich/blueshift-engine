#include "Graphics/Framebuffer.h"
#include "Graphics/RenderSystem.h"

using namespace Blueshift;
using namespace Graphics;

Framebuffer::Framebuffer() {

}

Framebuffer::~Framebuffer() {

}

Framebuffer& Framebuffer::AddTexture(TextureFormat Format, BackbufferRatio Ratio, uint8_t NumMips) {
	Texture* tex = new Texture(Format, Ratio, NumMips);
	this->textures.push_back(tex);
	return *this;
}

Framebuffer& Framebuffer::AddTexture(TextureFormat Format, uint16_t Width, uint16_t Height, uint8_t NumMips) {
	Texture* tex = new Texture(TextureType::Framebuffer, Format, Width, Height, 0, NumMips);
	this->textures.push_back(tex);
	return *this;
}

Framebuffer& Framebuffer::Complete() {
	bgfx::TextureHandle* handles = new bgfx::TextureHandle[this->textures.size()];
	for (int i = 0; i < this->textures.size(); i++) {
		handles[i] = this->textures[i]->GetHandle();
	}
	this->handle = bgfx::createFrameBuffer(this->textures.size(), handles, false);
	return *this;
}

Texture* Framebuffer::GetTexture(size_t index) {
	if (index >= this->textures.size()) {
		throw RuntimeError(Formatter() << "Tried to call GetTexture with invalid index of " << index << "; framebuffer only has " << this->textures.size() << " textures.");
	}
	return this->textures[index];
}

void Framebuffer::Use(uint8_t ViewID) {
	bgfx::setViewFrameBuffer(ViewID, this->handle);
}