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

#ifndef NODE_H
#define NODE_H

#include <memory>
#include <sstream>
#include <vector>

class Property;

class Node {
public:
  Node(const std::string &argLine, std::istringstream &argInStream, const Node * argParentNode = nullptr);
  
  uint_fast16_t GetLevel() const noexcept { return level; }
  static bool IsNodeEndLine(const std::string &argLine);
  static bool IsNodeStartLine(const std::string &argLine);
  
private:
  const uint_fast16_t level = 0;
  const std::string nodeName;
  std::vector<std::shared_ptr<Property>> properties;
  std::vector<Node> subNodes;
};

#endif // NODE_H
