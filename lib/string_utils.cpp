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

#include "string_utils.h"

#include <stdexcept>

constexpr char SPACE_CHAR = 0x20;
constexpr char TAB_CHAR = 0x09;

std::string ExtractNodeName(const std::string &argInputStr) {
  if (argInputStr.find('{') == std::string::npos) {
    throw std::invalid_argument{"Node line does not contain '{'"};
  }
  const auto withoutLeadingWhitespaceStr{RemoveLeadingWhitespace(argInputStr)};
  const auto nextSpaceIdx = withoutLeadingWhitespaceStr.find(0x20);
  return withoutLeadingWhitespaceStr.substr(0, nextSpaceIdx);
}

std::string RemoveLeadingWhitespace(const std::string &argInputStr) {
  std::string::size_type dataStartIdx = 0u;
  for (auto i = 0u; i < argInputStr.size(); ++i) {
    if ((argInputStr[i] != SPACE_CHAR) && (argInputStr[i] != TAB_CHAR)) {
      dataStartIdx = i;
      break;
    }
  }
  return argInputStr.substr(dataStartIdx);
}
