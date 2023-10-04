
#include "../Model/3DViewerV2.h"
namespace s21
{

  class ModelController
  {
  public:
    ModelController(Model &model);
    bool loadModelFromFile(const std::string &filePath);
    void moveModel(int dx, int dy, int dz);
    void rotateModel(int dx, int dy, int dz);
    void scaleModel(double scale);
    void rotateAroundAxisModel(int a, int b, double angle);
    std::vector<double> &getVertexData();
    std::vector<unsigned int> &getFacetData();
    void setVertexData(const std::vector<double> &data);
    void setFacetData(const std::vector<unsigned int> &data);

  private:
    Model &model;
  };

} // namespace s21
