#include "GUISystem.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui.h"

GUISystem::GUISystem(): m_Exposure(1.0f), m_GammaCorrection(2.2f)
{
}


GUISystem::~GUISystem()
{
}


void GUISystem::Setup()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(m_CurrentWindow, true);
	ImGui_ImplOpenGL3_Init("#version 400");
}


void GUISystem::Update()
{
	StartFrame();
	ChecGL_TEXTURE_WRAP_Sliders();

}

void GUISystem::Draw()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void GUISystem::ProcessEvent(Event*)
{
}

void GUISystem::StartFrame()
{
	//Start new frame for IMGUI
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	//Render IMGUI GUI
	ImGui::Begin("Editor");
	{
		ImGui::SliderFloat("Exposure", &m_Exposure, 0, 5);
		ImGui::SliderFloat("Gamma Correction", &m_GammaCorrection, 0, 5);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
	ImGui::End();
}

void GUISystem::ChecGL_TEXTURE_WRAP_Sliders()
{
	//Check post processing sliders
	if (m_LastSentExposure != m_Exposure || m_LastSentGammaCorrection != m_GammaCorrection)
	{
		UpdatePostProcessingParams *e = new UpdatePostProcessingParams();
		e->m_EventType = EventType::kUpdatePostProcessingParams;
		e->m_GammaCorrection = m_GammaCorrection;
		e->m_Exposure = m_Exposure;
		m_EventsController->AddEvent(e);
		
		m_LastSentExposure = m_Exposure;
		m_LastSentGammaCorrection = m_GammaCorrection;
	}
}
