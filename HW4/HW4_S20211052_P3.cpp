#include<iostream>
#include<fstream>
#include<string>
#include<vector>
using namespace std;

int main(){
    ifstream inputFile("input.txt");
    int cases;
    inputFile >> cases;

    vector<vector<int>> num(cases);
    for(int i=0;i<cases;i++){
        int len;
        inputFile >> len;
        vector<int> arr(len);
        for(int j=0;j<len;j++) inputFile >> arr[j];
        num[i] = arr;
    }
    inputFile.close();

    struct Result {
        int diff;
        int n;
        int n1;
        vector<int> subset1;
        vector<int> subset2;
    };
    vector<Result> results(cases);

    for(int i=0;i<cases;i++){
        int len = num[i].size();
        int total_sum = 0;
        for(int j=0;j<len;j++) total_sum += num[i][j];
        // DP table: sigma 1~i = total_sum ?
        vector<vector<bool>> dp(len+1, vector<bool>(total_sum+1, false));
        int half = total_sum / 2;

        dp[0][0] = true;
        for(int j=1;j<=len;j++){
            for(int sum=0;sum<=half;sum++){
                if(dp[j-1][sum]){ // not choose
                    dp[j][sum] = true;
                }
                int value = num[i][j-1];
                if(sum >= value && dp[j-1][sum - value]) dp[j][sum] = true; // choose
            }
        }

        int best_sum = 0;
        for(int s=half;s>=0;s--){
            if(dp[len][s]){
                best_sum = s;
                break;
            }
        }
        int diff = abs((total_sum - best_sum) - best_sum);

        //backtrack
        vector<int> subset1, subset2;
        int cur_sum = best_sum;
        for(int k=len;k>0;k--){
            // Check if k-th element is in subset1
            if(cur_sum >= num[i][k-1] && dp[k-1][cur_sum - num[i][k-1]]){
                subset1.push_back(num[i][k-1]);
                cur_sum -= num[i][k-1];
            }
            else{
                subset2.push_back(num[i][k-1]);
            }
        }

        results[i].diff = diff;
        results[i].n = len;
        results[i].n1 = subset1.size();
        results[i].subset1 = subset1;
        results[i].subset2 = subset2;
    }

    // Output
    ofstream outputFile("output.txt");
    outputFile << cases << endl;
    for(int i=0; i<cases; i++){
        outputFile << results[i].diff << " " << results[i].n << " " << results[i].n1 << endl;
        for(int val : results[i].subset1){
            outputFile << val << endl;
        }
        for(int val : results[i].subset2){
            outputFile << val << endl;
        }
    }
    outputFile.close();

    return 0;
}