#include "lucuma/math.h"
#include "lucuma/renderer.h"
#include "lucuma/input.h"
#include "lucuma/utils/page_allocator.h"
#include "lucuma/debug.h"
#include "lucuma/file.h"
#include "lucuma/utils/macros.h"
#include "lucuma/utils/loaders/obj_loader.h"
#include "lucuma/renderer/mesh.h"
#include "lucuma/utils/imgui/imgui_init.h"
#include "lucuma/utils/array.h"
#include <string.h>

using namespace lu;

void demo1()
{
	RendererDevice device;
	RendererContext context;
	Window window;
	RenderTarget mainRenderTarget;
	Texture2D dsTex;
	DepthStencilState dss;
	DepthStencilTarget dst;
	CommandList cmds;
	PageAllocator pageAllocator;
	VertexShader vs;
	PixelShader ps;
	Buffer cb;
	Buffer mcb;
	RasterizerState rs;
	Mat4 model;
	Mat4 view;
	Mat4 projection;
	Scissor sc;
	Viewport vp;
	Mesh meshObject;
	DearImGuiRenderingData imguiData;

	struct CBuffer
	{
		Mat4 MVP;
		Mat4 invModel;
		Mat4 model;
		Mat4 view;
		Mat4 projection;
		Vec4 cameraPosition;

	} cbuffer;

	struct M
	{
		Vec4 roughnessMetallic;
		Vec4 albedo;
		Vec4 lightPos;
		Vec4 lightColorRadius;
	}matbuffer;


#if 0
	LU_ASSERT(renderer::CreateWindowAndRendererDeviceWaitForRenderDoc("Lucuma", 640, 480, &window, &device, &context, &mainRenderTarget));
#else
	LU_ASSERT(renderer::CreateWindowAndInitializeGraphicsAPI("Lucuma", 640, 480, &window, &device, &context, &mainRenderTarget));
#endif

	struct Vertex
	{
		Vec3 position;
		Vec2 texCoord;
		Vec3 normal;
	};

	{
		SmallArray<int32_t, 100> a;
		Array<int32_t> b(&pageAllocator);
		b.initialize(a.getCapacity());

		b.push(90);
		b.push(91);
		b.push(92);
		b.push(93);

		a.push(10);
		a.push(20);
		a.push(30);
		a.push(40);

		for (uint32_t i = 0; i < a.getSize(); ++i)
		{
			int32_t d = a[i];
			debug::Log("value a: %d\n", d);
		}

		b.setArray(3, a);

		for (uint32_t i = 0; i < b.getSize(); ++i)
		{
			int32_t d = b[i];
			debug::Log("value b: %d\n", d);
		}

	}

	imgui::InitializeDearImGui(&pageAllocator, device, &mainRenderTarget, imguiData);

	model.setIdentity();
	model.scale(Vec3(0.5f));
	model.translate(Vec3(0.0f, 0.0f, -3.0f));
	model.rotateY(Rad(180.0f));
	view.setIdentity();
	projection.perspective(Rad(60.0f), (float32_t)window.width / (float32_t)window.height, 0.01f, 100.0f);
	//projection.orthographic(-2.0f, 2.0f, -2.0f, 2.0f, -100.0f, 100.0f);

	// Shaders
	{
		VertexLayout vl;
		vl.addAttribute(ResourceFormat::FORMAT_R32G32B32_FLOAT, LU_OFFSETOF(Vertex, position));
		vl.addAttribute(ResourceFormat::FORMAT_R32G32B32_FLOAT, LU_OFFSETOF(Vertex, normal));
		vl.addAttribute(ResourceFormat::FORMAT_R32G32_FLOAT, LU_OFFSETOF(Vertex, texCoord));

		LU_ASSERT(resources::CreateVertexShaderFromSourceFile(&pageAllocator, device, "code/lucuma/shaders/hlsl/default.hlsl", "DefaultVS", vl, &vs));
		LU_ASSERT(resources::CreatePixelShaderFromSourceFile(&pageAllocator, device, "code/lucuma/shaders/hlsl/default.hlsl", "DefaultPS", &ps));
	}

	LU_ASSERT(loader::OpenOBJFileAndCreateMesh(device, &pageAllocator, "data/bunny.obj", &meshObject));
	LU_ASSERT(resources::CreateBuffer(device, sizeof(cbuffer), BufferUsage::USAGE_DYNAMIC, BufferBind::BIND_CONSTANT_BUFFER, CPUAccess::CPU_ACCESS_WRITE, ResourceType::RESOURCE_NONE, 0, &cbuffer, &cb));
	LU_ASSERT(resources::CreateBuffer(device, sizeof(matbuffer), BufferUsage::USAGE_DYNAMIC, BufferBind::BIND_CONSTANT_BUFFER, CPUAccess::CPU_ACCESS_WRITE, ResourceType::RESOURCE_NONE, 0, &matbuffer, &mcb));
	LU_ASSERT(resources::CreateRasterizerState(device, lu::FillMode::SOLID, lu::CullMode::NONE, false, false, false, true, &rs));

	{
		LU_ASSERT(resources::CreateDepthStencilState(device, DepthState::GetDefault(), StencilState::GetDefault(), DepthStencilOp::GetDefault(), DepthStencilOp::GetDefault(), &dss));
		LU_ASSERT(resources::CreateTexture2D(device, ResourceFormat::FORMAT_D24_UNORM_S8_UINT, NULL, window.width, window.height, TextureBinding::DEPTH_STENCIL_BINDING, &dsTex));
		LU_ASSERT(resources::CreateDepthStencilTarget(device, dsTex, &dst));
	}

	LU_ASSERT(commands::CreateCommandList(&pageAllocator, 100, &cmds));

	float32_t clearColor[] = { 0,0,0,1 };
	uint32_t vbOffset = 0, vbStrides = sizeof(Vertex);

	sc.x = 0;
	sc.y = 0;
	sc.width = window.width;
	sc.height = window.height;

	vp.x = 0;
	vp.y = 0;
	vp.width = (float32_t)window.width;
	vp.height = (float32_t)window.height;
	vp.nearDepth = 0.0f;
	vp.farDepth = 1.0f;

	bool runProgram = true;

	Vec2 lastMouse;
	Vec3 eulerAngles;
	Vec3 lastRotation;
	Transform transform;
	Transform cameraTransform;

	//transform.scale *= 0.01f;
	cameraTransform.position.z -= 3.0f;

	float32_t time = 0.0f;

	matbuffer.roughnessMetallic.x = 0.5f;
	matbuffer.roughnessMetallic.y = 0.0f;
	matbuffer.albedo = 1.0f;

	matbuffer.lightPos = Vec4(0.0f, 1.0f, 0.5f, 0.0f);
	matbuffer.lightColorRadius = Vec4(2.5f, 2.5f, 10.0f, 1.0f);

	while (runProgram)
	{
		input::PollEvents(window);

		imgui::UpdateDearImGui(window, imguiData);
		// ImGui Rendering
		{
			ImGui::ColorEdit3("Albedo", (float32_t*)&matbuffer.albedo);
			ImGui::SliderFloat("Roughness", &matbuffer.roughnessMetallic.x, 0.1f, 1.0f);
			ImGui::SliderFloat("Metallic", &matbuffer.roughnessMetallic.y, 0.0f, 1.0f);
			ImGui::DragFloat3("Point Light Position", (float32_t*)&matbuffer.lightPos, 0.1f, -10.0f, 10.0f);
			//ImGui::ColorEdit3("Point Light Color", (float32_t*)&matbuffer.lightColorRadius, ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_Float);
			//ImGui::SliderFloat("Point Light Radius", &matbuffer.lightColorRadius.w, 0.0f, 10.0f);
		}

		if (renderer::ShouldResize(window))
		{
			sc.x = 0;
			sc.y = 0;
			sc.width = window.width;
			sc.height = window.height;

			vp.x = 0;
			vp.y = 0;
			vp.width = (float32_t)window.width;
			vp.height = (float32_t)window.height;
			vp.nearDepth = 0.0f;
			vp.farDepth = 1.0f;

			resources::DestroyDepthStencilState(device, &dss);
			resources::DestroyTexture2D(device, &dsTex);
			resources::DestroyDepthStencilTarget(device, &dst);

			renderer::ResizeWindow(device, window.width, window.height, &window, &mainRenderTarget);

			projection.perspective(Rad(60.0f), (float32_t)window.width / (float32_t)window.height, 0.001f, 100.0f);

			LU_ASSERT(resources::CreateDepthStencilState(device, DepthState::GetDefault(), StencilState::GetDefault(), DepthStencilOp::GetDefault(), DepthStencilOp::GetDefault(), &dss));
			LU_ASSERT(resources::CreateTexture2D(device, ResourceFormat::FORMAT_D24_UNORM_S8_UINT, NULL, window.width, window.height, TextureBinding::DEPTH_STENCIL_BINDING, &dsTex));
			LU_ASSERT(resources::CreateDepthStencilTarget(device, dsTex, &dst));
		}

		if (input::ShouldQuit(window) || input::IsKeyDown(window, KEY_ESC))
		{
			runProgram = false;
		}


		{
			static Vec3 rotation;
			Vec2 m = (input::GetMousePos(window) / Vec2((float32_t)window.width, (float32_t)window.height) * 2.0 - 1.0);
			if (input::IsMouseButtonClick(window, MouseButton::RIGHT))
			{
				lastMouse = m;
				lastRotation = rotation;
			}
			if (input::IsMouseButtonDown(window, MouseButton::RIGHT))
			{
				m = m - lastMouse;
				rotation.x = lastRotation.x + m.y;
				rotation.y = lastRotation.y + m.x;
			}

			cameraTransform.rotation.fromEulerAngles(rotation);

			Mat4 r = cameraTransform.rotation.toMat4();
			static Vec3 cameraVelocity;
			float32_t speed = .005f;

			cameraVelocity *= 0.9f;

			if (input::IsKeyDown(window, KeyCode::KEY_W))
			{
				cameraVelocity.z += speed;
			}
			else if (input::IsKeyDown(window, KeyCode::KEY_S))
			{
				cameraVelocity.z -= speed;
			}
			else if (input::IsKeyDown(window, KeyCode::KEY_A))
			{
				cameraVelocity.x += speed;
			}
			else if (input::IsKeyDown(window, KeyCode::KEY_D))
			{
				cameraVelocity.x -= speed;
			}

			if (cameraVelocity.lengthSqr() != 0.0f)
			{
				cameraTransform.position += (r.invert() * cameraVelocity.toVec4()).toVec3();
			}

			model.setIdentity();

			model.translate(transform.position);
			model.scale(transform.scale);
			model *= transform.rotation.toMat4();

			view.setIdentity();

			view *= cameraTransform.rotation.toMat4();
			view.translate(cameraTransform.position);
			view.scale(cameraTransform.scale);

			// Save matrices
			cbuffer.MVP = projection * view * model;
			cbuffer.model = model;
			cbuffer.view = view;
			cbuffer.projection = projection;
			cbuffer.cameraPosition = -cameraTransform.position.toVec4();
			cbuffer.invModel = model;
			cbuffer.invModel.invert();
			cbuffer.invModel.transpose();

			void* pCB = resources::MapBuffer(context, cb, MapType::MAP_WRITE_DISCARD);
			memcpy(pCB, &cbuffer, sizeof(cbuffer));
			resources::UnmapBuffer(context, cb);
		}

		// Material
		{
			void* pCB = resources::MapBuffer(context, mcb, MapType::MAP_WRITE_DISCARD);
			memcpy(pCB, &matbuffer, sizeof(matbuffer));
			resources::UnmapBuffer(context, mcb);
		}

		cmds.reset();
		commands::SetRasterizerState(cmds, rs);
		commands::SetViewports(cmds, 1, &vp);
		commands::SetScissors(cmds, 1, &sc);
		commands::SetRenderTargets(cmds, 1, &mainRenderTarget, &dst);
		commands::SetDepthStencilState(cmds, dss, 0);
		commands::ClearRenderTarget(cmds, mainRenderTarget, clearColor);
		commands::ClearDepthStencilTarget(cmds, dst, CLEAR_DEPTH | CLEAR_STENCIL, 1.0f, 0);
		commands::SetVertexShader(cmds, &vs);
		commands::SetPixelShader(cmds, &ps);
		commands::SetVertexShaderConstantBuffers(cmds, 1, &cb);
		commands::SetPixelShaderConstantBuffers(cmds, 1, &mcb);
		commands::SetPrimitiveTopology(cmds, lu::PrimitiveTopology::TRIANGLE_LIST);

		mesh::DrawMesh(cmds, &meshObject);

		imgui::DrawDearImGui(device, context, cmds, imguiData);

		commands::ExecuteCommandList(context, cmds);
		renderer::Present(window, 1);

		time += 0.05f;
	}

	imgui::FinalizeDearImGui(device, imguiData);
	resources::DestroyTexture2D(device, &dsTex);
	resources::DestroyDepthStencilState(device, &dss);
	resources::DestroyDepthStencilTarget(device, &dst);
	commands::DestroyCommandList(&pageAllocator, &cmds);
	resources::DestroyVertexShader(device, &vs);
	resources::DestroyPixelShader(device, &ps);
	resources::DestroyBuffer(device, &cb);
	resources::DestroyMesh(device, &meshObject);
	resources::DestroyRasterizerState(device, &rs);
	resources::DestroyRenderTarget(device, &mainRenderTarget);
	renderer::DestroyRendererContext(device, &context);
	renderer::DestroyRendererDevice(&device);
	renderer::CloseWindow(&window);
}
