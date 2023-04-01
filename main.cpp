#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>

using Pixel = int;
using Image = std::vector<std::vector<Pixel>>; // todo: поменять на Matrix


static constexpr size_t maxLabelCount = 10000;


/*Image negate(const Image& source) {
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
}*/


typedef std::vector<Pixel> Parent;

// поиск родительской метки для заданной метки множества
Pixel FindParent(Pixel label, const Parent& parent) {
  while (parent[label] != 0) {
    label = parent[label];
  }
  return label;
}

// Объединение двух множеств
void Union(Pixel x, Pixel y, Parent& parent) {
  x = FindParent(x, parent);
  y = FindParent(y, parent);
  if (x != y) {
    parent.at(y) = x;
  }
}


std::vector<std::pair<size_t, size_t>> GetPriorNeighbors(const Image& image,
                                                    size_t i, size_t j) {
  assert(!image.empty());
  // получение 2-ех соседей (северного и западного при 4-связности)
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

  return result;
}


void Initialize(Pixel& label, Parent& parent, size_t maxLabelCount) {
  label = 1;
  parent = Parent(maxLabelCount, 0);
}

std::vector<Pixel> GetLabels(const Image& image, const std::vector<std::pair<size_t, size_t>>& priorNeighbors)
{
    std::vector<Pixel> result;
    for (auto&& point : priorNeighbors)
    {
        size_t i = point.first;
        size_t j = point.second;
        if (image[i][j] != 0)
            result.emplace_back(image[i][j]);
    }
    return result;
}

void Classical_with_union_find(const Image& source, Image& result) {
  result.clear();

  Parent parent;
  Pixel label;
  Initialize(label, parent, maxLabelCount);
  Pixel m = 0;
  for (int i = 0; i < source.size(); ++i) {
    result.emplace_back(source[i].size(), 0);

    for (int j = 0; j < source[i].size(); ++j) {
      if (source[i][j] == 1) {
        auto priorNeighbors = GetPriorNeighbors(source, i, j);
        auto labels = GetLabels(result, priorNeighbors);
        if (labels.empty()) {
          m = label;
          label++;
        } else {
          m = *std::min_element(labels.begin(), labels.end());
        }
        result[i][j]= m;
        for (auto&& label : labels)
        {
            if (label != m)
            {
                Union(m, label, parent);
            }
        }
      }
    }
  }

  for (int i = 0; i < source.size(); ++i) {
    for (int j = 0; j < source[i].size(); ++j) {
      if (source[i][j] == 1) {
        result[i][j] = FindParent(result[i][j], parent);
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


// Метод исправляет значения элементов в массиве arr таким образом,
// чтобы множество значений элементов массива не имело "пропусков".
// Например, если множество значений элементов массива - {1, 3, 7, 10},
// То оно превратится в {1, 2, 3, 4}
// Соответственно, массив arr, равный {1, 3, 10, 7, 1, 3, 10, 7, 1} превратится в {1, 2, 4, 3, 1, 2, 4, 3, 1}
void FixNumeration(Pixel* arr, size_t size) {
  std::vector<bool> isLabelExist(maxLabelCount, false);
  for (size_t i = 0; i < size; ++i) {
    assert(arr[i] <
           isLabelExist.size());  // предполагается, что в массиве arr сейчас
                                  // возможны от 0 до maxLabelCount
    isLabelExist[arr[i]] = true;
  }

  // Получение значения максимальной метки
  Pixel maxLabel = 0;
  for (size_t i = 0; i < size; ++i) {
    if (arr[i] > maxLabel) {
      maxLabel = arr[i];
    }
  }

  // Получение списка замен
  struct Replace {
    Pixel before;
    Pixel after;

    Replace(Pixel before, Pixel after) : before(before), after(after) {}
  };
  std::vector<Replace> replaceArr;
  
  Pixel currentNumber = 1; // переменная для нумерации
  for (Pixel currentLabel = 1; currentLabel <= maxLabel;) {
    // currentLabel - значение текущей метки
    // если такой метки не существует, то идем до ближайшей существующей метки
    // В этом цикле мы не можем выйти за пределы допустимого диапазона [1; maxLabel], потому что метка
    // maxLabel обязательно существует.
    while (!isLabelExist[currentLabel]) ++currentLabel;

    if (currentLabel != currentNumber) {
      // если текущая метка не равна текущему номеру, то добавляем "замену" в список замен
      replaceArr.emplace_back(currentLabel, currentNumber);
    }

    ++currentLabel;
    ++currentNumber;
  }

  for (auto replace : replaceArr) {
    for (size_t i = 0; i < size; ++i) {
      if (arr[i] == replace.before) arr[i] = replace.after;
    }
  }

}


int main() {

  int arr[] = {1, 3, 10, 7, 1, 3, 10, 7, 1};
  FixNumeration(arr, sizeof(arr) / sizeof(int));


  /*Image source = {
      {1, 0, 1, 0, 1},
      {1, 0, 1, 1, 0}, 
      {1, 1, 1, 0, 1}, 
      {1, 0, 0, 0, 1},
      {1, 1, 1, 1, 1}
  };*/

  Image source = {
    {1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1}
};

  auto result = source;
  Classical_with_union_find(source, result);
  print(result);
}
