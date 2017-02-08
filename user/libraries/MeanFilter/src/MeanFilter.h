#ifndef MEAN_FILTER_H_
#define MEAN_FILTER_H_
#define HISTORY_NUM 12 
class MeanFilter
{
	public:
	MeanFilter(float init_value);
	MeanFilter(){}
	float input(float inVal);
	private:
	float buffer_[HISTORY_NUM] =
	{
		0
	}
	;
}
;
#endif