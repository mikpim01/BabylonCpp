#include <babylon/postprocesses/highlights_post_process.h>

namespace BABYLON {

HighlightsPostProcess::HighlightsPostProcess(const std::string& iName, float ratio,
                                             const CameraPtr& camera,
                                             const std::optional<unsigned int>& samplingMode,
                                             Engine* engine, bool reusable,
                                             unsigned int textureType)
    : PostProcess{iName,        "highlights", {},       {},      ratio,      camera,
                  samplingMode, engine,       reusable, nullptr, textureType}
{
}

HighlightsPostProcess::~HighlightsPostProcess() = default;

std::string HighlightsPostProcess::getClassName() const
{
  return "HighlightsPostProcess";
}

} // end of namespace BABYLON
