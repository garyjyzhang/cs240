#include "encode.h"
#include <cstdlib>

int main() {
  Encode encode;

  unsigned char *c;
  c = (unsigned char*) malloc(200000);

  string test = "Betty Botter bought a bit of butter; \"But,\" she said, \"this butter's bitter!  If I put it in my batter it will make my batter bitter.  But a bit of better butter will make my batter better.\"  so she bouth a bit of butter better than her bitter butter, made her bitter batter better.  So 'twas better Betty Botter bought a bit of better butter.";
  unsigned int compressed = encode.Compress(test, c);

  cout << (float) compressed / (float) test.size() << endl;
  string result = encode.Decompress(c, compressed);
  cout << "result: " << result << endl;


  delete [] c;
  return 0;
}