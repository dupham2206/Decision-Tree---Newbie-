#include<bits/stdc++.h>
using namespace std;


// Introduce Data, Node, Tree           V
// Calc Gini Index, Information Gain    V
// Split data                           V
// Build tree, split                    V
// print tree                           V
// get prediction                       V
// choose minSize, maxDepth(Why???)     V
// cross validation (buildBestModel)    V
// confusion matrix                     V


struct Data {
    char label;
    int attribute[4];
    int valueSort;
    Data(char getLabel,int a0,int a1,int a2,int a3){
        label = getLabel;
        attribute[0] = a0;
        attribute[1] = a1;
        attribute[2] = a2;
        attribute[3] = a3;
    }
};

struct Node {
    char label;
    int attributeCondition;
    double valueCondition;
    Node* left;
    Node* right;
    void setNode(int attr, double value, Node* l, Node* r,char tempLabel){
        attributeCondition = attr;
        valueCondition = value;
        left = l;
        right = r;
        label = tempLabel;
    }
};

struct Tree {
    Node* root;
};

struct SplitReturn {
    int attributeChoosen;
    double valueChoosen;
    vector<Data>* left;
    vector<Data>* right;
    double gini;
    SplitReturn(int attr,double value, vector<Data>* l, vector<Data>* r, double Gini){
        left = l;
        right = r;
        attributeChoosen = attr;
        valueChoosen = value;
        gini = Gini;
    }
};

vector<Data>* dataSet = new vector<Data>();
vector<Data>* unseenSet = new vector<Data>();
string AllLabel = "LBR";
const int labelSize = 3;
const int AttributeSize = 4;
const int kFold = 7;
const int SizeOfMinSize = 7;
const int SizeOfMaxdepth = 10;
const char NOT_TERMINATE_NODE = 'X';
int bestMaxDepth;
int bestMinSize;
int confusionMatrix[256][256];

void loadFile(vector<Data>* cur, string filePath){
    string s;
    ifstream inFile;
    inFile.open(filePath);
    if (!inFile) {
        cout << "Unable to open file";
        exit(1); // terminate with error
    }
    while (inFile >> s) {
        cur->push_back(Data(s[0], s[2] - '0', s[4] - '0', s[6] - '0', s[8] - '0'));
    }
    inFile.close();
}
double entropyEachGroup(vector<Data>* cur){
    double entropy = 0.0;
    int cnt[labelSize];
    int curSize = cur->size();
    for(int i = 0; i < labelSize; ++i) cnt[i] = 0;
    for(int i = 0; i < curSize; ++i){
        for(int j = 0; j < labelSize; ++j){
            if(cur->at(i).label == AllLabel[j]){
                ++cnt[j];
                break;
            }
        }
    }
    for(int i = 0; i < labelSize; ++i){
        if(cnt[i] == 0) continue;
        double score = (double)cnt[i] / curSize;
        entropy -= score * log2(score);
    }
    return entropy;
}

double giniIndexEachGroup(vector<Data>* cur){
    double gini = 1.0;
    int cnt[labelSize];
    int curSize = cur->size();
    for(int i = 0; i < labelSize; ++i) cnt[i] = 0;
    for(int i = 0; i < curSize; ++i){
        for(int j = 0; j < labelSize; ++j){
            if(cur->at(i).label == AllLabel[j]){
                ++cnt[j];
                break;
            }
        }
    }
    // buff weight of B label
    curSize = cnt[1] * 5 + cnt[0] + cnt[2];
    cnt[1] *= 5;
    for(int i = 0; i < labelSize; ++i){
        double score = (double)cnt[i] / curSize;
        gini -= score * score;
    }
    return gini;
}

SplitReturn getSplit(vector<Data>* cur){
    int curDataSize = cur->size();
    int attributeChoosen = -1;
    double valueChoosen = -1;
    double valueCompare = 2.0;
    vector<Data>* left;
    vector<Data>* right;
    for(int i = 0; i < AttributeSize; ++i){
        //for(int j = 0; j < curDataSize; ++j){
        for(int j = 1; j <= 5; ++j){
            //int valueSplit = cur->at(j).attribute[i];
            int valueSplit = j;
            vector<Data>* leftTemp = new vector<Data>;
            vector<Data>* rightTemp = new vector<Data>;
            for(int k = 0; k < curDataSize; ++k){
                if(cur->at(k).attribute[i] < valueSplit) leftTemp->push_back(cur->at(k));
                else rightTemp->push_back(cur->at(k));
            }
            if(!leftTemp->size() || !rightTemp->size()) continue;
            double ratioLeftTemp = (double)leftTemp->size() / curDataSize;
            double tempCompare = giniIndexEachGroup(leftTemp) * ratioLeftTemp + giniIndexEachGroup(rightTemp) * (1 - ratioLeftTemp);
            //double tempCompare = entropyEachGroup(leftTemp) * ratioLeftTemp + entropyEachGroup(rightTemp) * (1 - ratioLeftTemp);
            if(tempCompare < valueCompare){
                left = leftTemp;
                right = rightTemp;
                valueCompare = tempCompare;
                attributeChoosen = i;
                valueChoosen = valueSplit;
            }
        }
    }
    return SplitReturn(attributeChoosen, valueChoosen, left, right, valueCompare);
}

