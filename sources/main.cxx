#include <iostream> // std::cout, std::cin
#include <string> // std::string
#include <itkImage.h> // itk::Image
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkJPEGImageIOFactory.h>
#include <itkJPEGImageIO.h>

int main() // glowna funkcja programu
{      
	try {
		

		using PixelType = signed short;
		using ImageType = itk::Image<PixelType, 2>;
		using ReaderType = itk::ImageFileReader<ImageType>;
		using WriterType = itk::ImageFileWriter<ImageType>;

		std::string sciezkaWe = "F:/Projekt-pomwjo/dane/Szczeniory/brzusio1.jpg";
		std::string sciezkaWy = "F:/Projekt-pomwjo/wyniki/brzusio1kopia.jpg";

		ReaderType::Pointer reader = ReaderType::New();
		reader->SetFileName(sciezkaWe);
		reader->Update();

		ImageType::Pointer image = reader->GetOutput();

	/*	WriterType::Pointer writer = WriterType::New();
		writer->SetInput(image);

		writer->SetFileName(sciezkaWy);
		writer->Update();*/



	
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