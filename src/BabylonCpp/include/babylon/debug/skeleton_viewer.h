#ifndef BABYLON_DEBUG_SKELETON_VIEWER_H
#define BABYLON_DEBUG_SKELETON_VIEWER_H

#include <babylon/babylon_global.h>
#include <babylon/math/color3.h>
#include <babylon/tools/event_state.h>

namespace BABYLON {
namespace Debug {

/**
 * @brief Class used to render a debug view of a given skeleton.
 * @see http://www.babylonjs-playground.com/#1BZJVJ#8
 */
class BABYLON_SHARED_EXPORT SkeletonViewer {

public:
  /**
   * @brief Creates a new SkeletonViewer.
   * @param skeleton defines the skeleton to render
   * @param mesh defines the mesh attached to the skeleton
   * @param scene defines the hosting scene
   * @param autoUpdateBonesMatrices defines a boolean indicating if bones
   * matrices must be forced to update before rendering (true by default)
   * @param renderingGroupId defines the rendering group id to use with the
   * viewer
   */
  SkeletonViewer(Skeleton* skeleton, AbstractMesh* mesh, Scene* scene,
                 bool autoUpdateBonesMatrices  = true,
                 unsigned int renderingGroupId = 1);
  ~SkeletonViewer();

  /**
   * @brief Update the viewer to sync with current skeleton state.
   */
  void update();

  /**
   * @brief Release associated resources.
   */
  void dispose();

protected:
  /**
   * @brief Sets a boolean indicating if the viewer is enabled.
   */
  void set_isEnabled(bool value);

  /**
   * @brief Gets a boolean indicating if the viewer is enabled.
   */
  bool get_isEnabled() const;

private:
  void _getBonePosition(Vector3& position, Bone* bone, const Matrix& meshMat,
                        float x = 0.f, float y = 0.f, float z = 0.f) const;
  void _getLinesForBonesWithLength(const vector_t<unique_ptr_t<Bone>>& bones,
                                   const Matrix& meshMat);
  void _resizeDebugLines(size_t bonesSize);
  void _getLinesForBonesNoLength(const vector_t<unique_ptr_t<Bone>>& bones,
                                 const Matrix& meshMat);

public:
  /**
   * Gets or sets the color used to render the skeleton
   */
  Color3 color;

  /**
   * Defines the skeleton to render
   */
  Skeleton* skeleton;

  /**
   * Defines the mesh attached to the skeleton
   */
  AbstractMesh* mesh;

  /**
   * Defines a boolean indicating if bones matrices must be forced to update
   * before rendering (true by default)
   */
  bool autoUpdateBonesMatrices;

  /**
   * Defines the rendering group id to use with the viewer
   */
  unsigned int renderingGroupId;

  Property<SkeletonViewer, bool> isEnabled;

private:
  Scene* _scene;
  vector_t<vector_t<Vector3>> _debugLines;
  LinesMesh* _debugMesh;
  bool _isEnabled;
  ::std::function<void(Scene*, EventState&)> _renderFunction;

}; // end of class SkeletonViewer

} // end of namespace Debug
} // end of namespace BABYLON

#endif // end of BABYLON_DEBUG_SKELETON_VIEWER_H
