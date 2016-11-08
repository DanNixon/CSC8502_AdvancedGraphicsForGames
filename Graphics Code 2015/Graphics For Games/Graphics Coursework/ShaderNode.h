#pragma once

#include "SceneNode.h"

#include "ShaderProgram.h"

class ShaderNode : public SceneNode
{
public:
  ShaderNode(const std::string &name, ShaderProgram * program);
  virtual ~ShaderNode();

  inline bool Use() const
  {
    return m_use;
  }

  void SetUse(bool use);

  virtual void Render();

private:
  bool m_use;
  ShaderProgram * m_program;
};