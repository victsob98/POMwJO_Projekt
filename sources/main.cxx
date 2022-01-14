#include <iostream> // std::cout, std::cin
#include <string> // std::string
#include <itkImage.h> // itk::Image
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkJPEGImageIOFactory.h>
#include <itkJPEGImageIO.h>
#include <itkRGBToLuminanceImageFilter.h>
#include <itkIntensityWindowingImageFilter.h>
#include <itkThresholdImageFilter.h>
#include <itkBinaryThresholdImageFilter.h>

#include <itkNormalizeImageFilter.h>
#include <itkMinimumMaximumImageFilter.h>
#include <itkMinimumMaximumImageCalculator.h>
#include <itkStatisticsRelabelImageFilter.h>
#include <itkOtsuMultipleThresholdsImageFilter.h>
#include <itkMedianImageFilter.h>
#include <itkImageToHistogramFilter.h>
#include <itkBinaryThresholdImageFilter.h>
#include <itkRescaleIntensityImageFilter.h>
#include <itkSigmoidImageFilter.h>
#include <itkCurvatureAnisotropicDiffusionImageFilter.h>
#include <itkBinaryMorphologicalClosingImageFilter.h>
#include <itkFlatStructuringElement.h>
#include <itkSize.h>
#include <itkImageRegionConstIterator.h>
#include <itkImageRegionIterator.h>
#include <itkExpNegativeImageFilter.h>

#include <itkInvertIntensityImageFilter.h>
#include<itkMaskImageFilter.h>


