#include "encode.h"
#include <iostream>
#include <vector>
#include <map>
#include <cassert>
#include <cmath>
#include <algorithm>

using namespace std;

struct kvp {
  char c;
  int i;
  kvp(char c, int i) : c(c), i(i) {}
  friend bool operator< (const kvp& k1, const kvp& k2) {
    if (k1.c != k2.c)
      return k1.c < k2.c;
    else return k1.i < k2.i;
  }
};

string BWTDecompress(string s) {
  int size = s.size();
  string result = "";

  vector<kvp> v;
  map<int, char> map;
  for(int i = 0; i < size; i++) {
    v.push_back(kvp(s.at(i), i));
    map[i] = s.at(i);
  }


  sort(v.begin(), v.end());

  int index = 0;
  for(int i = 0; i < size; i++) {
    result += map[v[index].i];
    index = v[index].i;
  }

  return result.substr(1, result.size() - 1);
}

string BWTCompress(string s) {
  s += (char) 3;
  int size = s.size();
  vector<string> v;
  string result = "";
  for(int i = 0; i < size; i++)
 {
    char c = s.at(s.size() - 1);
    s = s.substr(0, size - 1).insert(0, 1, c);
    v.push_back(s);
 }

 sort(v.begin(), v.end());
 for(int i = 0; i < size; i++) {
    result.append(&(v[i].at(size - 1)));
 }

 return result;
}

vector<bool> Encode::convertBack(int x) {
  vector<bool> ret;
  while(x) {
    if (x&1)
      ret.push_back(1);
    else
      ret.push_back(0);
    x>>=1;
  }
  while(ret.size() < 8) {
    ret.push_back(0);
  }
  reverse(ret.begin(),ret.end());
  return ret;
}

void Encode::convertCharToInt(vector<int>& result, unsigned char *codedText, unsigned int length) {
  vector<bool> v;
  int index = 0;

  for(int i = 0; i < length; i++) {
    int num = codedText[i];
    vector<bool> ret = convertBack(num);

    v.insert(v.end(), ret.begin(), ret.end());
    if (v.size() % intLength == 0 && v.size() != 0) {
      for(int i = 0, j = intLength - 1; j < v.size(); i += intLength, j+= intLength) {
        int x = 0;
        for(int p = i; p <= j; p++) {
          x <<= 1;
          x |= v[p];
        }
        result.push_back(x);
      }
      v.clear();
    }
  }

  if (v.size() != 0) {
    for(int i = 0; i < trailing; i++) {
      v.pop_back();
    }


    assert(v.size() % intLength == 0);
    for(int i = 0, j = intLength - 1; j < v.size(); i += intLength, j+= intLength) {
        int x = 0;
        for(int p = i; p <= j; p++) {
          x <<= 1;
          x |= v[p];
        }
        result.push_back(x);
      }
  }

}


string Encode::LZWDecode( unsigned char * codedText, unsigned int length ) {
  if (length <= 0) return "";
  map<int, string> map;
  for (int c = 32; c <= 126; c++) {
    map[c] = string(1, (char) c);
  }
  map[127] = string(1, (char) 3);

  vector<int> originalCodes;
  convertCharToInt(originalCodes, codedText, length);


  string result = "";
  int index = 128, j = 0;
  int code = originalCodes[j++];

  string s = map[code];
  result.append(s);

  while(j < originalCodes.size()) {
    string sPrev = s;
    int code = originalCodes[j++];
    if (code == index)
      s = sPrev + sPrev.substr(0, 1);
    else
      s = map[code];
    result.append(s);
    map[index++] = sPrev + s.substr(0, 1);
  }

  return result;
}

unsigned int Encode::LZWEncode(string s, unsigned char *& result) {
  map<string, int> map;
  vector<int> resultList;
  for (int c = 32; c <= 126; c++) {
    map[string(1, (char) c)] = c;
  }
  map[string(1, (char) 3)] = 127;

  if (s.size() == 0) return 0;
  int i = 0, j = 0;
  string w = "", k;
  int index = 128;

  while (j < s.size()) {
    k = s.substr(j, 1);
    if (map.find(w + k) != map.end()) {
      w = w + k;
    }
    else {
      assert(map.find(w) != map.end());
      resultList.push_back(map[w]);
      map[w + k] = index++;
      if(index > pow(2, intLength) - 1){
        intLength++;
      }
      w = k;
    }
    j++;
  }

  resultList.push_back(map[w]);
  return convertIntToChar(resultList, result);
 }

vector<bool> Encode::convert(int x) {
  vector<bool> ret;
  while(x) {
    if (x&1)
      ret.push_back(1);
    else
      ret.push_back(0);
    x>>=1;
  }
  while(ret.size() < intLength) {
    ret.push_back(0);
  }
  reverse(ret.begin(),ret.end());
  return ret;
}


unsigned int Encode::convertIntToChar(vector<int>& list, unsigned char *& result) {
  vector<bool> v;
  int index = 0;

  for(int i = 0; i < list.size(); i++) {
    int num = list[i];
    vector<bool> ret = convert(num);

    v.insert(v.end(), ret.begin(), ret.end());
    if (v.size() % 8 == 0 && v.size() != 0) {
      for(int i = 0, j = 7; j < v.size(); i += 8, j+= 8) {
        int x = 0;
        for(int p = i; p <= j; p++) {
          x <<= 1;
          x |= v[p];
        }
        result[index++] = x;
      }
      v.clear();
    }
  }

  if (v.size() != 0) {
    int remainder = v.size() % 8;
    while(v.size() % 8 != 0) {
      v.push_back(0);
      trailing++;
    }

    for(int i = 0, j = 7; j < v.size(); i += 8, j+= 8) {
        int x = 0;
        for(int p = i; p <= j; p++) {
          x <<= 1;
          x |= v[p];
        }
        result[index++] = x;
      }
  }
  return index;
}


// Implement Encode Here if Needed
unsigned int Encode::Compress( const string &source, unsigned char *& result ) {
  string from(source);
  string bwtCode = BWTCompress(from);

  int length = LZWEncode(bwtCode, result);
  return length;
}


string Encode::Decompress( unsigned char * codedText, unsigned int length ) {
  return BWTDecompress(LZWDecode(codedText, length));
}
