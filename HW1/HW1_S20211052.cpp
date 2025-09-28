#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm> //max
#include <climits> // INT_MIN
using namespace std;

typedef struct {
    int maxSum;
    int start;
    int finish;
} Kadane;

Kadane kadane1d(const vector<int>& arr){
    int currentSum = 0;
    int maxSum = INT_MIN;
    int finish = -1;
    int start = 0; // kadane 안에서 진행되는 시작
    int finalStart = -1; //최종 선택될 시작

    for(int i=0;i<arr.size();i++){
        if(arr[i] > currentSum + arr[i]){
            currentSum = arr[i];
            start = i;
        }
        else{
            currentSum += arr[i];
        }

        if(currentSum > maxSum){
            maxSum = currentSum;
            finalStart = start;
            finish = i;
        }
    }
    return {maxSum,finalStart,finish};
}

void kadane2d(const string& inputPath, const string& outputPath){
    //file open
    ifstream inputFile(inputPath, ios::binary);
    if(!inputFile.is_open()){
        cerr << "Error " << inputPath << " cannot open" << endl;
        return;
    }

    int m,n;
    inputFile.read(reinterpret_cast<char*>(&m),sizeof(int));
    inputFile.read(reinterpret_cast<char*>(&n),sizeof(int));

    vector<vector<int>> array(m,vector<int>(n));

    for(int r=0;r<m;++r){
        for(int c=0;c<n;++c){
            inputFile.read(reinterpret_cast<char*>(&array[r][c]), sizeof(int));
        }
    }
    inputFile.close();

    int maxSum = INT_MIN, finalLeft = -1, finalRight = -1, finalTop = -1, finalBottom = -1;
    // left column -> right column
    for(int left=0;left<n;++left){
        vector<int> rowSum(m,0);
        for(int right=left;right<n;++right){
            for(int row=0;row<m;++row) rowSum[row] += array[row][right];

            Kadane result = kadane1d(rowSum);

            if(result.maxSum > maxSum){
                maxSum = result.maxSum;
                finalLeft = left;
                finalRight = right;
                finalTop = result.start;
                finalBottom = result.finish;
            }// 가급적 작은 인덱스
            else if(result.maxSum == maxSum){
                bool update = false;
                if(left<finalLeft) update = true;
                else if(left==finalLeft && right<finalRight) update = true;
                else if(left==finalLeft && right==finalRight && result.start<finalTop) update = true;
                else if(left==finalLeft && right==finalRight && result.start==finalTop && result.finish<finalBottom) update = true;

                if(update){
                maxSum = result.maxSum;
                finalLeft = left;
                finalRight = right;
                finalTop = result.start;
                finalBottom = result.finish;
            }
            } //compare index finish
            
        }
    }
    // output
    ofstream outputFile(outputPath);
    if(!outputFile.is_open()){
        cerr << "Error " << outputPath << "cannot generate" << endl;
        return;
    }

    outputFile << maxSum << endl;
    outputFile << finalTop << endl;
    outputFile << finalLeft << endl;
    outputFile << finalBottom << endl;
    outputFile << finalRight << endl;

    outputFile.close();
}

int main(){
    ifstream configFile("HW1_Data_Open/HW1_config.txt");
    if(!configFile.is_open()){
        cerr << "Error, cannot open HW1_config.txt." << endl;
        return 1;
    }

    int cases;
    configFile >> cases;
    
    string inputBinFile, outputTxtFile;

    for(int i=0;i<cases;i++){
        configFile >> inputBinFile >> outputTxtFile;

        string inputPath = "HW1_Data_Open/" + inputBinFile;
        string outputPath = "HW1_Data_Open/" + outputTxtFile;

        //for debug
        cout << "----processing " << inputBinFile << "------" <<endl;
        kadane2d(inputPath, outputPath);
    }
    configFile.close();
    return 0;
}

/*
if(left<finalLeft){
                    maxSum = result.maxSum;
                    finalLeft = left;
                    finalRight = right;
                    finalTop = result.start;
                    finalBottom = result.finish;
                }
                else if(left==finalLeft){
                    if(right<finalRight){
                        maxSum = result.maxSum;
                        finalLeft = left;
                        finalRight = right;
                        finalTop = result.start;
                        finalBottom = result.finish;
                    }
                    else if(right==finalRight){
                        if(result.start<finalTop){
                            maxSum = result.maxSum;
                            finalLeft = left;
                            finalRight = right;
                            finalTop = result.start;
                            finalBottom = result.finish;
                        }
                        else if(result.start==finalTop){
                            if(result.finish<finalBottom)
                            maxSum = result.maxSum;
                            finalLeft = left;
                            finalRight = right;
                            finalTop = result.start;
                            finalBottom = result.finish;
                        }
                    }
                }
*/