int main() // glowna funkcja programu
{	      
	try {

		int i;
		for (i = 1; i <= 15; i++)
		{
			//Powtarzany blok instrukcji
			std::string s = std::to_string(i);



			std::string sciezkaWe = "F:/Projekt-pomwjo/dane/Szczeniory/"+s+".jpg";
			std::string sciezkaWy = "F:/Projekt-pomwjo/wyniki/kopia"+s+".tiff";
			std::string sciezkaWyhistogram = "F:/Projekt-pomwjo/wyniki/histogram" + s + ".tiff";



			using ComponentType = unsigned char;
			using InputPixelType = itk::RGBPixel<ComponentType>;
			using RGBImageType = itk::Image<InputPixelType, 2>;

			using ReaderType = itk::ImageFileReader<RGBImageType>;
			ReaderType::Pointer reader = ReaderType::New();


			reader->SetFileName(sciezkaWe);
			reader->Update();

			using OutputPixelType = unsigned char;
			using GrayscaleImageType = itk::Image<OutputPixelType, 2>;

			using FilterType = itk::RGBToLuminanceImageFilter<RGBImageType, GrayscaleImageType>;
			FilterType::Pointer filter = FilterType::New();
			filter->SetInput(reader->GetOutput());

			using BinaryPixelType = unsigned char;
			using BinaryImageType = itk::Image<BinaryPixelType, 2>;

			
			
			using TresholdFilterType = itk::ThresholdImageFilter<GrayscaleImageType>;
			 TresholdFilterType::Pointer tresholdFilter = TresholdFilterType::New();
			 tresholdFilter->SetInput(filter->GetOutput());
			 //tresholdFilter->ThresholdAbove(256*0.7);

			 tresholdFilter->SetUpper(256 * 0.68);
			 tresholdFilter->SetOutsideValue(0); // zamien na -1024
		/*	 filter->ThresholdAbove(300);
			 filter->ThresholdBelow(100);
			filter->ThresholdOutside(100,300)*/
			 GrayscaleImageType::SizeType indexRadius;

			 indexRadius[0] =1; // radius along x
			 indexRadius[1] = 3; // radius along y

			 //
			 using MedianFilterType = itk::MedianImageFilter<GrayscaleImageType,GrayscaleImageType>;
			 MedianFilterType::Pointer medianFilter = MedianFilterType::New();
			 medianFilter->SetInput(tresholdFilter->GetOutput());
			 medianFilter->SetRadius(indexRadius);
			 
			 using BinaryTresholdFilterType = itk::BinaryThresholdImageFilter<GrayscaleImageType,GrayscaleImageType>;
			 BinaryTresholdFilterType::Pointer binarytresholdFilter = BinaryTresholdFilterType::New();
			 binarytresholdFilter->SetInput(medianFilter->GetOutput());
			 //tresholdFilter->ThresholdAbove(256*0.7);

			 binarytresholdFilter->SetUpperThreshold(0.0001);
		
					 // zamien na -1024
			 binarytresholdFilter->SetOutsideValue(0);

			 using StructuringElementType = itk::FlatStructuringElement<2>;
			 using StructuringElementTypeball = itk::FlatStructuringElement<2>;

			 StructuringElementTypeball::RadiusType radiusball;
			 radiusball.Fill(25);
			 StructuringElementTypeball structuringElementball = StructuringElementTypeball::Ball(radiusball);

			 StructuringElementType::RadiusType radius;
			 radius.Fill(25);
			 StructuringElementType structuringElement = StructuringElementType::Box(radius);

			 
			 using DilateImageFilter = itk::BinaryDilateImageFilter<GrayscaleImageType, GrayscaleImageType, StructuringElementType>;
			 DilateImageFilter::Pointer dilate = DilateImageFilter::New();
			 dilate->SetInput(binarytresholdFilter->GetOutput());
			 dilate->SetKernel(structuringElementball);
			 dilate->SetForegroundValue(1);
			 dilate->Update();
		

		

			using InvertFilterType = itk::InvertIntensityImageFilter<GrayscaleImageType, GrayscaleImageType>;
			InvertFilterType::Pointer invertFilter = InvertFilterType::New();
			invertFilter->SetInput(dilate->GetOutput());
			invertFilter->Update();

			using MaskerType = itk::MaskImageFilter<GrayscaleImageType, GrayscaleImageType>;
			MaskerType::Pointer maskFilter = MaskerType::New();
			maskFilter->SetInput1(filter->GetOutput());
			maskFilter->SetInput2(invertFilter->GetOutput());
			maskFilter->SetOutsideValue(0);
			maskFilter->Update();

			//using FilterTypeNegation = itk::ExpNegativeImageFilter<GrayscaleImageType, GrayscaleImageType>;
			//FilterTypeNegation::Pointer negateFilter = FilterTypeNegation::New();
			//negateFilter->SetInput(maska);
			//negateFilter->Update();


		/*	 using ConstIteratorType = itk::ImageRegionConstIterator< GrayscaleImageType>;
			 using IteratorType = itk::ImageRegionIterator< GrayscaleImageType >;
			 

			 ConstIteratorType constIterator(image2, image2->GetRequestedRegion());
			 IteratorType iterator(image, image->GetRequestedRegion());
			
			 unsigned char jedynka = 1;
			 unsigned char zero = 0;

			 for (iterator.GoToBegin(), constIterator.GoToBegin(); !iterator.IsAtEnd(); ++constIterator, ++iterator)
			 {
				 if (constIterator.Get() == jedynka) {
					 iterator.Set(zero);
				 }
			 }

			 */

			

		/*		using ErodeImageFilter =itk::BinaryErodeImageFilter<GrayscaleImageType, GrayscaleImageType, StructuringElementType>;
			 ErodeImageFilter::Pointer erode = ErodeImageFilter::New();
			 erode->SetInput(dilate->GetOutput());
			 erode->SetKernel(structuringElementball);
			 erode->SetForegroundValue(1);
			 erode->Update();*/


			/* using SigmoidFilterType = itk::SigmoidImageFilter<GrayscaleImageType, GrayscaleImageType>;
			 SigmoidFilterType::Pointer sigmoidFilter = SigmoidFilterType::New();
			 sigmoidFilter->SetInput(tresholdFilter->GetOutput());
			 sigmoidFilter->SetOutputMinimum(10);
			 sigmoidFilter->SetOutputMaximum(240);
			 sigmoidFilter->SetAlpha(60);
			 sigmoidFilter->SetBeta(170);*/



	/*		 using FilterTypeOtsu = itk::OtsuMultipleThresholdsImageFilter<GrayscaleImageType, GrayscaleImageType>;
			 FilterTypeOtsu::Pointer filterOtsu = FilterTypeOtsu::New();
			 filterOtsu->SetInput(tresholdFilter->GetOutput()); 
			 filterOtsu->SetNumberOfThresholds(4);
			 filterOtsu->SetLabelOffset(2);
			 filterOtsu->SetNumberOfHistogramBins(200);*/
			
		/* FilterTypeOtsu::ThresholdVectorType thresholds = filterOtsu->GetThresholds();			
			 std::cout << "Thresholds:" << std::endl;

			 for (double threshold : thresholds)
			 {
				 std::cout << threshold << std::endl;
			 }

			std::cout << std::endl;
			 using RescaleType = itk::RescaleIntensityImageFilter<GrayscaleImageType, GrayscaleImageType>;
			 RescaleType::Pointer rescaler = RescaleType::New();
			 rescaler->SetInput(filterOtsu->GetOutput());
			 rescaler->SetOutputMinimum(0);
			 rescaler->SetOutputMaximum(255);*/

			using WriterType = itk::ImageFileWriter<BinaryImageType>;
			WriterType::Pointer writer = WriterType::New();
			writer->SetFileName(sciezkaWy);
			writer->SetInput(maskFilter->GetOutput());
			writer->Update();

		












			//using FilterTypeW = itk::IntensityWindowingImageFilter<OutputImageType, OutputImageType>;
			//FilterTypeW::Pointer filter2 = FilterTypeW::New();
			////filter2->SetInput(reader->GetOutput());
			////filter2->SetWindowMaximum(200);
			////filter2->SetWindowMinimum(128);
			////filter2->SetOutputMaximum(155);
		 ////   filter2->SetOutputMinimum(0);
			//filter2->SetWindowLevel(255 + 25, (255 - 25) / 2);
			//filter2->SetInput(filter->GetOutput());

			//using FilterTypee = itk::ThresholdImageFilter<OutputImageType>;
			//FilterTypee::Pointer filter1 = FilterTypee::New();
			//filter1->SetLower(90);
			//filter1->SetInput(filter->GetOutput());
			//filter1->SetOutsideValue(-1048);



			
		}
	
	}
	catch (itk::ExceptionObject& ex) {
		ex.Print(std::cout);
	}
	catch (std::exception& ex) {
		std::cout << ex.what() << std::endl;
	}
	catch (...) {
		std::cout << "Unknown error!" << std::endl;
	}
	std::cout << "Hit [Enter]...";
	std::cin.get();
	return EXIT_SUCCESS; // albo EXIT_FAILURE
}