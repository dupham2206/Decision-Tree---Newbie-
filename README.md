# Decision Tree - Balance-Scale Analysis and Prediction
## 1. Giới thiệu bộ dữ liệu:
### Bộ dữ liệu về Balance-Scale gồm 4 thuộc tính và 3 nhãn L (Left), R(Right), B(Balance) với 625 bản ghi.
### Bộ dự liệu đầy đủ có tại file fullTest.txt.
### File trainTest.txt có 439 bản ghi dùng để build decision tree.
### File validTest.txt có 93 bản ghi dùng để test decision.tree.
### File hiddenTest.txt là 93 bản ghi còn để thầy Tuân test lại.
### File Decision Tree.pptx giới thiệu về những đặc điểm cơ bản Decision Tree, GiniIndex, Information Gain, Confusion Matrix, Hyperparameter Search, Cross Validation, Sklearn check, Present answer by plot.
## 2. Giới thiệu cấu trúc:
### struct Data: lưu lại 1 bản ghi của dữ liệu gồm 4 thuộc tính và nhãn.
### struct Node: gồm Nhãn node (Label), thuộc tính điều kiện (attributeCondition), giá trị thuộc tính điều kiện(valueCondition),Node left, Node right. Nếu Node chưa phải terminateNode thì sẽ được gán nhãn khác L, R, B.
### struct Tree: chứa node root.
### vector<Data>* dataSet : nhập dữ liệu từ trainTest.txt
### vector<Data>* unseenSet : nhập dữ liệu từ validTest.txt hoặc hiddenTest.txt
### void loadFile : load data từ file.
### double entropyEachGroup(vector<Data>* cur): tính entropy trong 1 tập dữ liệu.
### double giniIndexEachGroup(vector<Data>* cur): tính gini trong 1 tập dữ liệu.
### SplitReturn getSplit(vector<Data>* cur): với tập dữ liệu hiện tại, thì chọn thuộc tính nào với key nào thì sẽ cho giá trị entropy or ginu tốt nhất thông qua InformationGain.
### char toTerminate(vector<Data>* cur): với tập dữ liệu hiện tại, để nó thành terminateNode thì nhãn nào chiếm đa số sẽ là nhãn của tập.
### void split(Node* nodeTree, vector<Data>* cur, int maxDepth, int minSize, int depth): với giá trị maxDepth, minSize thì sẽ quyết định cách chia cuối cùng của tập dữ liệu hiện tại sang 2 Node con hoặc terminate Node hiện tại cũng như 2 Node con.
### void buildTree(Tree* decisionTree, vector<Data>* cur, int maxDepth, int minSize): Tạo decisionTree từ maxDepth, minSize và tập dữ liệu cho trước.
### void printTree(Node* nodeTree, int depth): in ra cây sau khi build.
### char labelPredictRecursion(Node* nodeTree, Data* cur): Hàm recursion để lấy nhãn dự đoán 1 bản ghi trên cây decisionTree đã build.
### double prediction(Tree* decisionTree, vector<Data>* cur): Accuracy khi dự đoán tập cur.
### void addConfusionMatrix(Tree* decisionTree, vector<Data>* cur): in ra ConfusionMatrix sau khi dự đoán hết các tập dữ liệu.
## Update 1: chạy Hyperparameter cho Minsize, Maxdepth with CrossValidation:
### void buildBestModel(vector<Data>* cur): tìm ra minSize và maxDepth tốt nhất có sử dụng CrossValidation với kFold = 7.
### double calcPrecisionKFord(): phục vụ cho chia crossValidation, có thể trả về 1 trong 3 loại giá trị để so sánh:
#### predictionTest * predictionTrain / (predictionTest + predictionTrain) : tác giả tự sáng tạo dựa trên kiểu tính F1 score.
#### predictionTest * predictionTrain: tác giả tự sáng tạo dựa trên việc cần cả tập train và tập test đều cao tránh việc Overfitting nên dùng phép nhân. (a * b <= a^2 + b^2. max khi a = b)
#### predictionTest: chỉ quan tâm để testSet.
## Update 2: MixData
###  Data sắp xếp tuần tự nhãn L đến R đến B. Trộn lại data đảm bảo sự ngẫu nhiên hơn trong crossValidation.
## Update 3: Plot answer (slide powerpoint)
## Update 4: Dùng True/False Positive/Negative, Precision and Recall, F1 scores
### Không thể update thêm do chưa xác định được nhãn nào là quan trọng hơn nhãn nào.
## Update 5: Improve Gini
### Để tính gini thì ta sẽ cntB, cntL, cntR. Ở dữ liệu ban đầu thì số bản ghi ở nhãn B đã ít hơn khoảng 5 lần so với R và L. Vì vậy mình nghĩ đến việc nâng cntB = cntB * 5 để tăng độ quan trọng của nhãn B.
### Kết quả:


