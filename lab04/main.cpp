#include "RIPEMD-160.h"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cout << endl << "Usage: ./RIPEMD-160 file_in" << endl << endl;

	return -1;
    }
    else
    {
        RIPEMD_160 hash;

        if (!hash.read_file(argv[1]))
            cout << "Read error!" << endl << endl;
        else if (!hash.write_file((char*)"RIPEMD-160sum.txt", hash.ripemd_160()))
            cout << "Write error!" << endl << endl;

        
    }

    return 0;
}