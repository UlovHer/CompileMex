#include "mex.h"
#include <iostream>
//#include <cstdlib>

//�ļ���׺��Ϊ.cpp,����Ȼ��������̵ı�̣�ֻ�ǻ�����C++��񣬲�Ӱ��mex����
//.cpp���Ը�Ϊ.c����C++��ӡ��ʽ����C�Ĵ�ӡ��ʽ����ɴ�C����

double add(double* inputs, int len, double* out)
{
	std::cout << "*****come into add function*****" << std::endl;
	//printf("*****come into add function*****\n");
	double sum = 0;
	for (int i = 0; i < len; i++)
	{
		sum += *(inputs + i);
	}
	*out = sum;
	//���ܷ���sum��ַ,sumΪ�ֲ�����
	return sum;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	double* input = mxGetPr(prhs[0]);
	int m = mxGetM(prhs[0]);
	int n = mxGetN(prhs[0]);
	//printf("inputs argvs numbers %d\n", nrhs);
	//printf("output argvs numbers %d\n", nlhs);
	std::cout << "inputs argvs numbers " << nrhs << "\toutput argvs numbers " << nlhs << std::endl;
	//printf("%d %d %f\n", m, n, *input);
	std::cout << "inputs argvs numbers are: ";
	for (int i = 0; i < m*n; i++)
	{
		std::cout << *(input + i) << "\t";
		//printf("the input argvs are: %f\t", *(input + i));
	}
	std::cout << std::endl;
	//printf("\n");
	//printf("*****call add function*****\n");
	std::cout << "*****call add function*****" << std::endl;
	plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
	double* out_matrix = mxGetPr(plhs[0]);
	double res = add(input, m*n, out_matrix);
	std::cout << "the sum is " << res << std::endl;
	//printf("the sum is %f\n", res);
}