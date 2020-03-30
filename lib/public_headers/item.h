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

#ifndef ITEM_H
#define ITEM_H

#include <string>

class Item {
public:
  virtual ~Item();

  virtual bool Compare(const Item *argOtherItem) const = 0;
  uint_fast16_t GetLevel() const noexcept { return level; }
  const std::string &GetName() const noexcept { return name; }
  virtual std::string GetStringRep() const = 0;
  virtual void Merge(const Item *argOtherItem) = 0;
  void Print() const;

protected:
  Item(uint_fast16_t argLevel, const std::string &argName)
      : level{argLevel}, name{argName} {}

  std::string GetPrependedTabs() const;

  const uint_fast16_t level = 0;
  const std::string name;
};

#endif // ITEM_H
