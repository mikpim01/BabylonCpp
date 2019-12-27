#include <babylon/misc/rgbd_texture_tools.h>

#include <babylon/engines/constants.h>
#include <babylon/engines/engine.h>
#include <babylon/engines/scene.h>
#include <babylon/materials/effect.h>
#include <babylon/materials/textures/internal_texture.h>
#include <babylon/materials/textures/irender_target_options.h>
#include <babylon/materials/textures/texture.h>
#include <babylon/postprocesses/post_process.h>
#include <babylon/postprocesses/post_process_manager.h>

namespace BABYLON {

void RGBDTextureTools::ExpandRGBDTexture(const TexturePtr& texture)
{
  auto& internalTexture = texture->_texture;
  if (!internalTexture || !texture->isRGBD()) {
    return;
  }

  // Gets everything ready.
  auto engine        = internalTexture->getEngine();
  auto& caps         = engine->getCaps();
  auto expandTexture = false;

  // If half float available we can uncompress the texture
  if (caps.textureHalfFloatRender && caps.textureHalfFloatLinearFiltering) {
    expandTexture         = true;
    internalTexture->type = Constants::TEXTURETYPE_HALF_FLOAT;
  }
  // If full float available we can uncompress the texture
  else if (caps.textureFloatRender && caps.textureFloatLinearFiltering) {
    expandTexture         = true;
    internalTexture->type = Constants::TEXTURETYPE_FLOAT;
  }

  if (expandTexture) {
    // Do not use during decode.
    internalTexture->isReady = false;
    internalTexture->_isRGBD = false;
    internalTexture->invertY = false;
  }

  texture->onLoadObservable().addOnce([expandTexture, internalTexture,
                                       engine](Texture* iTtexture, EventState & /*es*/) -> void {
    // Expand the texture if possible
    if (expandTexture) {
      // Simply run through the decode PP.
      auto rgbdPostProcess = PostProcess::New("rgbdDecode", "rgbdDecode", {}, {}, 1.f, nullptr,
                                              Constants::TEXTURE_TRILINEAR_SAMPLINGMODE, engine,
                                              false, "", internalTexture->type, "", {}, false);

      // Hold the output of the decoding.
      IRenderTargetOptions options;
      options.generateDepthBuffer   = false;
      options.generateMipMaps       = false;
      options.generateStencilBuffer = false;
      options.samplingMode          = Constants::TEXTURE_BILINEAR_SAMPLINGMODE;
      options.type                  = internalTexture->type;
      options.format                = Constants::TEXTUREFORMAT_RGBA;
      auto expandedTexture
        = engine->createRenderTargetTexture(static_cast<float>(internalTexture->width), options);

      rgbdPostProcess->getEffect()->executeWhenCompiled([&internalTexture, &expandedTexture,
                                                         &rgbdPostProcess, &engine,
                                                         &iTtexture](Effect * /*effect*/) -> void {
        // PP Render Pass
        rgbdPostProcess->onApply = [&internalTexture](Effect* effect, EventState & /*es*/) -> void {
          effect->_bindTexture("textureSampler", internalTexture);
          effect->setFloat2("scale", 1.f, 1.f);
        };
        iTtexture->getScene()->postProcessManager->directRender({rgbdPostProcess}, expandedTexture,
                                                                true);

        // Cleanup
        engine->restoreDefaultFramebuffer();
        engine->_releaseTexture(internalTexture.get());
        engine->_releaseFramebufferObjects(expandedTexture.get());
        if (rgbdPostProcess) {
          rgbdPostProcess->dispose();
        }

        // Internal Swap
        expandedTexture->_swapAndDie(internalTexture);

        // Ready to get rolling again.
        internalTexture->isReady = true;
      });
    }
  });
}

} // end of namespace BABYLON
