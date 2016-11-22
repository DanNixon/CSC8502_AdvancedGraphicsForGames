/** @file */

#include "ILight.h"

#include "CameraNode.h"
#include "FramebufferNode.h"
#include "Renderer.h"
#include "ShaderProgram.h"
#include "ShadowTexture.h"
#include "SubtreeNode.h"
#include "ShaderNode.h"
#include "Shaders.h"
#include "CameraSelectorNode.h"
#include "GenericControlNode.h"
#include "directories.h"

namespace GraphicsCoursework
{
/**
 * @brief Creates a new light.
 * @param name Node name
 */
ILight::ILight(const std::string &name)
    : ShaderDataNode(name)
    , m_colour(1.0f, 1.0f, 1.0f, 1.0f)
    , m_ambientIntensity(0.1f)
    , m_reach(10.0f)
    , m_shadowSceneRoot(nullptr)
{
}

ILight::~ILight()
{
  for (auto it = m_shadowTextures.begin(); it != m_shadowTextures.end(); ++it)
    delete *it;

  if (m_shadowSceneRoot != nullptr)
    delete m_shadowSceneRoot;
}

/**
 * @brief Creates the textures required to do shadow mapping for this light.
 * @param shadowTexDim Dimensions of the shadow texture
 * @param shadowSceneRoot Root node of the subtree of the scene graph that has shadows cast upon it by this light
 */
void ILight::InitShadows(GLuint shadowTexDim, SceneNode *shadowSceneRoot, const Vector2 &screenDims)
{
  for (size_t i = 0; i < NumDirections(); i++)
    m_shadowTextures.push_back(new ShadowTexture(shadowTexDim));

  m_shadowSceneRoot = new FramebufferNode(m_name + "_ShadowFramebuffer");
  m_shadowSceneRoot->SetIsProcessingNode(true);

  m_shadowCamera = new CameraNode(m_name + "_ShadowCamera");
  m_shadowCamera->SetIsProcessingNode(true);
  m_shadowSceneRoot->AddChild(m_shadowCamera);

  auto shadowShader = m_shadowCamera->AddChild(new ShaderNode(m_name + "_ShadowShader", new ShaderProgram({
      new VertexShader(CW_SHADER_DIR "ShadowVertex.glsl"),
      new FragmentShader(CW_SHADER_DIR "ShadowFragment.glsl")
    })));
  shadowShader->SetIsProcessingNode(true);

  CameraSelectorNode * shadowCameraSelect  = new CameraSelectorNode(m_name + "_ShadowCameraSelect");
  shadowCameraSelect->SetIsProcessingNode(true);
  shadowCameraSelect->SetCamera(m_shadowCamera);
  shadowShader->AddChild(shadowCameraSelect);

  auto shadowControlNode = shadowCameraSelect->AddChild(new GenericControlNode(m_name + "_ShadowControl", [shadowTexDim](ShaderProgram *s) {
    glViewport(0, 0, shadowTexDim, shadowTexDim);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
  }, [&screenDims](ShaderProgram * s) {
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glViewport(0, 0, (GLsizei)screenDims.x, (GLsizei)screenDims.y);
  }));
  shadowControlNode->SetIsProcessingNode(true);

  auto shadowSubtree = shadowControlNode->AddChild(new SubTreeNode(m_name + "_ShadowSubtree", shadowSceneRoot));
  shadowSubtree->SetIsProcessingNode(true);
}

/**
 * @brief Sets the index of this light in the shader uniform array.
 * @param index New light index
 */
void ILight::SetIndex(size_t index)
{
  m_index = index;
  SetUniformNames(std::to_string(m_index));
}

void ILight::DoShadowRender()
{
  RenderState state;
  state.processPass = true;

  std::vector<Vector3> directions;
  CastDirections(directions);

  for (size_t i = 0; i < NumDirections(); i++)
  {
    m_shadowSceneRoot->BindTexture(GL_DEPTH_ATTACHMENT, m_shadowTextures[i]);
    m_shadowCamera->LookInDirection(directions[i]);
    m_shadowSceneRoot->Render(state);
  }
}

void ILight::PreRender(RenderState &state)
{
  ShaderDataNode::PreRender(state);

  if (m_shadowSceneRoot != nullptr)
    DoShadowRender();
}

/**
 * @copydoc ShaderDataNode::ShaderBind
 */
void ILight::ShaderBind(ShaderProgram *s)
{
  glUniform3fv(glGetUniformLocation(s->Program(), m_shaderUniformNames[UNIFORM_POSITION].c_str()), 1,
               (float *)&m_worldTransform.GetPositionVector());

  glUniform4fv(glGetUniformLocation(s->Program(), m_shaderUniformNames[UNIFORM_COLOUR].c_str()), 1, (float *)&m_colour);

  glUniform1f(glGetUniformLocation(s->Program(), m_shaderUniformNames[UNIFORM_AMBIENT_INTENSITY].c_str()),
              m_ambientIntensity);

  glUniform1f(glGetUniformLocation(s->Program(), m_shaderUniformNames[UNIFORM_REACH].c_str()), m_reach);
}

/**
 * @copydoc ShaderDataNode::ShaderUnBind
 */
void ILight::ShaderUnBind(ShaderProgram *s)
{
  glUniform3fv(glGetUniformLocation(s->Program(), m_shaderUniformNames[UNIFORM_POSITION].c_str()), 1,
               (float *)&Vector3());

  glUniform4fv(glGetUniformLocation(s->Program(), m_shaderUniformNames[UNIFORM_COLOUR].c_str()), 1,
               (float *)&Vector4(0.0f, 0.0f, 0.0f, 0.0f));

  glUniform1f(glGetUniformLocation(s->Program(), m_shaderUniformNames[UNIFORM_AMBIENT_INTENSITY].c_str()), 0.0f);

  glUniform1f(glGetUniformLocation(s->Program(), m_shaderUniformNames[UNIFORM_REACH].c_str()), 0.0f);
}
}
