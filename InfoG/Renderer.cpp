#include "Renderer.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

Renderer::Renderer(Camera& camera) : camera(&camera)
{
	glGenFramebuffers(1, &framebuffer);
	glGenTextures(1, &texture);
	glGenRenderbuffers(1, &depthrenderbuffer);
	imagesToOutput = 0;

	cubemapShader = Shader("SkyboxVS.shader", "SkyboxFS.shader");

	std::vector<std::string> faces
	{
		"right.jpg",
		"left.jpg",
		"up.jpg",
		"down.jpg",
		"front.jpg",
		"back.jpg"
	};

	for (unsigned int i = 0; i < faces.size(); i++)
	{
		Image img = Image(faces.at(i));
		skyboxImages.push_back(img);
	}

	glGenTextures(1, &textureIDSkybox);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureIDSkybox);

	for (unsigned int i = 0; i < skyboxImages.size(); i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, skyboxImages.at(i).getWidth(), skyboxImages.at(i).getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, skyboxImages.at(i).getPixels());
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	float skyboxV[] = {
		-10000.0f,  10000.0f, -10000.0f,
		-10000.0f, -10000.0f, -10000.0f,
		 10000.0f, -10000.0f, -10000.0f,
		 10000.0f, -10000.0f, -10000.0f,
		 10000.0f,  10000.0f, -10000.0f,
		-10000.0f,  10000.0f, -10000.0f,

		-10000.0f, -10000.0f,  10000.0f,
		-10000.0f, -10000.0f, -10000.0f,
		-10000.0f,  10000.0f, -10000.0f,
		-10000.0f,  10000.0f, -10000.0f,
		-10000.0f,  10000.0f,  10000.0f,
		-10000.0f, -10000.0f,  10000.0f,

		 10000.0f, -10000.0f, -10000.0f,
		 10000.0f, -10000.0f,  10000.0f,
		 10000.0f,  10000.0f,  10000.0f,
		 10000.0f,  10000.0f,  10000.0f,
		 10000.0f,  10000.0f, -10000.0f,
		 10000.0f, -10000.0f, -10000.0f,

		-10000.0f, -10000.0f,  10000.0f,
		-10000.0f,  10000.0f,  10000.0f,
		 10000.0f,  10000.0f,  10000.0f,
		 10000.0f,  10000.0f,  10000.0f,
		 10000.0f, -10000.0f,  10000.0f,
		-10000.0f, -10000.0f,  10000.0f,

		-10000.0f,  10000.0f, -10000.0f,
		 10000.0f,  10000.0f, -10000.0f,
		 10000.0f,  10000.0f,  10000.0f,
		 10000.0f,  10000.0f,  10000.0f,
		-10000.0f,  10000.0f,  10000.0f,
		-10000.0f,  10000.0f, -10000.0f,

		-10000.0f, -10000.0f, -10000.0f,
		-10000.0f, -10000.0f,  10000.0f,
		 10000.0f, -10000.0f, -10000.0f,
		 10000.0f, -10000.0f, -10000.0f,
		-10000.0f, -10000.0f,  10000.0f,
		 10000.0f, -10000.0f,  10000.0f
	};

	unsigned int vbo;
	glGenVertexArrays(1, &vaoSkybox);
	glBindVertexArray(vaoSkybox);
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 108 * sizeof(float), &skyboxV[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glDeleteBuffers(1, &vbo);
}

void Renderer::drawSkybox() {

	glDepthMask(GL_FALSE);

	cubemapShader.use();

	unsigned int id = glGetUniformLocation(cubemapShader.getProgramID(), "VP");
	glUniformMatrix4fv(id, 1, GL_FALSE, &camera->calculateVP()[0][0]);

	glBindVertexArray(vaoSkybox);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureIDSkybox);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0); // Unbind the vertex array

	glDepthMask(GL_TRUE);
}

