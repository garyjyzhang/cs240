#include <iostream>
#include <string>
#include <vector>

using namespace std;

/*  Skeleton for Encode
    You may add as many functions/fields as required to implement your solution.
    You may NOT store a copy of the source text!  Doing so results in a score of 0. */

class Encode
{
    public:
        Encode() {intLength = 8; trailing = 0;};
        ~Encode() {};


        /*  Compress:
            ----------------------------------------------------
            Compress the source text into result.
            This function is responsible for allocating result.
            The caller is responsible for freeing result.
            Compress returns the length of the result.
            ---------------------------------------------------- */
        unsigned int Compress( const string &source, unsigned char *& result );


        /*  Decompress:
            ----------------------------------------------------
            Decompress codedText with the given length into a
            string.  Returns the decoded text as a string.
            Assumes codedText is not null, length > 0.
            ---------------------------------------------------- */
        string Decompress( unsigned char * codedText, unsigned int length );


    private:
        int intLength;
        unsigned int LZWEncode(string s, unsigned char *& result);
        string LZWDecode( unsigned char *, unsigned int );
        unsigned int convertIntToChar(vector<int>&, unsigned char *&);
        void convertCharToInt( vector<int>& , unsigned char *, unsigned int );
        vector<bool> convert(int);
        vector<bool> convertBack(int);
        int trailing;
        /*  SetBit:
            ----------------------------------------------------
            Set the k-th bit of the unsigned char (data) to 1.
            Assumes data is initially 0.
            Examples:
            SetBit( 0, data ) --> data = 0000 0001
            SetBit( 1, data ) --> data = 0000 0010
            SetBit( 2, data ) --> data = 0000 0100
            ---------------------------------------------------- */
        void SetBit( unsigned int k, unsigned char * data )
        {
            (* data) |= ( 1 << k );
        }

};