char toTerminate(vector<Data>* cur){
    int cnt[labelSize];
    int curSize = cur->size();
    for(int i = 0; i < labelSize; ++i) cnt[i] = 0;
    for(int i = 0; i < curSize; ++i){
        for(int j = 0; j < labelSize; ++j){
            if(cur->at(i).label == AllLabel[j]){
                ++cnt[j];
                break;
            }
        }
    }
    int ans = 0;
    for(int i = 0; i < labelSize; ++i){
        if(cnt[i] > cnt[ans]) ans = i;
    }
    return AllLabel[ans];
}

void split(Node* nodeTree, vector<Data>* cur, int maxDepth, int minSize, int depth){
    SplitReturn temp = getSplit(cur);
    if(temp.gini == 0.0 || depth >= maxDepth || (temp.left->size() <= minSize && temp.right->size() <= minSize)){
        char labelLeft = toTerminate(temp.left);
        char labelRight = toTerminate(temp.right);
        if(labelLeft == labelRight){
            nodeTree->setNode(-1, -1, nullptr, nullptr, labelLeft);
        }
        else {
            nodeTree->setNode(temp.attributeChoosen, temp.valueChoosen, new Node(), new Node(), NOT_TERMINATE_NODE);
            nodeTree->left->setNode(-1, -1, nullptr, nullptr, labelLeft);
            nodeTree->right->setNode(-1, -1, nullptr, nullptr, labelRight);
        }
        return;
    }
    nodeTree->setNode(temp.attributeChoosen, temp.valueChoosen, new Node(), new Node(), NOT_TERMINATE_NODE);
    if(temp.left->size() <= minSize){
        char labelLeft = toTerminate(temp.left);
        nodeTree->left->setNode(-1, -1, nullptr, nullptr, labelLeft);
    }
    else split(nodeTree->left, temp.left, maxDepth, minSize, depth + 1);
    if(temp.right->size() <= minSize){
        char labelRight = toTerminate(temp.right);
        nodeTree->right->setNode(-1, -1, nullptr, nullptr, labelRight);
    }
    else split(nodeTree->right, temp.right, maxDepth, minSize, depth + 1);
}

void buildTree(Tree* decisionTree, vector<Data>* cur, int maxDepth, int minSize){
    decisionTree->root = new Node();
    split(decisionTree->root, cur, maxDepth, minSize, 1);
}

void printTree(Node* nodeTree, int depth){
    if(nodeTree == nullptr) return;
    string print;
    for(int i = 1; i < depth; ++i) print += " ";
    if(nodeTree->label == NOT_TERMINATE_NODE)
        cout << print << "[X" << nodeTree->attributeCondition << " < " << nodeTree->valueCondition << "]\n";
    else
        cout << print << nodeTree->label << "\n";
    printTree(nodeTree->left, depth + 1);
    printTree(nodeTree->right, depth + 1);
}

char labelPredictRecursion(Node* nodeTree, Data* cur){
    if(nodeTree->label != NOT_TERMINATE_NODE) return nodeTree->label;
    if(cur->attribute[nodeTree->attributeCondition] < nodeTree->valueCondition)
        return labelPredictRecursion(nodeTree->left, cur);
    return labelPredictRecursion(nodeTree->right, cur);
}

double prediction(Tree* decisionTree, vector<Data>* cur){
    int curSize = cur->size();
    int cntPredictTrue = 0;
    for(int i = 0; i < curSize; ++i){
        char labelPredict = labelPredictRecursion(decisionTree->root, &cur->at(i));
        char labelTrue = cur->at(i).label;
        if(labelPredict == labelTrue) ++cntPredictTrue;
    }
    return (double)cntPredictTrue / curSize;
}

