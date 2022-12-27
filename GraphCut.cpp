/**
 *  Image GraphCut 3D Segmentation
 *
 *  Copyright (c) 2016, Zurich University of Applied Sciences, School of Engineering, T. Fitze, Y. Pauchard
 *
 *  Licensed under GNU General Public License 3.0 or later.
 *  Some rights reserved.
 */

#include "GraphCut.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

/** This example segments an image and writes the segmentation mask to a file.
* It can operate on a 3D image with one component per dimension (i.e.
* grayscale).
*/

void GraphCutAlgorithm::GraphCut(std::string fileName, std::string fgFileName, std::string bgFileName, std::string outFileName) 
{
	// Parse arguments
	std::string imageFilename = fileName;
	std::string foregroundFilename = fgFileName;   // This image should have non-zero pixels indicating foreground pixels and 0 elsewhere.
	std::string backgroundFilename = bgFileName;   // This image should have non-zero pixels indicating background pixels and 0 elsewhere.
	std::string outputFilename = outFileName;
	double sigma = 50;               // Noise parameter
	int boundaryDirection = 0;      // 0->bidirectional; 1->bright to dark; 2->dark to bright

	// Define all image types
	typedef itk::Image<short, 3> ImageType;
	typedef itk::Image<unsigned char, 3> ForegroundMaskType;
	typedef itk::Image<unsigned char, 3> BackgroundMaskType;
	typedef itk::Image<unsigned char, 3> OutputImageType;

	// Read the image
	std::cout << "*** Reading image ***" << std::endl;
	typedef itk::ImageFileReader<ImageType> ReaderType;
	ReaderType::Pointer reader = ReaderType::New();
	reader->SetFileName(imageFilename);

	// Read the foreground and background masks
	std::cout << "*** Reading foreground mask ***" << std::endl;
	typedef itk::ImageFileReader<ForegroundMaskType> ForegroundMaskReaderType;
	ForegroundMaskReaderType::Pointer foregroundMaskReader = ForegroundMaskReaderType::New();
	foregroundMaskReader->SetFileName(foregroundFilename);

	std::cout << "*** Reading background mask ***" << std::endl;
	typedef itk::ImageFileReader<BackgroundMaskType> BackgroundMaskReaderType;
	BackgroundMaskReaderType::Pointer backgroundMaskReader = BackgroundMaskReaderType::New();
	backgroundMaskReader->SetFileName(backgroundFilename);

	// Set up the graph cut
	typedef GraphCut::FilterType<ImageType, ForegroundMaskType, BackgroundMaskType, OutputImageType> 
		GraphCutFilterType;
	GraphCutFilterType::Pointer graphCutFilter = GraphCutFilterType::New();
	graphCutFilter->SetInputImage(reader->GetOutput());
	graphCutFilter->SetForegroundImage(foregroundMaskReader->GetOutput());
	graphCutFilter->SetBackgroundImage(backgroundMaskReader->GetOutput());

	// Set graph cut parameters
	graphCutFilter->SetVerboseOutput(true);
	graphCutFilter->SetSigma(sigma);
	switch (boundaryDirection) {
	case 1:
		graphCutFilter->SetBoundaryDirectionTypeToBrightDark();
		break;
	case 2:
		graphCutFilter->SetBoundaryDirectionTypeToDarkBright();
		break;
	default:
		graphCutFilter->SetBoundaryDirectionTypeToNoDirection();
	}

	// Define the color values of the output
	graphCutFilter->SetForegroundPixelValue(255);
	graphCutFilter->SetBackgroundPixelValue(0);

	// Start the computation
	std::cout << "*** Performing Graph Cut ***" << std::endl;
	try
	{
		graphCutFilter->Update();
	}
	catch (itk::ExceptionObject &excep) {
		std::cerr << excep << std::endl;
		std::cerr << " Exception caught !" << std::endl;
	}

	// Get and write the result
	std::cout << "*** Writing Result ***" << std::endl;
	typedef itk::ImageFileWriter<GraphCutFilterType::OutputImageType> WriterType;
	WriterType::Pointer writer = WriterType::New();
	writer->SetFileName(outputFilename);
	writer->SetInput(graphCutFilter->GetOutput());
	try {
		std::cout << "Writing output image " << outputFilename << std::endl;
		writer->Update();
	}
	catch (itk::ExceptionObject &err) {
		std::cerr << "ERROR: Exception caught while writing output image" << std::endl;
		std::cerr << err << std::endl;
	}
}
