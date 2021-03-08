#include "Renderer.h"

Renderer::Renderer(Camera& camera) : camera(&camera)
{
	glGenFramebuffers(1, &framebuffer);
	glGenTextures(1, &texture);
	glGenRenderbuffers(1, &depthrenderbuffer);
	imagesToOutput = 0;
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

	for (auto & [ID, rObject] : RObjectManager::getRObjects())
	{

		glm::mat4 MV = camera->calculateMV(rObject.getModelMatrix());
		glm::mat4 normalMatrix = glm::transpose(glm::inverse(MV));
		glUniformMatrix4fv(rObject.getNormalMatrixID(), 1, GL_FALSE, &normalMatrix[0][0]);
		glUniformMatrix4fv(rObject.getMVPMatrixID(), 1, GL_FALSE, &camera->calculateMVP()[0][0]);
		glUniformMatrix4fv(rObject.getMVMatrixID(), 1, GL_FALSE, &MV[0][0]);

		glUniform3fv(rObject.getUniformLocation("cameraPosition"), 1, &camera->getPosition()[0]); // Vec 3 ?
		
		glUniform1i(rObject.getUniformLocation("directionalLightCount"), 1);
		glUniform1i(rObject.getUniformLocation("pointLightCount"), 0);
		glUniform1i(rObject.getUniformLocation("spotLightCount"), 0);

		glm::vec3 direction{ 0.5,-1,-1 };
		glm::vec3 ambient{ .2,.2,.2 };
		glm::vec3 diffuse{ 1,.66,.66 };
		glm::vec3 specular{ .1,.1,.1 };

		for (int i = 0; i < 1; i++) {
			glUniform3fv(rObject.getUniformLocation("directionalLights[" + std::to_string(i) + "].direction"), 1, &direction[0]); // Vec 3 ?

			glUniform3fv(rObject.getUniformLocation("directionalLights[" + std::to_string(i) + "].ambient"), 1, &ambient[0]); // Vec 3 ?
			glUniform3fv(rObject.getUniformLocation("directionalLights[" + std::to_string(i) + "].diffuse"), 1, &diffuse[0]); // Vec 3 ?
			glUniform3fv(rObject.getUniformLocation("directionalLights[" + std::to_string(i) + "].specular"), 1, &specular[0]); // Vec 3 ?
		}

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
