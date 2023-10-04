#include "3DViewerV2.h"

namespace s21
{

  Model::Model() : vertexData(0), facetData(0)
  {

  }
  Model::~Model()
  {
    // Определение деструктора
  }
   /** \brief
 *Главная функция парсинга
 */
  bool Model::parseObjFile(const std::string &filePath)
  {
    ErrorCode error = ErrorCode::OK;
    facetData.clear();
    vertexData.clear();
    std::ifstream file(filePath);
    if (!file)
    {
      error = ErrorCode::FILE_NOT_FOUND;
    }
    if (error == ErrorCode::OK)
    {
      error = static_cast<ErrorCode>(pre_parser(file));
    }
    if (error == ErrorCode::OK)
    {
      error = static_cast<ErrorCode>(get_vertexes(file));
    }
    if (error == ErrorCode::OK)
    {
      error = static_cast<ErrorCode>(get_facets(file));
    }
    file.close();
    return error == ErrorCode::OK;
  }

  int Model::pre_parser(std::ifstream &file)
  {
    int error = static_cast<int>(ErrorCode::OK);
    std::string temp_string;
    int space_count = 0;
    while (std::getline(file, temp_string))
    {
      if (temp_string[0] == 'v' && temp_string[1] == ' ')
      {
        vertexData.push_back(0.0);
        vertexData.push_back(0.0);
        vertexData.push_back(0.0);
      }
      if (temp_string[0] == 'f' && temp_string[1] == ' ')
      {
        facetData.push_back(0);
        count_number_in_string(temp_string, space_count);
      }
    }
    facetData.resize(space_count * 2);
    return error;
  }
   /** \brief
 *Функция вертексов
 */
  int Model::get_vertexes(std::ifstream &file)
  {
    int error = static_cast<int>(ErrorCode::OK);
    file.clear();
    file.seekg(0, std::ios::beg);
    std::string temp_string;
    char seps[] = " ";
    std::vector<double> vertexDataTemp(3, 0);
    vertexDataTemp.reserve(vertexData.size() + 3);
    size_t arg_v_index = 3;
    while (std::getline(file, temp_string))
    {
      if (temp_string[0] == 'v' && temp_string[1] == ' ')
      {
        char temp_str[temp_string.size() + 1]; // Add +1 for null terminator
        strcpy(temp_str, temp_string.c_str());
        char *token = strtok(temp_str, seps);
        while (token != nullptr)
        {
          token = strtok(nullptr, seps);
          if (token == nullptr || *token == '\n')
            break;
          vertexDataTemp.push_back(std::atof(token));
          arg_v_index++;
        }
      }
    }
    vertexData = std::move(vertexDataTemp);
    return error;
  }
   /** \brief
 *Функция фейсетов
 */
  int Model::get_facets(std::ifstream &file)
  {
    int error = static_cast<int>(ErrorCode::OK);
    file.clear();
    file.seekg(0, std::ios::beg);
    facetData.clear();
    std::string temp_string;
    char seps[] = " ";
    size_t arg_f_index = 0;
    std::string temp;
    int countTokes = 0;
    int numberOfTokens = 0;
    while (std::getline(file, temp_string))
    {
      if (temp_string[0] == 'f' && temp_string[1] == ' ')
      {
        numberOfTokens = 1;
        countTokes = 0;
        count_number_in_string(temp_string, numberOfTokens);
        char temp_str[temp_string.size() + 1]; // Add +1 for null terminator
        strcpy(temp_str, temp_string.c_str());
        char *token = strtok(temp_str, seps);
        countTokes++;
        while (token != nullptr)
        {
          token = strtok(nullptr, seps);
          countTokes++;
          if (countTokes == 2)
          {
            temp = token;
          }
          if (token == nullptr || *token == '\n')
            break;
          if (countTokes == 2)
          {
            facetData.push_back(std::atoi(token));
            arg_f_index++;
          }
          else if (*token >= '1' && *token <= '9')
          {
            facetData.push_back(std::atoi(token));
            arg_f_index++;
            facetData.push_back(std::atoi(token));
            arg_f_index++;
          }
          if (numberOfTokens == countTokes)
          {
            facetData.push_back(std::atoi(temp.c_str()));
            arg_f_index++;
          }
        }
      }
    }
    return error;
  }

  int Model::count_number_in_string(const std::string &str, int &count)
  {
    int error = static_cast<int>(ErrorCode::OK);
    int temp_count = 0;
    for (size_t i = 0; i < str.length(); i++)
    {
      if (str[i] == ' ' && str[i + 1] >= '1' && str[i + 1] <= '9')
      {
        temp_count++;
      }
    }
    count += temp_count;
    return error;
  }

  void Model::move(int dx, int dy, int dz)
  {
    for (size_t i = 0; i < vertexData.size(); i += 3)
    {
      vertexData[i] += dx;
      vertexData[i + 1] += dy;
      vertexData[i + 2] += dz;
    }
  }
  // Метод для поворота модели на заданный угол относительно своих осей X, Y, Z
  void Model::rotate(double dx, double dy, double dz)
  {
    // Реализация вращения модели вокруг своих осей X, Y, Z
    // Здесь должен быть ваш код для вычисления матрицы поворота и применения ее к
    // vertexData
    if (dx)
    {
      rotateAroundAxis(1, 2, dx);
    }
    if (dy)
    {
      rotateAroundAxis(0, 2, dy);
    }
    if (dz)
    {
      rotateAroundAxis(0, 1, dz);
    }
  }
  // Вспомогательная функция для поворота модели вокруг заданных осей
  void Model::rotateAroundAxis(int a, int b, double angle)
  {
    angle = angle * M_PI / 180;
    for (size_t i = 0; i < vertexData.size(); i += 3) // Update loop iteration condition
    {
      double a1 = vertexData[i + a];
      double a2 = vertexData[i + b];
      vertexData[i + a] = cos(angle) * a1 - sin(angle) * a2;
      vertexData[i + b] = sin(angle) * a1 + cos(angle) * a2;
    }
  }

  void Model::scale(double scale)
  {
    for (size_t i = 0; i < vertexData.size(); i++)
    {
      vertexData[i] *= scale;
    }
  }

  void Model::setVertexData(const std::vector<double> &data)
  {
    vertexData = data;
  }

  void Model::setFacetData(const std::vector<unsigned int> &data)
  {
    facetData = data;
  }

  std::vector<double> &Model::getVertexData() { return vertexData; }

  std::vector<unsigned int> &Model::getFacetData()
  {
    return facetData;
  }

} // namespace s21
