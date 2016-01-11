#include "Graphics/Texture.h"
#include "Storage/File.h"

using namespace Blueshift;
using namespace Graphics;

Texture::Texture(
	TextureType Type,
	TextureFormat Format,
	uint16_t Width, uint16_t Height, uint16_t Depth,
	uint8_t NumMips,
	RenderData Data
	) : type(Type), format(Format), width(Width), height(Height), depth(Depth),
	num_mips(NumMips) {

	switch (Type) {
	case TextureType::Texture2D:
	{
		texture = bgfx::createTexture2D(width, height, num_mips, (bgfx::TextureFormat::Enum)format, flags, Data);
	}
	break;
	case TextureType::Texture3D:
	{
		texture = bgfx::createTexture3D(width, height, depth, num_mips, (bgfx::TextureFormat::Enum)format, flags, Data);
	}
	break;
	case TextureType::TextureCube:
	{
		texture = bgfx::createTextureCube(width, num_mips, (bgfx::TextureFormat::Enum)format, flags, Data);
	}
	case TextureType::Framebuffer:
	{	
		flags = BGFX_TEXTURE_RT			| 
				BGFX_TEXTURE_MIN_POINT	|
				BGFX_TEXTURE_MAG_POINT	|
				BGFX_TEXTURE_MIP_POINT	|
				BGFX_TEXTURE_U_CLAMP	|
				BGFX_TEXTURE_V_CLAMP;
		texture = bgfx::createTexture2D(width, height, num_mips, (bgfx::TextureFormat::Enum)format, flags, Data);
	}
	break;
	default:
		throw 0;
	}
}

Texture::Texture(TextureFormat Format, BackbufferRatio Ratio, uint8_t NumMips) {
	flags = BGFX_TEXTURE_RT |
		BGFX_TEXTURE_MIN_POINT |
		BGFX_TEXTURE_MAG_POINT |
		BGFX_TEXTURE_MIP_POINT |
		BGFX_TEXTURE_U_CLAMP |
		BGFX_TEXTURE_V_CLAMP;
	texture = bgfx::createTexture2D((bgfx::BackbufferRatio::Enum)Ratio, NumMips, (bgfx::TextureFormat::Enum)format, flags);
}

Texture::Texture(std::string path, bool keep_copy) {
	Storage::File f(path);
	from_file(f, keep_copy);
}
Texture::Texture(Storage::File* file, bool keep_copy) {
	from_file(*file, keep_copy);
}

void Texture::from_file(Storage::File& file, bool keep_copy) {
	size_t length = file.GetLength();
	RenderData Data = AllocateRenderData(length);
	file.Read(Data->data, length);

	if (keep_copy) {
		data = new uint8_t[Data->size];
		size = Data->size;

		memcpy(data, Data->data, size);
	}

	bgfx::TextureInfo info;
	texture = bgfx::createTexture(
		Data,
		flags, 0, &info
		);
	if (info.cubeMap) {
		type = TextureType::TextureCube;
	} else {
		//TODO: review dds capabilities and possibly choose between
		//2D and 3D
		//but 3D ddses aren't that common anyway
		type = TextureType::Texture2D;
	}
	format = (TextureFormat)info.format;
	width = info.width;
	height = info.height;
	depth = info.depth;
	num_mips = info.numMips;
}

Texture::~Texture() {
	if (texture.idx != bgfx::invalidHandle) {
		bgfx::destroyTexture(texture);
	}

	if (data != nullptr) {
		delete[] data;
	}
}

void Texture::AssignTo(uint8_t Unit, const Shader::Uniform& Uniform) {
	bgfx::setTexture(Unit, Uniform.Handle, texture);
}