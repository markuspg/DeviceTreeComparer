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

#include <regex>

class InvalidPropertyNameException : public std::exception {
  const char *what() const noexcept override;
};

const char *InvalidPropertyNameException::what() const noexcept {
  return "Encountered invalid property name on device tree parsing";
}

static const std::regex propertyNameRegex{"^\\t+([0-9a-zA-Z,._+?#-]+)( = |;)"};
constexpr auto VALID_PROPERTY_NAME_CHARS =
    "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ,._+?#-";

Property::Property(const std::string &argName, const Node *argParentNode)
    : Item{argParentNode->GetLevel() + 1, VerifyPropertyName(argName),
           argParentNode, Type::PROPERTY} {}

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
  std::smatch searchMatch;
  if (std::regex_search(argLine, searchMatch, propertyNameRegex) == false) {
    throw InvalidPropertyNameException{};
  }
  const auto propertyText{searchMatch.str(1)};

  const auto dividerPos = propertyText.find(" = ");
  if (dividerPos == std::string::npos) {
    return std::shared_ptr<Property>(
        new PropertyEmpty{propertyText, argParentNode});
  }

  return std::shared_ptr<Property>(new PropertyValueString{
      propertyText.substr(0, dividerPos), argParentNode,
      propertyText.substr(dividerPos + 3, std::string::npos)});
}

std::string Property::GetStringRep() const { return GetPrependedTabs() + name; }

void Property::Merge(const Item *argOtherItem, bool argAddFromOther,
                     bool argPurgeItemsNotInOther) {
  if (dynamic_cast<const Property *>(argOtherItem) == nullptr) {
    throw std::invalid_argument{"Try to merge unrelated class into Property"};
  }

  Item::Merge(argOtherItem, argAddFromOther, argPurgeItemsNotInOther);
}

const std::string &
Property::VerifyPropertyName(const std::string &argPropName) {
  if ((argPropName.size() < 1) || (argPropName.size() > 31)) {
    throw InvalidPropertyNameException{};
  }

  if (argPropName.find_first_not_of(VALID_PROPERTY_NAME_CHARS) !=
      std::string::npos) {
    throw InvalidPropertyNameException{};
  }

  return argPropName;
}

bool PropertyEmpty::Compare(const Item *argOtherItem) const {
  if (false == Property::Compare(argOtherItem)) {
    return false;
  }

  const auto otherProperty = dynamic_cast<const PropertyEmpty *>(argOtherItem);
  if (nullptr != otherProperty) {
    return true;
  }

  return false;
}

std::string PropertyEmpty::GetStringRep() const {
  return Property::GetStringRep() + ";";
}

void PropertyEmpty::Merge(const Item *argOtherItem, bool argAddFromOther,
                          bool argPurgeItemsNotInOther) {
  if (dynamic_cast<const PropertyEmpty *>(argOtherItem) == nullptr) {
    throw std::invalid_argument{
        "Try to merge unrelated class into PropertyValueLess"};
  }

  Property::Merge(argOtherItem, argAddFromOther, argPurgeItemsNotInOther);
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

void PropertyValueString::Merge(const Item *argOtherItem, bool argAddFromOther,
                                bool argPurgeItemsNotInOther) {
  const auto otherProperty =
      dynamic_cast<const PropertyValueString *>(argOtherItem);
  if (otherProperty == nullptr) {
    throw std::invalid_argument{
        "Try to merge unrelated class into PropertyValueString"};
  }

  Property::Merge(argOtherItem, argAddFromOther, argPurgeItemsNotInOther);

  value = otherProperty->value;
}
