/** @file */

#include "TextNode.h"

#include "TextMesh.h"

namespace GraphicsCoursework
{
/**
 * @brief Creates a new text node.
 * @param name Node name
 * @param font Text rendering font
 * @param len Maximum length of displayed text
 */
TextNode::TextNode(const std::string &name, Font *font, size_t len)
    : MeshNode(name, new TextMesh(len, font))
{
}

/**
 * @brief Creates a new text node.
 * @param name Node name
 * @param font Text rendering font
 * @param text Initial text to be displayed
 */
TextNode::TextNode(const std::string &name, Font *font, const std::string &text)
    : MeshNode(name, new TextMesh(text.size(), font))
{
  SetText(text);
}

TextNode::~TextNode()
{
}

/**
 * @brief Sets the text displayed by this node.
 * @param text String to display
 */
void TextNode::SetText(const std::string &text)
{
  ((TextMesh *)m_mesh)->SetText(text);
}
}
