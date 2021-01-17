#include "mex.hpp"
#include "mexAdapter.hpp"
#include <iostream>
//文件后缀名为.cpp,为面向对象的编程风格，类名必须为MexFunction，且继承matlab::mex::Function，重载()操作运算符

class MexFunction :public matlab::mex::Function
{
	//构造、析构函数不，重写使用编译器提供的默认构造、析构函数
public:
	void operator()(matlab::mex::ArgumentList outs, matlab::mex::ArgumentList ins)
	{
		//重载()操作运算符
		bool argv_flag = checkArguments(outs, ins);
		//检验输入参数是否符合要求
		if (argv_flag)
		{
			std::cout << "*****the input argvs are valid*****" << std::endl;
			matlab::data::TypedArray<double> in_matrix = std::move(ins[0]);
			//matlab::mex::ArgumentList中包含的是一个matlab::data::Array类型数据，需要定义相同类型的
			//类里重在了[]操作运算符，可以使用序号索引
			size_t nums = in_matrix.getNumberOfElements();
			//获取矩阵中的元素个数，返回值为unsign int
			add(in_matrix, nums);
			std::cout << "the sum is " << this->result << std::endl;
			//创建输出矩阵，用于存储返回的计算结果
			matlab::data::ArrayFactory factory;
			matlab::data::TypedArray<double> out_matrix = factory.createScalar<double>(this->result);
            //输出只有一个，创建标量即可，若不是，则创建矩阵factory.createArray<double>({ numRows, numColumns });
			//使用索引添加往out_matrix添加元素
			outs[0] = std::move(out_matrix);
		}
		else
		{
			std::cout << "*****the input argvs are invalid*****" << std::endl;
		}
	}

private:
	bool checkArguments(matlab::mex::ArgumentList outs, matlab::mex::ArgumentList ins)
	{
		std::shared_ptr < matlab::engine::MATLABEngine> matlabPtr = getEngine();
		matlab::data::ArrayFactory factory;
		if (ins.size() != 1)
		{
			matlabPtr->feval(matlab::engine::convertUTF8StringToUTF16String("error"),
				0, std::vector<matlab::data::Array>({ factory.createScalar("One inputs required") }));
			return false;
		}
		// 检验输入参数个数是否符合要求

		if (ins[0].getType() != matlab::data::ArrayType::DOUBLE ||
			ins[0].getType() == matlab::data::ArrayType::COMPLEX_DOUBLE)
			//ins[0].getNumberOfElements() != 1 可以检验输入矩阵元素个数
		{
			matlabPtr->feval(matlab::engine::convertUTF8StringToUTF16String("error"),
				0, std::vector<matlab::data::Array>({ factory.createScalar("Input matrix must be type double") }));
			return false;
		}
		//检验输入参数数据类型是否符合要求

		if (ins[0].getDimensions().size() != 2) {
			matlabPtr->feval(matlab::engine::convertUTF8StringToUTF16String("error"),
				0, std::vector<matlab::data::Array>({ factory.createScalar("Input must be m-by-n dimension") }));
			return false;
		}
		//检验输入参数是否为矩阵

		return true;
	}

private:
	void add(matlab::data::TypedArray<double> & inputs, size_t nums)
	{
		std::cout << "*****come into add function*****" << std::endl;
		double sum = 0;
		std::cout << "inputs argvs numbers are: ";
		for (int i = 0; i < nums; i++)
		{
			std::cout << inputs[i] << "\t";
			sum += inputs[i];
		}
		std::cout << std::endl;
		this->result = sum;
	}

private:
	double result = 0;
};