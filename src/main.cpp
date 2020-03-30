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
#include "root_node.h"

#include <iostream>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "At least two arguments are required - the two files to be "
                 "compared (a help text can be accessed with \"-h\")\n";
    return 2;
  }

  bool displayHelp = false;
  for (auto i = 1; i < argc; ++i) {
    if (std::string{argv[i]} == "-h") {
      displayHelp = true;
    }
  }

  if (displayHelp) {
    std::cout << "DeviceTreeComparer [OPTIONS] FILE_1 FILE_2\n\n"
              << "Without any options this tool compares the two device tree "
                 "source files and\nreturns '0' if they are equal or '1' if "
                 "they differ.\n\n"
              << "Options:\n"
              << "\t-h: Display this help text\n";
    return 0;
  }

  if (argc < 3) {
    std::cerr << "At least two positional arguments are required - the two "
                 "files to be compared\n";
    return 3;
  }

  const std::string file1{argv[argc - 2]};
  const std::string file2{argv[argc - 1]};

  DeviceTreeParser parserFile1{file1};
  const auto rootNode1 = parserFile1.ParseFile();
  if (!rootNode1) {
    std::cerr << "Failed to parse file: " << file1 << "\n";
    return 4;
  }
  DeviceTreeParser parserFile2{file2};
  const auto rootNode2 = parserFile2.ParseFile();
  if (!rootNode2) {
    std::cerr << "Failed to parse file: " << file2 << "\n";
    return 5;
  }
  CompareNodes(*rootNode1.get(), *rootNode2.get());

  return 0;
}
