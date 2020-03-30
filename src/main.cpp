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

  bool compare = true;
  bool displayHelp = false;
  bool extend = false;
  bool merge_file_2_into_file_1 = false;
  bool purge = false;
  for (auto i = 1; i < argc; ++i) {
    if (std::string{argv[i]} == "-e") {
      extend = true;
    }
    if (std::string{argv[i]} == "-h") {
      displayHelp = true;
      break;
    }
    if (std::string{argv[i]} == "-m") {
      compare = false;
      merge_file_2_into_file_1 = true;
    }
    if (std::string{argv[i]} == "-p") {
      purge = true;
    }
  }

  if (displayHelp) {
    std::cout
        << "DeviceTreeComparer [OPTIONS] FILE_1 FILE_2\n\n"
        << "Without any options this tool compares the two device tree "
           "source files and\nreturns '0' if they are equal or '1' if "
           "they differ.\n\n"
        << "Options:\n"
        << "\t-e: Add entries which are in FILE_2 but not in FILE_1 to "
           "FILE_1 (only\n\t    in combination with \"-m\")\n"
        << "\t-h: Display this help text\n"
        << "\t-m: Overwrite options of FILE_1 found both in FILE_1 and "
           "FILE_2 with\n\t    FILE_2's values and print the result to "
           "stdout\n"
        << "\t-p: Purge entries which are in FILE_1 but not in FILE_2 from "
           "FILE_1\n\t    (only in combination with \"-m\")\n";

    return 0;
  }

  if ((extend && !merge_file_2_into_file_1) ||
      (purge && !merge_file_2_into_file_1)) {
    std::cerr << "Invalid combination of commandline options\n";
    return 7;
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

  if (compare) {
    if (rootNode1->Compare(rootNode2.get()) == true) {
      return 0;
    }
    return 1;
  } else if (merge_file_2_into_file_1) {
    rootNode1->Merge(rootNode2.get());
    rootNode1->Print();
    return 0;
  }

  return 6;
}
