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

#include "node.h"
#include "property.h"
#include "string_utils.h"

#include <algorithm>
#include <iostream>

Node::Node(const std::string &argLine, std::istringstream &argInStream,
           const Node *argParentNode)
    : Item{argParentNode
               ? static_cast<uint_fast16_t>(argParentNode->GetLevel() + 1)
               : static_cast<uint_fast16_t>(0u),
           ExtractNodeName(argLine)} {
  std::string line;
  while (std::getline(argInStream, line)) {
    if (RemoveLeadingWhitespace(line).empty()) {
      continue;
    }
    if (Node::IsNodeStartLine(line)) {
      items.emplace_back(new Node{line, argInStream, this});
      continue;
    }
    if (Node::IsNodeEndLine(line)) {
      break;
    }
    items.emplace_back(Property::Construct(line, this));
  }
}

bool Node::Compare(const Item *argOtherItem) {
  if (Item::Compare(argOtherItem) == false) {
    return false;
  }

  const auto otherNode = dynamic_cast<const Node *>(argOtherItem);
  if (nullptr == otherNode) {
    return false;
  }

  // Check that all items of this node have equivalents in the other node
  for (const auto &item : items) {
    if (std::find_if(std::begin(otherNode->items), std::end(otherNode->items),
                     [&item](const std::shared_ptr<Item> &argSharedPtrItem) {
                       return item->Compare(argSharedPtrItem.get());
                     }) == std::end(otherNode->items)) {
      return false;
    }
  }

  // Check that all items of the other node have equivalents in this node
  for (const auto &item : otherNode->items) {
    if (std::find_if(std::begin(items), std::end(items),
                     [&item](const std::shared_ptr<Item> &argSharedPtrItem) {
                       return item->Compare(argSharedPtrItem.get());
                     }) == std::end(items)) {
      return false;
    }
  }

  return true;
}

std::string Node::GetStringRep() const {
  std::string resultStr;
  resultStr.append(GetPrependedTabs() + name + " {\n");
  for (const auto &item : items) {
    resultStr.append(item->GetStringRep() + "\n");
  }
  resultStr.append(GetPrependedTabs() + "};");
  return resultStr;
}

bool Node::IsNodeEndLine(const std::string &argLine) {
  return argLine.find("};") != std::string::npos;
}

bool Node::IsNodeStartLine(const std::string &argLine) {
  return argLine.find('{') != std::string::npos;
}

void Node::Merge(const Item *argOtherItem) {
  if (dynamic_cast<const Node *>(argOtherItem) == nullptr) {
    throw std::invalid_argument{"Try to merge unrelated class into Node"};
  }

  Item::Merge(argOtherItem);

  // TODO(markuspg) Add code for recursively merging all items
}

void CompareNodes(const Node &argNodeA, const Node &argNodeB) {
  bool found = false;

  // Find items in node A which are missing in node B
  for (const auto &item : argNodeA.items) {
    found = false;
    for (const auto &otherItem : argNodeB.items) {
      if (item->GetName() == otherItem->GetName()) {
        found = true;
        break;
      }
    }
    if (false == found) {
      std::cout << "FAILED TO FIND IN B: " << item->GetName()
                << "---------------\n";
    }
  }

  // Find items in node B which are missing in node A
  for (const auto &item : argNodeB.items) {
    found = false;
    for (const auto &otherItem : argNodeA.items) {
      if (item->GetName() == otherItem->GetName()) {
        found = true;
        break;
      }
    }
    if (false == found) {
      std::cout << "FAILED TO FIND IN A:\n";
      item->Print();
      std::cout << "---------------\n";
    }
  }
}
