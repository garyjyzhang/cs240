#include "encode.h"
#include <cstdlib>

int main() {
  Encode encode;

  unsigned char *c;
  c = (unsigned char*) malloc(1000);

  string test = "she sells sea shells by the sea shore";
  unsigned int compressed = encode.Compress(test, c);
  cout << (float) compressed / (float) test.size() << endl;
  string result = encode.Decompress(c, compressed);
  if(result != test) cout << "beeeee" << endl;

  delete [] c;
  return 0;
}