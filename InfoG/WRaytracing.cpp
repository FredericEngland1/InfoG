#include "WRaytracing.h"

#include <thread>
#include <mutex>

std::uniform_real_distribution<double> uniformRD01{ 0.0, 1.0 };
std::random_device rd;
std::mt19937 rng { rd() };

WRaytracing::WRaytracing(Camera& camera, std::string name, unsigned int width, unsigned int height, unsigned int rayPP, unsigned int subsamplingSize) : camera(camera), name(name), width(width), height(height), rayPP(rayPP), subsamplingSize(subsamplingSize)
{
    textureID = 0;
    firstUpdate = true;

    createAndCompileShader();
}

void WRaytracing::createAndCompileShader() {
    std::ifstream vShaderStream("TriangleIntersect.shader");
    std::stringstream vBuffer;
    vBuffer << vShaderStream.rdbuf();
    vShaderStream.close();

    std::string shaderCode = vBuffer.str();

    unsigned int program = glCreateProgram();
    unsigned int cShader = glCreateShader(GL_COMPUTE_SHADER);

    const char* vShaderCC = &shaderCode[0];
    glShaderSource(cShader, 1, &vShaderCC, NULL);
    glCompileShader(cShader);

    int compileResult;
    glGetShaderiv(cShader, GL_COMPILE_STATUS, &compileResult);

    // If no error finish else log the error
    if (compileResult == GL_FALSE) {

        int logLenght;
        glGetShaderiv(cShader, GL_INFO_LOG_LENGTH, &logLenght);

        std::vector<char> log(logLenght);
        glGetShaderInfoLog(cShader, logLenght, &logLenght, &log[0]);

        std::cout << "Failed to compile the shader : " << std::endl;
        std::copy(log.begin(), log.end(), std::ostream_iterator<char>(std::cout));
        std::cout << std::endl;

        glDeleteShader(cShader);
    }

    // What happens if shader doesn't compile ? Ugly error cause we deleted the shader

    glAttachShader(program, cShader);

    glLinkProgram(program);

    int isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE)
    {
        int maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<char> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

        std::string errorLog(infoLog.begin(), infoLog.end());
        MainWConsole::mainConsole.addLog(errorLog, LogType::error);

        glDeleteProgram(program);
    }
    glValidateProgram(program);

    glDeleteShader(cShader);

    shaderProgram = program;
}

// Möller–Trumbore intersection algorithm

glm::vec4 WRaytracing::intersect(glm::vec3 rOrigin, glm::vec3 rVector, glm::vec3 triangleVector1, glm::vec3 triangleVector2, glm::vec3 triangleVector3)
{
	float det, invDet, u, v, t;

    glm::vec3 h, e1, e2;

    e1 = triangleVector2 - triangleVector1;
    e2 = triangleVector3 - triangleVector1;

	h = glm::cross(rVector, e2);
	det = glm::dot(e1, h);

	// Check if triangle is parallel

	if (det < EPSILON) return { 0,0,0,0 }; // If negatif then backfacing, if close to 0 then parallel

    invDet = 1.0f / det;
    glm::vec3 s = rOrigin - triangleVector1;
    u = glm::dot(s, h) * invDet;

    if (u < 0.0 || u > 1.0) return {0,0,0,0};

    glm::vec3 q = glm::cross(s, e1);
    v = invDet * glm::dot(rVector, q);

    if (v < 0.0 || (u + v) > 1.0) return { 0,0,0,0 };

    t = invDet * glm::dot(e2, q);
    //if (t <= EPSILON) return; // TODO useless ?

    glm::vec3 intersectionPoint = rOrigin + (rVector * t);

    return { intersectionPoint.x, intersectionPoint.y, intersectionPoint.z, t };
}

