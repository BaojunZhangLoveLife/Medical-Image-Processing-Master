#include <iostream>
class ItkSegmentation
{
	public:
		//区域生长
		void RegionGrowing(std::string srcPath, std::string dstPath);
		//填孔操作
		void FillHoles(std::string srcPath, std::string dstPath);
		//修改像素值
		void ModifyPixels(std::string srcPath, std::string dstPath);
		//Dice系数计算
		void Dice(std::string image1Path, std::string image2Path);
		//执行多个加法操作
		void mathAddFilter();
		//对称操作
		void SymmetricalOperation(std::string src, std::string dst);
	private:
};