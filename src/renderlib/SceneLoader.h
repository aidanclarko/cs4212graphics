#pragma once

#include "ISceneLoader.h"
#include "Scene.h"
#include "Lambertian.h"
#include "Mirror.h"
#include "BlinnPhong.h"

class SceneLoader : public ISceneLoader {
private:
  std::shared_ptr<Scene> m_targetScene; // Reference to the external scene
  std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders;

public:
  // The caller provides the scene to be filled
  SceneLoader(std::shared_ptr<Scene> sceneToPopulate)
      : m_targetScene(sceneToPopulate), numShaders(0), numTextures(0) {}

  void reserveCameras(size_t count) override {
    // makes sure the vector has enough space
    m_targetScene->getCameras().reserve(count);
  }
  void reserveLights(size_t count) override {
    m_targetScene->getLights().reserve(count);
  }
  void reserveShapes(size_t count) override {
    m_targetScene->getShapes().reserve(count);
  }
  void reserveShaders(size_t count) override { numShaders = count; }
  void reserveTextures(size_t count) override { numTextures = count; }

  void setSceneParams(const SceneParams &sparams) override {
    std::cout << "Setting scene params." << std::endl;
  }

  void addCamera(const std::string &name, const std::string &type,
                 const ISceneLoader::vec &pos, const ISceneLoader::vec &viewDir,
                 float focalLength, float imagePlaneWidth) override {
    std::cout << "Creating camera: " << name << ", type:"  << std::endl;
    auto cam = std::make_shared<PerspectiveCamera>(
      point3(pos.x, pos.y, pos.z),
      vec3(viewDir.x, viewDir.y, viewDir.z),
      imagePlaneWidth, focalLength
    );
    // cam->type = type;
    m_targetScene->pushCamera(cam);
  }
  void addPointLight(const ISceneLoader::vec &pos,
                     const ISceneLoader::vec &intensity) override {
    std::cout << "Creating point light." << std::endl;
    auto light = std::make_shared<Light>(vec3(pos.x, pos.y, pos.z), vec3(intensity.x, intensity.y, intensity.z));
    // light->type = "Point Light";
    m_targetScene->pushLight(light);
  }
  void addAreaLight(const ISceneLoader::vec &pos,
                    const ISceneLoader::vec &color,
                    const ISceneLoader::vec &normal, float width,
                    float length) override {
    std::cout << "Creating area light." << std::endl;
  }
  void addShader(const ISceneLoader::ShaderDesc &shaderDesc) override {
    std::cout << "Creating shader: type=" << shaderDesc.type << std::endl;
    // I place my shaders on an unordered map often
    std::shared_ptr<Shader> shader;
    if (shaderDesc.type == "Lambertian") {
        shader = std::make_shared<Lambert>(vec3(shaderDesc.diffuse.data.x, shaderDesc.diffuse.data.y, shaderDesc.diffuse.data.z));
    } else if (shaderDesc.type == "BlinnPhong") {
        shader = std::make_shared<BlinnPhong>(
            vec3(shaderDesc.diffuse.data.x, shaderDesc.diffuse.data.y, shaderDesc.diffuse.data.z),
            vec3(shaderDesc.specular.data.x, shaderDesc.specular.data.y, shaderDesc.specular.data.z),
            shaderDesc.phongExp
        );
    } else if (shaderDesc.type == "Mirror") {
        shader = std::make_shared<Mirror>();
    }
    if (shader) m_shaders[shaderDesc.name] = shader;

  }
  void addShape(const ISceneLoader::ShapeDesc &shapeDesc) override {
    std::cout << "Creating shape: type=" << shapeDesc.type << std::endl;
    std::shared_ptr<Shape> shape;
    std::shared_ptr<Shader> shader;

    auto it = m_shaders.find(shapeDesc.shaderNameReference);
    if (it != m_shaders.end()) {
        shader = it->second;
    } else {
        std::cout << "WARNING: shader '" << shapeDesc.shaderNameReference << "' not found, using default" << std::endl;
        shader = std::make_shared<Lambert>();
    }
    
    if(shapeDesc.type == "sphere") {
       shape = std::make_shared<Sphere>(
        vec3(shapeDesc.center.x, shapeDesc.center.y, shapeDesc.center.z),
        shapeDesc.radius,
        shader
      );
    } else if(shapeDesc.type == "triangle") {
       shape = std::make_shared<Triangle>(
        point3(shapeDesc.v0.x, shapeDesc.v0.y, shapeDesc.v0.z),
        point3(shapeDesc.v1.x, shapeDesc.v1.y, shapeDesc.v1.z),
        point3(shapeDesc.v2.x, shapeDesc.v2.y, shapeDesc.v2.z),
        shader
      );
    }
    m_targetScene->pushShape(shape);
  }
  void addTexture(const std::string &type, const std::string &name,
                  const std::string &sourceFile) override {
    std::cout << "Creating texture: type=" << type << ", name=" << name
              << ", sourceFile=" << sourceFile << std::endl;
  }

public:
  int numShaders;
  int numTextures;
};