void addConfusionMatrix(Tree* decisionTree, vector<Data>* cur){
    int curSize = cur->size();
    for(int i = 0; i < curSize; ++i){
        char labelPredict = labelPredictRecursion(decisionTree->root, &cur->at(i));
        char labelTrue = cur->at(i).label;
        confusionMatrix[labelTrue][labelPredict]++;
    }
    cout << "Confusion matrix: horizontal axis is predict, vertical axis is true\n";
    cout << curSize << " ";
    for(int i = 0; i < labelSize; ++i) cout << AllLabel[i] << " ";
    cout << "\n";
    for(int j = 0; j < labelSize; ++j){
        for(int i = -1; i < labelSize; ++i){
            if(i == -1) cout << AllLabel[j] << "  ";
            else cout << confusionMatrix[AllLabel[j]][AllLabel[i]] << " ";
        }
        cout << "\n";
    }
}
double calcPrecisionKFord(int numberOfFold , vector<Data>* cur, int minSize, int maxDepth, int numberElementEachFold){
    vector<Data>* trainSet = new vector<Data>();
    vector<Data>* testSet = new vector<Data>();
    int beginTestSet = (numberOfFold - 1) * numberElementEachFold;
    int endTestSet = numberOfFold * numberElementEachFold - 1;
    //cout << beginTestSet << " " << endTestSet << "\n";
    int curSize = cur->size();
    for(int i = 0; i < curSize; ++i){
        if(beginTestSet <= i && i <= endTestSet) testSet->push_back(cur->at(i));
        else trainSet->push_back(cur->at(i));
    }
    Tree* decisionTree = new Tree();
    buildTree(decisionTree, trainSet, maxDepth, minSize);
    double predictionTest = prediction(decisionTree, testSet);
    double predictionTrain = prediction(decisionTree, trainSet);
    return 2 * predictionTest * predictionTrain / (predictionTest + predictionTrain); // something cal like F1
    //return prediction(decisionTree, testSet) * prediction(decisionTree, trainSet);
    //return prediction(decisionTree, testSet);

}

void buildBestModel(vector<Data>* cur){
    int numberElementEachFold = cur->size() / kFold;
    double tempSum = 0.0;
    for(int i = 2; i <= SizeOfMinSize; ++i){
        for(int j = 1; j <= SizeOfMaxdepth; ++j){
            double sumProductPrecisionTrainAndSet = 0.0;
            for(int k = 1; k <= kFold; ++k){
                //if(i == 1) cout << calcPrecisionKFord(k, cur, i, j, numberElementEachFold) << "\n";
                sumProductPrecisionTrainAndSet += calcPrecisionKFord(k, cur, i, j, numberElementEachFold);
            }
            cout << i << " " << j << " " << sumProductPrecisionTrainAndSet/kFold << "\n";
            if(sumProductPrecisionTrainAndSet > tempSum){
                tempSum = sumProductPrecisionTrainAndSet;
                bestMaxDepth = j;
                bestMinSize = i;
            }
        }
    }
}

bool cmp(Data a, Data b){
    return a.valueSort < b.valueSort;
}

void mixData(vector<Data>* cur){
    for(int i = 0; i < cur->size(); ++i){
        cur->at(i).valueSort = rand();
    }
    sort(cur->begin(), cur->end(), cmp);
}

signed main(){
    srand(time(NULL));
    loadFile(dataSet, "trainTest.txt");
    loadFile(unseenSet, "validTest.txt");
    //loadFile(unseenSet, "hiddentest.txt");
    mixData(dataSet);
    buildBestModel(dataSet);
    Tree* decisionTree = new Tree();
    buildTree(decisionTree, dataSet, bestMaxDepth, bestMinSize);
    printTree(decisionTree->root, 1);
    cout << "MaxDepth: " << bestMaxDepth << "\n";
    cout << "MinSize: " << bestMinSize << "\n";
    cout << "Accuracy of dataSet: " << prediction(decisionTree, dataSet) * 100 << "%\n";
    cout << "Accuracy of unseenSet: " << prediction(decisionTree, unseenSet) * 100 << "%\n";
    addConfusionMatrix(decisionTree, unseenSet);



    /*
    for(int i = 2; i <= SizeOfMinSize; ++i){
        ofstream file;
        file.open("answerForPlot.txt");

        for(int j = 1; j <= SizeOfMaxdepth; ++j){
            Tree* decisionTree = new Tree();
            buildTree(decisionTree, dataSet, j, i);
            //cout << "minsize:" << i << " " << "maxdepth:" << j << " ";
            //cout << prediction(decisionTree, dataSet) <<  " " << prediction(decisionTree, unseenSet) << "\n";
            file << j << "," << prediction(decisionTree, dataSet) << "," << prediction(decisionTree, unseenSet) << "\n";
        }
    }
    */
}

