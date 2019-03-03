/*
Copyright(c) 2016-2019 Panos Karabelas

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

//= INCLUDES ========================
#include "RHI_Shader.h"
#include "RHI_InputLayout.h"
#include "RHI_ConstantBuffer.h"
#include "../Logging/Log.h"
#include "../Core/Context.h"
#include "../Threading/Threading.h"
#include "../FileSystem/FileSystem.h"
//===================================

//= NAMESPACES =====
using namespace std;
//==================

namespace Directus
{
	RHI_Shader::RHI_Shader(const shared_ptr<RHI_Device>& rhi_device)
	{
		m_rhi_device	= rhi_device;
		m_input_layout	= make_shared<RHI_InputLayout>(rhi_device);
	}

	void RHI_Shader::Compile(const Shader_Type type, const string& shader, unsigned long input_layout_type)
	{
		// Deduce name or file path
		if (FileSystem::IsDirectory(shader))
		{
			m_name.clear();
			m_file_path = shader;
		}
		else
		{
			m_name = FileSystem::GetFileNameFromFilePath(shader);
			m_file_path.clear();
		}
		m_inputLayoutType = input_layout_type;

		// Compile
		if (type == Shader_Vertex)
		{
			m_compilation_state = Shader_Compiling;
			m_vertex_shader		= _Compile(type, shader);		
			m_compilation_state = m_vertex_shader ? Shader_Built : Shader_Failed;
		}
		else if (type == Shader_Pixel)
		{
			m_compilation_state = Shader_Compiling;
			m_pixel_shader		= _Compile(type, shader);
			m_compilation_state = m_pixel_shader ? Shader_Built : Shader_Failed;
		}
		else if (type == Shader_VertexPixel)
		{
			m_compilation_state = Shader_Compiling;
			m_vertex_shader		= _Compile(Shader_Vertex, shader);
			m_pixel_shader		= _Compile(Shader_Pixel, shader);
			m_compilation_state = (m_vertex_shader && m_pixel_shader) ? Shader_Built : Shader_Failed;
		}

		// Log result
		if (m_compilation_state == Shader_Built)
		{
			LOGF_INFO("Successfully compiled %s", shader.c_str());
		}
		else if (m_compilation_state == Shader_Failed)
		{
			LOGF_ERROR("Failed to compile %s", shader.c_str());
		}
	}

	void RHI_Shader::CompileAsync(Context* context, const Shader_Type type, const string& shader, unsigned long input_layout_type)
	{
		context->GetSubsystem<Threading>()->AddTask([this, type, shader, input_layout_type]()
		{
			Compile(type, shader, input_layout_type);
		});
	}

	bool RHI_Shader::CreateInputLayout(void* vertex_shader)
	{
		if (!vertex_shader)
		{
			LOG_ERROR_INVALID_PARAMETER();
			return false;
		}

		if (!m_input_layout->Create(vertex_shader, m_inputLayoutType))
		{
			LOGF_ERROR("Failed to create input layout for %s", FileSystem::GetFileNameFromFilePath(m_file_path).c_str());
			return false;
		}

		return true;
	}
}