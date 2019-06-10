#ifndef BABYLON_MESHES_BUILDERS_SPHERE_BUILDER_H
#define BABYLON_MESHES_BUILDERS_SPHERE_BUILDER_H

#include <memory>
#include <string>

#include <babylon/babylon_api.h>

namespace BABYLON {

class Mesh;
class Scene;
class SphereOptions;
using MeshPtr = std::shared_ptr<Mesh>;

/**
 * @brief Class containing static functions to help procedurally build meshes.
 */
struct BABYLON_SHARED_EXPORT SphereBuilder {

  /**
   * @brief Creates a sphere mesh.
   * * The parameter `diameter` sets the diameter size (float) of the sphere
   * (default 1)
   * * You can set some different sphere dimensions, for instance to build an
   * ellipsoid, by using the parameters `diameterX`, `diameterY` and `diameterZ`
   * (all by default have the same value of `diameter`)
   * * The parameter `segments` sets the sphere number of horizontal stripes
   * (positive integer, default 32)
   * * You can create an unclosed sphere with the parameter `arc` (positive
   * float, default 1), valued between 0 and 1, what is the ratio of the
   * circumference (latitude) : 2 x PI x ratio
   * * You can create an unclosed sphere on its height with the parameter
   * `slice` (positive float, default1), valued between 0 and 1, what is the
   * height ratio (longitude)
   * * You can also set the mesh side orientation with the values :
   * BABYLON.Mesh.FRONTSIDE (default), BABYLON.Mesh.BACKSIDE or
   * BABYLON.Mesh.DOUBLESIDE
   * * If you create a double-sided mesh, you can choose what parts of the
   * texture image to crop and stick respectively on the front and the back
   * sides with the parameters `frontUVs` and `backUVs` (Vector4). Detail here :
   * https://doc.babylonjs.com/babylon101/discover_basic_elements#side-orientation
   * * The mesh can be set to updatable with the boolean parameter `updatable`
   * (default false) if its internal geometry is supposed to change once created
   * @param name defines the name of the mesh
   * @param options defines the options used to create the mesh
   * @param scene defines the hosting scene
   * @returns the sphere mesh
   * @see https://doc.babylonjs.com/how_to/set_shapes#sphere
   */
  static MeshPtr CreateSphere(const std::string& name, SphereOptions& options,
                              Scene* scene = nullptr);

}; // end of struct SphereBuilder

} // end of namespace BABYLON

#endif // end of BABYLON_MESHES_BUILDERS_SPHERE_BUILDER_H
