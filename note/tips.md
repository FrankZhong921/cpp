1. ```cpp
   vector<int> v1(5);
   vector<int> v2;
   v1.push_back(1);
   v2.push_back(1);
   
   std::cout << v1.size() << std::endl; // 6
   std::cout << v2.size() << std::endl; // 1
   ```

2. ```cpp
   unordered_map<int,vector<int>> m;
   m.insert({1,{3,4,5}});		//返回指向pair的iter;
   for(auto vec_vaule : m.find(1)->second){
   	std::cout << vec_value << std::endl;
   }
   ```

3. 

