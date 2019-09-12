#include <babylon/materials/node/blocks/dot_block.h>

#include <babylon/core/string.h>
#include <babylon/materials/node/node_material_build_state.h>
#include <babylon/materials/node/node_material_connection_point.h>

namespace BABYLON {

DotBlock::DotBlock(const std::string& iName)
    : NodeMaterialBlock{iName, NodeMaterialBlockTargets::Neutral}
    , left{this, &DotBlock::get_left}
    , right{this, &DotBlock::get_right}
    , output{this, &DotBlock::get_output}
{
  registerInput("left", NodeMaterialBlockConnectionPointTypes::AutoDetect);
  registerInput("right", NodeMaterialBlockConnectionPointTypes::AutoDetect);
  registerOutput("output", NodeMaterialBlockConnectionPointTypes::Float);

  _linkConnectionTypes(0, 1);
}

DotBlock::~DotBlock()
{
}

const std::string DotBlock::getClassName() const
{
  return "DotBlock";
}

NodeMaterialConnectionPointPtr& DotBlock::get_left()
{
  return _inputs[0];
}

NodeMaterialConnectionPointPtr& DotBlock::get_right()
{
  return _inputs[1];
}

NodeMaterialConnectionPointPtr& DotBlock::get_output()
{
  return _outputs[0];
}

DotBlock& DotBlock::_buildBlock(NodeMaterialBuildState& state)
{
  NodeMaterialBlock::_buildBlock(state);

  const auto& output = _outputs[0];

  state.compilationString
    += _declareOutput(output, state)
       + String::printf(" = dot(%s, %s);\r\n",
                        left()->associatedVariableName().c_str(),
                        right()->associatedVariableName().c_str());

  return *this;
}

} // end of namespace BABYLON
