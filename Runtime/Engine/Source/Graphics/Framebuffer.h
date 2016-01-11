#pragma once
#include "bgfx/bgfx.h"
#include "Graphics/Texture.h"
#include "Graphics/RenderData.h"
#include <vector>

namespace Blueshift {
	namespace Graphics {

		/*! \class Framebuffer
			\brief Represents a texture that can be rendered to when attached to a view. 

			Uses a Builder Pattern to construct a framebuffer from an arbitary number of textures. Beware, if more than
			one texture is added, using the framebuffer requires multiple render target support.

			Example creation:

				Framebuffer fb;
				fb	.AddTexture(TextureFormat::RGBA8)		//Diffuse Color
					.AddTexture(TextureFormat::Single24)	//Depth
					.Complete();

		*/
		class Framebuffer {
		private:
			bgfx::FrameBufferHandle handle = BGFX_INVALID_HANDLE;
			std::vector<Texture*> textures;
		public:
			/*! \brief Use AddTexture() and Complete() methods to construct a Framebuffer.
			*/
			 Framebuffer();
			~Framebuffer();

			/*! \brief Adds a Texture whose size is decided by the current backbuffer using a ratio described by the enum BackbufferRatio.

				\sa BackbufferRatio, Texture
			*/
			Framebuffer& AddTexture(TextureFormat Format, BackbufferRatio Ratio = BackbufferRatio::Full, uint8_t NumMips = 1);
			/*! \brief Adds a texture of a specific size.
				
				\sa Texture
			*/
			Framebuffer& AddTexture(TextureFormat Format, uint16_t Width, uint16_t Height, uint8_t NumMips = 1);
			/*! \brief Finalizes construction and creates the framebuffer. 

				\throws RuntimeError If no textures are added.
				\sa Texture
			*/
			Framebuffer& Complete();

			/*!	\brief Retrieves a Texture in the Framebuffer.

				\throws RuntimeError If index is out of bounds
				\sa Texture
			*/
			Texture* GetTexture(size_t index);

			void Use(uint8_t ViewID);
		};

	}
}