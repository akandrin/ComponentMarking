#include <iostream>
#include <vector>
#include <cassert>

using Pixel = int;
using Image = std::vector<std::vector<Pixel>>; // todo: поменять на Matrix


Image negate(const Image& source) {
  auto image = source;
  for (auto&& row : image) {
    for (auto && value : row) {
      value = -value;
    }
  }
}

std::vector<std::pair<size_t, size_t>> GetNeighbors(const Image& image,
                                                    size_t i, size_t j) {
  assert(!image.empty());
  // получение максимум 4-ех соседей (4-связность)
  std::vector<std::pair<size_t, size_t>> result;
  if (image.empty()) return result;

  // верхний сосед
  if (i > 0) {
    result.emplace_back(i - 1, j);
  }

  // левый сосед
  if (j > 0) {
    result.emplace_back(i, j - 1);
  }

  // правый сосед
  if (j + 1 < image[0].size()) {
    result.emplace_back(i, j + 1);
  }

  // нижний сосед
  if (i + 1 < image.size()) {
    result.emplace_back(i + 1, j);
  }

  return result;
}

void search(Image& image, Pixel label, size_t i, size_t j) {
  image[i][j] = label;
  auto neighbors = GetNeighbors(image, i, j);
  for (auto && point : neighbors) {
    size_t i = point.first;
    size_t j = point.second;

    if (image[i][j] == -1) {
      search(image, label, i, j);
    }
  }
}

void find_components(Image& image, Pixel label) {
  for (size_t i = 0; i < image.size(); ++i) {
    for (size_t j = 0; j < image[i].size(); ++j) {
      if (image[i][j] == -1) {
        ++label;
        search(image, label, i, j);
      }
    }
  }
}

void print(const Image& image) {
  for (auto&& row : image) {
    for (auto&& value : row) {
      std::cout << value << ' ';
    }
    std::cout << '\n';
  }
}

void recursive_connected_components(const Image& source, Image& result) {
  result.clear();
  result = negate(source);
  Pixel label = 0;
  find_components(result, label);
  print(result);
}




int main()
{
    std::cout << "Hello World!\n";
}
