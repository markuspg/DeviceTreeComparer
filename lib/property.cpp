/*
 * Copyright (c) 2020 Markus Prasser
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "property.h"
#include "node.h"
#include "string_utils.h"

Property::~Property() {}

bool Property::Compare(const Item *argOtherItem) const {
  if (false == Item::Compare(argOtherItem)) {
    return false;
  }

  const auto otherProperty = dynamic_cast<const Property *>(argOtherItem);
  if (nullptr != otherProperty) {
    return true;
  }

  return false;
}

std::shared_ptr<Property> Property::Construct(const std::string &argLine,
                                              const Node *argParentNode) {
  const auto propertyText{
      RemoveTrailingSemicolon(RemoveLeadingWhitespace(argLine))};

  const auto dividerPos = propertyText.find(" = ");
  if (dividerPos == std::string::npos) {
    return std::shared_ptr<Property>(
        new PropertyValueLess{propertyText, argParentNode});
  }

  return std::shared_ptr<Property>(new PropertyValueString{
      propertyText.substr(0, dividerPos), argParentNode,
      propertyText.substr(dividerPos + 3, std::string::npos)});
}

std::string Property::GetStringRep() const { return GetPrependedTabs() + name; }

void Property::Merge(const Item *argOtherItem) {
  if (dynamic_cast<const Property *>(argOtherItem) == nullptr) {
    throw std::invalid_argument{"Try to merge unrelated class into Property"};
  }

  Item::Merge(argOtherItem);
}

bool PropertyValueLess::Compare(const Item *argOtherItem) const {
  if (false == Property::Compare(argOtherItem)) {
    return false;
  }

  const auto otherProperty =
      dynamic_cast<const PropertyValueLess *>(argOtherItem);
  if (nullptr != otherProperty) {
    return true;
  }

  return false;
}

std::string PropertyValueLess::GetStringRep() const {
  return Property::GetStringRep() + ";";
}

void PropertyValueLess::Merge(const Item *argOtherItem) {
  if (dynamic_cast<const PropertyValueLess *>(argOtherItem) == nullptr) {
    throw std::invalid_argument{
        "Try to merge unrelated class into PropertyValueLess"};
  }

  Property::Merge(argOtherItem);
}

bool PropertyValueString::Compare(const Item *argOtherItem) const {
  if (false == Property::Compare(argOtherItem)) {
    return false;
  }

  const auto otherProperty =
      dynamic_cast<const PropertyValueString *>(argOtherItem);
  if (nullptr == otherProperty) {
    return false;
  }

  if (value == otherProperty->value) {
    return true;
  }

  return false;
}

std::string PropertyValueString::GetStringRep() const {
  return Property::GetStringRep() + " = " + value + ";";
}

void PropertyValueString::Merge(const Item *argOtherItem) {
  const auto otherProperty =
      dynamic_cast<const PropertyValueString *>(argOtherItem);
  if (otherProperty == nullptr) {
    throw std::invalid_argument{
        "Try to merge unrelated class into PropertyValueString"};
  }

  Property::Merge(argOtherItem);

  value = otherProperty->value;
}
