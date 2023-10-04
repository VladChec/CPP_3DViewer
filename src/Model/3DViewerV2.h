#include <cstring>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace s21
{
  // Перечисление для кодов ошибок
  enum class ErrorCode
  {
    OK,
    FILE_NOT_FOUND,
    MEMORY_ERROR,
    INVALID_FORMAT,
    ERROR
  };

  class Model
  {
  public:
    Model();
    ~Model();

    bool parseObjFile(const std::string &filePath);
    void move(int dx, int dy, int dz);
    void rotate(double dx, double dy, double dz);
    void rotateAroundAxis(int a, int b, double angle);
    void scale(double scale);
    // void printInfo();
    std::vector<double> &getVertexData();
    std::vector<unsigned int> &getFacetData();

    void setVertexData(const std::vector<double> &data);
    void setFacetData(const std::vector<unsigned int> &data);

  private:
    std::vector<double> vertexData;
    std::vector<unsigned int> facetData;

    int pre_parser(std::ifstream &file);
    int get_vertexes(std::ifstream &file);
    int get_facets(std::ifstream &file);
    int count_number_in_string(const std::string &str, int &count);

  };
} // namespace s21
