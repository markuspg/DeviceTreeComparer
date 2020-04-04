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

#include "item.h"
#include "node.h"

#include <memory>

class Property : public Item {
public:
  static std::shared_ptr<Property> Construct(const std::string &argLine,
                                             const Node *argParentNode);
  virtual ~Property();

  bool Compare(const Item *argOtherItem) const override = 0;
  void Merge(const Item *argOtherItem, bool argAddFromOther) override = 0;

protected:
  Property(const std::string &argName, const Node *argParentNode)
      : Item{argParentNode->GetLevel() + 1, argName, Type::PROPERTY} {}
  Property(const Property &argItem) = default;
  Property &operator=(const Property &argItem) = default;

  std::string GetStringRep() const override;
};

class PropertyValueLess : public Property {
public:
  bool Compare(const Item *argOtherItem) const override;
  void Merge(const Item *argOtherItem, bool argAddFromOther) override;

protected:
  std::string GetStringRep() const override;

private:
  PropertyValueLess(const std::string &argName, const Node *argParentNode)
      : Property(argName, argParentNode) {}

  friend Property;
};

class PropertyValueString : public Property {
public:
  bool Compare(const Item *argOtherItem) const override;
  const std::string &GetValue() const noexcept { return value; }
  void Merge(const Item *argOtherItem, bool argAddFromOther) override;

protected:
  std::string GetStringRep() const override;

private:
  PropertyValueString(const std::string &argName, const Node *argParentNode,
                      const std::string &argValue)
      : Property{argName, argParentNode}, value{argValue} {}

  std::string value;

  friend Property;
};
