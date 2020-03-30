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

#include <fstream>
#include <iostream>
#include <vector>

DeviceTreeParser::DeviceTreeParser(const std::string &argFilePath)
    : deviceTreeFilePath{argFilePath} {}

DeviceTreeParser::~DeviceTreeParser() {}

std::unique_ptr<RootNode> DeviceTreeParser::ParseFile() {
  // Open the file and determine its size
  std::ifstream inputFile;
  inputFile.open(deviceTreeFilePath);
  if (inputFile.fail()) {
    std::cerr << "Failed to open device tree file: " << deviceTreeFilePath
              << "\n";
    return nullptr;
  }

  inputFile.seekg(0, std::ios_base::end);
  if (inputFile.fail()) {
    std::cerr << "Failed to seek to the end of file: " << deviceTreeFilePath
              << "\n";
    return nullptr;
  }
  const auto fileSize = inputFile.tellg();
  inputFile.seekg(0);
  if (inputFile.fail()) {
    std::cerr << "Failed to seek to the start of file: " << deviceTreeFilePath
              << "\n";
    return nullptr;
  }

  // Read the file into a buffer and close it afterwards
  std::vector<uint8_t> inputBuf(
      static_cast<std::vector<uint8_t>::size_type>(fileSize), 0);
  inputFile.read(reinterpret_cast<char *>(inputBuf.data()), fileSize);
  if (inputFile.gcount() != fileSize) {
    std::cerr << "Failed to read file: " << deviceTreeFilePath << "\n";
    return nullptr;
  }
  inputFile.close();
  if (inputFile.fail()) {
    std::cerr << "Failed to close file: " << deviceTreeFilePath << "\n";
    return nullptr;
  }

  // Create a string and a stream object working on that string
  std::string inputString{reinterpret_cast<char *>(inputBuf.data())};
  std::istringstream inputStream{inputString};

  // Iterate over all the lines of the file
  std::string line;
  std::unique_ptr<RootNode> rootNode;
  while (std::getline(inputStream, line)) {
    if (line.empty()) {
      continue;
    }
    if (line == "/dts-v1/;") {
      continue;
    }
    if (Node::IsNodeStartLine(line)) {
      rootNode = std::make_unique<RootNode>(line, inputStream);
      continue;
    }
    std::cout << line << "\n";
  }

  return rootNode;
}