float WRaytracing::intersectSphere(glm::vec3 rOrigin, glm::vec3 rVector, Sphere sphere)
{
    glm::vec3 delta = sphere.position - rOrigin;

    float a = glm::dot(delta, delta);
    float b = glm::dot(delta, rVector);

    float c = exp2(sphere.rayon);

    float d = b * b - a + c;

    if (d < 0) return 0;

    d = sqrt(d);

    float t = b - d;

    // valider si la distance de la première intersection est dans le seuil de tolérance
    if (t > EPSILON) return t;

    t = b + d;

    if (t > EPSILON) return t;

    return 0;
}

bool WRaytracing::intersectRayBox(glm::vec3 rOrigin, glm::vec3 rVector, RObject rObject)
{
    glm::vec3 inverseVector = { 1 / rVector.x, 1 / rVector.y , 1 / rVector.z };

    float tMin, tMax, tyMin, tyMax, tzMin, tzMax;

    glm::vec3 t1, t2;

    t1 = (rObject.getMesh().vMax - rOrigin) * inverseVector;
    t2 = (rObject.getMesh().vMin - rOrigin) * inverseVector;

    if (t1.x < t2.x) {
        tMin = t1.x;
        tMax = t2.x;
    }
    else {
        tMin = t2.x;
        tMax = t1.x;
    }

    if (t1.y < t2.y) {
        tyMin = t1.y;
        tyMax = t2.y;
    }
    else {
        tyMin = t2.y;
        tyMax = t1.y;
    }

    if (t1.z < t2.z) {
        tzMin = t1.z;
        tzMax = t2.z;
    }
    else {
        tzMin = t2.z;
        tzMax = t1.z;
    }


    if (tMin > tyMax || tyMin > tMax) return false;

    if (tyMin > tMin) tMin = tyMin;
    if (tyMax < tMax) tMax = tyMax;

    if (tMin > tzMax || tzMin > tMax) return false;

    return true;
}

void WRaytracing::updateObjectBounds()
{
    for (auto& [id, rObject] : RObjectManager::getRObjects()) {

        glm::vec3 vMin, vMax;

        vMin = rObject.getMesh().getVertices().front();
        vMax = rObject.getMesh().getVertices().front();

        for (glm::vec3 &vertice : rObject.getMesh().getVertices()) {
            if (vMin.x > vertice.x) vMin.x = vertice.x;
            else if (vMax.x < vertice.x) vMax.x = vertice.x;

            if (vMin.y > vertice.y) vMin.y = vertice.y;
            else if (vMax.y < vertice.y) vMax.y = vertice.y;

            if (vMin.z > vertice.z) vMin.z = vertice.z;
            else if (vMax.z < vertice.z) vMax.z = vertice.z;
        }

        rObject.mesh.vMin = vMin;
        rObject.mesh.vMax = vMax;
    }
}

