#include "pch.h"
#include "CppUnitTest.h"
#include "..\\2SAPlab_12.11.2\main.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:

		TEST_METHOD(TestMethod1)
        {
            Library library;
            library.addBook(123, "petro", " Book", 2022, 5);
            library.writeBooksToFile("test_output.txt");

            ifstream file("test_output.txt");
            
            Assert::IsTrue(file.good());
        }
	};
}
