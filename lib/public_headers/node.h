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

#include "item.h"

#include <sstream>
#include <vector>

class Node : public Item {
public:
  Node(const std::string &argLine, std::istringstream &argInStream,
       const Node *argParentNode);
  Node(const Node &argNode);
  Node &operator=(const Node &argNode);

  bool Compare(const Item *argOtherItem) const override;
  static bool IsNodeEndLine(const std::string &argLine);
  static bool IsNodeStartLine(const std::string &argLine);
  void Merge(const Item *argOtherItem) override;

protected:
  std::string GetStringRep() const override;

private:
  std::vector<SharedPtrItem> items;
};

#endif // NODE_H
