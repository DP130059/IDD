#include <iostream>
#include <cstring>
#include <vector>
#include <map>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>

using namespace std;
map<string, int> attrheader;
void Stringsplit(const string& str, const string& split, vector<string>& res)
{
	char* strc = new char[str.size() + 1];
	strcpy(strc, str.c_str());
	char* temp = strtok(strc, split.c_str());
	while (temp != NULL)
	{
		res.push_back(string(temp));
		temp = strtok(NULL, split.c_str()); // 下一个被分割的串
	}
	delete[] strc;
}
class modeltable
{
private:
	vector<string> left;
	vector<string> right;

public:
	modeltable() {}
	modeltable(string singleline)
	{
		vector<string> tmp;
		string split = "||";
		Stringsplit(singleline, split, tmp);
		string y = tmp[tmp.size() - 1];
		if (y.find("]") != y.npos)
		{
			y = y.erase(y.length() - 1);
		}
		split = ",";
		Stringsplit(tmp[0], split, left);
		for (int i = 0; i < left.size(); i++)
		{
			if (left[i].find("[") != left[i].npos)
			{
				left[i] = left[i].substr(1);
			}
			else
			{
				continue;
			}
		}
		Stringsplit(y, split, right);
	}
	string tostr()
	{
		string res = "[" + left[0];
		for (int i = 1; i < left.size(); i++)
		{
			res = res + "," + left[i];
		}
		res = res + "||" + right[0];
		for (int i = 1; i < right.size(); i++)
		{
			res = res + "," + right[i];
		}
		res = res + "]";
		return res;
	}
	vector<string> getLeft()
	{
		vector<string> result(left);
		return result;
	}
	vector<string> getRight()
	{
		vector<string> result(right);
		return result;
	}
	bool equals(modeltable other)
	{
		vector<string> otherLeft = other.getLeft();
		vector<string> otherRight = other.getRight();
		if (otherLeft != left)
		{
			return false;
		}
		if (otherRight != right)
		{
			return false;
		}
		return true;
	}
};

