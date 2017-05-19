#include "Editor.h"



void Editor::update(float dt)
{
	ImGui::Begin("Window");
	ImGui::Button("XD");
	ImGui::End();
}

void Editor::render(sf::RenderWindow * win)
{
}

Editor::Editor(Tilemap* t)
{
	this->map = t;
}


Editor::~Editor()
{
}
