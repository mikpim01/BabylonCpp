#ifndef BABYLON_BEHAVIORS_BEHAVIOR_H
#define BABYLON_BEHAVIORS_BEHAVIOR_H

#include <functional>
#include <memory>
#include <string>

#include <babylon/babylon_api.h>
#include <babylon/babylon_fwd.h>

namespace BABYLON {

FWD_CLASS_SPTR(AbstractMesh)

/**
 * @brief Interface used to define a behavior.
 */
template <class T>
struct BABYLON_SHARED_EXPORT Behavior {
  virtual ~Behavior() = default;
  /**
   * gets or sets behavior's name
   */
  std::string name;

  /**
   * @brief Function called when the behavior needs to be initialized (after attaching it to a
   * target).
   */
  virtual void init() = 0;
  /**
   * @brief Called when the behavior is attached to a target.
   * @param target defines the target where the behavior is attached to
   * @param predicate Predicate to use for pick filtering
   */
  virtual void attach(const std::shared_ptr<T>& target,
                      const std::function<bool(const AbstractMeshPtr& m)>& predicate = nullptr)
    = 0;
  /**
   * @brief Called when the behavior is detached from its target.
   */
  virtual void detach() = 0;

}; // end of struct Behavior<T>

} // end of namespace BABYLON

#endif // end of BABYLON_BEHAVIORS_BEHAVIOR_H
