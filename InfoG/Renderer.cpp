#include "Renderer.h"

Renderer::Renderer(Camera& camera) : camera(&camera)
{
}

void Renderer::render()
{
	for (auto & [key, value] : RObjectManager::getRObjects())
	{
		glUniformMatrix4fv(value.getMVPMatrixID(), 1, GL_FALSE, &camera->calculateMVP(value.getModelMatrix())[0][0]);

		value.render();
	}
}