void WRaytracing::updateObjectMeshImage()
{
    glUseProgram(shaderProgram);

    firstUpdate = false;
    for (auto& [id, rObject] : RObjectManager::getRObjects()) {

        float numberOfTrianglesHigh = ceil(rObject.getMesh().getIndices().size() / 680.0f);

        if (numberOfTrianglesHigh > 2048) {
            numberOfTrianglesHigh = 2048;
        }

        cv::Mat fakeImage = cv::Mat::zeros(cv::Size(2040, numberOfTrianglesHigh), CV_32FC4);

        unsigned int countX = 0;
        unsigned int countY = 0;

        for (glm::uvec3& indice : rObject.getMesh().getIndices()) {

            cv::Vec4f& position1 = fakeImage.at<cv::Vec4f>(countY, 0 + countX);

            position1[0] = rObject.getMesh().getVertices().at(indice.x).x;
            position1[1] = rObject.getMesh().getVertices().at(indice.x).y;
            position1[2] = rObject.getMesh().getVertices().at(indice.x).z;
            position1[3] = 0;

            cv::Vec4f& position2 = fakeImage.at<cv::Vec4f>(countY, 1 + countX);

            position2[0] = rObject.getMesh().getVertices().at(indice.y).x;
            position2[1] = rObject.getMesh().getVertices().at(indice.y).y;
            position2[2] = rObject.getMesh().getVertices().at(indice.y).z;
            position2[3] = 0;

            cv::Vec4f& position3 = fakeImage.at<cv::Vec4f>(countY, 2 + countX);

            position3[0] = rObject.getMesh().getVertices().at(indice.z).x;
            position3[1] = rObject.getMesh().getVertices().at(indice.z).y;
            position3[2] = rObject.getMesh().getVertices().at(indice.z).z;
            position3[3] = 0;

            countX += 3;

            if (countX == 2040) {
                countY++;
                countX = 0;
            }
        }

        rObject.meshImageHeight = numberOfTrianglesHigh;
        rObject.meshImageWidth = 2040;

        unsigned int meshImageIDIn = rObject.meshImageIDIn;
        unsigned int meshImageIDOut = rObject.meshImageIDOut;

        if (meshImageIDIn != 0) {
            unsigned int texturesToDelete[2]{ meshImageIDIn, meshImageIDOut };
            glDeleteTextures(2, &texturesToDelete[0]);
        }

        glGenTextures(1, &meshImageIDIn);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, meshImageIDIn);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, rObject.meshImageWidth, rObject.meshImageHeight, 0, GL_RGBA, GL_FLOAT, fakeImage.data);
        glBindImageTexture(0, meshImageIDIn, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

        rObject.meshImageIDIn = meshImageIDIn;

        glGenTextures(1, &meshImageIDOut);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, meshImageIDOut);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, rObject.meshImageWidth, rObject.meshImageHeight, 0, GL_RGBA, GL_FLOAT, NULL);
        glBindImageTexture(1, meshImageIDOut, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

        rObject.meshImageIDOut = meshImageIDOut;
    }
}

void WRaytracing::threadMethod(int subsamplingSize, int subsamplingSqrt, glm::vec3 axis_x, glm::vec3 axis_y, glm::vec3 axis_z)
{
}

glm::vec3 WRaytracing::trace(glm::vec3 rOrigin, glm::vec3 rVector, unsigned int depth)
{
    glm::vec3 radiance = { 0,0,0 };

    glm::vec4 currIntersect = { 0, 0, 0, FLT_MAX };

    glm::uvec3 currIndice;
    unsigned int objectId;

    bool intersected = false;

    for (auto& [id, rObject] : RObjectManager::getRObjects()) {
        if (intersectRayBox(rOrigin, rVector, rObject)) { // there is a possibility we'll hit that object with the ray

            // for each triangle in object

            for (glm::uvec3 &indice : rObject.getMesh().getIndices()) {
                glm::vec4 result = intersect(rOrigin, rVector, rObject.getMesh().getVertices().at(indice.x), rObject.getMesh().getVertices().at(indice.y), rObject.getMesh().getVertices().at(indice.z));

                if (result.t != 0 && result.t < currIntersect.w) {
                    currIndice = indice;
                    objectId = rObject.getID();
                    currIntersect = result;

                    intersected = true;

                    //break;
                }
            }
        }
    }

    if (!intersected) return radiance;

    radiance = { 255, 255, 255 }; // TODO remove, only for testing purposes

    return radiance;
}

