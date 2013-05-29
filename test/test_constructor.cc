class Test
{
public:
	Test(int num)
		:num_(num)
	{
	}
private:
	int num_;
};

class Test2
{
public:
	Test2(int num)
		:test_(num)
	{
	}
public:
	Test test_;
};
