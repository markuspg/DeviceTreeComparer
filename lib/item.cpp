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

#include <iostream>
#include <stdexcept>

Item::~Item() {}

bool Item::Compare(const Item *argOtherItem) const {
  if (nullptr == argOtherItem) {
    return false;
  }

  if (dynamic_cast<const Item *>(argOtherItem) == nullptr) {
    return false;
  }

  if ((level == argOtherItem->level) && (name == argOtherItem->name)) {
    return true;
  }

  return false;
}

std::string Item::GetPrependedTabs() const { return std::string(level, '\t'); }

void Item::Merge(const Item *argOtherItem) {
  // Assure that the to be merged items are equal in terms of level and name
  if ((level != argOtherItem->level) || (name != argOtherItem->name)) {
    throw std::invalid_argument{"Attempt to merge non-related Item instance"};
  }
}

void Item::Print() const { std::cout << GetStringRep() << "\n"; }
