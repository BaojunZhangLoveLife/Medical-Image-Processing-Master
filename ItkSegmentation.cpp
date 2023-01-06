#include "ItkSegmentation.h"
#include "itkConnectedThresholdImageFilter.h"
#include "itkImage.h"
#include "itkCastImageFilter.h"
#include "itkCurvatureFlowImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkBinaryFillholeImageFilter.h"
#include "itkVotingBinaryHoleFillingImageFilter.h"
#include "itkConnectedComponentImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkLabelShapeKeepNObjectsImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkMultiplyImageFilter.h"
#include "Globals.h"

// Region Growing
void ItkSegmentation::RegionGrowing(std::string srcPath, std::string dstPath) {
	using InternalImageType = FloatImage;
	using OutputImageType = FloatImage;
	using CastingFilterType = itk::CastImageFilter<InternalImageType, OutputImageType>;
	using ReaderType = itk::ImageFileReader<InternalImageType>;
	using WriterType = itk::ImageFileWriter<OutputImageType>;
	
	auto caster = CastingFilterType::New();
	auto reader = ReaderType::New();
	auto writer = WriterType::New();

	reader->SetFileName(srcPath);
	reader->Update();

	using ConnectedFilterType = itk::ConnectedThresholdImageFilter<InternalImageType, InternalImageType>;
	auto connectedThreshold = ConnectedFilterType::New();
	connectedThreshold->SetInput(reader->GetOutput());
	connectedThreshold->SetLower(1);
	connectedThreshold->SetUpper(1);
	connectedThreshold->SetReplaceValue(1);
	ImageIndex index = {234 ,324, 120};
	connectedThreshold->SetSeed(index);
	caster->SetInput(connectedThreshold->GetOutput());

	writer->SetFileName(dstPath);
	writer->SetInput(caster->GetOutput());
	writer->Update();
}

//填孔操作
void ItkSegmentation::FillHoles(std::string srcPath, std::string dstPath) {
	using InternalImageType = FloatImage;
	using OutputImageType = FloatImage;
	using CastingFilterType = itk::CastImageFilter<InternalImageType, OutputImageType>;
	using ReaderType = itk::ImageFileReader<InternalImageType>;
	using WriterType = itk::ImageFileWriter<OutputImageType>;
	using FillHoleType = itk::VotingBinaryHoleFillingImageFilter<InternalImageType, OutputImageType> ;

	auto caster = CastingFilterType::New();
	auto reader = ReaderType::New();
	auto writer = WriterType::New();

	reader->SetFileName(srcPath);
	reader->Update();
	
	FillHoleType::Pointer fillHoleFilter = FillHoleType::New();
	InternalImageType::SizeType indexRadius;
	indexRadius[0] = 4;
	indexRadius[1] = 4;
	indexRadius[2] = 4;
	fillHoleFilter->SetInput(reader->GetOutput());
	fillHoleFilter->SetRadius(indexRadius);
	fillHoleFilter->SetBackgroundValue(0);
	fillHoleFilter->SetForegroundValue(1);
	fillHoleFilter->SetMajorityThreshold(2);
	fillHoleFilter->Update();

	writer->SetFileName(dstPath);
	writer->SetInput(fillHoleFilter->GetOutput());
	writer->Update();
}


