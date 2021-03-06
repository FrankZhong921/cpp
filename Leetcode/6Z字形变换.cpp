class Solution {
public:
    string convert(string s, int numRows) {
        if(numRows >= s.size() || numRows ==1  ) return s;
        int itemSize = numRows + numRows-2;
        int length = s.size();
        int numItems = (length % itemSize)? length/itemSize + 1: length/itemSize;
        int i = 0;
        string res = "";
        
        for(int row = 0; row < numRows ; ++row){
            int span =  2 *(numRows -1 - row ) ;  //
            for(int item = 0; item < length;item += itemSize){
                if(item +row < length){
                    res += s[item + row];
                    if( row != numRows-1  && span < itemSize && (item + span < length)){
                        res += s[item + row + span];
                    }
                }
                
            }
        }
       return res;
        
    }
};
