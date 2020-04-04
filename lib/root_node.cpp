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

#include "root_node.h"

RootNode::RootNode(const std::string &argLine, std::istringstream &argInStream)
    : Node{argLine, argInStream, nullptr} {}

bool RootNode::Compare(const Item *argOtherItem) const {
  if (dynamic_cast<const RootNode *>(argOtherItem) == nullptr) {
    return false;
  }

  return Node::Compare(argOtherItem);
}

std::string RootNode::GetStringRep() const {
  return "/dts-v1/;\n\n" + Node::GetStringRep();
}

void RootNode::Merge(const Item *argOtherItem, bool argAddFromOther) {
  if (dynamic_cast<const RootNode *>(argOtherItem) == nullptr) {
    throw std::invalid_argument{"Try to merge unrelated class into RootNode"};
  }

  Node::Merge(argOtherItem, argAddFromOther);
}
