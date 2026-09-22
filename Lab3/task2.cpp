#include<iostream>
#include<string>
using namespace std;

class StringPool
{
    private:
        string** stringPool;   // array of addresses
        int currentsize;
        int maxsize;

    public:
        StringPool()
        {
            maxsize = 5;
            currentsize = 0;
            stringPool = new string*[maxsize];

            // empty slots start as nullptr so a filled slot is detectable later
            for(int i = 0; i < maxsize; i++)
            {
                stringPool[i] = nullptr;
            }
        }

        void addString(string s)
        {
            if(currentsize >= maxsize)
            {
                cout << "Pool is full! Cannot add \"" << s << "\"" << endl;
                return;
            }

            stringPool[currentsize] = new string(s);
            currentsize++;
            cout << "Added: " << s << endl;
        }

        // Deliberately faulty: drops the count but never frees.
        // The string stays allocated with nothing pointing at it.
        void removeString()
        {
            if(currentsize == 0)
            {
                cout << "Pool is empty! Nothing to remove." << endl;
                return;
            }

            currentsize--;
            cout << "Removed: " << *stringPool[currentsize]
                 << "   <-- memory NOT freed" << endl;
        }

        void fixMemoryLeak()
        {
            int recovered = 0;

            // slots at or past currentsize are abandoned anything still holding an address there is a leaked string
            for(int i = currentsize; i < maxsize; i++)
            {
                if(stringPool[i] != nullptr)
                {
                    cout << "Leak found in slot " << i << ": " << *stringPool[i] << endl;
                    delete stringPool[i];      // free it first
                    stringPool[i] = nullptr;   // forget the address
                    recovered++;
                }
            }

            if(recovered == 0)
            {
                cout << "No leaks found." << endl;
            }
            else
            {
                cout << recovered << " leaked string(s) freed." << endl;
            }
        }

        void displayPool()
        {
            cout << "--- Pool status: " << currentsize << "/" << maxsize << " ---" << endl;

            if(currentsize == 0)
            {
                cout << "(empty)" << endl;
            }

            for(int i = 0; i < currentsize; i++)
            {
                cout << i << ": " << *stringPool[i] << endl;
            }
            cout << endl;
        }

        // strings first, then the array that held their addresses
        ~StringPool()
        {
            for(int i = 0; i < currentsize; i++)
            {
                delete stringPool[i];
            }
            delete[] stringPool;
        }
};

int main()
{
    StringPool pool;

    cout << " 1. Adding strings " << endl;
    pool.addString("hello");
    pool.addString("world");
    pool.addString("data");
    pool.addString("structures");
    cout << endl;
    pool.displayPool();

    cout << " 2. Removing without freeing " << endl;
    pool.removeString();
    pool.removeString();
    cout << endl;
    pool.displayPool();

    cout << "Pool reports 2/5, but \"data\" and \"structures\" are still" << endl;
    cout << "in memory with nothing pointing at them." << endl << endl;

    cout << " 3. Detecting and fixing the leak " << endl;
    pool.fixMemoryLeak();

    return 0;
}
