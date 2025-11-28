#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

int main() {
    string binFileName;
    int s, f, p;

    ifstream inputFile("input.txt");
    inputFile >> binFileName;
    inputFile >> s >> f >> p;
    inputFile.close();
    ifstream binFile("HW4_Open_Data/Prob_1/" + binFileName, ios::binary);

    int m, n;
    binFile.read((char*)&m, sizeof(int));
    binFile.read((char*)&n, sizeof(int));

    string X(m, '\0');
    binFile.read(&X[0], m);

    string Y(n, '\0');
    binFile.read(&Y[0], n);

    binFile.close();

    vector<vector<int>> dp(m+1, vector<int>(n+1, 0));

    //initialize
    for(int i=0;i<=m;i++) dp[i][0] = -p * i;
    for(int j=0;j<=n;j++) dp[0][j] = -p * j;

    // fill dp table
    for(int i = 1; i <= m; i++){
        for(int j = 1; j <= n; j++){
            int match_score = (X[i-1] == Y[j-1]) ? s : -f;
            dp[i][j] = max({dp[i-1][j-1] + match_score, dp[i-1][j] - p, dp[i][j-1] - p});
        }
    }
    int max_score = dp[m][n];
    // backtrack to find gaps
    int i = m, j = n;
    vector<int> gap_positionsX;
    vector<int> gap_positionsY;
    int total_length = 0;

    while(i > 0 || j > 0){
        total_length++;
        if(i > 0 && j > 0){
            int match_score = (X[i-1] == Y[j-1]) ? s : -f;
            if(dp[i][j] == dp[i-1][j-1] + match_score){
                i--;
                j--;
            }
            else if(dp[i][j] == dp[i-1][j] - p){
                gap_positionsX.push_back(i);
                i--;
            }
            else{
                gap_positionsY.push_back(j);
                j--;
            }
        }
        else if(i > 0){
            gap_positionsX.push_back(i);
            i--;
        }
        else{
            gap_positionsY.push_back(j);
            j--;
        }
    }

    ofstream outputFile("output.txt");

    outputFile << max_score << endl;

    outputFile << total_length << endl;

    outputFile << gap_positionsX.size() << endl;
    for (int pos : gap_positionsX) {
        outputFile << pos << endl;
    }

    outputFile << gap_positionsY.size() << endl;
    for (int pos : gap_positionsY) {
        outputFile << pos << endl;
    }

    outputFile.close();

    return 0;
}
