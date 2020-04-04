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
#include "root_node.h"

#include <exception>
#include <iostream>
#include <stdexcept>

class EmptyOrInvalSharedPtrItemException : public std::exception {
  const char *what() const noexcept override;
};

const char *EmptyOrInvalSharedPtrItemException::what() const noexcept {
  return "Encountered empty or invalid SharedPtrItem on copy attempt";
}

class UnknownSharedPtrItemMemberTypeException : public std::exception {
  const char *what() const noexcept override;
};

const char *UnknownSharedPtrItemMemberTypeException::what() const noexcept {
  return "Encountered unknown member type in SharedPtrItem";
}

Item::~Item() {}

bool Item::Compare(const Item *argOtherItem) const {
  if (nullptr == argOtherItem) {
    return false;
  }

  if (dynamic_cast<const Item *>(argOtherItem) == nullptr) {
    return false;
  }

  if ((level == argOtherItem->level) && (name == argOtherItem->name) &&
      (type == argOtherItem->type)) {
    return true;
  }

  return false;
}

std::string Item::GetPrependedTabs() const { return std::string(level, '\t'); }

void Item::Merge(const Item *argOtherItem, bool argAddFromOther,
                 bool argPurgeItemsNotInOther) {
  (void)argAddFromOther;
  (void)argPurgeItemsNotInOther;

  // Assure that the to be merged items are equal in terms of level and name
  if ((level != argOtherItem->level) || (name != argOtherItem->name) ||
      (type != argOtherItem->type)) {
    throw std::invalid_argument{"Attempt to merge non-related Item instance"};
  }
}

void Item::Print() const { std::cout << GetStringRep() << "\n"; }

Item::SharedPtrItem
CopySharedPtrItem(const Item::SharedPtrConstItem &argSharedPtrConstItem) {
  const auto itemPtr = dynamic_cast<const Item *>(argSharedPtrConstItem.get());

  if (itemPtr == nullptr) {
    throw EmptyOrInvalSharedPtrItemException{};
  }

  if (dynamic_cast<const Node *>(itemPtr)) {
    return std::make_shared<Node>(*reinterpret_cast<const Node *>(itemPtr));
  }

  if (dynamic_cast<const PropertyEmpty *>(itemPtr)) {
    return std::make_shared<PropertyEmpty>(
        *reinterpret_cast<const PropertyEmpty *>(itemPtr));
  }

  if (dynamic_cast<const PropertyValueString *>(itemPtr)) {
    return std::make_shared<PropertyValueString>(
        *reinterpret_cast<const PropertyValueString *>(itemPtr));
  }

  if (dynamic_cast<const RootNode *>(itemPtr)) {
    return std::make_shared<RootNode>(
        *reinterpret_cast<const RootNode *>(itemPtr));
  }

  throw UnknownSharedPtrItemMemberTypeException{};
}
