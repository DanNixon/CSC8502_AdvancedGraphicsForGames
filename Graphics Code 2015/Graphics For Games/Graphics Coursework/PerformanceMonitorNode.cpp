/** @file */

#include "PerformanceMonitorNode.h"

#include "CameraNode.h"
#include "ShaderProgram.h"
#include "Shaders.h"

namespace GraphicsCoursework
{
PerformanceMonitorNode::PerformanceMonitorNode(const std::string &name, Font *font)
    : TextNode(name, font, 64)
{
}

PerformanceMonitorNode::~PerformanceMonitorNode()
{
}

void PerformanceMonitorNode::Update(float msec)
{
  // TODO
  //SetText("msec is " + std::to_string(msec));
}
}
