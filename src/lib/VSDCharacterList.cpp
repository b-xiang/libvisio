/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* libvisio
 * Version: MPL 1.1 / GPLv2+ / LGPLv2+
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License or as specified alternatively below. You may obtain a copy of
 * the License at http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * Major Contributor(s):
 * Copyright (C) 2011 Fridrich Strba <fridrich.strba@bluewin.ch>
 * Copyright (C) 2011 Eilidh McAdam <tibbylickle@gmail.com>
 *
 *
 * All Rights Reserved.
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPLv2+"), or
 * the GNU Lesser General Public License Version 2 or later (the "LGPLv2+"),
 * in which case the provisions of the GPLv2+ or the LGPLv2+ are applicable
 * instead of those above.
 */

#include "VSDCollector.h"
#include "VSDCharacterList.h"

namespace libvisio
{

class VSDCharacterListElement
{
public:
  VSDCharacterListElement(unsigned id, unsigned level) : m_id(id), m_level(level) {}
  virtual ~VSDCharacterListElement() {}
  virtual void handle(VSDCollector *collector) const = 0;
  virtual VSDCharacterListElement *clone() = 0;
  virtual unsigned getCharCount() const = 0;
  virtual void setCharCount(unsigned charCount) = 0;
protected:
  unsigned m_id, m_level;
};

class VSDCharIX : public VSDCharacterListElement
{
public:
  VSDCharIX(unsigned id, unsigned level, const boost::optional<unsigned> &charCount, const boost::optional<VSDName> &font,
            const boost::optional<Colour> &fontColour, const boost::optional<double> &fontSize, const boost::optional<bool> &bold,
            const boost::optional<bool> &italic, const boost::optional<bool> &underline, const boost::optional<bool> &doubleunderline,
            const boost::optional<bool> &strikeout, const boost::optional<bool> &doublestrikeout, const boost::optional<bool> &allcaps,
            const boost::optional<bool> &initcaps, const boost::optional<bool> &smallcaps, const boost::optional<bool> &superscript,
            const boost::optional<bool> &subscript) :
    VSDCharacterListElement(id, level), m_charCount(FROM_OPTIONAL(charCount, 0)), m_font(FROM_OPTIONAL(font, VSDName())),
    m_fontColour(FROM_OPTIONAL(fontColour, Colour())), m_fontSize(FROM_OPTIONAL(fontSize, 0.1668)), m_bold(FROM_OPTIONAL(bold, false)),
    m_italic(FROM_OPTIONAL(italic, false)), m_underline(FROM_OPTIONAL(underline, false)), m_doubleunderline(FROM_OPTIONAL(doubleunderline, false)),
    m_strikeout(FROM_OPTIONAL(strikeout, false)), m_doublestrikeout(FROM_OPTIONAL(doublestrikeout, false)),
    m_allcaps(FROM_OPTIONAL(allcaps, false)), m_initcaps(FROM_OPTIONAL(initcaps, false)), m_smallcaps(FROM_OPTIONAL(smallcaps, false)),
    m_superscript(FROM_OPTIONAL(superscript, false)), m_subscript(FROM_OPTIONAL(subscript, false)) {}
  ~VSDCharIX() {}
  void handle(VSDCollector *collector) const;
  VSDCharacterListElement *clone();
  unsigned getCharCount() const
  {
    return m_charCount;
  }
  void setCharCount(unsigned charCount)
  {
    m_charCount = charCount;
  }

  unsigned m_charCount;
  VSDName m_font;
  Colour m_fontColour;
  double m_fontSize;
  bool m_bold, m_italic, m_underline, m_doubleunderline, m_strikeout, m_doublestrikeout;
  bool m_allcaps, m_initcaps, m_smallcaps, m_superscript, m_subscript;
};
} // namespace libvisio


void libvisio::VSDCharIX::handle(VSDCollector *collector) const
{
  collector->collectCharIX(m_id, m_level, m_charCount, m_font, m_fontColour, m_fontSize, m_bold, m_italic, m_underline,
                           m_doubleunderline, m_strikeout, m_doublestrikeout, m_allcaps, m_initcaps, m_smallcaps,
                           m_superscript, m_subscript);
}

libvisio::VSDCharacterListElement *libvisio::VSDCharIX::clone()
{
  return new VSDCharIX(m_id, m_level, m_charCount, m_font, m_fontColour, m_fontSize, m_bold, m_italic, m_underline,
                       m_doubleunderline, m_strikeout, m_doublestrikeout, m_allcaps, m_initcaps, m_smallcaps,
                       m_superscript, m_subscript);
}


libvisio::VSDCharacterList::VSDCharacterList() :
  m_elements(),
  m_elementsOrder()
{
}

libvisio::VSDCharacterList::VSDCharacterList(const libvisio::VSDCharacterList &charList) :
  m_elements(),
  m_elementsOrder(charList.m_elementsOrder)
{
  std::map<unsigned, VSDCharacterListElement *>::const_iterator iter = charList.m_elements.begin();
  for (; iter != charList.m_elements.end(); ++iter)
    m_elements[iter->first] = iter->second->clone();
}

