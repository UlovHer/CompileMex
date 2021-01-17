#include "mex.hpp"
#include "mexAdapter.hpp"
#include <iostream>
//�ļ���׺��Ϊ.cpp,Ϊ�������ı�̷����������ΪMexFunction���Ҽ̳�matlab::mex::Function������()���������

class MexFunction :public matlab::mex::Function
{
	//���졢��������������дʹ�ñ������ṩ��Ĭ�Ϲ��졢��������
public:
	void operator()(matlab::mex::ArgumentList outs, matlab::mex::ArgumentList ins)
	{
		//����()���������
		bool argv_flag = checkArguments(outs, ins);
		//������������Ƿ����Ҫ��
		if (argv_flag)
		{
			std::cout << "*****the input argvs are valid*****" << std::endl;
			matlab::data::TypedArray<double> in_matrix = std::move(ins[0]);
			//matlab::mex::ArgumentList�а�������һ��matlab::data::Array�������ݣ���Ҫ������ͬ���͵�
			//����������[]���������������ʹ���������
			size_t nums = in_matrix.getNumberOfElements();
			//��ȡ�����е�Ԫ�ظ���������ֵΪunsign int
			add(in_matrix, nums);
			std::cout << "the sum is " << this->result << std::endl;
			//��������������ڴ洢���صļ�����
			matlab::data::ArrayFactory factory;
			matlab::data::TypedArray<double> out_matrix = factory.createScalar<double>(this->result);
            //���ֻ��һ���������������ɣ������ǣ��򴴽�����factory.createArray<double>({ numRows, numColumns });
			//ʹ�����������out_matrix���Ԫ��
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
		// ����������������Ƿ����Ҫ��

		if (ins[0].getType() != matlab::data::ArrayType::DOUBLE ||
			ins[0].getType() == matlab::data::ArrayType::COMPLEX_DOUBLE)
			//ins[0].getNumberOfElements() != 1 ���Լ����������Ԫ�ظ���
		{
			matlabPtr->feval(matlab::engine::convertUTF8StringToUTF16String("error"),
				0, std::vector<matlab::data::Array>({ factory.createScalar("Input matrix must be type double") }));
			return false;
		}
		//��������������������Ƿ����Ҫ��

		if (ins[0].getDimensions().size() != 2) {
			matlabPtr->feval(matlab::engine::convertUTF8StringToUTF16String("error"),
				0, std::vector<matlab::data::Array>({ factory.createScalar("Input must be m-by-n dimension") }));
			return false;
		}
		//������������Ƿ�Ϊ����

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