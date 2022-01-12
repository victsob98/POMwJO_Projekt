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
#include <itkNormalizeImageFilter.h>
#include <itkMinimumMaximumImageFilter.h>
#include <itkMinimumMaximumImageCalculator.h>
#include <itkStatisticsRelabelImageFilter.h>
#include <itkOtsuMultipleThresholdsImageFilter.h>
#include <itkMedianImageFilter.h>
#include <itkImageToHistogramFilter.h>
#include <itkBinaryThresholdImageFilter.h>
#include <itkRescaleIntensityImageFilter.h>


int main() // glowna funkcja programu
{	      
	try {

		int i;
		for (i = 1; i <= 17; i++)
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
		
			
			using TresholdFilterType = itk::ThresholdImageFilter<GrayscaleImageType>;
			 TresholdFilterType::Pointer tresholdFilter = TresholdFilterType::New();
			 tresholdFilter->SetInput(filter->GetOutput());
			 tresholdFilter->SetUpper(255*0.68);
			 //filter->SetUpper(300);
			 tresholdFilter->SetOutsideValue(0); // zamien na -1024
			 //filter->ThresholdAbove(300);
			 //filter->ThresholdBelow(100);
			//filter->ThresholdOutside(100,300)
			 //GrayscaleImageType::SizeType indexRadius;

			 //indexRadius[0] = 5; // radius along x
			 //indexRadius[1] = 5; // radius along y

			 //
			 //using MedianFilterType = itk::MedianImageFilter<GrayscaleImageType,GrayscaleImageType>;
			 //MedianFilterType::Pointer medianFilter = MedianFilterType::New();
			 //medianFilter->SetInput(tresholdFilter->GetOutput());
			 //medianFilter->SetRadius(indexRadius);

			 using FilterTypeOtsu = itk::OtsuMultipleThresholdsImageFilter<GrayscaleImageType, GrayscaleImageType>;
			 FilterTypeOtsu::Pointer filterOtsu = FilterTypeOtsu::New();
			 filterOtsu->SetInput(tresholdFilter->GetOutput()); 
			 filterOtsu->SetNumberOfThresholds(4);
			 filterOtsu->SetLabelOffset(2);
			 filterOtsu->SetNumberOfHistogramBins(200);
			
			 FilterTypeOtsu::ThresholdVectorType thresholds = filterOtsu->GetThresholds();			
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
			 rescaler->SetOutputMaximum(255);

			using WriterType = itk::ImageFileWriter<GrayscaleImageType>;
			WriterType::Pointer writer = WriterType::New();
			writer->SetFileName(sciezkaWy);
			writer->SetInput(rescaler->GetOutput());
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