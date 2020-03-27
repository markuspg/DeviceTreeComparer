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

#include "device_tree_parser.h"

#include <iostream>

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cerr << "Exactly two arguments are required - the two files to be "
                 "compared\n";
    return 1;
  }

  const std::string file1{argv[1]};
  const std::string file2{argv[2]};

  DeviceTreeParser parserFile1{file1};
  if (parserFile1.ParseFile() == false) {
    std::cerr << "Failed to parse file: " << file1 << "\n";
    return 2;
  }
  DeviceTreeParser parserFile2{file2};
  if (parserFile2.ParseFile() == false) {
    std::cerr << "Failed to parse file: " << file2 << "\n";
    return 3;
  }

  return 0;
}
