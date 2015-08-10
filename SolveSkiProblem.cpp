#include <iostream>
#include <string>
#include <iomanip>
#include <map>
#include <list>
#include <array>
#include <fstream>

using namespace std;

class Utility
{
private:
	static int** mMap;
	std::multimap<int, std::list<int>> mpathsList;
	int mapindex;
	const int ROWCOUNT = 1000;
	const int COLUMNCOUNT = 1000;
	enum{
		top   = (1 << 0),
		down  = (1 << 1),
		left  = (1 << 2),
		right = (1 << 3)
	};
public:
	std::multimap<int, std::list<int>> getmpathsList()
	{
		return mpathsList;
	}

	int** getMap()
	{
		return mMap;
	}
	void setMap(int ** ptr)
	{
		mMap = ptr;
	}

	void ExecuteSearch()
	{
		for (int j = 0; j < ROWCOUNT; j++)
		{
			int *row = mMap[j];

			for (int i = 0; i < COLUMNCOUNT; i++)
			{
				std::list<int> dummylist;
				dummylist.push_back(row[i]);
				cout << "Row : " << j << "\tColumn: " << i << "\n";
				findwhichdirectiontomove(((j * COLUMNCOUNT) + i), dummylist);
			}
		}
	}

	void getindex(int d, int &rowindex, int &columnindex)
	{
		if (d == top)
		{
			rowindex = rowindex - 1;
		}
		else if (d == down)
		{
			rowindex = rowindex + 1;
		}
		else if (d == left)
		{
			columnindex = columnindex - 1;
		}
		else if (d == right)
		{
			columnindex = columnindex + 1;
		}
	}

	void findwhichdirectiontomove(int index, std::list<int> templist)
	{
		int rowindex = index / COLUMNCOUNT;
		int columnindex = index % COLUMNCOUNT;
		int temprowindex = rowindex;
		int tempcolumnindex = columnindex;
		static int directions = 0;
		static int mlargestize = 0;

		int centre = mMap[rowindex][columnindex];
		int top = centre;
		int down = centre;
		int left = centre;
		int right = centre;

		if (rowindex - 1 >= 0)
			top = mMap[rowindex - 1][columnindex];
		if (rowindex + 1 < ROWCOUNT)
			down = mMap[rowindex + 1][columnindex];
		if (columnindex - 1 >= 0)
			left = mMap[rowindex][columnindex - 1];
		if (columnindex + 1 < COLUMNCOUNT)
			right = mMap[rowindex][columnindex + 1];

		int deltatop = centre - top;
		int deltadown = centre - down;
		int deltaleft = centre - left;
		int deltaright = centre - right;

		std::list<std::pair<int, int>> l;
		if (/*(fromdirection & 2)== 0 && */deltatop > 0)
			l.push_back(std::pair<int, int>(deltatop, 1));
		if (/*(fromdirection & 1) == 0 && */deltadown > 0)
			l.push_back(std::pair<int, int>(deltadown, 2));
		if (/*(fromdirection & 8) == 0 && */deltaleft > 0)
			l.push_back(std::pair<int, int>(deltaleft, 4));
		if (/*(fromdirection & 4) == 0 && */deltaright > 0)
			l.push_back(std::pair<int, int>(deltaright, 8));

		if (l.size() == 0)
		{
			int count = templist.size();
			if (count > mlargestize)
			{
				mlargestize = count;
				mpathsList.clear();
				mpathsList.insert(std::pair<int, std::list<int>>(mapindex, templist));
			}
			else if (count == mlargestize)
			{
				mapindex++;
				mpathsList.insert(std::pair<int, std::list<int>>(mapindex, templist));
			}
			directions = 0;
			return;
		}

		l.sort();
		for (std::list<std::pair<int, int>>::iterator itr = l.begin(); itr != l.end(); itr++)
		{
			temprowindex = rowindex;
			tempcolumnindex = columnindex;
			getindex(itr->second, temprowindex, tempcolumnindex);
			if (itr->second & top)
			{
				directions |= down;
			}
			else if (itr->second & down)
			{
				directions |= top;
			}
			else if (itr->second & left)
			{
				directions |= right;
			}
			else if (itr->second & right)
			{
				directions |= left;
			}
			templist.push_back(mMap[temprowindex][tempcolumnindex]);
			findwhichdirectiontomove(tempcolumnindex + (temprowindex * COLUMNCOUNT), templist);
			templist.pop_back();
		}
	}
	friend class FileDecoder;
};

int** Utility::mMap = NULL;

class FileDecoder
{
private:
	static FileDecoder *mthisObject;
	Utility u;
	
	FileDecoder(){}
	FileDecoder(const FileDecoder &obj){}
	FileDecoder& operator= (const FileDecoder obj){}
public:
	static FileDecoder* getInstance()
	{
		if (mthisObject == NULL)
			mthisObject =  new FileDecoder();
		return mthisObject;
	}

	void CreateArrayFromFile(char *filename)
	{
		ifstream file(filename);
		int **matrix = NULL;
		matrix = new int *[u.ROWCOUNT];
		for (int count = 0; count < u.ROWCOUNT; count++)
		{
			matrix[count] = new int[u.COLUMNCOUNT];
		}
		for (int i = 0; i < u.ROWCOUNT; ++i) {
			for (int j = 0; j < u.COLUMNCOUNT; ++j) {
				file >> matrix[i][j];
			}
		}
		u.setMap(matrix);
	}
};

FileDecoder *FileDecoder::mthisObject = NULL;

int main()
{
	FileDecoder::getInstance()->CreateArrayFromFile("C:/Users/Naveetha R/Documents/Visual Studio 2013/Projects/practice/practice/map.txt");

	Utility obj;
	obj.ExecuteSearch();
	
	int depth = 0;
	std::list<int> finalpath;
	std::multimap<int, std::list<int>> paths = obj.getmpathsList();

	for (std::map<int, std::list<int>>::iterator itr = paths.begin(); itr != paths.end(); itr++)
	{
		cout << "\n\n" << "Potential Path : [" << itr->first << "]" << "\n";
		std::list<int> path = itr->second;
		std::list<int>::iterator firstnumber = path.begin();
		int lastnumber = 0;
		
		for (std::list<int>::iterator itr1 = path.begin(); itr1 != path.end(); itr1++)
		{
			lastnumber = *itr1;
			cout << lastnumber << " ";
		}
		int diff = *firstnumber - lastnumber;
		cout << "\tDrop : " << diff;
		if (depth < diff)
		{
			finalpath = path;
			depth = diff;
		}
	}

	cout << "\n\nLongest Path with largest drop: ";
	for (std::list<int>::iterator itr1 = finalpath.begin(); itr1 != finalpath.end(); itr1++)
	{
		cout << *itr1 << " ";
	}
	cout << "\nsize: " << finalpath.size() << " and Drop: " << depth;

	getchar();
	return 0;
}