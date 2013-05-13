// This section has not been tested, move into Optional_Func.h
//template <typename T>
//bool compareVec(vector<T> const &vec1, vector<T> const &vec2){
//	if (vec1.empty() && vec2.empty())
//	{
//		return true;
//	}
//	size_t s1 = vec1.size();
//	size_t s2 = vec2.size();
//	if(s1 != s2){
//		return false;
//	}
//	for (int i = 0; i < s1; i++)
//	{
//		if (vec1[i] != vec2[i])
//		{
//			return false;
//		}
//	}
//	return true;
//}
//
//template <typename T>
//bool compareVec_2D(vector<vector<T> > const &vec1, vector<vector<T> > const &vec2){
//	if (vec1.empty() && vec2.empty())
//	{
//		return true;
//	}
//	size_t r1 = vec1.size();	
//	size_t r2 = vec2.size();	
//	if (r1 != 0 && r2 != 0 && r1 != r2)
//	{
//		return false;		
//	}
//	size_t c1 = vec1[0].size();
//	size_t c2 = vec2[0].size();
//	if (c1 != 0 && c2 != 0 && c1 != c2)
//	{
//		return false;
//	}
//
//	for (int r = 0; r < r1; r++)
//	{
//		for (int c = 0; c < c1; c++)
//		{
//			if (vec1[r][c] != vec2[r][c])
//			{
//				return false;
//			}
//		}
//	}
//	return true;
//}