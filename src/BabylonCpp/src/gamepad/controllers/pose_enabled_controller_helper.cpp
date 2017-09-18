#include <babylon/gamepad/controllers/pose_enabled_controller_helper.h>

#include <babylon/core/string.h>
#include <babylon/gamepad/controllers/generic_controller.h>
#include <babylon/gamepad/controllers/oculus_touch_controller.h>
#include <babylon/gamepad/controllers/vive_controller.h>
#include <babylon/interfaces/ibrowser_gamepad.h>

namespace BABYLON {

WebVRController* PoseEnabledControllerHelper::InitiateController(
  const shared_ptr_t<IBrowserGamepad>& vrGamepad)
{
  // Oculus Touch
  if (String::contains(vrGamepad->id, "Oculus Touch")) {
    return new OculusTouchController(vrGamepad);
  }
  // Windows Mixed Reality controllers
  // else if (String::contains(vrGamepad->id, "Spatial Control")) {
  //  return new WindowsMixedRealityController(vrGamepad);
  // }
  // HTC Vive
  else if (String::contains(String::toLowerCase(vrGamepad->id), "openvr")) {
    return new ViveController(vrGamepad);
  }
  // Generic
  else {
    return new GenericController(vrGamepad);
  }
}

} // end of namespace BABYLON