unsigned int Renderer::render(unsigned int width, unsigned int height, glm::vec4 clearColor)
{
	// What if not using phong ?

	if (camera->getProjectionType() == ProjectionType::perspective) ((PerspectiveCamera*)camera)->setAspectRatio((float)width/height);

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);

	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);

	glViewport(0, 0, width, height);

	drawSkybox();

	for (auto & [ID, rObject] : RObjectManager::getRObjects())
	{

		rObject.useShader();

		glm::mat4 MV = camera->calculateMV(rObject.getModelMatrix());
		glm::mat4 normalMatrix = glm::transpose(glm::inverse(MV));
		glUniformMatrix4fv(rObject.getNormalMatrixID(), 1, GL_FALSE, &normalMatrix[0][0]);
		glUniformMatrix4fv(rObject.getMVPMatrixID(), 1, GL_FALSE, &camera->calculateMVP()[0][0]);
		glUniformMatrix4fv(rObject.getMVMatrixID(), 1, GL_FALSE, &MV[0][0]);

		glUniform3fv(rObject.getUniformLocation("cameraPosition"), 1, &camera->getPosition()[0]); // Vec 3 ?

		int dLightCounter = 0;
		int pLightCounter = 0;
		int sLightCounter = 0;

		for (auto& light : RObjectManager::getLights()) {

			DirectionalLight* drl;
			PointLight* pl;
			SpotLight* sl;

			switch (light.second->lightType) {
				case LightType::directional :

					drl = (DirectionalLight*)(light.second);

					glUniform3fv(rObject.getUniformLocation("directionalLights[" + std::to_string(dLightCounter) + "].direction"), 1, &drl->direction[0]); // Vec 3 ?
					glUniform3fv(rObject.getUniformLocation("directionalLights[" + std::to_string(dLightCounter) + "].ambient"), 1, &drl->ambient[0]); // Vec 3 ?
					glUniform3fv(rObject.getUniformLocation("directionalLights[" + std::to_string(dLightCounter) + "].diffuse"), 1, &drl->diffuse[0]); // Vec 3 ?
					glUniform3fv(rObject.getUniformLocation("directionalLights[" + std::to_string(dLightCounter) + "].specular"), 1, &drl->specular[0]); // Vec 3 ?

					dLightCounter++;

					break;
				case LightType::point :

					pl = (PointLight*)(light.second);

					glUniform3fv(rObject.getUniformLocation("pointLights[" + std::to_string(pLightCounter) + "].position"), 1, &pl->position[0]); // Vec 3 ?

					glUniform3fv(rObject.getUniformLocation("pointLights[" + std::to_string(pLightCounter) + "].ambient"), 1, &pl->ambient[0]); // Vec 3 ?
					glUniform3fv(rObject.getUniformLocation("pointLights[" + std::to_string(pLightCounter) + "].diffuse"), 1, &pl->diffuse[0]); // Vec 3 ?
					glUniform3fv(rObject.getUniformLocation("pointLights[" + std::to_string(pLightCounter) + "].specular"), 1, &pl->specular[0]); // Vec 3 ?

					glUniform1f(rObject.getUniformLocation("pointLights[" + std::to_string(pLightCounter) + "].constant"), pl->constant);
					glUniform1f(rObject.getUniformLocation("pointLights[" + std::to_string(pLightCounter) + "].linear"), pl->linear);
					glUniform1f(rObject.getUniformLocation("pointLights[" + std::to_string(pLightCounter) + "].quadratic"), pl->quadratic);

					pLightCounter++;

					break;
				case LightType::spot :

					sl = (SpotLight*)(light.second);

					glUniform3fv(rObject.getUniformLocation("spotLights[" + std::to_string(sLightCounter) + "].position"), 1, &sl->position[0]); // Vec 3 ?
					glUniform3fv(rObject.getUniformLocation("spotLights[" + std::to_string(sLightCounter) + "].direction"), 1, &sl->direction[0]); // Vec 3 ?

					glUniform3fv(rObject.getUniformLocation("spotLights[" + std::to_string(sLightCounter) + "].ambient"), 1, &sl->ambient[0]); // Vec 3 ?
					glUniform3fv(rObject.getUniformLocation("spotLights[" + std::to_string(sLightCounter) + "].diffuse"), 1, &sl->diffuse[0]); // Vec 3 ?
					glUniform3fv(rObject.getUniformLocation("spotLights[" + std::to_string(sLightCounter) + "].specular"), 1, &sl->specular[0]); // Vec 3 ?

					glUniform1f(rObject.getUniformLocation("spotLights[" + std::to_string(sLightCounter) + "].constant"), sl->constant);
					glUniform1f(rObject.getUniformLocation("spotLights[" + std::to_string(sLightCounter) + "].linear"), sl->linear);
					glUniform1f(rObject.getUniformLocation("spotLights[" + std::to_string(sLightCounter) + "].quadratic"), sl->quadratic);
					glUniform1f(rObject.getUniformLocation("spotLights[" + std::to_string(sLightCounter) + "].cutOff"), sl->cutOff);
					glUniform1f(rObject.getUniformLocation("spotLights[" + std::to_string(sLightCounter) + "].outerCutOff"), sl->outerCutOff);

					sLightCounter++;

					break;
			}
		}

		glUniform1i(rObject.getUniformLocation("directionalLightCount"), dLightCounter);
		glUniform1i(rObject.getUniformLocation("pointLightCount"), pLightCounter);
		glUniform1i(rObject.getUniformLocation("spotLightCount"), sLightCounter);

		glUniform3fv(rObject.getUniformLocation("material.ambient"), 1, &rObject.getMaterial().ambient[0]);
		glUniform3fv(rObject.getUniformLocation("material.diffuse"), 1, &rObject.getMaterial().diffuse[0]);
		glUniform3fv(rObject.getUniformLocation("material.specular"), 1, &rObject.getMaterial().specular[0]);
		glUniform1f(rObject.getUniformLocation("material.shininess"), rObject.getMaterial().shininess);

		//glUniform1ui(rObject.getUniformLocation("material.diffuseMap"), rObject.getMaterial().diffuseMap.getTextureID());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, rObject.getMaterial().diffuseMap.getTextureID());

		rObject.render();
	}

	if (imagesToOutput > 0) {
		cv::Mat image = cv::Mat(ImGui::GetWindowHeight(), ImGui::GetWindowWidth(), CV_8UC3);

		glPixelStorei(GL_PACK_ALIGNMENT, (image.step & 3) ? 1 : 4);
		glPixelStorei(GL_PACK_ROW_LENGTH, image.step / image.elemSize());

		glReadPixels(0, 0, image.cols, image.rows, GL_BGR, GL_UNSIGNED_BYTE, image.data);

		cv::Mat imageFlipper = cv::Mat::zeros(cv::Size(ImGui::GetWindowHeight(), ImGui::GetWindowWidth()), CV_8UC1);

		cv::flip(image, imageFlipper, 0);

		std::string time = std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());

		cv::imwrite("ImagesOutput/" + time + ".jpg", imageFlipper);

		imagesToOutput--;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return texture;
}
