#include <babylon/samples/specialfx/convolution_post_process_scene.h>

#include <babylon/cameras/arc_rotate_camera.h>
#include <babylon/engine/scene.h>
#include <babylon/lights/directional_light.h>
#include <babylon/materials/standard_material.h>
#include <babylon/materials/textures/cube_texture.h>
#include <babylon/materials/textures/texture_constants.h>
#include <babylon/mesh/mesh.h>
#include <babylon/postprocess/convolution_post_process.h>

namespace BABYLON {
namespace Samples {

ConvolutionPostProcessScene::ConvolutionPostProcessScene(ICanvas* iCanvas)
    : IRenderableScene(iCanvas)
    , _sphere0{nullptr}
    , _sphere1{nullptr}
    , _sphere2{nullptr}
    , _cube{nullptr}
    , _alpha{0.f}
{
}

ConvolutionPostProcessScene::~ConvolutionPostProcessScene()
{
}

const char* ConvolutionPostProcessScene::getName()
{
  return "Convolution Post-process Scene";
}

void ConvolutionPostProcessScene::initializeScene(ICanvas* canvas, Scene* scene)
{
  auto camera
    = ArcRotateCamera::New("Camera", 0, 0, 10, Vector3::Zero(), scene);
  auto light = DirectionalLight::New("dir01", Vector3(0.f, -1.f, -0.2f), scene);
  auto light2 = DirectionalLight::New("dir02", Vector3(-1.f, 2.f, -1.f), scene);
  light->position  = Vector3(0.f, 30.f, 0.f);
  light2->position = Vector3(10.f, 20.f, 10.f);

  light->intensity  = 0.6f;
  light2->intensity = 0.6f;

  camera->setPosition(Vector3(-40.f, 40.f, 0.f));
  camera->attachControl(canvas, true);

  // Skybox
  auto skybox                     = Mesh::CreateBox("skyBox", 500.f, scene);
  auto skyboxMaterial             = StandardMaterial::New("skyBox", scene);
  skyboxMaterial->backFaceCulling = false;
  skyboxMaterial->reflectionTexture
    = CubeTexture::New("/textures/skybox/skybox", scene);
  skyboxMaterial->reflectionTexture()->coordinatesMode
    = TextureConstants::SKYBOX_MODE;
  skyboxMaterial->diffuseColor    = Color3(0.f, 0.f, 0.f);
  skyboxMaterial->specularColor   = Color3(0.f, 0.f, 0.f);
  skyboxMaterial->disableLighting = true;
  skybox->material                = skyboxMaterial;
  skybox->infiniteDistance        = true;

  // Spheres
  _sphere0 = Mesh::CreateSphere("Sphere0", 16, 10, scene);
  _sphere1 = Mesh::CreateSphere("Sphere1", 16, 10, scene);
  _sphere2 = Mesh::CreateSphere("Sphere2", 16, 10, scene);
  _cube    = Mesh::CreateBox("Cube", 10.f, scene);

  auto sphereMaterial          = StandardMaterial::New("white", scene);
  sphereMaterial->diffuseColor = Color3(0.5, 0.5, 1.0);
  _sphere0->material           = sphereMaterial;

  _sphere1->material = sphereMaterial;
  _sphere2->material = sphereMaterial;

  _sphere0->convertToFlatShadedMesh();
  _sphere1->convertToFlatShadedMesh();
  _sphere2->convertToFlatShadedMesh();

  auto cubeMaterial           = StandardMaterial::New("red", scene);
  cubeMaterial->diffuseColor  = Color3(1.f, 0.5f, 0.5f);
  cubeMaterial->specularColor = Color3(0.f, 0.f, 0.f);
  _cube->material             = cubeMaterial;

  // Post-process
  auto postProcess = ConvolutionPostProcess::New(
    "convolution", ConvolutionPostProcess::EmbossKernel, 1.f, camera);

  // Animations
  _scene->registerBeforeRender([this](Scene* /*scene*/, EventState& /*es*/) {
    _sphere0->position
      = Vector3(20.f * std::sin(_alpha), 0.f, 20.f * std::cos(_alpha));
    _sphere1->position
      = Vector3(20.f * std::sin(_alpha), -20.f * std::cos(_alpha), 0.f);
    _sphere2->position
      = Vector3(0.f, 20.f * std::cos(_alpha), 20.f * std::sin(_alpha));

    _cube->rotation().y += 0.01f;
    _cube->rotation().z += 0.01f;

    _alpha += 0.05f;
  });
}

} // end of namespace Samples
} // end of namespace BABYLON
