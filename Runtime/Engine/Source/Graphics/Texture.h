#pragma once
#include "bgfx/bgfx.h"
#include "Graphics/RenderData.h"
#include "Graphics/Shader.h"
#include "Storage/FileSystem.h"

namespace Blueshift {
	namespace Graphics {
		enum class BackbufferRatio {
			Full = bgfx::BackbufferRatio::Equal,
			Double = bgfx::BackbufferRatio::Double,
			Half = bgfx::BackbufferRatio::Half,
			Quarter = bgfx::BackbufferRatio::Quarter,
			Eighth = bgfx::BackbufferRatio::Eighth,
			Sixteenth = bgfx::BackbufferRatio::Sixteenth
		};

		enum class TextureFormat {
			DXT1 = bgfx::TextureFormat::BC1,
			DXT3 = bgfx::TextureFormat::BC2,
			DXT5 = bgfx::TextureFormat::BC3,

			Unknown = bgfx::TextureFormat::Unknown,

			Float = bgfx::TextureFormat::R32F,
			RGBA8 = bgfx::TextureFormat::RGBA8U,
			Vector = bgfx::TextureFormat::RGBA32F,

			Single24 = bgfx::TextureFormat::D24F
		};

		enum class TextureType {
			FromFile = 0,
			Texture2D,
			Texture3D,
			TextureCube,
			Framebuffer
		};

		class Texture : public Storage::IAsset {
		protected:
			bgfx::TextureHandle texture = BGFX_INVALID_HANDLE;

			TextureType type;
			TextureFormat format;
			uint8_t* data;
			size_t size;
			uint32_t flags;

			uint16_t width, height, depth;
			uint8_t num_mips;

			void from_file(Storage::File& file, bool keep_copy);
		public:
			Texture(
				TextureType Type,
				TextureFormat Format,
				uint16_t Width = 0, uint16_t Height = 0, uint16_t Depth = 0,
				uint8_t NumMips = 1,
				RenderData Data = nullptr
			);
			Texture(TextureFormat Format, BackbufferRatio Ratio, uint8_t NumMips);
			Texture(std::string path, bool keep_copy = false);
			Texture(Storage::File* file, bool keep_copy = false);
			virtual ~Texture();

			void AssignTo(uint8_t Unit, const Shader::Uniform& Uniform);

			inline bgfx::TextureHandle GetHandle() { return this->texture; }
		};

	}
}