#include<iostream>
#include<string>
#include<cctype>
using namespace std;

bool isPalindrome(string s)
{
    int left=0;
    int right=s.length()-1;
    while(left<right)
    {
        if(!isalnum(s[left]))
        {
            left++;
            continue;
        }
        if(!isalnum(s[right]))
        {
            right--;
            continue;
        }
        if(tolower(s[left])!=tolower(s[right]))
        {
            return false;
        }
        left++;
        right--;
    }
    return true;
}

int main()
{
    string s;
    cout<<"Type out your string: ";
    getline(cin,s);
    if(isPalindrome(s))
    {
        cout<<"A palindrome!"<<endl;
    }
    else
    {
        cout<<"Not a palindrome"<<endl;
    }
    return 0;
}
    