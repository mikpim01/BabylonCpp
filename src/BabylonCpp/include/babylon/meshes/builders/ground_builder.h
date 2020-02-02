#ifndef BABYLON_MESHES_BUILDERS_GROUND_BUILDER_H
#define BABYLON_MESHES_BUILDERS_GROUND_BUILDER_H

#include <memory>
#include <string>

#include <babylon/babylon_api.h>

namespace BABYLON {

class GroundFromHeightMapOptions;
class GroundMesh;
class GroundOptions;
class Mesh;
class TiledGroundOptions;
class Scene;
using GroundMeshPtr = std::shared_ptr<GroundMesh>;
using MeshPtr       = std::shared_ptr<Mesh>;

/**
 * @brief Class containing static functions to help procedurally build meshes.
 */
struct BABYLON_SHARED_EXPORT GroundBuilder {

  /**
   * @brief Creates a ground mesh.
   * * The parameters `width` and `height` (floats, default 1) set the width and height sizes of the
   * ground
   * * The parameter `subdivisions` (positive integer) sets the number of subdivisions per side
   * * The mesh can be set to updatable with the boolean parameter `updatable` (default false) if
   * its internal geometry is supposed to change once created
   * @param name defines the name of the mesh
   * @param options defines the options used to create the mesh
   * @param scene defines the hosting scene
   * @returns the ground mesh
   * @see https://doc.babylonjs.com/how_to/set_shapes#ground
   */
  static GroundMeshPtr CreateGround(const std::string& name, GroundOptions& options,
                                    Scene* scene = nullptr);

  /**
   * @brief Creates a tiled ground mesh.
   * * The parameters `xmin` and `xmax` (floats, default -1 and 1) set the ground minimum and
   * maximum X coordinates
   * * The parameters `zmin` and `zmax` (floats, default -1 and 1) set the ground minimum and
   * maximum Z coordinates
   * * The parameter `subdivisions` is a javascript object `{w: positive integer, h: positive
   * integer}` (default `{w: 6, h: 6}`). `w` and `h` are the numbers of subdivisions on the ground
   * width and height. Each subdivision is called a tile
   * * The parameter `precision` is a javascript object `{w: positive integer, h: positive integer}`
   * (default `{w: 2, h: 2}`). `w` and `h` are the numbers of subdivisions on the ground width and
   * height of each tile
   * * The mesh can be set to updatable with the boolean parameter `updatable` (default false) if
   * its internal geometry is supposed to change once created.
   * @param name defines the name of the mesh
   * @param options defines the options used to create the mesh
   * @param scene defines the hosting scene
   * @returns the tiled ground mesh
   * @see https://doc.babylonjs.com/how_to/set_shapes#tiled-ground
   */
  static MeshPtr CreateTiledGround(const std::string& name, TiledGroundOptions& options,
                                   Scene* scene = nullptr);

  /**
   * @brief Creates a ground mesh from a height map.
   * * The parameter `url` sets the URL of the height map image resource.
   * * The parameters `width` and `height` (positive floats, default 10) set the ground width and
   * height sizes.
   * * The parameter `subdivisions` (positive integer, default 1) sets the number of subdivision per
   * side.
   * * The parameter `minHeight` (float, default 0) is the minimum altitude on the ground.
   * * The parameter `maxHeight` (float, default 1) is the maximum altitude on the ground.
   * * The parameter `colorFilter` (optional Color3, default (0.3, 0.59, 0.11) ) is the filter to
   * apply to the image pixel colors to compute the height.
   * * The parameter `onReady` is a javascript callback function that will be called  once the mesh
   * is just built (the height map download can last some time).
   * * The parameter `alphaFilter` will filter any data where the alpha channel is below this value,
   * defaults 0 (all data visible)
   * * The mesh can be set to updatable with the boolean parameter `updatable` (default false) if
   * its internal geometry is supposed to change once created.
   * @param name defines the name of the mesh
   * @param url defines the url to the height map
   * @param options defines the options used to create the mesh
   * @param scene defines the hosting scene
   * @returns the ground mesh
   * @see https://doc.babylonjs.com/babylon101/height_map
   * @see https://doc.babylonjs.com/how_to/set_shapes#ground-from-a-height-map
   */
  static GroundMeshPtr CreateGroundFromHeightMap(const std::string& name, const std::string& url,
                                                 GroundFromHeightMapOptions& options,
                                                 Scene* scene = nullptr);

}; // end of struct GroundBuilder

} // end of namespace BABYLON

#endif // end of BABYLON_MESHES_BUILDERS_GROUND_BUILDER_H