glm::vec3 WRaytracing::trace2(glm::vec3 rOrigin, glm::vec3 rVector, unsigned int depth)
{
    glm::vec3 radiance = { 0,0,0 };

    glm::vec4 currIntersect = { 0, 0, 0, FLT_MAX };

    glm::uvec3 currIndice;
    unsigned int objectId;

    bool intersected = false;

    // Check for spheres

    for (auto& [id, rObject] : RObjectManager::getRObjects()) {
        if (intersectRayBox(rOrigin, rVector, rObject)) { // there is a possibility we'll hit that object with the ray

            //cv::Mat fakeImageResult = cv::Mat::zeros(cv::Size(rObject.meshImageWidth, rObject.meshImageHeight), CV_32FC4);

            unsigned int meshImageIDOut = rObject.meshImageIDOut;

            unsigned int texturesToDelete[1]{ meshImageIDOut };
            glDeleteTextures(1, &texturesToDelete[0]);

            glGenTextures(1, &meshImageIDOut);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, meshImageIDOut);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, rObject.meshImageWidth, rObject.meshImageHeight, 0, GL_RGBA, GL_FLOAT, NULL);
            glBindImageTexture(1, meshImageIDOut, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);


            std::vector<glm::vec4> imageResult;

            imageResult.resize(rObject.meshImageWidth * rObject.meshImageHeight);

            glActiveTexture(GL_TEXTURE0);
            glBindImageTexture(0, rObject.meshImageIDIn, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);

            glUniform1i(glGetUniformLocation(shaderProgram, "img_input"), 0); // use GL_TEXTURE1
            glUniform3fv(glGetUniformLocation(shaderProgram, "rOrigin"), 1, &rOrigin[0]); // Vec 3 ?
            glUniform3fv(glGetUniformLocation(shaderProgram, "rVector"), 1, &rVector[0]); // Vec 3 ?

            glDispatchCompute(rObject.meshImageWidth, rObject.meshImageHeight, 1);

            glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

            glDispatchCompute(rObject.meshImageWidth, rObject.meshImageHeight, 1);

            glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

            glActiveTexture(GL_TEXTURE1);
            glBindImageTexture(1, rObject.meshImageIDOut, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
            glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, &imageResult[0]);

            rObject.meshImageIDOut = meshImageIDOut;
            
            // Find the closest intersect

            for (unsigned int i = 0; i < rObject.meshImageWidth * rObject.meshImageHeight; i++) {
                if (imageResult.at(i).w != 0 && imageResult.at(i).w < currIntersect.w) {
                    
                    currIntersect = imageResult.at(i);
                    objectId = rObject.getID();

                    intersected = true;
                }

            }
            
        }
    }

    if (!intersected) return radiance;

    glm::vec3 n = glm::normalize(glm::vec3(currIntersect) - RObjectManager::getRObjects().at(objectId).getPosition());
    glm::vec3 na = glm::dot(n, rVector) < 0 ? n : n * glm::vec3( -1, -1, -1 );

    // Should check uv map and diffuse to get texture

    glm::vec3 f = { 150,150,150 };

    if (++depth > 5)
    {
        if (uniformRD01(rng) < 0.75f) f = f * glm::vec3((1 / 150), (1 / 150), (1 / 150));
        else return {10,10,10};
    }

    double r1 = 2 * 3.14159265 * uniformRD01(rng);
    double r2 = uniformRD01(rng);
    double r2s = sqrt(r2);

    glm::vec3 w = na;
    glm::vec3 u = glm::normalize(glm::cross((fabs(w.x) > 0.1 ? glm::vec3(0, 1, 0) : glm::vec3(1, 0, 0)), w));
    glm::vec3 v = glm::cross(w, u);
    glm::vec3 d = glm::normalize(u * glm::vec3(cos(r1), cos(r1), cos(r1)) * glm::vec3(r2s, r2s, r2s) + v * glm::vec3(sin(r1), sin(r1), sin(r1)) * glm::vec3(r2s, r2s, r2s) + w * glm::vec3(sqrt(1 - r2), sqrt(1 - r2), sqrt(1 - r2)));

    radiance = glm::vec3(10, 10, 10) + f * trace2(glm::vec3(currIntersect), d, depth);

    return radiance;
}