class CFD
{
private:
	vector<string> left;
	vector<string> right;
	modeltable table;
	map<string, long> leftconst;
	map<string, long> rightconst;
	/*vector<string>leftconstindex;
	vector<string>rightconstindex;*/
	vector<string> leftvarindex;
	vector<string> rightvarindex;

public:
	CFD() {}
	CFD(string line)
	{
		vector<string> split;
		string delim = ";";
		Stringsplit(line, delim, split);
		string tableline = split[1];
		table = modeltable(tableline);
		string attrline = split[0];
		string delim2 = ">";
		vector<string> attrsplit;
		Stringsplit(attrline, delim2, attrsplit);
		string attr1 = attrsplit[0], attr2 = attrsplit[1];
		string delim3 = ",";
		vector<string> attr1split;
		Stringsplit(attr1, delim3, attr1split);
		for (int i = 0; i < attr1split.size(); i++)
		{
			if (i == 0)
			{
				attr1split[i] = attr1split[i].substr(1);
			}
			if (i == attr1split.size() - 1)
			{
				attr1split[i] = attr1split[i].erase(attr1split[i].length() - 1);
			}

			string tmp(attr1split[i]);
			left.push_back(tmp);
		}
		vector<string> attr2split;
		Stringsplit(attr2, delim3, attr2split);
		for (int i = 0; i < attr2split.size(); i++)
		{
			if (i == 0)
			{
				attr2split[i] = attr2split[i].substr(1);
			}
			if (i == attr2split.size() - 1)
			{
				attr2split[i] = attr2split[i].erase(attr2split[i].length() - 1);
			}
			string tmp(attr2split[i]);
			right.push_back(tmp);
		}
		/*vector<string>tleft=table.getLeft();
		for(int i=0;i<left.size();i++){
			string attr=left[i];
			if(tleft[i]!="_"){
				long tlc=stol(tleft[i]);
				leftconst.insert(pair<string,long>(attr,tlc));
				leftconstindex.push_back(attr);
			}else{
				leftvarindex.push_back(attr);
			}
		}
		vector<string>tright=table.getRight();
		for(int i=0;i<right.size();i++){
			string attr=right[i];
			if(tright[i]!="_"){
				long rlc=stol(tright[i]);
				rightconst.insert(pair<string,long>(attr,rlc));
				rightconstindex.push_back(attr);
			}else{
				rightvarindex.push_back(attr);
			}
		}*/
	}
	vector<string> getLeft()
	{
		vector<string> result(left);
		return result;
	}
	vector<string> getRight()
	{
		vector<string> result(right);
		return result;
	}
	modeltable getModelTable()
	{
		return table;
	}
	/*vector<string>getLeftconstIndex(){
		vector<string>result(leftconstindex);
		return result;
	}
	vector<string>getRightconstIndex(){
		vector<string>result(rightconstindex);
		return result;
	}*/
	string tostr()
	{
		string result = "{" + left[0];
		for (int i = 1; i < left.size(); i++)
		{
			result = result + "," + left[i];
		}
		result = result + "}>{" + right[0];
		for (int i = 1; i < right.size(); i++)
		{
			result = result + "," + right[i];
		}
		result = result + "};" + table.tostr();
		return result;
	}
	int getLeftSize()
	{
		int res = left.size();
		return res;
	}
	int getRightSize()
	{
		int res = right.size();
		return res;
	}
	map<string, long> getLeftConst()
	{
		map<string, long> res(leftconst);
		return res;
	}
	map<string, long> getRightConst()
	{
		map<string, long> res(rightconst);
		return res;
	}
};
class TNode
{
private:
	bool consistent;
	vector<long> tleft;
	vector<long> tids;

public:
	TNode() {}
	TNode(bool flag, vector<long> tupleleft, vector<long> tupleids)
	{
		consistent = flag;
		tleft = tupleleft;
		tids = tupleids;
	}
	bool ifconsistent()
	{
		return consistent;
	}
	void setconsistent(bool flag)
	{
		consistent = flag;
	}
	vector<long> getTleft()
	{
		vector<long> res(tleft);
		return res;
	}
	void setTleft(vector<long> otherleft)
	{
		tleft = otherleft;
	}
	vector<long> getTids()
	{
		vector<long> res(tids);
		return res;
	}
	void setTids(vector<long> otherTids)
	{
		tids = otherTids;
	}
	string tostr()
	{
		string res;
		res = res + "{";
		if (consistent)
		{
			res += "true";
		}
		else
		{
			res += "false";
		}
		res = res + ";{" + to_string(tleft[0]);
		for (int i = 1; i < tleft.size(); i++)
		{
			res.append("," + to_string(tleft[i]));
		}
		res.append("};{");
		res.append(to_string(tids[0]));
		for (int i = 1; i < tids.size(); i++)
		{
			res.append("," + to_string(tids[i]));
		}
		res.append("}}");
		return res;
	}
};
vector<long> parsedata(string dataline)
{
	vector<long> result;
	vector<string> datalinesplit;
	string split = "\t";
	Stringsplit(dataline, split, datalinesplit);
	for (auto it = datalinesplit.begin(); it != datalinesplit.end(); it++)
	{
		string tmp = (string)*it;
		long data = stol(tmp);
		result.push_back(data);
	}
	return result;
}
bool checklcons(vector<long> data, CFD cfd)
{
	map<string, long> cfdlc = cfd.getLeftConst();
	for (auto it : cfdlc)
	{
		string attr = it.first;
		long cfdcons = it.second;
		int k = attrheader[attr];
		long datacons = data[k];
		if (cfdcons != datacons)
		{
			return false;
		}
	}
	return true;
}
bool checkrcons(vector<long> data, CFD cfd)
{
	map<string, long> cfdlc = cfd.getRightConst();
	for (auto it : cfdlc)
	{
		string attr = it.first;
		long cfdcons = it.second;
		int k = attrheader[attr];
		long datacons = data[k];
		if (cfdcons != datacons)
		{
			return false;
		}
	}
	return true;
}
vector<long> parseldata(vector<long> data, CFD cfd)
{
	vector<long> res;
	vector<string> cfdlattrs = cfd.getLeft();
	for (int i = 0; i < cfdlattrs.size(); i++)
	{
		string attr = cfdlattrs[i];
		int k = attrheader[attr];
		res.push_back(data[k]);
	}
	return res;
}
vector<long> parserdata(vector<long> data, CFD cfd)
{
	vector<long> res;
	vector<string> cfdlattrs = cfd.getRight();
	for (int i = 0; i < cfdlattrs.size(); i++)
	{
		string attr = cfdlattrs[i];
		int k = attrheader[attr];
		res.push_back(data[k]);
	}
	return res;
}
void nativeincd()
{
	vector<long> result;
	map<vector<long>, TNode> valueTogroup;
	fstream filestream;
	filestream.open("/home/gyp/projects/IDD/CFD", ios::in);
	vector<CFD> CFDs;
	if (filestream.is_open())
	{
		string CFDline;
		while (getline(filestream, CFDline))
		{
			CFD tmpcfd(CFDline);
			CFDs.push_back(tmpcfd);
		}
		filestream.close();
	}
	cout << CFDs[0].tostr() << endl;
	filestream.open("/home/gyp/projects/IDD/Data", ios::in);
	if(filestream.is_open()) {
		string dataline;
		getline(filestream, dataline);
		string split = "|";
		vector<string> headers;
		Stringsplit(dataline, split, headers);
		for (int i = 0; i < headers.size(); i++)
		{
			attrheader.insert(pair<string, int>(headers[i], i));
			cout << headers[i] << endl;
		}
		CFD cfd = CFDs[0];
		while (getline(filestream, dataline))
		{
			vector<long> tuple = parsedata(dataline);
			cout << tuple[0] << endl;
			if (!checklcons(tuple, cfd))
				continue;
			if (!checkrcons(tuple, cfd))
			{
				result.push_back(tuple[0]);
				continue;
			}
			if (cfd.getRightSize() == 0)
			{
				continue;
			}
			vector<long> tleft = parseldata(tuple, cfd);
			vector<long> tright = parserdata(tuple, cfd);
			if (valueTogroup.find(tleft) == valueTogroup.end())
			{
				vector<long> ids;
				TNode pnode(true, tright, ids);
				valueTogroup.insert(pair<vector<long>, TNode>(tleft, pnode));
			}
			else
			{
				TNode pnode = valueTogroup.find(tleft)->second;
				if (pnode.ifconsistent())
				{
					vector<long> firstv = pnode.getTleft();
					if (firstv == tright)
					{
						vector<long> tids = pnode.getTids();
						tids.push_back(tuple[0]);
						pnode.setTids(tids);
						valueTogroup[tleft] = pnode;
					}
					else
					{
						pnode.setconsistent(false);
						valueTogroup[tleft] = pnode;
						vector<long>falseids = pnode.getTids();
						for (auto it = falseids.begin(); it != falseids.end(); it++) {
							long tmp = (long)*it;
							result.push_back(tmp);
						}
					}
				}
				else
				{
					result.push_back(tuple[0]);
				}
			}
		}
		filestream.close();
	}
	cout << result.size() << endl;
	filestream.open("/home/gyp/projects/IDD/Result", ios::out);
	if (filestream.is_open()) {
		for (int i = 0; i < result.size(); i++) {
			filestream << result[i] << endl;
		}
		filestream.close();
	}
}

int main(int argc, char const* argv[])
{
	time_t begin, end;
	begin = clock();
	nativeincd();
	end = clock();
	double result = (end - begin) / CLOCKS_PER_SEC;
	cout << "running time is\t" << result << endl;
	getchar();
	return 0;
}
