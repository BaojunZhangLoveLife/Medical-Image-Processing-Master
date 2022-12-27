#include "itkImage.h"
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include "itkImageSeriesReader.h"
#include "itkImageFileWriter.h"
#include "itkAddImageFilter.h"
//ITK
const unsigned int Dimension = 3;

typedef itk::Image<float, Dimension> FloatImage;
typedef itk::Image<short, Dimension> ShortImage;
typedef itk::Image<char, Dimension> CharImage;
typedef itk::Image<int, Dimension> IntImage;
typedef itk::Image<unsigned char, Dimension> UCharImage;
typedef itk::Image<unsigned int, Dimension> UIntImage;

typedef FloatImage::Pointer FloatImagePtr;
typedef ShortImage::Pointer ShortImagePtr;
typedef CharImage::Pointer CharImagePtr;
typedef IntImage::Pointer IntImagePtr;
typedef UCharImage::Pointer UCharImagePtr;
typedef UIntImage::Pointer UIntImagePtr;

typedef UCharImage::SizeType ImageSize;
typedef UCharImage::IndexType ImageIndex;
typedef UCharImage::RegionType ImageRegion;

typedef itk::ImageFileReader<FloatImage> ReaderType;
typedef itk::ImageFileWriter<FloatImage> WriterType;
typedef itk::AddImageFilter<FloatImage, FloatImage> mathAddFilterType;

//PCL
typedef pcl::PointXYZ ZbjPointT;
typedef pcl::PointCloud<ZbjPointT> ZbjPointCloudT;
typedef ZbjPointCloudT::Ptr ZbjPointCloudPtr;

//获取根路径
void GetRootPath(std::string path, std::vector<std::string>& files);
//获取子路径
void GetSubPath(std::string path, std::vector<std::string>& files);