#ifndef BABYLONCPP_SAMPLES_INFO_H
#define BABYLONCPP_SAMPLES_INFO_H

#include <string>
#include <unordered_map>
#include <map>
#include <functional>
#include <memory>
#include <optional>
#include <babylon/babylon_api.h>
#include <nlohmann/json.hpp>

namespace BABYLON {

class ICanvas;
class IRenderableScene;
using IRenderableScenePtr = std::shared_ptr<IRenderableScene>;

namespace SamplesInfo {

using CategoryName = std::string;
using SampleName = std::string;
using SampleFactoryFunction = std::function<IRenderableScenePtr(ICanvas* iCanvas)>;


// These informations are extracted from to source file
struct BABYLON_SHARED_EXPORT SampleSourceInfo {
  std::string HeaderFile;
  std::string SourceFile;
  std::string Brief;
  std::vector<std::string> Links;
};

enum class BABYLON_SHARED_EXPORT SampleRunStatus {
  success,
  unhandledException,
  tooSlowOrHung,
  empty3d,
  unknown
};
void to_json(nlohmann::json& j, const SampleRunStatus& v);
void from_json(const nlohmann::json& j, SampleRunStatus& v);

struct BABYLON_SHARED_EXPORT SampleSearchQuery {
  std::string QueryName = "";
  std::string QueryCategory = "";
  std::map<SampleRunStatus, bool> IncludeStatus = {
    { SampleRunStatus::success, true },
    { SampleRunStatus::unhandledException, false },
    { SampleRunStatus::tooSlowOrHung, false },
    { SampleRunStatus::empty3d, false },
    { SampleRunStatus::unknown, false },
  };
};

struct BABYLON_SHARED_EXPORT SampleRunInfo {
  SampleRunStatus sampleRunStatus = SampleRunStatus::unknown;
  std::string unhandledExceptionStackTrace = "";
};

struct BABYLON_SHARED_EXPORT SampleData {
  CategoryName categoryName;
  SampleName sampleName;
  SampleSourceInfo sourceInfo;
  SampleRunInfo runInfo;
  SampleFactoryFunction factoryFunction = {};
};

using SampleStats = std::map<SampleRunStatus, std::size_t>;

std::string SamplesProjectFolder();

class BABYLON_SHARED_EXPORT SamplesCollection
{
public:
  static SamplesCollection& Instance();

  const std::vector<SampleData>& AllSamples() const { return _allSamples; }

  std::map<SamplesInfo::CategoryName, std::vector<const SampleData *>>
    SearchSamples(const SampleSearchQuery& query);


  SampleData* GetSampleByName(const SampleName& sampleName); // returns nullptr if not found
  const SampleData* GetSampleByName(const SampleName& sampleName) const; // returns nullptr if not found

  SampleStats GetSampleStats();
  std::string GetSampleStatsString();

  void SetSampleRunInfo(const SampleName& sampleName, const SampleRunInfo& sampleRunInfo);
  void SaveAllSamplesRunStatuses();

  IRenderableScenePtr createRenderableScene(const std::string& sampleName, ICanvas* iCanvas) const;

private:
  SamplesCollection();
  void ReadSamplesSourceInfos();
  void ReadAllSampleStatuses();

  std::vector<SampleData> _allSamples;
};


BABYLON_SHARED_EXPORT std::string screenshotsDirectory();

} // namespace SamplesInfo

} // namespace BABYLON

#endif // BABYLONCPP_SAMPLES_INFO_H
