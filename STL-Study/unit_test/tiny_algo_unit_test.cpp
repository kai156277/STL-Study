#include "tiny_algo_unit_test.h"
#include <cstdio>
#include "tiny_algobase.h"
#include "tiny_vector.h"
#include "tiny_print.h"

using namespace tiny;
void tiny_copy_unit_test()
{
	const char ccs[5] = { 'a', 'b', 'c', 'd', 'e' };
	char ccd[5];
	printf("1. ccs => ccd \n");
	tiny::copy(ccs, ccs + 5, ccd);
	for (char c : ccd)
		putchar(c);
	printf("\n");

	const wchar_t cwcs[5] = { 'a', 'b', 'c', 'd', 'e' };
	wchar_t cwcd[5];
	printf("\n 2. cwcs => cwcd \n");
	tiny::copy(cwcs, cwcs + 5, cwcd);
	for (wchar_t wc : cwcd)
		putwchar(wc);
	printf("\n");

	int ia[5] = { 0, 1, 2, 3, 4 };
	int _ia[5];
	printf("\n 3. int[] \n");
	copy(ia, ia + 5, _ia);
	for (int i : _ia)
		printf("%d ", i);
	putchar('\n');

#if 0
	std::list<int> ilists(ia, ia + 5);
	std::list<int> ilistd(5);
	printf("\n 4. list<int> \n");
	tiny::copy(ilists.begin(), ilists.end(), ilistd.begin());
	for (int i : ilistd)
		printf("%d ", i);
	putchar('\n');
#endif // 0

	vector<int> ivecs(ia, ia + 5);
	vector<int> ivecd(5);
	printf("\n 5. vector<int> \n");
	copy(ivecs.begin(), ivecs.end(), ivecd.begin());
	for (int i : ivecd)
		printf("%d ", i);
	putchar('\n');


	copy_class c[5];
	vector<copy_class> Cvs(c, c + 5);
	vector<copy_class> Cvd(5);
	printf("\n 6. vector<copy_class> \n");
	copy(Cvs.begin(), Cvs.end(), Cvd.begin());
	for (size_t i = 0; i < Cvd.size(); ++i)
		printf("%d ", Cvd[i]._data);
	putchar('\n');

	copy_class_def c1[5];
	vector<copy_class_def> Cvs1(c1, c1 + 5);
	vector<copy_class_def> Cvd1(5);
	printf("\n 6.1. vector<copy_class_def> \n");
	copy(Cvs1.begin(), Cvs1.end(), Cvd1.begin());
	for (size_t i = 0; i < Cvd1.size(); ++i)
		printf("%d ", Cvd1[i]._data);
	putchar('\n');
	for (size_t i = 0; i < Cvd1.size(); ++i)
		printf("%d ", Cvd1[i]._data1);
	putchar('\n');
}

void tiny_copy_backward_unit_test()
{
	const char ccs[5] = { 'a', 'b', 'c', 'd', 'e' };
	char ccd[5];
	printf("1. ccs => ccd \n");
	tiny::copy_backward(ccs, ccs + 5, ccd + 5);
	for (char c : ccd)
		putchar(c);
	printf("\n");

	const wchar_t cwcs[5] = { 'a', 'b', 'c', 'd', 'e' };
	wchar_t cwcd[5];
	printf("\n 2. cwcs => cwcd \n");
	tiny::copy_backward(cwcs, cwcs + 5, cwcd + 5);
	for (wchar_t wc : cwcd)
		putwchar(wc);
	printf("\n");

	int ia[5] = { 0, 1, 2, 3, 4 };
	int _ia[5];
	printf("\n 3. int[] \n");
	copy_backward(ia, ia + 5, _ia + 5);
	for (int i : _ia)
		printf("%d ", i);
	putchar('\n');

#if 0
	std::list<int> ilists(ia, ia + 5);
	std::list<int> ilistd(5);
	printf("\n 4. list<int> \n");
	tiny::copy(ilists.begin(), ilists.end(), ilistd.begin());
	for (int i : ilistd)
		printf("%d ", i);
	putchar('\n');
#endif // 0

	vector<int> ivecs(ia, ia + 5);
	vector<int> ivecd(5);
	printf("\n 5. vector<int> \n");
	copy_backward(ivecs.begin(), ivecs.end(), ivecd.end());
	for(int i: ivecd)
		printf("%d ", i);
	putchar('\n');


	copy_class c[5];
	vector<copy_class> Cvs(c, c + 5);
	vector<copy_class> Cvd(5);
	printf("\n 6. vector<copy_class> \n");
	copy_backward(Cvs.begin(), Cvs.end(), Cvd.end());
	for (size_t i = 0; i < Cvd.size(); ++i)
		printf("%d ", Cvd[i]._data);
	putchar('\n');

	copy_class_def c1[5];
	vector<copy_class_def> Cvs1(c1, c1 + 5);
	vector<copy_class_def> Cvd1(5);
	printf("\n 6.1. vector<copy_class_def> \n");
	copy_backward(Cvs1.begin(), Cvs1.end(), Cvd1.end());
	for (size_t i = 0; i < Cvd1.size(); ++i)
		printf("%d ", Cvd1[i]._data);
	putchar('\n');
	for (size_t i = 0; i < Cvd1.size(); ++i)
		printf("%d ", Cvd1[i]._data1);
	putchar('\n');
}

struct Test_print
{
	Test_print(double _a, double _b) : a(_a), b(_b)
	{}
	double a;
	double b;
};

template<>
struct item_printf<Test_print>
{
	void operator()(const Test_print& item)
	{
		printf("%lf, %lf \n", item.a, item.b);
	}
};

void tiny_STL_print_unit_test()
{
	vector<Test_print> vec1;
	for (int i = 0; i < 10; ++i)
	vec1.push_back(Test_print(i, i + 1));

	STL_printf(vec1);

}