libvisio::VSDCharacterList &libvisio::VSDCharacterList::operator=(const libvisio::VSDCharacterList &charList)
{
  clear();
  std::map<unsigned, VSDCharacterListElement *>::const_iterator iter = charList.m_elements.begin();
  for (; iter != charList.m_elements.end(); ++iter)
    m_elements[iter->first] = iter->second->clone();
  m_elementsOrder = charList.m_elementsOrder;
  return *this;
}

libvisio::VSDCharacterList::~VSDCharacterList()
{
  clear();
}

void libvisio::VSDCharacterList::addCharIX(unsigned id, unsigned level, const boost::optional<unsigned> &charCount,
    const boost::optional<VSDName> &font, const boost::optional<Colour> &fontColour, const boost::optional<double> &fontSize,
    const boost::optional<bool> &bold, const boost::optional<bool> &italic, const boost::optional<bool> &underline,
    const boost::optional<bool> &doubleunderline, const boost::optional<bool> &strikeout, const boost::optional<bool> &doublestrikeout,
    const boost::optional<bool> &allcaps, const boost::optional<bool> &initcaps, const boost::optional<bool> &smallcaps,
    const boost::optional<bool> &superscript, const boost::optional<bool> &subscript)
{
  VSDCharIX *tmpElement = dynamic_cast<VSDCharIX *>(m_elements[id]);
  if (!tmpElement)
  {
    std::map<unsigned, VSDCharacterListElement *>::iterator iter = m_elements.find(id);
    if (m_elements.end() != iter)
    {
      if (iter->second)
        delete iter->second;
      m_elements.erase(iter);
    }

    m_elements[id] = new VSDCharIX(id, level, charCount, font, fontColour, fontSize, bold, italic, underline, doubleunderline,
                                   strikeout, doublestrikeout, allcaps, initcaps, smallcaps, superscript, subscript);
  }
  else
  {
    ASSIGN_OPTIONAL(charCount, tmpElement->m_charCount);
    ASSIGN_OPTIONAL(font, tmpElement->m_font);
    ASSIGN_OPTIONAL(fontColour, tmpElement->m_fontColour);
    ASSIGN_OPTIONAL(fontSize, tmpElement->m_fontSize);
    ASSIGN_OPTIONAL(bold, tmpElement->m_bold);
    ASSIGN_OPTIONAL(italic, tmpElement->m_italic);
    ASSIGN_OPTIONAL(underline, tmpElement->m_underline);
    ASSIGN_OPTIONAL(doubleunderline, tmpElement->m_doubleunderline);
    ASSIGN_OPTIONAL(strikeout, tmpElement->m_strikeout);
    ASSIGN_OPTIONAL(doublestrikeout, tmpElement->m_doublestrikeout);
    ASSIGN_OPTIONAL(allcaps, tmpElement->m_allcaps);
    ASSIGN_OPTIONAL(initcaps, tmpElement->m_initcaps);
    ASSIGN_OPTIONAL(smallcaps, tmpElement->m_smallcaps);
    ASSIGN_OPTIONAL(superscript, tmpElement->m_superscript);
    ASSIGN_OPTIONAL(subscript, tmpElement->m_subscript);
  }
}

unsigned libvisio::VSDCharacterList::getCharCount(unsigned id) const
{
  std::map<unsigned, VSDCharacterListElement *>::const_iterator iter = m_elements.find(id);
  if (iter != m_elements.end() && iter->second)
    return iter->second->getCharCount();
  else
    return MINUS_ONE;
}

void libvisio::VSDCharacterList::setCharCount(unsigned id, unsigned charCount)
{
  std::map<unsigned, VSDCharacterListElement *>::iterator iter = m_elements.find(id);
  if (iter !=  m_elements.end() && iter->second)
    iter->second->setCharCount(charCount);
}

void libvisio::VSDCharacterList::setElementsOrder(const std::vector<unsigned> &elementsOrder)
{
  m_elementsOrder.clear();
  for (unsigned i = 0; i<elementsOrder.size(); i++)
    m_elementsOrder.push_back(elementsOrder[i]);
}

void libvisio::VSDCharacterList::handle(VSDCollector *collector) const
{
  if (empty())
    return;
  std::map<unsigned, VSDCharacterListElement *>::const_iterator iter;
  if (!m_elementsOrder.empty())
  {
    for (unsigned i = 0; i < m_elementsOrder.size(); i++)
    {
      iter = m_elements.find(m_elementsOrder[i]);
      if (iter != m_elements.end() && (0 == i || iter->second->getCharCount()))
        iter->second->handle(collector);
    }
  }
  else
  {
    for (iter = m_elements.begin(); iter != m_elements.end(); ++iter)
      if (m_elements.begin() == iter || iter->second->getCharCount())
        iter->second->handle(collector);
  }
}

void libvisio::VSDCharacterList::clear()
{
  for (std::map<unsigned, VSDCharacterListElement *>::iterator iter = m_elements.begin(); iter != m_elements.end(); ++iter)
    delete iter->second;
  m_elements.clear();
  m_elementsOrder.clear();
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
