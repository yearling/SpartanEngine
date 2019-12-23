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

#pragma once

//= INCLUDES =================
#include <vector>
#include "RHI_Texture.h"
#include "RHI_Viewport.h"
#include "RHI_Definition.h"
#include "../Math/Vector4.h"
#include "../Math/Rectangle.h"
#include "RHI_PipelineCache.h"
//============================

namespace Spartan
{
	class Profiler;

    enum RHI_Cmd_List_State
    {
        RHI_Cmd_List_Idle,
        RHI_Cmd_List_Idle_Sync_Cpu_To_Gpu,
        RHI_Cmd_List_Recording,
        RHI_Cmd_List_Ended
    };

	class SPARTAN_CLASS RHI_CommandList
	{
	public:
		RHI_CommandList(uint32_t index, RHI_SwapChain* swap_chain, Context* context);
		~RHI_CommandList();

        // Passes
        bool Begin(const std::string& pass_name);                                           // Marker
		bool Begin(RHI_PipelineState& pipeline_state);                                      // Pass
        inline bool Begin(const std::string& pass_name, RHI_PipelineState& pipeline_state)  // Pass & Marker
        {
            if (Begin(pass_name))
                if (Begin(pipeline_state))
                    return true;

            return false;
        }
        bool End();

		// Draw
		void Draw(uint32_t vertex_count);
		void DrawIndexed(uint32_t index_count, uint32_t index_offset = 0, uint32_t vertex_offset = 0);

		// Viewport
		void SetViewport(const RHI_Viewport& viewport);

        // Scissor
		void SetScissorRectangle(const Math::Rectangle& scissor_rectangle);

        // Primitive topology
		void SetPrimitiveTopology(RHI_PrimitiveTopology_Mode primitive_topology);

		// Input layout
		void SetInputLayout(const RHI_InputLayout* input_layout);
        inline void SetInputLayout(const std::shared_ptr<RHI_InputLayout>& input_layout) { SetInputLayout(input_layout.get()); }

		// Depth-stencil state
		void SetDepthStencilState(const RHI_DepthStencilState* depth_stencil_state);
        inline void SetDepthStencilState(const std::shared_ptr<RHI_DepthStencilState>& depth_stencil_state) { SetDepthStencilState(depth_stencil_state.get()); }

		// Rasterizer state
		void SetRasterizerState(const RHI_RasterizerState* rasterizer_state);
        inline void SetRasterizerState(const std::shared_ptr<RHI_RasterizerState>& rasterizer_state) { SetRasterizerState(rasterizer_state.get()); }

		// Blend state
		void SetBlendState(const RHI_BlendState* blend_state);
        inline void SetBlendState(const std::shared_ptr<RHI_BlendState>& blend_state) { SetBlendState(blend_state.get()); }

		// Vertex buffer
		void SetBufferVertex(const RHI_VertexBuffer* buffer);
        inline void SetBufferVertex(const std::shared_ptr<RHI_VertexBuffer>& buffer) { SetBufferVertex(buffer.get()); }

		// Index buffer
		void SetBufferIndex(const RHI_IndexBuffer* buffer);
        inline void SetBufferIndex(const std::shared_ptr<RHI_IndexBuffer>& buffer) { SetBufferIndex(buffer.get()); }

		// Vertex shader
		void SetShaderVertex(const RHI_Shader* shader);
        inline void SetShaderVertex(const std::shared_ptr<RHI_Shader>& shader) { SetShaderVertex(shader.get()); }

		// Pixel shader
		void SetShaderPixel(const RHI_Shader* shader);
        inline void SetShaderPixel(const std::shared_ptr<RHI_Shader>& shader) { SetShaderPixel(shader.get()); }

        // Compute shader
        void SetShaderCompute(const RHI_Shader* shader);
        inline void SetShaderCompute(const std::shared_ptr<RHI_Shader>& shader) { SetShaderCompute(shader.get()); }

		// Constant buffer
        void SetConstantBuffer(const uint32_t slot, uint8_t scope, RHI_ConstantBuffer* constant_buffer);
        inline void SetConstantBuffer(const uint32_t slot, uint8_t scope, const std::shared_ptr<RHI_ConstantBuffer>& constant_buffer) { SetConstantBuffer(slot, scope, constant_buffer.get()); }

		// Sampler
        void SetSampler(const uint32_t slot, RHI_Sampler* sampler);
        inline void SetSampler(const uint32_t slot, const std::shared_ptr<RHI_Sampler>& sampler) { SetSampler(slot, sampler.get()); }

		// Texture
        void SetTexture(const uint32_t slot, RHI_Texture* texture);
        inline void SetTexture(const uint32_t slot, const std::shared_ptr<RHI_Texture>& texture) { SetTexture(slot, texture.get()); }
        
		// Render targets
		void SetRenderTargets(const void* render_targets, uint32_t render_target_count, void* depth_stencil = nullptr);
        inline void SetRenderTarget(void* render_target, void* depth_stencil = nullptr)                                { SetRenderTargets(render_target, 1, depth_stencil); }
        inline void SetRenderTarget(const std::shared_ptr<RHI_Texture>& render_target, void* depth_stencil = nullptr)  { SetRenderTargets(render_target ? render_target->GetResource_RenderTarget() : nullptr, 1, depth_stencil); }
		void ClearRenderTarget(void* render_target, const Math::Vector4& color);
		void ClearDepthStencil(void* depth_stencil, uint32_t flags, float depth, uint8_t stencil = 0);

		bool Submit();
        void Flush();

	private:
        std::vector<bool> m_marker_begun;
        RHI_Cmd_List_State m_cmd_state              = RHI_Cmd_List_Idle;
        RHI_PipelineState* m_pipeline_state         = nullptr;
		RHI_Pipeline* m_pipeline	                = nullptr; 
        RHI_SwapChain* m_swap_chain                 = nullptr;
        Renderer* m_renderer                        = nullptr;
        RHI_PipelineCache* m_rhi_pipeline_cache     = nullptr;
        RHI_Device* m_rhi_device                    = nullptr;
        Profiler* m_profiler                        = nullptr;
        void* m_cmd_buffer                          = nullptr;
        void* m_cmd_list_consumed_fence             = nullptr;
	};
}
