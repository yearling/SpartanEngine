/*
Copyright(c) 2016-2017 Panos Karabelas

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is furnished
to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

//= INCLUDES ====================
#include "../Resource/Resource.h"
#include <memory>
//===============================

namespace Directus
{
	class D3D11Texture;

	enum TextureType
	{
		TextureType_Unknown,
		TextureType_Albedo,
		TextureType_Roughness,
		TextureType_Metallic,
		TextureType_Normal,
		TextureType_Height,
		TextureType_Occlusion,
		TextureType_Emission,
		TextureType_Mask,
		TextureType_CubeMap,
	};

	enum TextureFormat
	{
		RGBA_32_FLOAT,
		RGBA_16_FLOAT,
		RGBA_8_UNORM,
		R_8_UNORM
	};

	class DLL_API Texture : public Resource
	{
	public:
		Texture(Context* context);
		Texture(unsigned int width, unsigned int height)
		{
			m_width = width;
			m_height = height;
			m_isUsingMipmaps = false;
			m_isDirty = true;
		}

		Texture(bool useMimaps)
		{
			m_isUsingMipmaps = useMimaps;
			m_isDirty = true;
		}
		~Texture();

		//= RESOURCE INTERFACE =============================================
		bool SaveToFile(const std::string& filePath) override;
		bool LoadFromFile(const std::string& filePath) override;
		unsigned int GetMemoryUsageKB() override { return m_memoryUsageKB; }
		//==================================================================

		//= NATIVE TEXTURE HANDLING (BINARY) =========
		bool Serialize(const std::string& filePath);
		bool Deserialize(const std::string& filePath);	
		void Clear();
		//============================================

		//= PROPERTIES ======================================================
		unsigned int GetWidth() { return m_width; }
		void SetWidth(unsigned int width);

		unsigned int GetHeight() { return m_height; }
		void SetHeight(unsigned int height);

		TextureType GetTextureType() { return m_type; }
		void SetTextureType(TextureType type);

		bool GetGrayscale() { return m_isGrayscale; }
		void SetGrayscale(bool grayscale);

		bool GetTransparency() { return m_isTransparent; }
		void SetTransparency(bool transparency);

		unsigned int GetBPP() { return m_bpp; }
		void SetBPP(unsigned int bpp);

		unsigned int GetChannels() { return m_channels; }
		void SetChannels(unsigned int channels);

		std::vector<std::vector<unsigned char>>& GetRGBA() { return m_rgba; }
		void SetRGBA(const std::vector<std::vector<unsigned char>>& rgba);

		void EnableMimaps(bool enable);
		bool IsUsingMimmaps() { return m_isUsingMipmaps; }
		//===================================================================
		
		//= SHADER RESOURCE ============================
		void** GetShaderResource();
		// Creates a shader resource from memory
		bool CreateShaderResource(
			unsigned int width, 
			unsigned int height, 
			unsigned int channels, 
			std::vector<unsigned char> rgba, 
			TextureFormat format
		);
		// Creates a shader resource from memory
		bool CreateShaderResource();
		//==============================================

	private:		
		bool LoadFromForeignFormat(const std::string& filePath);
		TextureType TextureTypeFromString(const std::string& type);
		int ToAPIFormat(TextureFormat format);
		unsigned int ComputeMemoryUsageKB();

		bool m_isDirty;
		unsigned int m_memoryUsageKB = 0;
		std::shared_ptr<D3D11Texture> m_textureAPI;
		TextureFormat m_format = RGBA_8_UNORM;

		//= DATA ======================================
		unsigned int m_bpp = 0;
		unsigned int m_width = 0;
		unsigned int m_height = 0;
		unsigned int m_channels = 0;
		bool m_isGrayscale = false;
		bool m_isTransparent = false;
		bool m_isUsingMipmaps = false;
		std::vector<std::vector<unsigned char>> m_rgba;
		TextureType m_type = TextureType_Unknown;
		//=============================================
	};
}