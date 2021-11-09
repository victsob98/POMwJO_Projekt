#include <iostream> // std::cout, std::cin
#include <string> // std::string
#include <itkImage.h> // itk::Image
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkJPEGImageIOFactory.h>
#include <itkJPEGImageIO.h>
#include <itkRGBToLuminanceImageFilter.h>
int main() // glowna funkcja programu
{      
	try {
		


		std::string sciezkaWe = "F:/Projekt-pomwjo/dane/Szczeniory/brzusio1.jpg";
		std::string sciezkaWy = "F:/Projekt-pomwjo/wyniki/brzusio1kopia.jpg";

		

		using ComponentType = unsigned char;
		using InputPixelType = itk::RGBPixel<ComponentType>;
		using InputImageType = itk::Image<InputPixelType, 2>;
		
		using ReaderType = itk::ImageFileReader<InputImageType>;
		ReaderType::Pointer reader = ReaderType::New();


	    reader->SetFileName(sciezkaWe);
		reader->Update();

		

		using OutputPixelType = unsigned char;
		using OutputImageType = itk::Image<OutputPixelType, 2>;

		using FilterType = itk::RGBToLuminanceImageFilter<InputImageType, OutputImageType>;
		FilterType::Pointer filter = FilterType::New();
		filter->SetInput(reader->GetOutput());

		using WriterType = itk::ImageFileWriter<OutputImageType>;
		WriterType::Pointer writer = WriterType::New();
		writer->SetFileName(sciezkaWy);
		writer->SetInput(filter->GetOutput());
		writer->Update();

	
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