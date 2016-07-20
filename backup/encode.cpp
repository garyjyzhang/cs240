#include "encode.h"
#include <iostream>
#include <vector>
#include <map>
#include <cassert>
#include <cmath>
#include <bitset>
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
    cout << "char: " << s.at(i) << " index: " << i << endl;
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

string Encode::LZWDecode( unsigned char * codedText, unsigned int length ) {
  if (length <= 0) return "";
  map<int, string> map;
  for (int c = 32; c <= 126; c++) {
    map[c] = string(1, (char) c);
  }
  map[127] = string(1, (char) 3);

  string result = "";
  int index = 128, j = 0;
  int code = codedText[j++];

  string s = map[code];
  result.append(s);

  while(j < length) {
    string sPrev = s;
    int code = codedText[j++];
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
      result[i++] = map[w];
      map[w + k] = index++;
      w = k;
    }
    j++;
  }

  result[i] = map[w];

  cout << "length: " << i + 1 << endl;
  return i + 1;
 }



// Implement Encode Here if Needed
unsigned int Encode::Compress( const string &source, unsigned char *& result ) {
  string from(source);
  string bwtCode = BWTCompress(from);

  cout << bwtCode << endl;
  return LZWEncode(bwtCode, result);
}


string Encode::Decompress( unsigned char * codedText, unsigned int length ) {
  return BWTDecompress(LZWDecode(codedText, length));
}