void WRaytracing::renderImage()
{
    //if (firstUpdate) {
        updateObjectMeshImage();
    //}

    glUseProgram(shaderProgram);

    cv::Mat imgNonFlipper = cv::Mat::zeros(cv::Size(width, height), CV_8UC1);
    cv::cvtColor(imgNonFlipper, imgNonFlipper, cv::COLOR_BGR2RGBA);

    // Update the rough hitbox of all objects in the scene

    updateObjectBounds(); // TODO verify if this actually affects the object or not because of reference shinanygans

    //glm::vec3 cameraOrientation = glm::normalize(camera.getLookAt() - camera.getPosition());
    glm::vec3 cameraOrientation = glm::normalize(camera.getPosition() - camera.getLookAt());

    float fov = 0.5135;

    glm::vec3 axis_z = cameraOrientation;
    glm::vec3 axis_x = { (width * fov / height), 0, 0 };
    glm::vec3 axis_y = glm::normalize(glm::cross(axis_x, axis_z)) * fov;

    // Loop over all pixels

    int subsamplingSize = 2;
    int subsamplingSqrt = (subsamplingSize * subsamplingSize);

    auto t1 = std::chrono::high_resolution_clock::now();

    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {

            // Subsampling

            glm::vec4 pixel = { 0,0,0,255 };

            for (int sy = 0; sy < subsamplingSize; sy++) {
                for (int sx = 0; sx < subsamplingSize; sx++) {

                    // Boucle pour les RayPerPixel

                    glm::vec3 radiance = {0,0,0};

                    for (int r = 0; r < rayPP; r++) {

                        float r1 = 2.0 * uniformRD01(rng), dx = r1 < 1.0f ? sqrt(r1) - 1.0f : 1.0f - sqrt(2.0f - r1);
                        float r2 = 2.0 * uniformRD01(rng), dy = r2 < 1.0f ? sqrt(r2) - 1.0f : 1.0f - sqrt(2.0f - r2);

                        glm::vec3 dist = axis_x * (((sx + 0.5f + dx) / 2.0f + x) / width - 0.5f) +
                            axis_y * (((sy + 0.5f + dy) / 2.0f + y) / height - 0.5f) + axis_z;

                        radiance += trace2(camera.getPosition() + dist/* * 140.0f */, glm::normalize(dist), 0) * (1.0f / rayPP);// TODO not sure 140.0f is appropriate // TODO CHANGE TRACE 2

                    }

                    //image.pixel[index] += glm::clamp(radiance, 0.0f, 1.0f) * 0.25f; TODO CHANGE THIS FOR OPENCV IMAGE

                    pixel.x += glm::clamp(radiance.x, 0.0f, 255.0f) / subsamplingSqrt;
                    pixel.y += glm::clamp(radiance.y, 0.0f, 255.0f) / subsamplingSqrt;
                    pixel.z += glm::clamp(radiance.z, 0.0f, 255.0f) / subsamplingSqrt;
                    pixel.w += 0;

                }
            }

            cv::Vec4b& color = imgNonFlipper.at<cv::Vec4b>(y, x);

            color[0] = pixel.x;
            color[1] = pixel.y;
            color[2] = pixel.z;
            color[3] = pixel.w;
        }
        std::cout << "Y : " << y << std::endl;
    }

    auto t2 = std::chrono::high_resolution_clock::now();
    auto render_time = t2 - t1;
    std::cout << "Render time " << std::chrono::duration_cast<std::chrono::seconds>(render_time).count() << std::endl;

    img = cv::Mat::zeros(cv::Size(width, height), CV_8UC1);
    cv::cvtColor(img, img, cv::COLOR_BGR2RGBA);

    cv::flip(imgNonFlipper, img, -1);

    if (textureID != 0) {
        unsigned int texturesToDelete[1]{ textureID };
        glDeleteTextures(1, &texturesToDelete[0]);
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.data);
}

void WRaytracing::render()
{
    if (!p_open) return;

    ImGui::SetNextWindowSize(ImVec2(640, 360), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin("Raytracing", &p_open))
    {
        ImGui::End();
        return;
    }

    glBindTexture(GL_TEXTURE_2D, textureID);
    ImGui::Image((void*)(static_cast<intptr_t>(textureID)), ImVec2(width, height));

    ImGui::End();
}
