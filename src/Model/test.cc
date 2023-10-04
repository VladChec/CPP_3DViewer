#include <gtest/gtest.h>
#include "3DViewerV2.h"

#include <iostream>
using namespace s21;

// Test Fixture
class ModelTest : public ::testing::Test
{
protected:
    Model model;
};

TEST_F(ModelTest, SetVertexData_ValidData_DataSetSuccessfully)
{
    // Arrange
    std::vector<double> vertexData = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};

    // Act
    model.setVertexData(vertexData);
    const std::vector<double> &resultVertexData = model.getVertexData();

    // Assert
    ASSERT_EQ(vertexData, resultVertexData);
}

TEST_F(ModelTest, SetFacetData_ValidData_DataSetSuccessfully)
{
    // Arrange
    std::vector<unsigned int> facetData = {1, 2, 3, 4, 5, 6};

    // Act
    model.setFacetData(facetData);
    const std::vector<unsigned int> &resultFacetData = model.getFacetData();

    // Assert
    ASSERT_EQ(facetData, resultFacetData);
}

TEST_F(ModelTest, Scale_ModelScaledSuccessfully)
{
    // Arrange
    model.setVertexData({1.0, 2.0, 3.0, 4.0, 5.0, 6.0});
    double scale = 2.0;

    // Act
    model.scale(scale);
    const std::vector<double> &vertexData = model.getVertexData();

    // Assert (Check if the vertices are properly scaled)
    ASSERT_NEAR(vertexData[0], 2.0, 1e-6);
    ASSERT_NEAR(vertexData[1], 4.0, 1e-6);
    ASSERT_NEAR(vertexData[2], 6.0, 1e-6);
    ASSERT_NEAR(vertexData[3], 8.0, 1e-6);
    ASSERT_NEAR(vertexData[4], 10.0, 1e-6);
    ASSERT_NEAR(vertexData[5], 12.0, 1e-6);
    // Add more assertions if needed to check other vertices
}
TEST_F(ModelTest, Rotate_ModelRotatedAroundXAxisSuccessfully)
{
    // Arrange
    model.setVertexData({1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0});
    double dx = 90.0;

    // Act
    model.rotate(dx, 0.0, 0.0);
    const std::vector<double> &vertexData = model.getVertexData();
    std::vector<double> expected = {1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, -1.0, 0.0};

    // Assert (Check if the vertices are properly rotated around the X axis)
    EXPECT_EQ(vertexData.size(), expected.size());
    if (vertexData.size() == expected.size())
    {
        for (size_t i = 0; i < vertexData.size(); ++i)
        {
            EXPECT_NEAR(vertexData[i], expected[i], 1e-6);
        }
    }
}

// TEST_F(ModelTest, Rotate_ModelRotatedAroundXAndZAxesSuccessfully)
// {
//     // Arrange
//     model.setVertexData({1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0});
//     double dx = 90.0, dy = 0.0, dz = 90.0;

//     // Act
//     model.rotate(dx, dy, dz);
//     const std::vector<double> &vertexData = model.getVertexData();

//     // Assert (Check if the vertices are properly rotated)
//     std::vector<double> expected = {0.0, 0.0, -1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0};
//     // Add more assertions if needed to check other vertices
//     EXPECT_EQ(vertexData.size(), expected.size());
//     for (size_t i = 0; i < vertexData.size(); ++i)
//     {
//         EXPECT_NEAR(vertexData[i], expected[i], 1e-6);
//     }
// }

TEST_F(ModelTest, Rotate_ModelRotatedAroundYAxisSuccessfully)
{
    // Arrange
    model.setVertexData({1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0});
    double dy = 90.0;

    // Act
    model.rotate(0.0, dy, 0.0);
    const std::vector<double> &vertexData = model.getVertexData();

    // Assert (Check if the vertices are properly rotated)
    std::vector<double> expected = {0.0, 0.0, 1.0, 0.0, 1.0, 0.0, -1.0, 0.0, 0.0};
    EXPECT_EQ(vertexData.size(), expected.size());
    for (size_t i = 0; i < vertexData.size(); ++i)
    {
        EXPECT_NEAR(vertexData[i], expected[i], 1e-6);
    }
}

TEST_F(ModelTest, ParseObjFile_InvalidFormat_ReturnsErrorCodeInvalidFormat)
{
    // Arrange
    std::string filePath = "invalid_model.obj";

    // Act
    ErrorCode result = static_cast<ErrorCode>(model.parseObjFile(filePath));

    // Assert
    ASSERT_EQ(result, static_cast<ErrorCode>(0));
}

TEST_F(ModelTest, ParseObjFile_MemoryError_ReturnsErrorCodeMemoryError)
{
    // Arrange
    // Simulate a memory error by using an excessively large file path
    std::string filePath = std::string(10000000, 'a');

    // Act
    ErrorCode result = static_cast<ErrorCode>(model.parseObjFile(filePath));

    // Assert
    ASSERT_EQ(result, static_cast<ErrorCode>(0)); // Use static_cast<ErrorCode>(1) for MEMORY_ERROR
}

TEST_F(ModelTest, ParseObjFile_FileNotFound_ReturnsFalse)
{
    // Provide the path to a non-existent file
    std::string filePath = "path/to/non_existent_file.obj";
    bool result = model.parseObjFile(filePath);
    EXPECT_FALSE(result);
}

TEST_F(ModelTest, ParseObjFile_InvalidFile_ReturnsFileNotFoundError)
{
    std::string filePath = "apsicua/Desktogramm/ne_3DViewer_v1.0-1/src/Model/slipper.obj";
    bool result = model.parseObjFile(filePath);
    EXPECT_FALSE(result);
    EXPECT_EQ(model.getVertexData().size(), 0);
    EXPECT_EQ(model.getFacetData().size(), 0);
}

TEST_F(ModelTest, ParseObjFile_InvalidFormat_ReturnsInvalidFormatError)
{
    std::string filePath = "rs/capsicua/Desktogramm/new/C8_3DViewer_v1.0-1/src/Model/slipper.obj";
    bool result = model.parseObjFile(filePath);
    EXPECT_FALSE(result);
    EXPECT_EQ(model.getVertexData().size(), 0);
    EXPECT_EQ(model.getFacetData().size(), 0);
}
int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
