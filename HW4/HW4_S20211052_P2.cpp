#include<iostream>
#include<fstream>
#include<string>
#include<vector>
using namespace std;

int main(){
    vector<pair<int, string>> test_cases;
    ifstream inputFile("input.txt");
    int case_num;
    inputFile >> case_num;

    for(int i=0;i<case_num;i++){
        int len;
        string str;
        inputFile >> len;
        inputFile >> str;
        test_cases.push_back({len, str});
    }
    inputFile.close();

    vector<vector<string>> results(case_num);

    // palindrome
    for(int num=0;num<case_num;num++){
        int len = test_cases[num].first;
        string str = test_cases[num].second;

        int left = 0, right = len - 1;
        vector<vector<bool>> dp(len, vector<bool>(len, false));

        for(int i=0;i<len;i++) dp[i][i] = true;
        
        for(int i=0;i<len-1;i++) dp[i][i+1] = (str[i]==str[i+1]);

        for(int l=3;l<=len;l++){
            for(int i=0;i<=len-l;i++){
                int j = i + l - 1;
                dp[i][j] = (str[i]==str[j]) && dp[i+1][j-1];
            }
        }

        vector<int> counts(len);
        counts[0] = 1;
        for(int i=1;i<len;i++){
            // whole palindrome
            if(dp[0][i]) counts[i] = 1;
            else{
                counts[i] = len;
                for(int j=0;j<i;j++){
                    // back is palindrome
                    if(dp[j+1][i]) counts[i] = min(counts[i], counts[j]+1);
                }
            }
        }

        // backtrack
        vector<string> palindromes;
        int idx = len - 1;
        while(idx >= 0){
            if(dp[0][idx]){
                palindromes.push_back(str.substr(0, idx+1));
                break;
            }

            for(int j=0;j<idx;j++){
                if(dp[j+1][idx] && counts[idx] == counts[j] + 1){
                    palindromes.push_back(str.substr(j+1, idx - j));
                    idx = j;
                    break;
                }
            }
        }
        reverse(palindromes.begin(), palindromes.end());
        results[num] = palindromes;
    }

    // Output
    ofstream outputFile("output.txt");
    outputFile << case_num << endl;
    for(int i=0; i<case_num; i++){
        outputFile << results[i].size() << endl;
        for(const string& p : results[i]){
            outputFile << p << endl;
        }
    }
    outputFile.close();

    return 0;
}