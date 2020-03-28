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

#include <iostream>

Node::Node(const std::string &argLine, std::istringstream &argInStream,
           const Node *argParentNode)
    : Item{ExtractNodeName(argLine)},
      level{argParentNode
                ? static_cast<uint_fast16_t>(argParentNode->GetLevel() + 1)
                : static_cast<uint_fast16_t>(0u)} {
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
    items.emplace_back(Property::Construct(line));
  }
}

bool Node::IsNodeEndLine(const std::string &argLine) {
  return argLine.find("};") != std::string::npos;
}

bool Node::IsNodeStartLine(const std::string &argLine) {
  return argLine.find('{') != std::string::npos;
}

void CompareNodes(const Node &argNodeA, const Node &argNodeB) {
  bool found = false;
  for (const auto &item : argNodeA.items) {
    for (const auto &otherItem : argNodeB.items) {
      if (item->GetName() == otherItem->GetName()) {
        found = true;
        std::cout << item->GetName() << " == " << otherItem->GetName() << "\n";
        break;
      }
    }
    if (false == found) {
      std::cout << "FAILED TO FIND " << item->GetName() << "---------------\n";
    }
  }
}
