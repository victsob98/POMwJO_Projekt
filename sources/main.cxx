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
#include <itkNormalizeImageFilter.h>
#include <itkDoubleThresholdImageFilter.h>
#include <itkMultiplyImageFilter.h>
#include <itkSubtractImageFilter.h>
#include <itkHoughTransform2DCirclesImageFilter.h>
#include <itkLabelSelectionLabelMapFilter.h>

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

			using OutputPixelType = short;
			using GrayscaleImageType = itk::Image<OutputPixelType, 2>;
			using OutputPixelType2 = signed short;
			using GrayscaleImageType2 = itk::Image<OutputPixelType2, 2>;

			using FilterType = itk::RGBToLuminanceImageFilter<RGBImageType, GrayscaleImageType>;
			FilterType::Pointer filter = FilterType::New();
			filter->SetInput(reader->GetOutput());




			

			using RescaleType = itk::RescaleIntensityImageFilter<GrayscaleImageType, GrayscaleImageType>;
			RescaleType::Pointer rescaler = RescaleType::New();
			rescaler->SetInput(filter->GetOutput());
			rescaler->SetOutputMinimum(0);
			rescaler->SetOutputMaximum(255);
			
		//	using TresholdFilterType = itk::ThresholdImageFilter<GrayscaleImageType>;
		//	 TresholdFilterType::Pointer tresholdFilter = TresholdFilterType::New();
		//	 tresholdFilter->SetInput(rescaler->GetOutput());
		//	 //tresholdFilter->ThresholdAbove(256*0.7);

		//	 tresholdFilter->SetUpper(256 * 0.68);
		//	 tresholdFilter->SetOutsideValue(0); // zamien na -1024
		///*	 filter->ThresholdAbove(300);
		//	 filter->ThresholdBelow(100);
		//	filter->ThresholdOutside(100,300)*/

			using DoubleThresholdFilterType = itk::DoubleThresholdImageFilter<GrayscaleImageType, GrayscaleImageType>;
			DoubleThresholdFilterType::Pointer doubleThresholdFilter = DoubleThresholdFilterType::New();
			doubleThresholdFilter->SetInput(rescaler->GetOutput());
			doubleThresholdFilter->SetFullyConnected(true);
			doubleThresholdFilter->SetThreshold1(152);
			doubleThresholdFilter->SetThreshold2(180);
			doubleThresholdFilter->SetThreshold3(250);
			doubleThresholdFilter->SetThreshold4(255);
			doubleThresholdFilter->SetOutsideValue(0);
			doubleThresholdFilter->SetInsideValue(255);

			GrayscaleImageType::SizeType indexRadius;

			 indexRadius[0] =1; // radius along x
			 indexRadius[1] = 3; // radius along y
			
			 using MedianFilterType = itk::MedianImageFilter<GrayscaleImageType,GrayscaleImageType>;
			 MedianFilterType::Pointer medianFilter = MedianFilterType::New();
			 medianFilter->SetInput(doubleThresholdFilter->GetOutput());
			 medianFilter->SetRadius(indexRadius);
			 
			 
			

			 using StructuringElementType = itk::FlatStructuringElement<2>;
			

			 StructuringElementType::RadiusType radiusball;
			 radiusball.Fill(15);
			 StructuringElementType structuringElementball = StructuringElementType::Ball(radiusball);

			 StructuringElementType::RadiusType radius;
			 radius.Fill(15);
			 StructuringElementType structuringElement = StructuringElementType::Box(radius);

			 StructuringElementType::RadiusType radiusPolygon;
			 radiusPolygon.Fill(23);
			 StructuringElementType::RadiusType radiusPolygon2;
			 radiusPolygon2.Fill(35);
			 StructuringElementType::RadiusType radiusPolygon3;
			 radiusPolygon3.Fill(50);
			 StructuringElementType::RadiusType radiusPolygon4;
			 radiusPolygon4.Fill(25);

			 StructuringElementType structuringElementpolygon = StructuringElementType::Polygon(radiusPolygon,3);
			 StructuringElementType structuringElementpolygon2 = StructuringElementType::Polygon(radiusPolygon2, 2);
			 StructuringElementType structuringElementpolygon3 = StructuringElementType::Polygon(radiusPolygon3, 6);
			 StructuringElementType structuringElementpolygon4 = StructuringElementType::Polygon(radiusPolygon4, 9);

			 using ErodeImageFilter =itk::BinaryErodeImageFilter<GrayscaleImageType, GrayscaleImageType, StructuringElementType>;
			 ErodeImageFilter::Pointer erode = ErodeImageFilter::New();
			 erode->SetInput(medianFilter->GetOutput());
			 erode->SetKernel(structuringElementpolygon);
			 erode->SetForegroundValue(255);
			 erode->Update();
		
			 using DilateImageFilter = itk::BinaryDilateImageFilter<GrayscaleImageType, GrayscaleImageType, StructuringElementType>;
			 DilateImageFilter::Pointer dilate = DilateImageFilter::New();
			 dilate->SetInput(erode->GetOutput());
			 dilate->SetKernel(structuringElementpolygon2);
			 dilate->SetForegroundValue(255);
			 dilate->Update();
		
			 using ClosingImageFilter = itk::BinaryMorphologicalClosingImageFilter<GrayscaleImageType, GrayscaleImageType, StructuringElementType>;
			 ClosingImageFilter::Pointer closing = ClosingImageFilter::New();
			 closing->SetInput(dilate->GetOutput());
			 closing->SetKernel(structuringElementpolygon3);
			 closing->SetForegroundValue(255);
			 closing->Update();

			 using DilateImageFilter2 = itk::BinaryDilateImageFilter<GrayscaleImageType, GrayscaleImageType, StructuringElementType>;
			 DilateImageFilter2::Pointer dilate2 = DilateImageFilter2::New();
			 dilate2->SetInput(closing->GetOutput());
			 dilate2->SetKernel(structuringElementball);
			 dilate2->SetForegroundValue(255);
			 dilate2->Update();

			 using ClosingImageFilter2 = itk::BinaryMorphologicalClosingImageFilter<GrayscaleImageType, GrayscaleImageType, StructuringElementType>;
			 ClosingImageFilter2::Pointer closing2 = ClosingImageFilter2::New();
			 closing2->SetInput(dilate2->GetOutput());
			 closing2->SetKernel(structuringElementpolygon4);
			 closing2->SetForegroundValue(255);
			 closing2->Update();

		/*	 using BinaryTresholdFilterType = itk::BinaryThresholdImageFilter<GrayscaleImageType, GrayscaleImageType>;
			 BinaryTresholdFilterType::Pointer binarytresholdFilter = BinaryTresholdFilterType::New();
			 binarytresholdFilter->SetInput(closing2->GetOutput());
			 binarytresholdFilter->SetOutsideValue(1);
			 binarytresholdFilter->SetInsideValue(0);
			 binarytresholdFilter->SetLowerThreshold(200);
			 binarytresholdFilter->Update();*/

			/* using FilterTypeRescaler = itk::RescaleIntensityImageFilter<GrayscaleImageType, GrayscaleImageType>;
			 FilterTypeRescaler::Pointer filterRescaler = FilterTypeRescaler::New();
			 filterRescaler->SetInput(closing2->GetOutput());
			 filterRescaler->SetOutputMinimum(0);
			 filterRescaler->SetOutputMaximum(1);

			using InvertFilterType = itk::InvertIntensityImageFilter<GrayscaleImageType, GrayscaleImageType>;
			InvertFilterType::Pointer invertFilter = InvertFilterType::New();
			invertFilter->SetInput(filterRescaler->GetOutput());
			invertFilter->Update();
		/*	 using NegativeFilterType = itk::ExpNegativeImageFilter<GrayscaleImageType, GrayscaleImageType>;
			 NegativeFilterType::Pointer negativeFilter = NegativeFilterType::New();
			negativeFilter->SetInput(filter->GetOutput());
			negativeFilter->SetFactor(1);*/

			using MaskerType = itk::MaskImageFilter<GrayscaleImageType, GrayscaleImageType>;
			MaskerType::Pointer maskFilter = MaskerType::New();
			maskFilter->SetInput(filter->GetOutput());
			maskFilter->SetMaskImage(closing2->GetOutput());
			maskFilter->SetOutsideValue(0);
			maskFilter->Update();	

			using SubtractImageFilterType = itk::SubtractImageFilter<GrayscaleImageType, GrayscaleImageType>;
			SubtractImageFilterType::Pointer subtractFilter = SubtractImageFilterType::New();
			subtractFilter->SetInput1(filter->GetOutput());
			subtractFilter->SetInput2(maskFilter->GetOutput());
			subtractFilter->Update();

		

			//using HoughTransformFilterType = itk::HoughTransform2DCirclesImageFilter<OutputPixelType,OutputPixelType,double>;

			//auto houghFilter = HoughTransformFilterType::New();
			//houghFilter->SetInput(subtractFilter->GetOutput());
			//const auto& spatialObject=houghFilter->GetCircles().;
			//spatialObject->ComputeObjectToWorldTransform();

			/*using DoubleThresholdFilterType2 = itk::DoubleThresholdImageFilter<GrayscaleImageType, GrayscaleImageType>;
			DoubleThresholdFilterType2::Pointer doubleThresholdFilter2 = DoubleThresholdFilterType2::New();
			doubleThresholdFilter2->SetInput(maskFilter->GetOutput());
			doubleThresholdFilter2->SetFullyConnected(true);
			doubleThresholdFilter2->SetThreshold1(120);
			doubleThresholdFilter2->SetThreshold2(125);
			doubleThresholdFilter2->SetThreshold3(128);
			doubleThresholdFilter2->SetThreshold4(130);
			doubleThresholdFilter2->SetOutsideValue(0);
			doubleThresholdFilter2->SetInsideValue(255);*/
			/*using SigmoidFilterType = itk::SigmoidImageFilter<GrayscaleImageType, GrayscaleImageType>;
			 SigmoidFilterType::Pointer sigmoidFilter = SigmoidFilterType::New();
			 sigmoidFilter->SetInput(doubleThresholdFilter2->GetOutput());
			 sigmoidFilter->SetOutputMinimum(10);
			 sigmoidFilter->SetOutputMaximum(250);
			
			 sigmoidFilter->SetAlpha(-100);
			 sigmoidFilter->SetBeta(50);*/
			using IntensityWindowingImageFilterType = itk::IntensityWindowingImageFilter<GrayscaleImageType, GrayscaleImageType>;

			IntensityWindowingImageFilterType::Pointer intensityFilter = IntensityWindowingImageFilterType::New();
			intensityFilter->SetInput(subtractFilter->GetOutput());
			intensityFilter->SetWindowMinimum(77);
			intensityFilter->SetWindowLevel(72,114);
			intensityFilter->SetWindowMaximum(145);
			intensityFilter->SetOutputMinimum(0);
			intensityFilter->SetOutputMaximum(255);
			intensityFilter->Update();


	


			 using FilterTypeOtsu = itk::OtsuMultipleThresholdsImageFilter<GrayscaleImageType, GrayscaleImageType>;
			 FilterTypeOtsu::Pointer filterOtsu = FilterTypeOtsu::New();
			 filterOtsu->SetInput(intensityFilter->GetOutput()); 
			 filterOtsu->SetNumberOfThresholds(4);
			 filterOtsu->SetLabelOffset(1);
			 filterOtsu->SetNumberOfHistogramBins(255);
			 
			
		 FilterTypeOtsu::ThresholdVectorType thresholds = filterOtsu->GetThresholds();	

		 using RescaleType2 = itk::RescaleIntensityImageFilter<GrayscaleImageType, GrayscaleImageType>;
		 RescaleType2::Pointer rescaler2 = RescaleType2::New();
		 rescaler2->SetInput(filterOtsu->GetOutput());
		 rescaler2->SetOutputMinimum(0);
		 rescaler2->SetOutputMaximum(255); 
		 
		 
		using CastFilterType = itk::CastImageFilter<GrayscaleImageType, GrayscaleImageType2>;
		 CastFilterType::Pointer castFilter = CastFilterType::New();
		 castFilter->SetInput(maskFilter->GetOutput());

		 using LabelObjectType = itk::LabelObject<OutputPixelType2, 2>;
		 using LabelMapType = itk::LabelMap<LabelObjectType>;

		using LabImgToLabMapType = itk::LabelImageToLabelMapFilter<GrayscaleImageType2,LabelMapType>;
			LabImgToLabMapType::Pointer img2map = LabImgToLabMapType::New();
			img2map->SetInput(castFilter->GetOutput());
			img2map->SetBackgroundValue(itk::NumericTraits<OutputPixelType2>::Zero);
			img2map->Update();

			using SelectorType = itk::LabelSelectionLabelMapFilter<LabelMapType>;
			SelectorType::Pointer selector = SelectorType::New();
			selector->SetInput(img2map->GetOutput());
			selector->SetLabel(1);
			
			
			using LabelMapToLabelImageFilterType = itk::LabelMapToLabelImageFilter<LabelMapType, GrayscaleImageType2>;
			LabelMapToLabelImageFilterType::Pointer labelImageConverter = LabelMapToLabelImageFilterType::New();
			labelImageConverter->SetInput(selector->GetOutput());

		

			using WriterType = itk::ImageFileWriter<GrayscaleImageType2>;
			WriterType::Pointer writer = WriterType::New();
			writer->SetFileName(sciezkaWy);
			writer->SetInput(castFilter->GetOutput());
			writer->Update();

		












			
			

			


			
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