//修改像素值操作
void ItkSegmentation::ModifyPixels(std::string srcPath, std::string dstPath) {

	using InternalImageType = FloatImage;
	using OutputImageType = FloatImage;
	using ReaderType = itk::ImageFileReader<InternalImageType>;
	using WriterType = itk::ImageFileWriter<OutputImageType>;

	auto reader = ReaderType::New();
	auto writer = WriterType::New();

	reader->SetFileName(srcPath);
	reader->Update();

	FloatImagePtr dstImage = FloatImage::New();
	dstImage->Graft(reader->GetOutput());
	ImageRegion region = reader->GetOutput()->GetLargestPossibleRegion();
	ImageSize size = region.GetSize();
	ImageIndex idx;
	for (int iz = 0; iz < size[2]; iz++){
		for (int iy = 0; iy < size[1]; iy++){
			for (int ix = 0; ix < size[0]; ix++){
				idx[0] = ix;
				idx[1] = iy;
				idx[2] = iz;
				if (reader->GetOutput()->GetPixel(idx) == 1){
					dstImage->SetPixel(idx, reader->GetOutput()->GetPixel(idx) + 2);
				}	
			}
		}
	}
	writer->SetFileName(dstPath);
	writer->SetInput(dstImage);
	writer->Update();
}
//Dice系数
void ItkSegmentation::Dice(std::string image1Path, std::string image2Path) {
	using InternalImageType = FloatImage;
	using OutputImageType = FloatImage;
	using ReaderType = itk::ImageFileReader<InternalImageType>;
	using WriterType = itk::ImageFileWriter<OutputImageType>;
	using mathFilterType = itk::MultiplyImageFilter<FloatImage, FloatImage>;
	int mixedCount = 0;		//两个图像交集中像素1的个数
	int image1Count = 0;	//图像1中像素1的个数
	int image2Count = 0;	//图像1中像素1的个数
	float dice = 0.0f;

	auto image1PathReader = ReaderType::New();
	auto image2PathReader = ReaderType::New();
	auto imageWriter = WriterType::New();
	
	//乘法运算
	image1PathReader->SetFileName(image1Path);
	image1PathReader->Update();

	image2PathReader->SetFileName(image2Path);
	image2PathReader->Update();

	mathFilterType::Pointer mathFilter = mathFilterType::New();
	mathFilter->SetInput1(image1PathReader->GetOutput());
	mathFilter->SetInput2(image2PathReader->GetOutput());
	mathFilter->Update();

	ImageRegion region = mathFilter->GetOutput()->GetLargestPossibleRegion();
	ImageSize size = region.GetSize();
	ImageIndex idx;
	for (int iz = 0; iz < size[2]; iz++){
		for (int iy = 0; iy < size[1]; iy++){
			for (int ix = 0; ix < size[0]; ix++){
				idx[0] = ix;
				idx[1] = iy;
				idx[2] = iz;
				if (mathFilter->GetOutput()->GetPixel(idx) == 1){
					mixedCount++;
				}
			}
		}
	}
	std::cout << "mixedCount = " << mixedCount << std::endl;

	 region = image1PathReader->GetOutput()->GetLargestPossibleRegion();
	 size = region.GetSize();
	for (int iz = 0; iz < size[2]; iz++){
		for (int iy = 0; iy < size[1]; iy++){
			for (int ix = 0; ix < size[0]; ix++){
				idx[0] = ix;
				idx[1] = iy;
				idx[2] = iz;
				if (image1PathReader->GetOutput()->GetPixel(idx) == 1){
					image1Count++;
				}
			}
		}
	}
	std::cout << "image1Count = " << image1Count << std::endl;

	region = image2PathReader->GetOutput()->GetLargestPossibleRegion();
	size = region.GetSize();
	for (int iz = 0; iz < size[2]; iz++){
		for (int iy = 0; iy < size[1]; iy++){
			for (int ix = 0; ix < size[0]; ix++){
				idx[0] = ix;
				idx[1] = iy;
				idx[2] = iz;
				if (image2PathReader->GetOutput()->GetPixel(idx) == 1){
					image2Count++;
				}
			}
		}
	}
	std::cout << "image2Count = " << image2Count << std::endl;

	dice = (float)(2 * mixedCount) / (image1Count + image2Count);

	std::cout << "Dice系数 = " << dice << std::endl;
}
void ItkSegmentation::mathAddFilter(){
	std::string src = "";
	std::vector<std::string> mFileVec;

	GetRootPath(src, mFileVec);

	std::string dst;	//标注组合后的文件名
	for (int i = 0; i < mFileVec.size(); i++){
		auto maskReader0 = ReaderType::New();
		auto maskReader1 = ReaderType::New();
		auto writer = WriterType::New();
		auto mathFilter = mathAddFilterType::New();

		dst = mFileVec[i].substr(0, 32) + ".nii.gz";
		std::cout << dst << std::endl;
		std::string src = mFileVec[i];

		std::vector<std::string> mFileVec1;
		GetSubPath(src, mFileVec1);
		maskReader0->SetFileName(mFileVec1[0]);
		maskReader0->Update();

		FloatImagePtr newImage = FloatImage::New();
		newImage->Graft(maskReader0->GetOutput());
		ImageSize size = newImage->GetLargestPossibleRegion().GetSize();
		ImageIndex idx;
		for (int iz = 0; iz < size[2]; iz++){
			for (int iy = 0; iy < size[1]; iy++){
				for (int ix = 0; ix < size[0]; ix++){
					idx[0] = ix;
					idx[1] = iy;
					idx[2] = iz;
					newImage->SetPixel(idx, 0);
				}
			}
		}
		writer->SetFileName(dst);
		writer->SetInput(newImage);
		writer->Update();

		for (int i = 0; i < mFileVec1.size(); i++){
			std::cout << mFileVec1[i] << std::endl;

			maskReader0 = ReaderType::New();
			maskReader0->SetFileName(dst);
			maskReader0->Update();

			maskReader1 = ReaderType::New();
			maskReader1->SetFileName(mFileVec1[i]);
			maskReader1->Update();

			mathFilter = mathAddFilterType::New();
			mathFilter->SetInput1(maskReader0->GetOutput());
			mathFilter->SetInput2(maskReader1->GetOutput());
			mathFilter->Update();

			writer->SetFileName(dst);
			writer->SetInput(mathFilter->GetOutput());
			writer->Update();
		}
	}
}
//对称操作
void ItkSegmentation::SymmetricalOperation(std::string src, std::string dst){
	auto reader1 = ReaderType::New();
	reader1->SetFileName(src);
	reader1->Update();

	FloatImagePtr newImage1 = FloatImage::New();

	newImage1->Graft(reader1->GetOutput());

	ImageSize size1 = newImage1->GetLargestPossibleRegion().GetSize();
	ImageIndex idx1;


	for (int iz = 0; iz < size1[2]; iz++){
		for (int iy = 0; iy < size1[1]; iy++){
			for (int ix = 0; ix < size1[0]; ix++){
				idx1[0] = ix;
				idx1[1] = iy;
				idx1[2] = iz;

				newImage1->SetPixel(idx1, 0);
			}

		}
	}
	auto writer1 = WriterType::New();
	writer1->SetFileName(dst);
	writer1->SetInput(newImage1);
	writer1->Update();


	auto reader2 = ReaderType::New();
	reader2->SetFileName(dst);
	reader2->Update();

	auto reader3 = ReaderType::New();
	reader3->SetFileName(src);
	reader3->Update();

	FloatImagePtr newImage2 = FloatImage::New();

	newImage2->Graft(reader2->GetOutput());

	ImageSize size2 = newImage2->GetLargestPossibleRegion().GetSize();
	ImageIndex idx2;
	ImageIndex idx3;

	for (int iz = 0; iz < size2[2]; iz++){
		for (int iy = 0; iy < size2[1]; iy++){
			for (int ix = 0; ix < size2[0]; ix++){
				idx2[0] = ix;
				idx2[1] = iy;
				idx2[2] = iz;

				idx3[0] = size2[0] - ix;
				idx3[1] = iy;
				idx3[2] = iz;

				newImage2->SetPixel(idx2, reader3->GetOutput()->GetPixel(idx3));
			}
		}
	}
	auto writer2 = WriterType::New();
	writer2->SetFileName(dst);
	writer2->SetInput(newImage2);
	writer2->Update();
}