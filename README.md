# Decision Tree - Balance-Scale Analysis and Prediction
### Bài Tập Lớn Decision Tree môn Cấu trúc dữ liệu và giải thuật kỳ 1 năm học 2021-2022.
### Thành viên: Phạm Tiến Du MSV 20020039.
### Giáo viên hướng dẫn: CN Nguyễn Đình Tuân.
***
## 1. Giới thiệu bộ dữ liệu và output:
### Input:
- Bộ dữ liệu về Balance-Scale gồm 4 thuộc tính và 3 nhãn L (Left), R(Right), B(Balance) với 625 bản ghi.
- Bộ dự liệu đầy đủ có tại file fullTest.txt.
- File trainTest.txt có 439 bản ghi dùng để build decision tree.
- File validTest.txt có 93 bản ghi dùng để test decision.tree.
- File hiddenTest.txt là 93 bản ghi còn để thầy Tuân test lại.
- File Decision Tree.pptx giới thiệu về những đặc điểm cơ bản Decision Tree, GiniIndex, Information Gain, Confusion Matrix, Hyperparameter Search, Cross Validation, Sklearn check, Present answer by plot.
### Output:
- Giả sử hệ thống của ta chỉ có file trainTest.txt và che đi validTest.txt, hiddenTest.txt . Ta sẽ dự đoán cho validTest. hiddenTest sẽ là thầy kiểm tra lại.
- Print tree: in ra cây quyết định.
- Max Depth: Giá trị maxDepth chọn thông qua Hyperparameter search.
- Min Size: Giá trị minSize chọn thông qua Hyperparameter search.
- Accuracy of dataSet: accuracy khi dự đoán chính trainTest đã xây cây.
- Accuracy of unseenSet: accuracy khi dự đoán validTest hoặc hoặc hiddenTest.
- confusionMatrix: Bảng 3x3 với cột là dự doán. Cột là true label, hàng là predict label.
- **output được lưu trong output.txt người đọc có thể xem tiện.**
***
## 2. Giới thiệu thuật toán, module implement:
- Decision Tree: xây dựng cây quyết định theo cách đệ quy.
- Hyperparameter search: chạy các trường hợp minSize và maxDepth từ đó đánh giá giá trị minSize và maxDepth tốt nhất.
- Cross validation: Để chạy Hyperparameter cần chia trainSet ra làm phần để build và phần test. Cross validation sẽ chia đều cho các phần rồi tính giá trị trung bình. Từ đó sẽ khách quan hơn, giảm overfitting.
- Confusion Matrix: Bảng đối chiếu nxn với n là số thuộc tính để biết được số lượng nhãn true label là i và hệ thống đoán là j với 1 <= i <= n, 1<= j <= n.
***
## 3. Giới thiệu cấu trúc:
- source code ở file decisionTree.cpp.
- struct Data: lưu lại 1 bản ghi của dữ liệu gồm 4 thuộc tính và nhãn.
- struct Node: gồm Nhãn node (Label), thuộc tính điều kiện (attributeCondition), giá trị thuộc tính điều kiện(valueCondition),Node left, Node right. Nếu Node chưa phải terminateNode thì sẽ được gán nhãn khác L, R, B.
- struct Tree: chứa node root.
- vector<Data>* dataSet : nhập dữ liệu từ trainTest.txt
- vector<Data>* unseenSet : nhập dữ liệu từ validTest.txt hoặc hiddenTest.txt
- void loadFile : load data từ file.
- double entropyEachGroup(vector<Data>* cur): tính entropy trong 1 tập dữ liệu.
- double giniIndexEachGroup(vector<Data>* cur): tính gini trong 1 tập dữ liệu.
- SplitReturn getSplit(vector<Data>* cur): với tập dữ liệu hiện tại, thì chọn thuộc tính nào với key nào thì sẽ cho giá trị entropy or ginu tốt nhất thông qua InformationGain.
- char toTerminate(vector<Data>* cur): với tập dữ liệu hiện tại, để nó thành terminateNode thì nhãn nào chiếm đa số sẽ là nhãn của tập.
- void split(Node* nodeTree, vector<Data>* cur, int maxDepth, int minSize, int depth): với giá trị maxDepth, minSize thì sẽ quyết định cách chia cuối cùng của tập dữ liệu hiện tại sang 2 Node con hoặc terminate Node hiện tại cũng như 2 Node con.
- void buildTree(Tree* decisionTree, vector<Data>* cur, int maxDepth, int minSize): Tạo decisionTree từ maxDepth, minSize và tập dữ liệu cho trước.
- void printTree(Node* nodeTree, int depth): in ra cây sau khi build.
- char labelPredictRecursion(Node* nodeTree, Data* cur): Hàm recursion để lấy nhãn dự đoán 1 bản ghi trên cây decisionTree đã build.
- double prediction(Tree* decisionTree, vector<Data>* cur): Accuracy khi dự đoán tập cur.
***
## Update 1: chạy Hyperparameter cho Minsize, Maxdepth with CrossValidation:
- void buildBestModel(vector<Data>* cur): tìm ra minSize và maxDepth tốt nhất có sử dụng CrossValidation với kFold = 7 (để tránh overfitting).
- double calcPrecisionKFord(): phục vụ cho chia crossValidation, có thể trả về 1 trong 3 loại giá trị để so sánh:
+++ predictionTest * predictionTrain / (predictionTest + predictionTrain) : tác giả tự sáng tạo dựa trên kiểu tính F1 score.
+++ predictionTest * predictionTrain: tác giả tự sáng tạo dựa trên việc cần cả tập train và tập test đều cao tránh việc Overfitting nên dùng phép nhân. (a * b <= a^2 + b^2. max khi a = b)
+++ predictionTest: chỉ quan tâm để testSet.
- **Cải thiện: Thay vì chọn ngẫu nhiên Minsize, Maxdepth thì đã cố định được tốt nhất và tránh được overfitting khi chọn. Accuracy in validTest: 78% và Accuracy in trainTest: 95%.**
***
## Update 2: Cải thiện tốc độ thuật toán
- Do chạy DecisionTree đã khá lâu, thêm chạy Hyperparameter + CrossValidation sẽ làm mất 2 phút để chạy toàn bộ thuật toán.
- Nhận thấy tất cả các thuộc tính đều chỉ có giá trị từ 1-5. Vì vậy ta không for tất cả các bản ghi cho các thuộc tính như cây decision tree bình thường mà ta sẽ for từ 1 -> 5.
- **Cải thiện: Tốc độ theo tính toán sẽ giảm 439/5 = 88 lần. Thực tế chạy khoảng trong vòng 1s thay vì khoảng 120s như trước.**
***
## Update 3: MixData
- Data sắp xếp tuần tự nhãn L đến R đến B. Trộn lại data đảm bảo sự ngẫu nhiên hơn trong crossValidation. Tránh xảy ra tình trạng build toàn data nhãn L, R nhưng phải đoán nhãn B và ngược lại.
- **Cải thiện: Accuracy in validTest: 81% và Accuracy in trainTest: 95%.**
***
## Update 4: Plot answer để thấy rõ Overfitting (trong slide powerpoint)
- **Cải thiện: Thấu hiểu rõ hơn về data cũng như Overfitting trong bài toán.**
***
## Update 5: Confusion Matrix
- void addConfusionMatrix(Tree* decisionTree, vector<Data>* cur): in ra ConfusionMatrix sau khi dự đoán hết các tập dữ liệu.
- **Cải thiện: Thấu hiểu rõ hơn về việc cần cải thiện việc đoán nhãn B, dẫn đến update 6.**
## Update 6: Dùng True/False Positive/Negative, Precision and Recall, F1 scores
- Không thể update thêm do chưa xác định được nhãn nào là quan trọng hơn nhãn nào.
***
## Update 7: Improve Gini
- Để tính gini thì ta sẽ cntB, cntL, cntR. Ở dữ liệu ban đầu thì số bản ghi ở nhãn B đã ít hơn khoảng 5 lần so với R và L. Vì vậy mình nghĩ đến việc nâng cntB = cntB * 5 để tăng độ quan trọng của nhãn B.
- **Cải thiện: Accuracy in validTest: 83% và Accuracy in trainTest: 95%. Trong đó đã đoán thêm đúng một vài nhãn B.**
